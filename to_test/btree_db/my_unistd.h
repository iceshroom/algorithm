extern "C"
{
        #include <unistd.h>
        #include <sys/stat.h>
        #include <fcntl.h>
        #include <stdio.h>
        #include <signal.h>
        #include <string.h>
        #include <errno.h>
        #include <stdlib.h>
}

#include "err.h"

//一些系统调用或库函数的封装
//首字母大写代表对应的封装函数，一般只加入了错误处理

#ifndef MY_UNISTD_H
#define MY_UNISTD_H

static err<void> errdeal;      //类模板函数

void Open(int & fd, const char *path, int oflag)
{
    fd = open(path, oflag);
    if ( ! (fd > 0) ) 
    {
        printf("open file: %s Fail ! Error string : %s \n", path, strerror(errno));
        exit(1);
    }
}

size_t Read(int fd,  void * buf, size_t size)
{
        int i = 0, num_r;
        while(num_r = read( fd, buf, size) && i < size )
        {
                if(num_r == -1)
                {
                        errdeal("read() fault!\n");
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

int System(const char * command)
{
        int i = system(command);
        if ( i == -1)
                printf("ERROR : system(\"%s\") Couldn't create child process! \n", command);
        return i;
}

int Pipe(int pipefd[2])
{
        if (pipe(pipefd) != 0)
        {
                errdeal("pipe() fault!\n");
                exit(1);
        }
        return 0;
}

int Sigaction(int signum, const struct sigaction *act,struct sigaction *oldact)
{
        if (sigaction( signum, act, oldact) == -1)
        {
                errdeal("sigaction() fault!\n");
                exit(1);
        }
        return 0;
}

void Kill(int pid)
{
       if( kill((pid_t)pid, SIGTERM) < 0)
               errdeal("kill fault!\n");
}


#endif
