#ifndef __SIMPLE__
#define __SIMPLE__
typedef struct _student{
    char name[10];
    int age;
    char grade;
}student;

#define MY_IOC_MAGIC 'c'

#define MY_IOC_READ     _IOR(MY_IOC_MAGIC, 0, student)
#define MY_IOC_WRITE    _IOW(MY_IOC_MAGIC, 1, student)

#define MY_IOC_MAXNR 2

#endif

