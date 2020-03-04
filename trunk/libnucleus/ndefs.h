
#ifndef __NDEFS_H__
#define __NDEFS_H__


#define __def_adr(adr, ret, ...) \
        return ((ret (*)())((adr)))(__VA_ARGS__); \

#define __defn_adr(adr, ...) \
        ((void (*)())( adr ))(__VA_ARGS__); \



#endif
