
#include "src/one.c"
LuaCEmbedNamespace  lua;

LuaCEmbedNamespace  lua_n;

LuaCEmbedResponse *test(LuaCEmbed *args){
    LuaCEmbedTable *t= lua.args.get_table(args,0);
    lua.tables.append_long(t,20);
    lua.tables.append_string(t,"aaa");
    lua.tables.set_long_by_index(t,1,50);
    return NULL;
}
int luaopen_minha_biblioteca(lua_State *L) {
    lua =  newLuaCEmbedNamespace();
    LuaCEmbed * l = lua.newLuaLib(L, true);
    lua.add_callback(l, "test", test);

    return  lua.perform(l);
}



LuaCEmbedResponse  * add_func(LuaCEmbed *args){


    double num1 = lua_n.args.get_double_arg_clojure_evalation(args,0,"function(t) return t.num1  end ");
    double num2 = lua_n.args.get_double_arg_clojure_evalation(args,1,"function(t) return t.num2  end ");
    //LuaCEmbed_get_double_arg_clojure_evalation()
  //  private_LuaCembed_run_code_with_args()
    if(lua_n.has_errors(args)){
        char *error_message = lua_n.get_error_message(args);
        return lua_n.response.send_error(error_message);
    }

    return lua_n.response.send_double(num1+num2);
    return NULL;
}

int main(int argc, char *argv[]){

    lua_n =  newLuaCEmbedNamespace();
    LuaCEmbed * l = lua_n.newLuaEvaluation();
    lua_n.add_callback(l,"add",add_func);


    double result = lua_n.get_evaluation_double(l,"add({num1=10, num2=30})");
    if(lua_n.has_errors(l)){
        printf("error: %s\n",lua_n.get_error_message(l));
    }
    printf("resullt :%lf\n",result);

    lua_n.free(l);

    return 0;
}
//gcc -Wall -shared -fpic -o minha_biblioteca.so  main.c && lua teste.lua