


LuaCEmbedTable * newLuaCembedTable(LuaCEmbed *main_embed, const char *format, ...);


void LuaCEmbedTable_set_method(LuaCEmbedTable *self, const char *name, LuaCEmbedResponse *(*callback)(LuaCEmbedTable  *self, LuaCEmbed *args));

void  LuaCEmbedTable_set_string_prop(LuaCEmbedTable *self, const char *name, const char *value);

void  LuaCEmbedTable_set_long_prop(LuaCEmbedTable *self, const char *name, long  value);

void  LuaCEmbedTable_set_double_prop(LuaCEmbedTable *self, const char *name, double  value);

void  LuaCEmbedTable_set_bool_prop(LuaCEmbedTable *self, const char *name, bool value);
