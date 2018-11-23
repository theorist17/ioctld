#include <linux/module.h>    
#include <linux/kernel.h>    
#include <linux/cdev.h>    
#include <linux/device.h>    
#include <linux/fs.h>              
#include <linux/uaccess.h>    
#include "ledio.h"

dev_t id;    
struct cdev cdev;    
struct class *class;    
struct device *dev;    
        
char temp[100];    
student data;        
        
#define DEVICE_NAME "Ex_05_dev"    
    
int ledio_open (struct inode *inode, struct file *filp)    
{    
    printk( "open\n" );    
    return 0;    
}    
    
int ledio_close (struct inode *inode, struct file *filp)    
{    
    printk( "close\n" );    
    return 0;    
}    
        
ssize_t ledio_read(struct file *filp, char *buf, size_t size, loff_t *offset)    
{    
    printk( "read\n" );
    return strlen(temp);    
}    
        
ssize_t ledio_write (struct file *filp, const char *buf, size_t size, loff_t *offset)    
{    
    printk( "write\n" );    
    return size;    
}    
        
long ledio_ioctl ( struct file *filp, unsigned int cmd, unsigned long arg)    
{
    printk( "ioctl\n" );    
    if(_IOC_TYPE(cmd) != MY_IOC_MAGIC)
        return -EINVAL;
    if(_IOC_NR(cmd) >= MY_IOC_MAXNR)
        return -EINVAL;

    switch(cmd){
        case MY_IOC_WRITE:
        {
            if(copy_from_user((void *)&data, (void *)arg, sizeof(student)))
                return -EINVAL;
            printk("%s / %d / %c\n", data.name, data.age, data.grade);
            break;
        }
        case MY_IOC_READ:
        {
            if(copy_to_user((void *)arg, (void *)&data, sizeof(student)))
                return -EINVAL;
            break;
        }
    }    
    return 0;    
}    
        
struct file_operations ledio_fops =    
{    
    .owner           = THIS_MODULE,    
    .read            = ledio_read,         
    .write           = ledio_write,        
    .unlocked_ioctl  = ledio_ioctl,        
    .open            = ledio_open,         
    .release         = ledio_close,      
};    
        
int ledio_init(void)    
{    
    int ret;    
        
    ret = alloc_chrdev_region( &id, 0, 1, DEVICE_NAME );    
    if ( ret ){    
        printk( "alloc_chrdev_region error %d\n", ret );    
        return ret;    
    }    
        
    cdev_init( &cdev, &ledio_fops );    
    cdev.owner = THIS_MODULE;    
        
    ret = cdev_add( &cdev, id, 1 );    
    if (ret){    
        printk( "cdev_add error %d\n", ret );    
        unregister_chrdev_region( id, 1 );    
        return ret;    
    }    
        
    class = class_create( THIS_MODULE, DEVICE_NAME );    
    if ( IS_ERR(class)){    
        ret = PTR_ERR( class );    
        printk( "class_create error %d\n", ret );    
        
        cdev_del( &cdev );    
        unregister_chrdev_region( id, 1 );    
        return ret;    
    }    
        
    dev = device_create( class, NULL, id, NULL, DEVICE_NAME );    
    if ( IS_ERR(dev) ){    
        ret = PTR_ERR(dev);    
        printk( "device_create error %d\n", ret );    
        
        class_destroy(class);    
        cdev_del( &cdev );    
        unregister_chrdev_region( id, 1 );    
        return ret;    
    }
    
    return 0;    
}    
        
    
void ledio_exit(void)    
{    
    device_destroy(class, id );    
    class_destroy(class);    
    cdev_del( &cdev );    
    unregister_chrdev_region( id, 1 );    
}    
    
        
module_init(ledio_init);    
module_exit(ledio_exit);  
    
MODULE_LICENSE("Dual BSD/GPL");

