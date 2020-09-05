#ifndef __LKMAP
#define __LKMAP

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LKMAP_SET
#define LKMAP_KEY long
#define DATA_TYPE char
static unsigned long long data_max_size = 1024*1024*4; //4MB

enum {
	FAILURE = 0,
	SUCCESS = 1,
	NOT_A_KEY = -1,
    ALREADY_EXISTS = -2
};

typedef struct node_ node;
typedef struct lkmap_ lkmap;

struct node_{
    char color; //'r' or 'b'
    LKMAP_KEY key;
    DATA_TYPE * data;
	size_t data_size;
    node * parent;
    node * left;
    node * right;
};

struct lkmap_{
	node * root;
    node * NIL;
	int size;
};
#endif

lkmap * map_init();
size_t map_getsize(const lkmap * map);
node * make_node(LKMAP_KEY key, const void * const data, size_t datasize);
int free_node(lkmap * map, node * to_free);
int map_insert(lkmap * map, node * to_insert);
int map_delete(lkmap * map, LKMAP_KEY key);
node * map_cutout(lkmap * map, LKMAP_KEY key);
int map_equalrange(const lkmap * map, LKMAP_KEY key, node list[] ,size_t listsize);
int map_getdata(const lkmap * map, LKMAP_KEY key_to_find, void * ret, size_t retsize);
int map_count(lkmap * map,LKMAP_KEY find);
void map_destroy(lkmap * map);

int map_debug_countnode(lkmap * map);

#define INSERT(map, key, data, datasize) map_insert((map), make_node((key), (data), (datasize)))

#ifdef __cplusplus
}
#endif

#endif