#ifdef __cplusplus
extern "C"{
#endif

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "my_lib.h"

//size_t Read_db();

int Open(int * fd, const char *path)
{
	int oflag;
	mode_t mode;

	if(!access(path, F_OK) && path != NULL)
	{
		oflag = O_RDWR | O_NONBLOCK;
		mode = 0;
	} 
	else 
	{
		oflag = O_RDWR | O_CREAT | O_NONBLOCK;
		mode = 0770;
	}
	if((*fd = open(path, oflag , mode)) < 0){
		printf("open call Fail ! Error string : %s \n", strerror(errno));
		return -1;
	}
	return 0;
}

size_t Read(int fd,  void * buf, size_t size)
{
        int i = 0, num_r;
        while(num_r = read( fd, buf, size) && i < size )
        {
                if(num_r == -1)
                {
                        printf("read call Fail ! Error string : %s \n", strerror(errno));
                        return(-1);
                }
                i += num_r;
        }
        return i;
}

size_t Write(int fd, const void * buf, size_t size)
{
        int i = 0, num_w;
        while( i < size)
        {
            if ((num_w = write(fd,buf,size)) < 0)
            {
                printf("write call Fail ! Error string : %s \n", strerror(errno));
                return(-1);
            } 
            if ( num_w == 0 )
            {
                if( i < size )
                     continue;
                else 
                     break;
            }
            i+=num_w;
        }
        return i;
}

inline void * Malloc(size_t size)
{
        void * to = malloc(size);
        if( to == NULL)
        {
                printf("malloc call Fail ! Error string : %s \n", strerror(errno));
                abort();
        }
        return to;
}

inline unsigned int power2(unsigned int x, unsigned int y)
{
        return x<<y;
}


#ifdef __cplusplus
}
#endif
