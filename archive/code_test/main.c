#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>

#define GPIO_GET(addr,pin) ((*(addr+13))>>pin)&1

int main()
{
    char test[100];
    for(int i = 0; i < 100; i++)
    {
        test[i] = i;
    }
    char* ptr = &test[36];
    int status = GPIO_GET(ptr, 1);
    if (GPIO_GET(ptr, 1) == 0) printf("ok la\n");
}