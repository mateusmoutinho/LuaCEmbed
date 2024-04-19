
#include "src/one.c"
LuaCEmbedNamespace  lua_n;



LuaCEmbedResponse  * hello(LuaCEmbed *args){
    printf("hello world");
    return NULL;
}


int main(){


    lua_n =  newLuaCEmbedNamespace();
    LuaCEmbed * l = lua_n.newLuaEvaluation();
    lua_n.add_callback(l,"hello",hello);

    lua_n.evaluete_file(l, "hello()");

    if(lua_n.has_errors(l)){
        printf("error: %s\n",lua_n.get_error_message(l));
    }
    lua_n.free(l);

    return 0;
}
//gcc -Wall -shared -fpic -o minha_biblioteca.so  main.c 