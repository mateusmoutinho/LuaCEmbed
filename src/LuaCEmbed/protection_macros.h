#define PRIVATE_LUA_CEMBED_PROTECT_VOID \
    if(!self){                          \
    return;                              \
    }


#define PRIVATE_LUA_CEMBED_PROTECT_NUM \
    if(!self){                          \
    return LUA_CEMBED_GENERIC_ERROR;      \
    }

#define PRIVATE_LUA_CEMBED_PROTECT_BOOL \
    if(!self){                          \
    return false;      \
    }

#define PRIVATE_LUA_CEMBED_PROTECT_NULL \
    if(!self){                          \
    return NULL;      \
    }
