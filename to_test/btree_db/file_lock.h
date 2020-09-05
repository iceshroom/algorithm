//文件锁包装函数
#include <unistd.h>
#include <fcntl.h>

#ifndef FILE_LOCK_H_
#define FILE_LOCK_H_

int record_lock(int fd, int cmd, short l_type, short l_whence, off_t l_start, off_t l_len)
{
        flock lock_;
        lock_.l_type = l_type;
        lock_.l_whence = l_whence;
        lock_.l_start = l_start;
        lock_.l_len = l_len;

        return ( fcntl(fd, cmd, lock_));
}

int write_lock(int fd, short l_whence, off_t l_start, off_t l_len)
{
        return (record_lock(fd, F_SETLKW, F_WRLCK, l_whence, l_start, l_len));
}

int read_lock(int fd, short l_whence, off_t l_start, off_t l_len)
{
        return (record_lock(fd, F_SETLKW, F_RDLCK, l_whence, l_start, l_len));
}

#endif