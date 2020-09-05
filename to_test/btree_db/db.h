#ifndef BT_DB_
#define BT_DB_

#include "db_file_struct.h"
#include "my_unistd.h"
extern "C"{
        #include<stdio.h>
        #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
}


template <typename key, typename value>
class btree_db
{
        private:

                key max;
                key min;
                value * node_dump[3] = {NULL, NULL, NULL};
                int db_file;
                db_head<key,value> head_dump;

                btree_db();
                void load_node()
                {
                        
                }
                
        public:
                bool load_db_head()
                {
                        if ( Read(db_file, &head_dump, sizeof(head_dump)) != sizeof(head_dump) )
                        {
                                printf("The file: %s is not a database!\n", filen);
                                return false;
                        }
                        return true;
                }


                btree_db(const char * filen, unsigned long t) : max(0), min(0) 
                {
                        if ( t < 2 ) {
                                printf("B tree's T value should >= 2 \n"); 
                                return;
                        }
                        head_dump.t_value = t;
                        Open(db_file, filen, O_CREAT | O_EXCL | O_RDWR );
                        Write(db_file, &head_dump, sizeof(head_dump));
                        for (int i = 0; i < 3; i++)
                                node_dump[i] = new value[2*head_dump.t_value];
                }

                btree_db(const char * filen) : max(0), min(0) 
                {
                        Open(db_file, filen, O_RDWR);
                        if ( load_db_head() )
                        {
                                for (int i = 0; i < 3; i++)
                                        node_dump[i] = new value[2*head_dump.t_value];
                        }
                }


                ~btree_db()
                {
                        for (int i = 0; i < 3; i++)
                        {
                                if( node_dump[i] != NULL)
                                        delete [] node_dump[i];
                        }
                }

                bool checkhead() const
                {
                        return ( head_dump.t_value >= 2  );
                }

        //在数据库文件中分配一个新的node
        off_t db_new_node(value * dump)
        {
                int i;
                off_t return_;

                while(head_dump.emptys.nums > 0)
                {
                        i = head_dump.emptys.head;
                        return_ = lseek(db_file, head_dump.emptys.empty[i], SEEK_SET);
                        head_dump.emptys.nums--;
                        head_dump.emptys.head = (++i)%1024;
                        return return_;
                }

                lseek(db_file, 0 , SEEK_END);
                if(dump == NULL)
                {
                        
                }
        }

};


#endif