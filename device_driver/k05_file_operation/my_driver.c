#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>

dev_t dev = MKDEV(11, 0);
static struct class *my_dev_class;
static struct cdev my_cdev;

// function prototypes
static int file_open(struct inode *inode, struct file *file);
static int file_release(struct inode *inode, struct file *file);
static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t file_write(struct file *filp, const char *buf, size_t len, loff_t *off);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = file_read,
    .write = file_write,
    .open = file_open,
    .release = file_release,
};

static int file_open(struct inode *inode, struct file *file){
    pr_info("drive open function called \r\n");
    return 0;
}
static int file_release(struct inode *inode, struct file *file){
    pr_info("drive release function call \r\n");
    return 0;
}
static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
    pr_info("drive read function call \r\n");
    return 0;
}
static ssize_t file_write(struct file *filp, const char *buf, size_t len, loff_t *off){
    pr_info("drive write function call \r\n");
    return len;
}
/*
* module function start
*/
static int __init start_func(void){
    pr_info("starting \r\n");
    register_chrdev_region(&dev, 1, "my_dev");
    pr_info("major = %d minor = %d \n",MAJOR(dev), MINOR(dev));

    cdev_init(&my_cdev, &fops);
    if(cdev_add(&my_cdev,dev,1) < 0){
        pr_err("Can\'t create the character device \n");
        class_destroy(my_dev_class);
        return -1;
    }
    if(IS_ERR(my_dev_class = class_create(THIS_MODULE,"my_class"))){
        pr_err("Can\'t create the struct class \r\n");
        class_destroy(my_dev_class);
        return -1;
    }
    if(IS_ERR(device_create(my_dev_class, NULL, dev, NULL, "my_dev"))){
        pr_err("Can\'t create the device \r\n");
        unregister_chrdev_region(dev, 1);
        return -1;
    }
    pr_info("device driver inserted \r\n");
    return 0;
}
/*
* module function end
*/
static void __exit end_func(void){
    device_destroy(my_dev_class, dev);
    class_destroy(my_dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev,1);
    pr_info("end !...");
}

module_init(start_func);
module_exit(end_func);

MODULE_AUTHOR("nk");
MODULE_LICENSE("GLP");
MODULE_DESCRIPTION("file operations");
MODULE_VERSION("0:0.5");

