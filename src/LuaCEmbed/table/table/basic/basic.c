



LuaCEmbedTable * private_newLuaCembedTable(LuaCEmbed *main_embed, const char *format, ...){

    LuaCEmbedTable  *self = (LuaCEmbedTable*)malloc(sizeof (LuaCEmbedTable));
    *self = (LuaCEmbedTable){0};
    self->main_object =main_embed;

    va_list  args;
    va_start(args,format);
    self->global_name = private_LuaCembed_format_vaarg(format,args);
    va_end(args);

    self->sub_tables = (void*)newprivateLuaCEmbedTableArray();

    return self;
}

 int  private_lua_cEmbed_unpack(LuaCEmbedTable *self,const char * previews_function){

    long size = LuaCEmbedTable_get_listable_size(self);
     lua_settop(self->main_object->state, 0);

     private_lua_cembed_memory_limit = self->main_object->memory_limit;

     lua_getglobal(self->main_object->state,self->global_name);
     int table_index = lua_gettop(self->main_object->state);
     int total = 0;
     lua_pushnil(self->main_object->state);
     while(lua_next(self->main_object->state,table_index)){
         char *formated = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MULTIRETURN,total);
         lua_pushvalue(self->main_object->state,-1);
         lua_setglobal(self->main_object->state,formated);
         free(formated);
         lua_pop(self->main_object->state,1);
         total+=1;
     }
    if(previews_function){
        lua_getglobal(self->main_object->state,previews_function);
    }

    for(int i = 0; i < size; i++){
        char *formated = private_LuaCembed_format(PRIVATE_LUA_CEMBED_MULTIRETURN,i);
        lua_getglobal(self->main_object->state,formated);
        free(formated);
    }


    return (int)size;
}
void privateLuaCEmbedTable_free(LuaCEmbedTable *self){

    free(self->global_name);

    if(self->prop_name){
        free(self->prop_name);
    }

    privateLuaCEmbedTableArray_free((privateLuaCEmbedTableArray*)self->sub_tables);
    free(self);
}

void privateLuaCEmbedTable_free_setting_nill(LuaCEmbedTable *self){

    lua_getglobal(self->main_object->state,self->global_name);
    lua_pushnil(self->main_object->state);
    lua_setglobal(self->main_object->state,self->global_name);
    privateLuaCEmbedTable_free(self);
}
