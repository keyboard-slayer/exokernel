{C_response_type} {rpc_function}({rpc_args})
{{
    json_t {rpc_module}_req = json_object();
    json_push(&{rpc_module}_req, "rpc_function", json_string("{rpc_function}"));

    json_t rpc_args = json_object();
    {rpc_args_push}
    json_push(&{rpc_module}_req, "rpc_args", rpc_args);

    syscall(SYS_SENDIPC, (uint64_t) json_dump({rpc_module}_req), {rpc_identifier}, 1);
    json_t resp = syscall(SYS_RECVIPC_SYNC, 0);
    json_t success = json_get(resp, "rpc_success");
    assert(success._bool);

    json_t resp = json_get(resp, "rpc_response");
    assert(resp.type == JSON_{rpc_response_type});
    {rpc_return}

    json_free(&rpc_args);
    json_free(&{rpc_module}_req);
    json_free(&resp);
    json_free(&success);
    {rpc_ret}
}}

void rpc_response_{rpc_function}(pid_t pid, json_t reponse)
{{
    json_t {rpc_module}_resp = json_object();
    json_push(&{rpc_module}_resp, "rpc_function", {rpc_function_UPPER});
    json_push(&{rpc_module}_resp, "rpc_response", response);
    json_push(&{rpc_module}_resp, "rpc_success", json_bool(true));
    
    syscall(SYS_SENDIPC, (uint64_t) json_dump({rpc_module}_resp), pid);
    json_free(&{rpc_module}_resp);
}}

