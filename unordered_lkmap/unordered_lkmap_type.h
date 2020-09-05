#ifndef UNORDERED_LKMAP_TYPE
#define UNORDERED_LKMAP_TYPE

#include <limits.h>
#include "lkmap.h"

#ifdef __cplusplus
extern "C"{
#endif

/*KEY's type*/
#define KEY_ long
#define MAX_KEY_BIT 32

#define BEGIN_SIZE	4  //Initial size of hash_table's subtable array
#define ARRAY_SIZE	64  // 4 stores in each array;
#define MAX_LEVEL 16L

#define HASH(key) hash_mul( (key) )

//m(key * A mod 1) 0 < A < 1
//A*B = [（√5-1）* B ]/2  , B = 2^32 , or UINT_MAX+1
#define A_MUL_B 2654435769L
#define MAX_MASK ULONG_MAX

typedef lkmap subtable;
typedef node store;
typedef struct hashtable_ hashtable;


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