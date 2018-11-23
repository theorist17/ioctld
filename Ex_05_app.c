#include <stdio.h>    
#include <sys/types.h>    
#include <sys/stat.h>    
#include <sys/ioctl.h>    
#include <fcntl.h>    
#include <unistd.h>    
#include "simple.h"
#include <string.h>

#define DEVICE_FILENAME  "/dev/Ex_03_dev"    
        
int main()    
{    
    int dev, ret;    
    student data;

    memset(&data, 0, sizeof(student));    
    int size = sizeof(student);    
        
    printf( "device file open\n");     
    dev = open( DEVICE_FILENAME, O_RDWR|O_NDELAY );    
        
    if( dev >= 0 )
    {  
        strcpy(data.name, "Kim");
        data.age = 20;
        data.grade = 'A';

        printf( "data write!\n");
        ret = ioctl(dev, MY_IOC_WRITE, &data );    
        printf( "ret = %d\n", ret );    

        printf("data change!\n");
        strcpy(data.name, "Lee");
        data.age = 30;
        data.grade = 'C';
        printf("%s %d %c\n", data.name, data.age, data.grade);

        printf("data read!\n");
        ret = ioctl(dev, MY_IOC_READ, &data);    
        printf("%s %d %c\n", data.name, data.age, data.grade);

        printf( "device file close\n");    
        ret = close(dev);    
        printf( "ret = %d\n", ret );    
    }    
        
    return 0;    
}

