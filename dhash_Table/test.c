#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "dhash_table.h"


void insert(hashtable * ht, store * to) {
	switch (ht_insert(ht, to))
	{
        case 0:
                //printf("INSERT SUCCESS\n");
		break;
	case ALLREADYHAVE:
		printf("ALL READY HAVE\n");
		break;

	default:
		break;
	}
}

void find(hashtable * ht, KEY_ * key, int size)
{
	int found = 0;
	int notfound = 0;
	store  find; 
	for(int i = 0; i< size; i++)
	{
		ht_get(ht, key[i], &find);
		if(find.key < 0L)
		{
			++notfound;
		}
		else
		{
			//printf("found key %ld, value = %s", key[i], find.value);
			++found;
		}
	}
	
	printf("found = %d, notfound = %d\n", found , notfound);
	return;
}

int main()
{
	hashtable test;
	ht_init(&test);
	store to_insert;
	srandom(time(NULL));
	long int rand;

	long tb[400000];
	for(int i = 0; i < 400000; ++i)
	{
		rand = random();
		tb[i] = (rand > 0) ? rand : -rand;
	}
	for(int i = 0; i < 400000; ++i)
	{
		to_insert.key = tb[i];
		snprintf(to_insert.value, 40, "Here is value of key %ld \n", tb[i]);
		insert(&test, &to_insert);
	}

	find(&test, tb, 400000);

	unsigned long size = 400000*44;
	printf("we have stored : %ld Byte size , or %ld KB, or %lf MB\n", size, size/1024, size/(1024.0*1024));
	ht_takeinfo(&test);

	for(int i = 0; i < 400000; ++i)
	{
		if(ht_delete(&test, tb[i]) == NOTFOUND )
			printf("NOTFOUND\n");
	}

	find(&test, tb, 400000);
	
	ht_takeinfo(&test);

	for(int i = 0; i < 400000; ++i)
	{
		rand = random();
		tb[i] = (rand > 0) ? rand : -rand;
	}
	for(int i = 0; i < 400000; ++i)
	{
		to_insert.key = tb[i];
		snprintf(to_insert.value, 40, "Here is value of key %ld \n", tb[i]);
		insert(&test, &to_insert);
	}

	find(&test, tb, 400000);

	printf("we have stored : %ld Byte size , or %ld KB, or %lf MB\n", size, size/1024, size/(1024.0*1024));
	ht_takeinfo(&test);

	ht_destory(&test);

        return 0;
}