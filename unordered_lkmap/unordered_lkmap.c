#include "unordered_lkmap.h"
#include "my_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#ifdef __cplusplus
extern "C"{
#endif

//hash function
static KEY_ hash_mul(const KEY_ key);

//sub functions
static subtable * allocate_subtable();
static int add_subtable(hashtable * ht);
static int reduce_subtable(hashtable * ht);
static void remove_subtable(subtable * to_remove);

inline static KEY_ hash_mul(const KEY_ key) {
        KEY_ temp = (key * A_MUL_B) & (MAX_MASK >> (sizeof(KEY_)*8 - MAX_KEY_BIT));
        return temp >> (MAX_KEY_BIT - MAX_LEVEL) ;
}

inline static subtable * allocate_subtable()
{
	return (subtable *)map_init();
}

//use free() to remove a subtable list
static void remove_subtable(subtable * to_remove)
{
	map_destroy(to_remove);
}

//use this function when find a overflow;
static int add_subtable(hashtable * ht)
{
	subtable * reinsert = ht->head[ht->next];
	ht->head[ht->next] = allocate_subtable();

	++(ht->size);
	if(ht->size >= power2(BEGIN_SIZE, ht->level + 1))
	{
		subtable **  newer = (subtable **)Malloc(sizeof(subtable *) * (ht->size) * 2);
		subtable ** older = ht->head;
		memcpy(newer, older, sizeof(subtable *) * (ht->size - 1));
		free(older);
		ht->head=newer;
		if (ht->level <= MAX_LEVEL)
			++(ht->level);
        ht->next=0;
	}
	else
		++(ht->next);

	ht->head[(ht->size) - 1] = allocate_subtable();
	node * dump;
	while(reinsert->size > 0)
	{
		dump = map_cutout(reinsert, reinsert->root->key);
		ht_insert(ht, dump);
	}
	remove_subtable(reinsert);
	return 0;
}

static int reduce_subtable(hashtable * ht)
{
	//if level == 0 , do nothing
	if(ht->level == 0)
		return 0;
	/*if the subtable behind next can't take so much element, do nothing*/
	int next_subtable = ht->next - 1;
	if ( next_subtable < 0 )
		next_subtable = power2(BEGIN_SIZE, ht->level ) / 2 - 1;
	if((ht->head[(ht->size)-1]->size + ht->head[next_subtable]->size) >= ARRAY_SIZE)	
		return 0;
	
	subtable * reinsert = ht->head[ht->size - 1];
	ht->head[ht->size-1] = NULL;
    --(ht->size);
	if(ht->size < power2(BEGIN_SIZE, ht->level))
	{
		if(ht->size < power2(BEGIN_SIZE, ht->level-1))
		{
        	subtable ** older = ht->head;
			subtable **  newer = (subtable **)Malloc(sizeof(subtable *) * power2(BEGIN_SIZE, ht->level));
			memcpy(newer, older, sizeof(subtable *) * (ht->size));
			free(older);
			ht->head=newer;
		}
	    --(ht->level);
	}

	ht->next = next_subtable;
	node * dump;
	while(reinsert->size != 0)
	{
		dump = map_cutout(reinsert, reinsert->root->key);
		ht_insert(ht, dump);
	}
	remove_subtable(reinsert);

	if(ht->head[ht->size-1]->size <= 0)
		reduce_subtable(ht);

	return 0;
}

//init
hashtable * ht_init()
{
	hashtable * ht = Malloc(sizeof(hashtable));
	ht->head = (subtable ** )Malloc(sizeof(subtable *) * BEGIN_SIZE * 2);
	ht->level = 0;
	ht->size = BEGIN_SIZE;
	ht->next = 0;
	for(int i = 0; i < BEGIN_SIZE; ++i)
		ht->head[i] = allocate_subtable();
	return ht;
}

subtable * ht_locate(hashtable * ht, KEY_ key) {
	KEY_ locate = HASH((key));
	KEY_ count = locate & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level));
	if(count < ht->next)
		count = locate & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level - 1));
	return ht->head[count];
}

//insert
int ht_insert(hashtable * ht, store * s) {
	KEY_ key = HASH((s->key));
	KEY_ count;
	count = key & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level));
	if(count < ht->next)
		count = key & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level - 1));
	subtable * at = ht->head[count];
	int ret = map_insert(at, s);
	if(ret == SUCCESS && at->size >= ARRAY_SIZE)
		add_subtable(ht);
	return ret;
}



//get(search)
int ht_get(hashtable * ht, KEY_ to_search, DATA_TYPE * dump, int dump_size) 
{
	KEY_ key = HASH(to_search);
    KEY_ count;
	count = key & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level));
	if(count < ht->next)
		count = key & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level - 1));
	subtable * a = ht->head[count];
    return map_getdata(a, to_search, dump, dump_size);
}

//delete
int ht_delete(hashtable * ht, KEY_ to_delete)
{
	KEY_ key = HASH(to_delete);
    KEY_ count;
	count = key & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level));
	if(count < ht->next)
		count = key & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level - 1));
	subtable * locate = ht->head[count];
	int ret = map_delete(locate, to_delete);
	if(ret == FAILURE)
		return ret;
	if(locate->size <= 0)
		reduce_subtable(ht);
	return ret;
}


//destory a hash table
void ht_destory(hashtable * ht) {
	for(int a = 0; a < ht->size; ++a) 
		map_destroy(ht->head[a]);
	free(ht->head);
}

void ht_takeinfo(hashtable * ht) {
	unsigned long size;
	long total_store = 0;
	unsigned int overflows = 0;
	unsigned int biggest = 0;
	unsigned int now_size = 0;

	size = sizeof(subtable) * ht->size;
	for(int i = 0; i<ht->size; ++i)
	{
		subtable * Onext = ht->head[i];
		now_size = map_getsize(Onext);
		if(now_size > ARRAY_SIZE)
			++overflows;
		biggest = biggest > now_size ? biggest : now_size;
		total_store += now_size;
		now_size = 0;
	}
	printf ("This table take over : %ld Byte size , or %ld KB, or %lf MB\n", size, size/1024, size/(1024.0*1024));
	printf("Total subtable's number is ht->size = %d , total store = %ld\n", ht->size, total_store);
	printf("ht.level = %d, ht.next = %d, ht.size = %d\n", ht->level, ht->next, ht->size);
	printf("Total %u overflow happened,  The biggest subtable (include overflow's size) size is %u\n\n", overflows, biggest);
}


#ifdef __cplusplus
}
#endif