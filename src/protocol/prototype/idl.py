#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import ast
import os
import sys
from os.path import basename
from inspect import getfile, getsource
from hashlib import sha1

type_translator = {}
headers = set([])
compiled = []
func_enum = []
source_code = ""

def to_json_type(type_name):
    if type_name in type_translator.values():
        return "int"

    match type_name:
        case "char *":
            return "string"
        case "long":
            return "int"
        case "bool":
            return "bool"
        case "void":
            return "null"
        case _:
            print(f"Unknown type: {type_name}")
            exit(1)

def to_ctype(type_name):
    if type_name in type_translator:
        return type_translator[type_name]

    match type_name:
        case "str":
            return "char *"
        case "int":
            return "long"
        case "bool":
            headers.add("stdbool.h")
            return "bool"
        case _:
            print(f"Unknown type: {type_name}")
            exit(1)


class HGen(ast.NodeTransformer):
    def __init__(self, module_name):
        self.module_name = module_name

        if not os.path.isdir(os.path.join(sys.argv[1], "inc")):
            os.mkdir(os.path.join(sys.argv[1], "inc"))

    def visit_FunctionDef(self, node):
        global source_code

        identifier = "0x"+str(sha1(self.module_name.encode()).hexdigest())

        func_name = node.name
        func_args = [(arg.arg, to_ctype(arg.annotation.id)) for arg in node.args.args]

        if len(func_args) == 0:
            args = "void"
        else:
            args = ", ".join(
                [f"{arg_type} {arg_name}" for arg_name, arg_type in func_args]
            )

        if type(node.returns) == ast.Constant:
            if node.returns.value is None:
                return_type = "void"
            else:
                print(f"Unknown return type: {node.returns.value}")
                exit(1)
        else:
            return_type = to_ctype(node.returns.id)

        source_code += (
            f"{return_type} {self.module_name.lower()}_{func_name}({args});\n"
        )

        func_enum.append(f"{self.module_name.upper()}_{func_name.upper()}")

        with open(os.path.join(sys.argv[1], "inc", f"{self.module_name}.h"), "w") as f:
            f.write(f"#ifndef PROT_INC_{self.module_name.upper()}_H\n")
            f.write(f"#define PROT_INC_{self.module_name.upper()}_H\n\n")
            f.write(f"#define {self.module_name.upper()}_IDENTIFIER {identifier}\n")

            for header in headers:
                f.write(f"#include <{header}>\n")

            f.write(f"\nenum {self.module_name.upper()}_FUNC_ENUM \n{{\n")
            f.write("    " + ",\n    ".join(func_enum))
            f.write("\n};\n")

            f.write("\n")
            f.write(source_code)
            f.write("\n")
            f.write(f"#endif /* !PROT_INC_{self.module_name.upper()}_H */\n")


class CGen(ast.NodeTransformer):
    def __init__(self, module_name):
        self.module_name = module_name.lower()

        if not os.path.isdir(os.path.join(sys.argv[1], "src")):
            os.mkdir(os.path.join(sys.argv[1], "src"))

    def visit_FunctionDef(self, node):
        func_name = node.name
        func_args = [(arg.arg, to_ctype(arg.annotation.id)) for arg in node.args.args]

        if len(func_args) == 0:
            args = "void"
        else:
            args = ", ".join(
                [f"{arg_type} {arg_name}" for arg_name, arg_type in func_args]
            )

        if type(node.returns) == ast.Constant:
            if node.returns.value is None:
                return_type = "void"
            else:
                print(f"Unknown return type: {node.returns.value}")
                exit(1)
        else:
            return_type = to_ctype(node.returns.id)

        if self.module_name not in compiled:
            compiled.append(self.module_name)
            with open(os.path.join(sys.argv[1], "src", f"{self.module_name}.c"), "w") as f:
                # --- HEADERS 
                f.write("#include <assert.h>\n")
                f.write("#include <json.h>\n")
                f.write("#include <unistd.h>\n\n")
                f.write(f'#include "../inc/{self.module_name}.h"\n\n')
                f.write(f"")

        with open(os.path.join(sys.argv[1], "src", f"{self.module_name}.c"), "a") as f:
            # --- CLIENT ENDPOINT
            with open(os.path.join(os.path.dirname(__file__), "endpoint.c"), "r") as endpoint:
                template = endpoint.read()

            f.write(template.format(
                C_response_type = return_type,
                rpc_response_type = to_json_type(return_type),
                rpc_function = f"{self.module_name}_{func_name}",
                rpc_function_UPPER = f"{self.module_name.upper()}_{func_name.upper()}",
                rpc_args = args,
                rpc_module = f"{self.module_name}_{func_name}",
                rpc_identifier = f"{self.module_name.upper()}_IDENTIFIER",
                rpc_args_push = "\n    ".join(f"json_push(&rpc_args, \"{arg_name}\", json_{to_json_type(arg_type)}({arg_name}));" for arg_name, arg_type in func_args),
                rpc_return = "" if return_type == "void" else  f"{return_type} ret = resp._{to_json_type(return_type)};",
                rpc_ret = "" if return_type == "void" else  f"\n    return ret"
            ))


def endpoint(cls):
    module_name = basename(getfile(cls)).split(".")[0]
    endpoint_func = filter(lambda x: not x.startswith("__"), dir(cls))

    for func in endpoint_func:
        func_source = getsource(getattr(cls, func)).strip()
        header = HGen(module_name)
        source = CGen(module_name)
        header.visit(ast.parse(func_source))
        source.visit(ast.parse(func_source))


def enum(cls):
    global source_code
    global type_translator

    enum_fields = list(filter(lambda x: not x.startswith("__"), dir(cls)))
    enum_as_dict = {k: getattr(cls, k) for k in enum_fields}

    type_translator[cls.__name__] = f"enum {cls.__name__.upper()}"

    source_code += f"""enum {cls.__name__.upper()}
{{
    {f'{chr(10)}    '.join(f"{k} = {v}," for k, v in enum_as_dict.items())}
}};\n\n"""
