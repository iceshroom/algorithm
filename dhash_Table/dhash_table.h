#ifndef DHASH_TABLE
#define DHASH_TABLE

#include "dhash_type.h"
#include "dhash_table.h"
#include "my_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#ifdef __cplusplus
extern "C"{
#endif

//hash table basic function
void ht_init(hashtable * ht);
int ht_insert(hashtable * ht, store * s);
store * ht_get(hashtable * ht, KEY_ to_search, store * returned );
int ht_delete(hashtable * ht, KEY_ to_delete);
void ht_destory(hashtable * ht);
void ht_takeinfo(hashtable * ht);
int ht_changevalue(hashtable * ht, store * s);



#ifdef __cplusplus
}
#endif

#endif
