#ifndef DHASH_TYPE
#define DHASH_TYPE

#include <limits.h>

#ifdef __cplusplus
extern "C"{
#endif

/*KEY's type*/
#ifndef KEY_
#define KEY_ long
#define MAX_KEY_BIT 32
#define NOT_A_KEY -1
#endif

/*VALUE array's type*/
#ifndef VALUE_TYPE_
#define VALUE_TYPE_ char
#endif

/*VALUE array's size*/
#ifndef VALUE_NUM_
#define VALUE_NUM_ 40
#endif

#define BEGIN_SIZE	4  //Initial size of hash_table's subtable array
#define ARRAY_SIZE	8  // 8 stores in each array;
#define MAX_LEVEL 16L

//return num's macro
#define ALLREADYHAVE 2
#define NOTFOUND 3
#define ERROR_ -1

#define HASH(key) hash_mul( (key) )

//m(key * A mod 1) 0 < A < 1
//A*B = [（√5-1）* B ]/2  , B = 2^32 , or UINT_MAX+1
#define A_MUL_B 2654435769L
#define MAX_MASK ULONG_MAX

#define subtable struct subtable_
#define store struct store_
#define hashtable struct hashtable_

struct store_{
	KEY_ key;
	VALUE_TYPE_ value[VALUE_NUM_] ;
} ;

struct subtable_  {
	store head[ARRAY_SIZE];
	int size;
	subtable * front;
	subtable * overflow;
};

struct hashtable_{
	subtable ** head;
	int next;
	int size;
	int level;
};


#ifdef __cplusplus
}
#endif
#endif