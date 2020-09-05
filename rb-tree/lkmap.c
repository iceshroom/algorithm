#ifdef __cplusplus
extern "C" {
#endif

#include "lkmap.h"
#include "my_lib.h"

static inline void left_rotate(lkmap * map, node * x);
static inline void right_rotate(lkmap * map, node * x);
static inline void insert_fixup(lkmap * map, node * z);
static inline void delete_fixup(lkmap * map, node * x);
static inline node * findout(const lkmap * map, LKMAP_KEY key);
static inline void transplant(lkmap * map, node * u, node * v);
static inline node * tree_minimum(lkmap * map, node * x);

static inline void left_rotate(lkmap * map, node * x) {
	node * y = x->right;
	x->right = y->left;
	if(y->left != map->NIL)
		y->left->parent = x;
	y->parent = x->parent;
	if(x->parent == map->NIL)
		map->root = y;
	else if(x == x->parent->left)
		x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
}

static inline void right_rotate(lkmap * map, node * x) {
	node * y = x->left;
	x->left = y->right;
	if(y->right != map->NIL)
		y->right->parent = x;
	y->parent = x->parent;
	if(x->parent == map->NIL)
		map->root = y;
	else if(x == x->parent->left)
		x->parent->left = y;
	else x->parent->right = y;
	y->right = x;
	x->parent = y;
}

static inline void insert_fixup(lkmap * map, node * z) {
	node * y;
	while(z->parent->color == 'r') {
		if(z->parent == z->parent->parent->left) {
			y = z->parent->parent->right;
			if(y->color == 'r') {
				z->parent->color = 'b';
				y->color = 'b';
				z->parent->parent->color = 'r';
				z = z->parent->parent;
				continue;
			} else if (z == z->parent->right) {
				z = z->parent;
				left_rotate(map,z);
			}
			z->parent->color = 'b';
			z->parent->parent->color = 'r';
			right_rotate(map,z->parent->parent);
		} else {
			y = z->parent->parent->left;
			if(y->color == 'r') {
				z->parent->color = 'b';
				y->color = 'b';
				z->parent->parent->color = 'r';
				z = z->parent->parent;
				continue;
			} else if (z == z->parent->left) {
				z = z->parent;
				right_rotate(map,z);
			}
			z->parent->color = 'b';
			if(z->parent->parent == map->NIL) return;
			z->parent->parent->color = 'r';
			left_rotate(map,z->parent->parent);
		}
	}
	map->NIL->color = 'b';
	map->root->color = 'b';
}

static inline void delete_fixup(lkmap * map, node * x) {
	node * w;
	while(x != map->root && x->color == 'b'){
		if(x == x->parent->left) {
			w = x->parent->right;
			if(w->color == 'r'){
				w->color = 'b';
				x->parent->color = 'r';
				left_rotate(map,x->parent);
				w = x->parent->right;
			}
			if(w->left->color == 'b' && w->right->color == 'b') {
				w->color = 'r';
				x = x->parent;
				continue;
			} else if (w->right->color == 'b') {
				w->left->color = 'b';
				w->color = 'r';
				right_rotate(map,w);
				w = x->parent->right;
			}
			w->color = x->parent->color;
			x->parent->color = 'b';
			w->right->color = 'b';
			left_rotate(map, x->parent);
			x = map->root;
		} else {
			w = x->parent->left;
			if(w->color == 'r'){
				w->color = 'b';
				x->parent->color = 'r';
				right_rotate(map,x->parent);
				w = x->parent->left;
			}
			if(w->right->color == 'b' && w->left->color == 'b') {
				w->color = 'r';
				x = x->parent;
				continue;
			} else if (w->left->color == 'b') {
				w->right->color = 'b';
				w->color = 'r';
				left_rotate(map,w);
				w = x->parent->left;
			}
			w->color = x->parent->color;
			x->parent->color = 'b';
			w->left->color = 'b';
			right_rotate(map, x->parent);
			x = map->root;
		}
	}
	x->color = 'b';
}

static inline node * findout(const lkmap * map, LKMAP_KEY key) {
	node * x = map->root;
	while(x != map->NIL) {
		if(key < x->key)
			x = x->left;
		else if(key > x->key)
			x = x->right;
		else
			break;
	}
	return x;
}

static inline void transplant(lkmap * map, node * u, node * v) {
	if(u->parent == map->NIL)
		map->root = v;
	else if(u == u->parent->left)
		u->parent->left = v;
	else u->parent->right = v;
	v->parent = u->parent;
}

static inline node * tree_minimum(lkmap * map, node * x) {
	while(x->left != map->NIL)
		x = x->left;
	return x;
}


inline size_t map_getsize(const lkmap * map) {
	return map->size;
}

node * make_node(LKMAP_KEY key, const void * const data, size_t datasize) {
	if(datasize > data_max_size)
		return NULL;
	node * newnode = (node *)Malloc(sizeof(node));
	newnode->data = (char *)Malloc(datasize);
	newnode->data_size = datasize;
	memcpy(newnode->data, data, datasize);
	newnode->color = 'r';
	newnode->key = key;
	return newnode;
}

int free_node(lkmap * map, node * to_free) {
	if(to_free != map->NIL) {
		free(to_free->data);
		free(to_free);
		to_free = map->NIL;
		return SUCCESS;
	} else 
		return FAILURE;
}


lkmap * map_init() {
	lkmap * ret = (lkmap *)Malloc(sizeof(lkmap));
	ret->NIL = Malloc(sizeof(node));
    ret->NIL->color = 'b',
	ret->NIL->key = -1;
	ret->NIL->data = NULL;
    ret->NIL->parent = ret->NIL;
    ret->NIL->left = ret->NIL;
    ret->NIL->right = ret->NIL,
	ret->root = ret->NIL;
	ret->size = 0;
	return ret;
}

int map_insert(lkmap * map, node * to_insert) {
	if(to_insert->key == NOT_A_KEY)
		return FAILURE;
	to_insert->left = map->NIL;
	to_insert->right = map->NIL;
	to_insert->parent = map->NIL;
	to_insert->color = 'r';
	if(findout(map, to_insert->key) != map->NIL)
		return ALREADY_EXISTS;
	node * y = map->NIL;
	node * x = map->root;
	while(x != map->NIL) {
		y = x;
		if(to_insert->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	to_insert->parent = y;
	if(y == map->NIL)
		map->root = to_insert;
	else if(to_insert->key < y->key)
		y->left = to_insert;
	else y->right = to_insert;
	insert_fixup(map,to_insert);
	++map->size;
	return SUCCESS;
}

int map_delete(lkmap * map, LKMAP_KEY key) {
	node * tofree = map_cutout(map, key);
	if(tofree == map->NIL)
		return FAILURE;
	return free_node(map, tofree);
}

node * map_cutout(lkmap * map, LKMAP_KEY key) {
	node * z = findout(map, key);
	if(z == map->NIL)
		return map->NIL;
	node * y = z;
	node * x;
	char y_color = y->color;
	if(z->left == map->NIL) {
		x = z->right;
		transplant(map,z,z->right);
	} else if (z->right == map->NIL){
		x = z->left;
		transplant(map,z,z->left);
	} else {
		y = tree_minimum(map, z->right);
		y_color = y->color;
		x= y->right;
		if(y->parent != z){
			transplant(map,y,y->right);
			y->right = z->right;
			y->right->parent = y;
		} else 
			x->parent = y; //when x == map->NIL
		transplant(map, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	if(y_color == 'b')
		delete_fixup(map, x);
	--map->size;
	return z;
}

int map_equalrange(const lkmap * map, LKMAP_KEY key, node list[] ,size_t listsize) {
	const node * Find = findout(map, key);
	if(Find == map->NIL)
		return FAILURE;
	list[0].key = Find->key;
	list[0].left = map->NIL;
	list[0].right = map->NIL;
	list[0].parent = map->NIL;
	list[0].data_size = (Find->data_size > list[0].data_size ? list[0].data_size : Find->data_size);
	memcpy(list[0].data, Find->data, list[0].data_size);
	return SUCCESS;
}

int map_getdata(const lkmap * map, LKMAP_KEY key_to_find, void * ret, size_t retsize) {
	const node * Find = findout(map, key_to_find);
	if(Find == map->NIL)
		return FAILURE;
	memcpy(ret, Find->data, (Find->data_size > retsize ? retsize : Find->data_size));
	return SUCCESS;
}

int map_count(lkmap * map,LKMAP_KEY find) {
	const node * Find = findout(map, find);
	if(Find == map->NIL)
		return FAILURE;
	else
		return SUCCESS;
}

int _count(lkmap * map, node * here) {
	if(here != map->NIL) {
		return _count(map, here->right) + _count(map, here->left) + 1;
	}
	return 0;
}

int map_debug_countnode(lkmap * map) {
	return _count(map, map->root);
}

void _destroy(lkmap * map, node * here) {
	if(here != map->NIL) {
		_destroy(map, here->left);
		_destroy(map, here->right);
		free(here->data);
		free(here);
	}
}

void map_destroy(lkmap * map) {
	_destroy(map, map->root);
	free(map->NIL);
	free(map);
}

#ifdef __cplusplus
}
#endif