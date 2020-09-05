#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "hash_table.h"

void insert(hash_table * ht, store * to) {
	switch (ht_insert(ht, to))
	{
	case ALLREADYHAVE:
		printf("ALLREADYHAVE\n");
		break;
	case OVERFLOW:
		printf("OVERFLOW\n");
		break;

	default:
		break;
	}
}

void find(hash_table * ht, KEY_ key)
{
	const store * find = ht_find(ht, key);
	int i = 0;
	store re;
	re.key = key;
	snprintf(re.value, 40, "reinsert one key = %d \n", re.key);

	while(find == NULL && i < 3)
	{
		printf("NOT FOUND key = %u , try reinsert %d\n", key, i);
		insert(ht, &re);
		find = ht_find(ht, key);
		++i;
	}
	
	//printf(find->value);
	return;
}

int main()
{
	hash_table test;
	ht_init(&test);
	store to_insert;

	unsigned int tb[2000];
	for(int i = 0; i < 2000; ++i)
		tb[i] = i+100;
	for(int i = 0; i < 2000; ++i)
	{
		to_insert.key = tb[i];
		snprintf(to_insert.value, 40, "Here is value of key %d \n", tb[i]);
		insert(&test, &to_insert);
	}
	for(int i = 0; i < 2000; ++i)
		find(&test, tb[i]);

	unsigned long size = 2000*44;
	printf("we have stored : %ld Byte size , or %ld KB, or %lf MB\n ", size, size/1024, size/(1024.0*1024));
	ht_takesize(&test);

        return 0;
}