#include "dhash_table.h"
#include "my_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#ifdef __cplusplus
extern "C"{
#endif

//hash function
static KEY_ hash_mul(const KEY_ * key);

//sub functions
static subtable * allocate_subtable();
static int add_subtable(hashtable * ht);
static int reduce_subtable(hashtable * ht);
static void remove_subtable(subtable * to_remove);
static store * ht_find(hashtable * ht, KEY_ to_search, subtable ** locate,  KEY_ * p);

inline static KEY_ hash_mul(const KEY_ * key) {
        KEY_ temp = (*key * A_MUL_B) & (MAX_MASK >> (sizeof(KEY_)*8 - MAX_KEY_BIT));
        return temp >> (MAX_KEY_BIT - MAX_LEVEL) ;
}

inline static subtable * allocate_subtable()
{
	size_t size = sizeof(subtable);
	subtable * to = (subtable *)Malloc(size);
	for(int a = 0; a < ARRAY_SIZE; a++)
		to->head[a].key = NOT_A_KEY;
	to->size = 0;
	to->overflow = NULL;
        to->front = NULL;
	return to;
}

//use free() to remove a subtable list
static void remove_subtable(subtable * to_remove)
{
	if( to_remove->front != NULL)
		to_remove->front = NULL;
	subtable * Onext = to_remove->overflow;
	subtable * dump;
        while (Onext != NULL)
        {
                dump = Onext;
                Onext = Onext->overflow;
                free(dump);
        }
        free(to_remove);
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
	subtable * Onext = reinsert;
	for(int i = 0;  1 ; ++i)
	{
		if ( i == Onext->size )
		{
			if(Onext->overflow != NULL) {
				Onext = Onext->overflow;
				i = -1;
				continue;
			}
			break;
		}
		ht_insert(ht, &(Onext->head[i]));
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
        store * tables = reinsert->head;
	subtable * Onext = reinsert;
	for(int i = 0;  1 ; ++i)
	{
		if ( i == Onext->size )
		{
			if(Onext->overflow != NULL){
				tables = Onext->overflow->head;
				Onext = Onext->overflow;
				i = -1;
				continue;
			}
			break;
		}
		ht_insert(ht, &(tables[i]));
	}
	remove_subtable(reinsert);

	if(ht->head[ht->size-1]->size <= 0)
		reduce_subtable(ht);

	return 0;
}

//init
void ht_init(hashtable * ht)
{
	ht->head = (subtable ** )Malloc(sizeof(subtable *) * BEGIN_SIZE * 2);
	ht->level = 0;
	ht->size = BEGIN_SIZE;
	ht->next = 0;
	for(int i = 0; i < BEGIN_SIZE; ++i)
		ht->head[i] = allocate_subtable();
}

//insert
int ht_insert(hashtable * ht, store * s) {
	KEY_ key = HASH(&(s->key));
	KEY_ count;
	int Oflag = 0;

	count = key & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level));
	if(count < ht->next)
		count = key & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level - 1));
	subtable * at = ht->head[count];

	while(1)
	{
		if(at->size == ARRAY_SIZE)
		{
			if(at->overflow != NULL)
				at = at->overflow;
			else
			{
				at->overflow = allocate_subtable();
				at->overflow->front = at;
			}
			Oflag = 1;
			continue;
		}
		memcpy(&(at->head[at->size]), s, sizeof(store));
		++(at->size);
		break;
	}

	if(Oflag == 1)
		add_subtable(ht);
	return 0;
}

//get(search)
store * ht_get(hashtable * ht, KEY_ to_search, store * returned ) 
{
        store * findout;
        if((findout = ht_find(ht, to_search,NULL, NULL) )!= NULL)
        {
                returned->key = to_search;
                memcpy(returned->value, findout->value, sizeof(VALUE_TYPE_)*VALUE_NUM_);
        }
        else
        {
                returned->key=NOT_A_KEY;
                memcpy(returned->value, "NOTFOUND\0", 10);
        }
        return returned;
}


store * ht_find(hashtable * ht, KEY_ to_search, subtable ** locate,  KEY_ * p) 
{
	KEY_ key = HASH(&to_search);
        KEY_ count;
	count = key & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level));
	if(count < ht->next)
		count = key & (MAX_MASK >> (sizeof(KEY_)*8 - 2 - ht->level - 1));
	subtable * a = ht->head[count];

	if(locate != NULL)
                *locate = NULL;
	if(p != NULL)
		*p = NOT_A_KEY;

	for(int i = 0;  1 ; i++)
	{
		if ( i >=8 )
		{
			if(a->overflow != NULL) {
				a = a->overflow;
				i = -1;
				continue;
			}
			else
				return NULL;
		}
		if (a->head[i].key == to_search)
		{
			if(locate != NULL)
                		*locate = a;
			if(p != NULL)
				*p = count;
                        return &(a->head[i]);
		}
		else if(a->head[i].key == NOT_A_KEY)
			return NULL;
	}
}

//delete(Some bug here , But I'm not find yet)
int ht_delete(hashtable * ht, KEY_ to_delete)
{
        subtable * locate = NULL;
	int is_overflow = 0;
        KEY_ p;
	store * findout = ht_find(ht, to_delete, &locate, &p);

	if (findout == NULL)
		return NOTFOUND;
	
	if(locate->size <= 1 )
	{
		if(locate->front != NULL)
		{
			locate->front->overflow = NULL;
			free(locate);
		}
		else
		{
			findout->key = -1;
			--(locate->size);
		}
		goto end;
	}

	subtable * Onext = locate;
	store * last_one = NULL;
	while(1)
	{
		if(Onext->overflow != NULL)
		{
			Onext = Onext->overflow;
			continue;
		}
		last_one = &(Onext->head[Onext->size - 1]);
		--(Onext->size);
		break;
	}
	memcpy(findout, last_one, sizeof(store));
	if(Onext->size <= 0)
	{
		Onext->front->overflow = NULL;
		free(Onext);
	}

end:
	if(ht->head[p]->size <= 0)
		reduce_subtable(ht);
	
	return 0;
}


//destory a hash table
void ht_destory(hashtable * ht) {
	for(int a = 0; a < ht->size; ++a) 
		free(ht->head[a]);
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
		now_size += Onext->size;
		while(Onext->overflow != NULL)
		{
			++overflows;
			Onext = Onext->overflow;
			now_size += Onext->size;
		}
		if(now_size > biggest)
			biggest = now_size;
		total_store += now_size;
		now_size = 0;
	}
	printf ("This table take over : %ld Byte size , or %ld KB, or %lf MB\n", size, size/1024, size/(1024.0*1024));
	printf("Total subtable's number is ht->size = %d , total store = %ld\n", ht->size, total_store);
	printf("ht.level = %d, ht.next = %d, ht.size = %d\n", ht->level, ht->next, ht->size);
	printf("Total %u overflow happened,  The biggest subtable (include overflow's size) size is %u\n\n", overflows, biggest);
}

int ht_changevalue(hashtable * ht, store * s)
{
	if(s->key < 0)
		return ERROR_;
	store * to_change = ht_find(ht, s->key, NULL, NULL);
	if(to_change == NULL)
		return NOTFOUND;
	memcpy(to_change, s, sizeof(store));
	return 0;
}


#ifdef __cplusplus
}
#endif