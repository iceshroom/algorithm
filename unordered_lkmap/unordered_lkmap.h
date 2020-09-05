#ifndef UNORDERED_LKMAP
#define UNORDERED_LKMAP

#include "unordered_lkmap.h"
#include "unordered_lkmap_type.h"
#include "my_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#ifdef __cplusplus
extern "C"{
#endif

//hash table basic function
hashtable * ht_init();
subtable * ht_locate(hashtable * ht, KEY_ key);
int ht_insert(hashtable * ht, store * s);
int ht_get(hashtable * ht, KEY_ to_search, DATA_TYPE * dump, int dump_size);
int ht_delete(hashtable * ht, KEY_ to_delete);
void ht_destory(hashtable * ht);
void ht_takeinfo(hashtable * ht);

#define HT_INSERT(ht, key, data, datasize) ht_insert((ht), make_node((key), (data), (datasize)))



#ifdef __cplusplus
}
#endif

#endif
