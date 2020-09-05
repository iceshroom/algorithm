#ifndef HASH_DB
#define HASH_DB

#include "hash_type.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

//hash table basic function
void ht_init(hash_table * ht);
int ht_insert(hash_table * ht, store * s);
store * ht_find(hash_table * ht, KEY_ to_search);
int ht_delete(hash_table * ht, KEY_ to_delete);
void ht_destory(hash_table * ht);
void ht_takesize(hash_table * ht);


//count hash function
KEY_ hash_mul(const KEY_ * key);
KEY_ sub_hash(const KEY_ *key, unsigned int i);

KEY_ hash_mul(const KEY_ * key) {
        KEY_ temp = (*key * 2654435769);
        return temp >> 21 ;
}

KEY_ sub_hash(const KEY_ *key, unsigned int i) {
	KEY_ h1 = (*key * 2654435769);
	h1 = h1 >> 16 ;

	KEY_ h2 = *key%(SUB_SIZE-1);
	if (h2%2 != 0)
		++h2;
	
	return (h1+i*h2)%SUB_SIZE;
}

/*KEY_ hash_mod(hash_table * ht, KEY_ * key)
{
        return *key%701;
}*/	//not used


void ht_init(hash_table * ht) {
	ht->head = (sub_table *)(malloc(sizeof(sub_table)));
	memset(ht->head, 0, sizeof(sub_table));
	ht->size = 1;
}


//insert
int ht_insert(hash_table * ht, store * s) {
	unsigned int table_size = ht->size;

	KEY_ key = HASH(&(s->key));
	KEY_ where_we_start = key;

start:
	if (key > table_size){
		unsigned int exband = 1;
		while (key > (table_size) * exband)
			exband*=2;
		sub_table * older = ht->head;
		sub_table * newer = (sub_table *)calloc(table_size*exband, sizeof(sub_table));
		memset(&(newer[table_size]), 0, sizeof(sub_table)*table_size*(exband - 1) );
		memcpy(newer, older, table_size*sizeof(sub_table));
		ht->head = newer;
		table_size = (ht->size*=exband);
		free(older);
	}


	KEY_ sub_key;
	if(ht->head[key].size == 0){
			ht->head[key].size = 1;
			ht->head[key].head = (store *)malloc(sizeof(store));
			memset(ht->head[key].head, 0 , sizeof(store));
		}
	store * to_find = ht->head[key].head;

	unsigned int sub_size = ht->head[key].size;
	for(int i = 0; i < SUB_SIZE; ++i) {
		sub_key = sub_hash(&(s->key), i);
		unsigned int exband = 1;

		while (sub_key > sub_size * exband)
			exband*=2;
		if( exband > 1) {
			store * older = to_find;
			store * newer = (store *)calloc(sub_size * exband, sizeof(store));
			memset(&newer[sub_size], 0, sizeof(store)*sub_size*(exband-1));
			memcpy(newer, older, sub_size*sizeof(store));
			ht->head[key].size *= exband;
			sub_size = ht->head[key].size;
			ht->head[key].head = newer;
			to_find = ht->head[key].head;
		}
		if( to_find[sub_key].key == s->key)
			return ALLREADYHAVE;
		else if (to_find[sub_key].key == 0) {
			to_find[sub_key].key = s->key;
			memcpy(to_find[sub_key].value, s->value, sizeof(s->value));
			return 0;
		}
	}

	key = (key+1) % TABLE_SIZE;
	if(key == where_we_start)
			return OVERFLOW;
	goto start;
}

//find
store * ht_find(hash_table * ht, KEY_ to_search) {
	KEY_ key = HASH(&to_search);
	KEY_ where_we_start = key;
	sub_table * to_find;
	KEY_ sub_key;

start:
	to_find = &(ht->head[key]);
	for(int i = 0; i < SUB_SIZE; ++i) {
		sub_key = sub_hash(&to_search, i);
		if(sub_key > to_find->size)
			return NULL;
		if( (to_find->head)[sub_key].key == to_search)
			return  &(to_find->head)[sub_key];
		else if ((to_find->head)[sub_key].key == 0) {
			return NULL;
		}
	}
	key = (key+1) % ht->size;
	if(key == where_we_start)
		return NULL;
	goto start;
}

//delete
int ht_delete(hash_table * ht, KEY_ to_delete) {
	store * findout = ht_find(ht, to_delete);
	if (findout == NULL)
		return NOTFOUND;
	findout->key = 0;
	memset(findout->value, 0, sizeof(findout->value));
	return 0;
}

void ht_destory(hash_table * ht) {
	for(int a = 0; a < ht->size; ++a) 
		free(ht->head[a].head);
	free(ht->head);
}

void ht_takesize(hash_table * ht) {
	unsigned long size;
	size = sizeof(sub_table) * ht->size;
	for (int i = 0; i < ht->size; ++i) {
		size += ht->head[i].size * sizeof(store);
	}
	printf ("This table take over : %ld Byte size , or %ld KB, or %ld MB\n ", size, size/1024, size/(1024*1024));
}

#ifdef __cplusplus
}
#endif

#endif
