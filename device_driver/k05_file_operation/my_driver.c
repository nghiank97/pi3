#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define printd(comment)     printk(KERN_ALERT "%s: %s %d\n",comment, __FUNCTION__, __LINE__); 
#define DEVICE_NAME         "etx_device"
#define CLASS_NAME          "etx_class"
#define IRQ_NO              11

// setting up the device
int major;
static struct class*  my_class  = NULL;
static struct device* my_device = NULL;

static int file_open(struct inode *inode, struct file *file){
    printd("\t info ! ");
    return 0;
}
static int file_release(struct inode *inode, struct file *file){
    printd("\t info ! ");
    return 0;
}
static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
    printd("\t info ! ");
    return 0;
}
static ssize_t file_write(struct file *filp, const char *buf, size_t len, loff_t *off){
    printd("\t info ! ");
    return len;
}

static struct file_operations fops = {
    .open = file_open,
    .release = file_release,
    .read = file_read,
    .write = file_write
};

static int __init etx_device_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if ( major < 0 )
        printd("\t error ! ");
    // Register device class
    my_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(my_class)) {
        printd("\t error ! ");
        goto r_class;
    }
    // Register the device driver
    my_device = device_create(my_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        printd("\t error ! ");
        goto r_device;
    }

    printd("\t init ! ");
    return 0;
r_class:
    unregister_chrdev(major, DEVICE_NAME);
r_device:
    class_destroy(my_class);
    unregister_chrdev(major, DEVICE_NAME);
    return -1;
}

static void __exit etx_device_exit(void) {
    device_destroy(my_class, MKDEV(major, 0));              // remove the device
    class_unregister(my_class);                             // unregister the device class
    class_destroy(my_class);                                // remove the device class
    unregister_chrdev(major, DEVICE_NAME);                  // unregister the major number
    printd("\t exit ! ");
}

module_init(etx_device_init);
module_exit(etx_device_exit);

MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple device");
MODULE_LICENSE("GPL");