#define PRIVATE_LUA_CEMBED_EVALUATION_NAME "private_lua_c_embed_evaluation"
#define PRIVATE_LUA_CEMBED_FUNCTION_ARGS_EVALUATION_CODE "function %s(f)\n return f%s \n end\n"
#define  PRIVATE_LUA_CEMBED_FUNCTION_TABLE_RESPONSE_EVALUATION_CODE "function %s()\n return %s \n end"
#define  PRIVATE_LUA_CEMBED_GLOBAL_EVALUATION_CODE "%s = %s\n"
#define PRIVATE_LUA_CEMBED_MAIN_LIB_TABLE_NAME "private_lua_c_embed_main_lib_table"
#define PRIVATE_LUA_CEMBED_DEL_PREFIX "__gc"
#define PRIVATE_LUA_CEMBED_META_TABLE "private_lua_c_embed_main_meta_table"

#define PRIVATE_LUA_CEMBED_SUB_TABLE_FORMAT "private_lua_c_embed%s_%s"