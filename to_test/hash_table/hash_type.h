#ifndef HASH_TYPE
#define HASH_TYPE

#ifdef __cplusplus
extern "C"{
#endif

#ifndef KEY_
#define KEY_ unsigned int
#endif

#ifndef VALUE_TYPE_
#define VALUE_TYPE_ char
#endif

#ifndef VALUE_NUM_
#define VALUE_NUM_ 40
#endif

#define SUB_SIZE   65536  //2^16;
#define TABLE_SIZE 2048 //2^16 * 2048  , total 134217728 structs
#define OVERFLOW 1
#define ALLREADYHAVE 2
#define NOTFOUND 3

#define HASH(key) hash_mul( (key) )

struct store;
struct hash_table;
struct sub_table;

typedef struct{
	KEY_ key;
	VALUE_TYPE_ value[VALUE_NUM_] ;
} store;

typedef struct {
	store * head;
	unsigned int size;
} sub_table;

typedef struct {
	sub_table * head;
	unsigned int size;
} hash_table;


#ifdef __cplusplus
}
#endif
#endif