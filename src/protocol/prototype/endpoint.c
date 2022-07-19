{C_response_type} {rpc_module}_{rpc_function}({rpc_args})
{{
    json_t {rpc_module}_{rpc_function}_req = json_object();
    json_push(&{rpc_module}_{rpc_function}_req, "rpc_function", json_string("{rpc_function}"));

    json_t rpc_args = json_object();
    {rpc_args_push}
    json_push(&{rpc_module}_{rpc_function}_req, "rpc_args", rpc_args);

    ipc_t message = {{
        .identifier = {rpc_identifier},
        .payload = (uintptr_t) json_dump({rpc_module}_{rpc_function}_req),
        .payload_require_free = true
    }};

    syscall(SYS_SENDIPC, (uint64_t) &message);
    json_t *resp = (json_t *) syscall(SYS_RECVIPC_SYNC, 0);
    json_t success = json_get(*resp, "rpc_success");
    assert(success._bool);

    json_t rpc_resp = json_get(*resp, "rpc_response");
    assert(rpc_resp.type == JSON_{rpc_response_type});
    {rpc_return}

    json_free(&{rpc_module}_{rpc_function}_req);
    json_free(&rpc_args);
    json_free(resp);
    {rpc_ret}
}}

void rpc_response_{rpc_function}(pid_t pid, json_t response)
{{
    json_t {rpc_module}_{rpc_function}_resp = json_object();
    json_push(&{rpc_module}_{rpc_function}_resp, "rpc_function", json_number({rpc_function_UPPER}));
    json_push(&{rpc_module}_{rpc_function}_resp, "rpc_response", response);
    json_push(&{rpc_module}_{rpc_function}_resp, "rpc_success", json_bool(true));

    ipc_t message = {{
        .dst = pid,
        .payload = (uintptr_t) json_dump({rpc_module}_{rpc_function}_resp),
        .payload_require_free = true
    }};
    
    syscall(SYS_SENDIPC, (uint64_t) &message);
    json_free(&{rpc_module}_{rpc_function}_resp);
}}

{rpc_module}_req_{rpc_function}_t rpc_request_{rpc_function}_unpack(json_t request)
{{
    json_t rpc_args = json_get(request, "rpc_args");
    {rpc_module}_req_{rpc_function}_t ret = {{
        {rpc_args_unpack}
    }};

    return ret;
}}
