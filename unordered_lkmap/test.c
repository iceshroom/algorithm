#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "unordered_lkmap.h"


void insert(hashtable * ht, KEY_ key, char * s, int size) {
	switch (HT_INSERT(ht, key, s, size))
	{
        case SUCCESS:
                //printf("INSERT SUCCESS\n");
		break;
	case ALREADY_EXISTS:
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
	DATA_TYPE s[data_max_size];
	for(int i = 0; i< size; i++)
	{
		int ret = ht_get(ht, key[i], s, data_max_size);
		if(ret == FAILURE)
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
	hashtable * test = ht_init();
	KEY_ key = 0;
	char s[40];
	srandom(time(NULL));
	long int rand;

	int size = 400000;
	long tb[size];
	for(int i = 0; i < size; ++i)
	{
		tb[i] = random();
	}
	for(int i = 0; i < size; ++i)
	{
		key = tb[i];
		snprintf(s, 40, "Here is value of key %ld \n", tb[i]);
		insert(test, key, s, 40);
	}

	find(test, tb, size);

	unsigned long sizel = size*44;
	printf("we have stored : %ld Byte size , or %ld KB, or %lf MB\n", sizel, sizel/1024, sizel/(1024.0*1024));
	ht_takeinfo(test);

	for(int i = 0; i < size; ++i)
	{
		if(ht_delete(test, tb[i]) == FAILURE )
			printf("ht_delete NOTFOUND\n");
	}

	find(test, tb, size);
	
	ht_takeinfo(test);

	for(int i = 0; i < size; ++i)
	{
		rand = random();
		tb[i] = (rand > 0) ? rand : -rand;
	}
	for(int i = 0; i < size; ++i)
	{
		key = tb[i];
		snprintf(s, 40, "Here is value of key %ld \n", tb[i]);
		insert(test, key, s, 40);
	}

	find(test, tb, size);

	printf("we have stored : %ld Byte size , or %ld KB, or %lf MB\n", sizel, sizel/1024, sizel/(1024.0*1024));
	ht_takeinfo(test);

	ht_destory(test);

    return 0;
}
