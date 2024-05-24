
int privateLuaCEmbed_put_arg_on_top(LuaCEmbed *self, int index){
    long  formatted_index = index + LUA_CEMBED_INDEX_DIF;

    char *formated_arg = private_LuaCembed_format(PRIVATE_LUA_CEMBED_ARGS_,
                                                  private_LuaCEmbed_get_stack_size(self),
                                                  formatted_index-1);
    lua_getglobal(self->state,formated_arg);

    if(lua_type(self->state,-1) == LUA_CEMBED_NOT_FOUND){

        char *error = private_LuaCembed_format(PRIVATE_LUA_CEMBED_ARG_NOT_PROVIDED,formatted_index,self->current_function);

        privateLuaCEmbed_raise_error_not_jumping(self,error);
        free(error);
        free(formated_arg);
        return LUA_CEMBED_GENERIC_ERROR;
    }
    free(formated_arg);
    return LUA_OK;
}

int  LuaCEmbed_get_total_args(LuaCEmbed *self){
    return self->total_args;
}



int  LuaCEmbed_get_arg_type(LuaCEmbed *self,int index){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    privateLuaCEmbed_put_arg_on_top(self,index);

    int type =  lua_type(self->state, -1);
    lua_settop(self->state,0);
    return type;
}


long long LuaCEmbed_get_long_arg(LuaCEmbed *self, int index){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    privateLuaCEmbed_put_arg_on_top(self,index);
    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_NUMBER)){
        lua_settop(self->state,0);
        return (long )LUA_CEMBED_NOT_FOUND;
    }
    long long result =  (long long)lua_tonumber(self->state,-1);
    lua_settop(self->state,0);
    return result;
}


double LuaCEmbed_get_double_arg(LuaCEmbed *self, int index){
    PRIVATE_LUA_CEMBED_PROTECT_NUM

    privateLuaCEmbed_put_arg_on_top(self,index);
    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_NUMBER)){
        lua_settop(self->state,0);
        return (long )LUA_CEMBED_NOT_FOUND;
    }
    double result = lua_tonumber(self->state,-1);
    lua_settop(self->state,0);
    return result;
}

bool LuaCEmbed_get_bool_arg(LuaCEmbed *self, int index){
    PRIVATE_LUA_CEMBED_PROTECT_BOOL

    privateLuaCEmbed_put_arg_on_top(self,index);
    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_BOOL)){
        lua_settop(self->state,0);
        return (long )LUA_CEMBED_NOT_FOUND;
    }
    bool result = lua_toboolean(self->state,-1);
    lua_settop(self->state,0);
    return result;
}


char * LuaCEmbed_get_str_arg(LuaCEmbed *self, int index){
    PRIVATE_LUA_CEMBED_PROTECT_NULL
    lua_settop(self->state,0);
    privateLuaCEmbed_put_arg_on_top(self,index);
    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_STRING)){
        return NULL;
    }
    char *result =  (char*)lua_tostring(self->state,-1);
    lua_settop(self->state,0);
    return result;
}

LuaCEmbedTable  * LuaCEmbed_get_arg_table(LuaCEmbed *self,int index){
    PRIVATE_LUA_CEMBED_PROTECT_NULL

    privateLuaCEmbed_put_arg_on_top(self,index);
    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_TABLE)){
        return NULL;
    }

    int formatted_index = index + LUA_CEMBED_INDEX_DIF;
    char *formated_arg = private_LuaCembed_format(PRIVATE_LUA_CEMBED_ARGS_,
                                                  private_LuaCEmbed_get_stack_size(self),
                                                  formatted_index-1);
    LuaCEmbedTable  *created = LuaCembed_get_global_table(self,formated_arg);
    free(formated_arg);
    lua_settop(self->state,0);
    return created;

}

LuaCEmbedTable* LuaCEmbed_run_args_lambda(LuaCEmbed *self, int index, LuaCEmbedTable *args_to_call, int total_returns){

    long  formatted_index = index + LUA_CEMBED_INDEX_DIF;
    char *formatted_arg = private_LuaCembed_format(PRIVATE_LUA_CEMBED_ARGS_,
                                                   private_LuaCEmbed_get_stack_size(self),
                                                   formatted_index-1);
    privateLuaCEmbed_put_arg_on_top(self,index);

    if(private_LuaCEmbed_ensure_top_stack_arg_type(self,index,LUA_CEMBED_FUNCTION)){
        free(formatted_arg);
        lua_settop(self->state,0);
        return  NULL;
    }

    int total_args = private_lua_cEmbed_unpack(args_to_call,formatted_arg);

    if(lua_pcall(self->state,total_args,total_returns,0)){
        privateLuaCEmbed_raise_error_not_jumping(self, lua_tostring(self->state,-1));
        free(formatted_arg);
        lua_settop(self->state,0);
        return  NULL;
    }

    for(int i = 0; i < total_returns; i++){
        char *formatted = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MULTIRETURN_,private_LuaCEmbed_get_stack_size(self),i);
        int position = (i +1) * -1;
        lua_pushvalue(self->state,position);
        lua_setglobal(self->state,formatted);
        free(formatted);
    }

    LuaCEmbedTable  *result = LuaCembed_new_anonymous_table(self);
    for(int i = 0; i < total_returns; i++){
        lua_getglobal(self->state,result->global_name);
        lua_pushinteger(self->state,i+1);
        char *formatted = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MULTIRETURN_,private_LuaCEmbed_get_stack_size(self),i);
        lua_getglobal(self->state,formatted);
        lua_settable(self->state,-3);
        free(formatted);
    }
    free(formatted_arg);
    lua_settop(self->state,0);

    return result;

 }
