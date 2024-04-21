



void LuaCEmbedTable_set_method(LuaCEmbedTable *self , const char *name, LuaCEmbedResponse *(*callback)(LuaCEmbedTable  *self, LuaCEmbed *args)){

    lua_getglobal(self->main_object->state,self->global_buffer);
    lua_pushstring(self->main_object->state,name);

    //creating the clojure
    lua_pushlightuserdata(self->main_object->state,(void*)true);//is a method
    lua_pushlightuserdata(self->main_object->state,(void*)self->main_object); //self
    lua_pushlightuserdata(self->main_object->state,(void*)name);//calback name
    lua_pushlightuserdata(self->main_object->state,(void*)self);//table
    lua_pushlightuserdata(self->main_object->state,(void*)callback);

    //add these clojure to be handled by the callbacks
    lua_pushcclosure(self->main_object->state,privateLuaCEmbed_main_callback_handler,5);
    lua_settable(self->main_object->state,-3);
}

void  LuaCEmbedTable_set_string_prop(LuaCEmbedTable *self , const char *name, const char *value){
    lua_getglobal(self->main_object->state,self->global_buffer);
    lua_pushstring(self->main_object->state,name);
    lua_pushstring(self->main_object->state,value);

    lua_settable(self->main_object->state,-3);
}

void  LuaCEmbedTable_set_long_prop(LuaCEmbedTable *self , const char *name, long  value){
    lua_getglobal(self->main_object->state,self->global_buffer);
    lua_pushstring(self->main_object->state,name);
    lua_pushnumber(self->main_object->state,(double)value);
    lua_settable(self->main_object->state,-3);
}

void  LuaCEmbedTable_set_double_prop(LuaCEmbedTable *self , const char *name, double  value){
    lua_getglobal(self->main_object->state,self->global_buffer);
    lua_pushstring(self->main_object->state,name);
    lua_pushnumber(self->main_object->state,value);
    lua_settable(self->main_object->state,-3);
}

void  LuaCEmbedTable_set_bool_prop(LuaCEmbedTable *self , const char *name, bool value){
    lua_getglobal(self->main_object->state,self->global_buffer);
    lua_pushstring(self->main_object->state,name);
    lua_pushboolean(self->main_object->state,value);
    lua_settable(self->main_object->state,-3);
}
