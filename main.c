
#include "src/one.c"
LuaCEmbedNamespace  lua;

LuaCEmbedResponse * print_lua_value(LuaCEmbed *l){
    if(lua.args.get_type(l,0) ==lua.types.NUMBER){
        printf("%lf\n", lua.args.get_double(l,0));
    }

    if(lua.args.get_type(l,0) == lua.types.STRING){
        printf("%s\n", lua.args.get_str(l,0));
    }

    return NULL;

}
LuaCEmbedResponse * soma(LuaCEmbed *l){

    int arg1 = (int)lua.args.get_long(l,0);
    int arg2 = (int)lua.args.get_long(l,1);
    return lua.response.send_long(arg1 + arg2);
}

// Função para carregar a biblioteca
int luaopen_minha_biblioteca(lua_State *L) {
    lua =  newLuaCEmbedNamespace();
    LuaCEmbed * l = lua.newLuaCEmbed();
    lua_close(l->state);
    l->state  = L;
    lua.add_callback(l, "puts", print_lua_value);
    lua.add_callback(l, "soma", soma);
    return 1;

}
