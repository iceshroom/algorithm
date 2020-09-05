#ifndef MY_LIB_H
#define MY_LIB_H

#ifdef __cplusplus
extern "C"{
#endif

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int Open(int * fd, const char *path);
size_t Read(int fd,  void * buf, size_t size);
size_t Write(int fd, const void * buf, size_t size);
void * Malloc(size_t size);
unsigned int power2(unsigned int x, unsigned int y);

#endif

#ifdef __cplusplus
}
#endif