
#include "src/one.c"
LuaCEmbedNamespace  lua;

LuaCEmbedResponse * print_lua_value(LuaCEmbed *l){

    char *name = lua.args.table.get_arg_string(l,0,"{'name'}");
    printf("name: %s\n",name);
    long age = lua.args.table.get_arg_long(l,0,"{'age'}");
    printf("age: %ld\n",age);
    double height = lua.args.table.get_arg_double(l,0,"{'height'}");
    printf("height: %lf\n",height);
    bool maired = lua.args.table.get_arg_bool(l,0,"{'maried'}");
    printf("maried: %d\n",maired);

    long cars_size = lua.args.table.get_arg_size(l,0,"{'cars'}");
    printf("cars: \n");
    for(int i = 0; i < cars_size; i++){
        char *current_car = lua.args.table.get_arg_string(l,0,"{'cars',%d}",i);
        printf("%s\n",current_car);
    }


    return NULL;

}


LuaCEmbedResponse * soma(LuaCEmbed *l){

    int arg1 = (int)lua.args.get_long(l,0);
    int arg2 = (int)lua.args.get_long(l,1);

    return lua.response.send_long(arg1 + arg2);
}


// Função para carregar a biblioteca
int luaopen_lib(lua_State *L) {
    lua =  newLuaCEmbedNamespace();
    LuaCEmbed * l = lua.newLuaLib(L,false);

    lua.add_callback(l, "puts", print_lua_value);
    lua.add_callback(l, "soma", soma);
    lua.perform(l);
    return 1;

}

void teste(char *format,...){
    va_list  args;
    va_start(args,format);
    private_LuaCembed_format(format,args);
    va_end(args);
}
int main(){

    teste("%s%s1234","foda","foda");
    return 0;
    lua =  newLuaCEmbedNamespace();
    LuaCEmbed * l = lua.newLuaEvaluation();
    lua.add_callback(l,"puts",print_lua_value);
    lua.evaluete_file(l,"teste.lua");


    if(lua.has_errors(l)){
        printf("error: %s\n",lua.get_error_message(l));
    }
    lua.free(l);

    return 0;
}
//gcc -Wall -shared -fpic -o minha_biblioteca.so  main.c 