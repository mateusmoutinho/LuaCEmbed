

int privateLuaCEmbed_main_callback_handler(lua_State  *L){

    bool is_a_method = lua_toboolean(L, lua_upvalueindex(1));
    bool is_a_function = !is_a_method;
    LuaCEmbedResponse *possible_return = NULL;
    LuaCEmbed  *self = (LuaCEmbed*)lua_touserdata(L,lua_upvalueindex(2));
    self->total_args =  lua_gettop(self->state);

    const char *func_name =  lua_tostring(L,lua_upvalueindex(3));
    self->current_function = func_name;
    self->func_tables = (void*)newprivateLuaCEmbedTableArray();

    if(is_a_method){
        LuaCEmbedResponse *(*method_callback)(LuaCEmbedTable *tb, LuaCEmbed *self);
        lua_pushvalue(L, lua_upvalueindex(4));
        lua_setglobal(L,PRIVATE_LUA_CEMBED_SELFNAME);

        LuaCEmbedTable  *table = newLuaCembedTable(self,false, PRIVATE_LUA_CEMBED_SELFNAME);
        method_callback = (LuaCEmbedResponse *(*)(LuaCEmbedTable *tb, LuaCEmbed *self))lua_touserdata(L, lua_upvalueindex(5));
        possible_return = method_callback(table,self);
        privateLuaCEmbedTable_free(table);
    }

    if(is_a_function){
        LuaCEmbedResponse *(*function_callback)(LuaCEmbed *self);
        function_callback = (LuaCEmbedResponse *(*)(LuaCEmbed *self))lua_touserdata(L, lua_upvalueindex(4));
        possible_return = function_callback(self);
    }

    privateLuaCEmbedTableArray_free((privateLuaCEmbedTableArray*)self->func_tables);
    self->current_function = NULL;

    if(!possible_return){
        return PRIVATE_LUACEMBED_NO_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_ERROR_RESPONSE){
        lua_pushstring(L, possible_return->string_val);
        private_LuaCEmbedResponse_free(possible_return);
        lua_error(L);
        return PRIVATE_LUACEMBED_NO_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_LONG_RESPONSE){
        lua_pushinteger(L, (int)possible_return->num_val);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_BOOL_RESPONSE){
        lua_pushboolean(L, (bool)possible_return->num_val);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_DOUBLE_RESPONSE){
        lua_pushnumber(L, possible_return->num_val);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_STRING_RESPONSE){
        lua_pushstring(L, possible_return->string_val);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_DOUBLE_RESPONSE){

        lua_pushboolean(L, (bool)possible_return->num_val);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_TABLE_RESPONSE){
        lua_getglobal(self->state, possible_return->string_val);
        lua_pushvalue(self->state,-1);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;
    }

    if(possible_return->type == PRIVATE_LUA_CEMBED_EVALUATION){
        char *formated_function =private_LuaCembed_format(
                PRIVATE_LUA_CEMBED_GLOBAL_EVALUATION_CODE,
                PRIVATE_LUA_CEMBED_EVALUATION_NAME,
                possible_return->string_val
        );

        int error_code = LuaCEmbed_evaluate_string_no_return(self, formated_function);
        free(formated_function);

        if(error_code){
            private_LuaCEmbedResponse_free(possible_return);
            return PRIVATE_LUACEMBED_NO_RETURN;
        }

        lua_getglobal(self->state, PRIVATE_LUA_CEMBED_EVALUATION_NAME);
        int v_type = lua_type(self->state,-1);
        if(v_type == LUA_CEMBED_FUNCTION){
            const int TOTAL_ARGS =0;
            const int TOTAL_RETURNS =1;
            if(lua_pcall(self->state,TOTAL_ARGS,TOTAL_RETURNS,0)){
                private_LuaCEmbedResponse_free(possible_return);
                return PRIVATE_LUACEMBED_NO_RETURN;
            }
        }

        lua_pushvalue(self->state,-1);
        private_LuaCEmbedResponse_free(possible_return);
        return PRIVATE_LUACEMBED_ONE_RETURN;

    }

    private_LuaCEmbedResponse_free(possible_return);
    return PRIVATE_LUACEMBED_NO_RETURN;

}


void private_LuaCEmbed_add_lib_callback(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args) ){

    //get the table
    lua_getglobal(self->state,PRIVATE_LUA_CEMBED_MAIN_LIB_TABLE_NAME);
    lua_pushvalue(self->state,-1);

    //set the function name
    lua_pushstring(self->state,callback_name);

    //creating the clojure

    //creating the clojure
    lua_pushboolean(self->state,false);//is a method
    lua_pushlightuserdata(self->state,(void*)self); //self
    lua_pushstring(self->state,callback_name);//calback name
    lua_pushlightuserdata(self->state,(void*)callback);//calback

    lua_pushcclosure(self->state,privateLuaCEmbed_main_callback_handler,4);


    lua_settable(self->state,-3);
    if(self->public_functions){
        //it points the function to a global function
        //like: callback = private_lua_c_embed_main_lib_table.callback
        lua_getglobal(self->state,PRIVATE_LUA_CEMBED_MAIN_LIB_TABLE_NAME);
        lua_getfield(self->state,-1,callback_name);
        lua_setglobal(self->state, callback_name);
    }


}

void private_LuaCEmbed_add_evaluation_callback(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args) ){

    //creating the clojure
    lua_pushboolean(self->state,false);//is a method
    lua_pushlightuserdata(self->state,(void*)self); //self
    lua_pushstring(self->state,callback_name);//calback name
    lua_pushlightuserdata(self->state,(void*)callback);//calback

    lua_pushcclosure(self->state,privateLuaCEmbed_main_callback_handler,4);
    lua_setglobal(self->state, callback_name);

}


void LuaCEmbed_add_callback(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args) ){


    if(self->is_lib){
        private_LuaCEmbed_add_lib_callback(self,callback_name,callback);
        return;
    }
    private_LuaCEmbed_add_evaluation_callback(self,callback_name,callback);

}
