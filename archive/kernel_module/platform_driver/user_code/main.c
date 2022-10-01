#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int fd = -1;
    int option = 0;
    int count =1 ;
    unsigned char c;
    fd = open("/dev/gpio_driver", O_RDWR);
    if(fd == -1) return -1;
    do
    {
        printf("1. set the led status\n");
        printf("2. exits\n");
        scanf("%d", &option);
        getchar();
        switch (option)
        {
        case 1:
         {
            char c = getchar();
            count = write(fd, &c, 1);
            printf("write %d \n", c);
            if(count == -1) break;
         }
            break;
        
        case 2:
            close(fd);
            fd = -1;
            break;
        
        default:
            break;
        }
    } while(option != 2);
    return 0;
}