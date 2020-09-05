#include<stdio.h>
#include "lkmap.h"
#include "my_lib.h"
#include <time.h>
int main() {
	lkmap * test1 = map_init();
	lkmap * test2 = map_init();
	char s[40];
	int size = 10;
	LKMAP_KEY a[10] = {292,464,190,152,91,618,197,14,310,394};
	
	LKMAP_KEY key;
	srandom(time(NULL));
	/*for(int i = 0; i < size; ++i)
		a[i] = random()%1000;*/
	int suc = 0;
	for(int i = 0; i < size/2; ++i) {
		snprintf(s, 40,  "HERE IS DATA OF KEY: %ld\n", a[i]);
		node * dump = make_node(a[i], s, 40);
		if(map_insert(test1, dump)==SUCCESS) ++suc;
	}
	for(int i = size/2; i < size; ++i) {
		snprintf(s, 40,  "HERE IS DATA OF KEY: %ld\n", a[i]);
		node * dump = make_node(a[i], s, 40);
		if(map_insert(test2, dump)==SUCCESS) ++suc;
	}
	printf("INSERT SUCCESS: %d\n", suc);
	printf("TEST1 DEBUG NODE NUM: %d\n", map_debug_countnode(test1));
	printf("TEST2 DEBUG NODE NUM: %d\n", map_debug_countnode(test2));

	printf("\n");
	suc = 0;
	for(int i = 0; i < size/2; ++i) {
		if(map_insert(test2, map_cutout(test1, a[i]))==SUCCESS) ++suc;
	}
	printf("INSERT SUCCESS: %d\n", suc);
	printf("TEST1 DEBUG NODE NUM: %d\n", map_debug_countnode(test1));
	printf("TEST2 DEBUG NODE NUM: %d\n", map_debug_countnode(test2));

	printf("\n______________________________________________________\n");
	suc = 0;
	for(int i = 0; i < size/2; ++i)
	{
		if(map_delete(test1, a[i]) != FAILURE)
			;//printf("DELETE KEY %ld SUCCESS!\n",a[i]);
		else
			suc++;
		if (map_getdata(test1, a[i], s, 40))
			printf("%s",s);
	}
	printf("TEST1 DELETE FAILURE: %d\n", suc);
	printf("TEST1 DEBUG NODE NUM: %d\n", map_debug_countnode(test1));

	suc = 0;
	for(int i = 0; i < size; ++i)
	{
		if(map_delete(test2, a[i]) != FAILURE)
			;//printf("DELETE KEY %ld SUCCESS!\n",a[i]);
		else
			suc++;
		if (map_getdata(test2, a[i], s, 40))
			printf("%s",s);
	}
	printf("TEST2 DELETE FAILURE: %d\n", suc);
	printf("TEST2 DEBUG NODE NUM: %d\n", map_debug_countnode(test2));


	map_destroy(test1);
	map_destroy(test2);
	return 0;
}
