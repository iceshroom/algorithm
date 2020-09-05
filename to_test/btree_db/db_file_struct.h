#include<utility>
#include <cmath>
extern "C" {
#include <sys/types.h>
#include <unistd.h>
}

#ifndef DB_FILE_STRUCT_
#define DB_FILE_STRUCT_

//node中存储的结构，包括右孩子指针
template <typename key, typename value> 
struct store_struct { 
        std::pair<key , value> storehere; 
        off_t right_child; 
        };

struct cyc_list
{
        unsigned short head = 0;
        unsigned short nums = 0;
        off_t empty[1024] = {0};
};

//数据库文件头，存储一个empty数组用于标出文件空洞
template <typename key, typename value> 
struct db_head
{
        //b树t值
        unsigned long t_value; 
        //整个db文件中最大key值
        key max;
        //整个db文件中最小key值
        key min;
        //如果empty满了，在文件尾再分配一个dump[1024]，next_empty指向相应的位置
        off_t next_emptys; 
        //empty数组
        cyc_list emptys;

        db_head() : t_value(0), max(0), min(0), next_emptys(0){}
};


struct node_head
{
        off_t leftmost_child;       //最左边的左孩子
        unsigned short max;      //node中最大值的编号
};

#endif