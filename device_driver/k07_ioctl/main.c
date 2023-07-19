#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/err.h>

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)
int32_t value = 0;
dev_t dev = MKDEV(11, 0);
static struct class *dev_class;
static struct cdev my_cdev;
/*
** Function Prototypes
*/
static int      __init my_driver_init(void);
static void     __exit my_driver_exit(void);
static int      my_open(struct inode *inode, struct file *file);
static int      my_release(struct inode *inode, struct file *file);
static ssize_t  my_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  my_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
/*
** File operation sturcture
*/
static struct file_operations fops ={
    .owner          = THIS_MODULE,
    .read           = my_read,
    .write          = my_write,
    .open           = my_open,
    .unlocked_ioctl = my_ioctl,
    .release        = my_release,
};
/*
** This function will be called when we open the Device file
*/
static int my_open(struct inode *inode, struct file *file){
    pr_info("Device File Opened...!!!\n");
    return 0;
}
/*
** This function will be called when we close the Device file
*/
static int my_release(struct inode *inode, struct file *file){
    pr_info("Device File Closed...!!!\n");
    return 0;
}
/*
** This function will be called when we read the Device file
*/
static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
    pr_info("Read Function\n");
    return 0;
}
/*
** This function will be called when we write the Device file
*/
static ssize_t my_write(struct file *filp, const char __user *buf, size_t len, loff_t *off){
    pr_info("Write function\n");
    return len;
}
/*
** This function will be called when we write IOCTL on the Device file
*/
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
    switch(cmd) {
        case WR_VALUE:
            if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) ){
                pr_err("Data Write : Err!\n");
            }
            pr_info("Value = %d\n", value);
            break;
        case RD_VALUE:
            if( copy_to_user((int32_t*) arg, &value, sizeof(value)) ){
                pr_err("Data Read : Err!\n");
            }
            break;
        default:
            pr_info("Default\n");
            break;
    }
    return 0;
}
 
/*
** Module Init function
*/
static int __init my_driver_init(void)
{
    register_chrdev_registe(dev, 1, "my_dev");
    pr_info("Major = %d\r\nMinor = %d\r\n",MAJOR(dev), MINOR(dev));
 
    /*Creating cdev structure*/
    cdev_init(&my_cdev,&fops);
 
    /*Adding character device to the system*/
    if((cdev_add(&my_cdev,dev,1)) < 0){
        pr_err("Cannot add the device to the system\n");
        unregister_chrdev_region(dev,1);
        return -1;
    }
 
    /*Creating struct class*/
    if(IS_ERR(dev_class = class_create(THIS_MODULE,"my_class"))){
        pr_err("Cannot create the struct class\n");
        unregister_chrdev_region(dev,1);
        return -1;
    }
 
    /*Creating device*/
    if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"my_device"))){
        pr_err("Cannot create the Device 1\n");
        class_destroy(dev_class);
    }
    pr_info("Device Driver Insert...Done!!!\n");
    return 0;
}
/*
** Module exit function
*/
static void __exit my_driver_exit(void){
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(my_driver_init);
module_exit(my_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("kn");
MODULE_DESCRIPTION("IO control");
MODULE_VERSION("0:7.0");