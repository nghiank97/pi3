
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kobject.h>

/* function prototypes */
static int __init module_start(void);
static void __exit module_end(void);
static int file_open(struct inode *inode, struct file *file);
static int file_release(struct inode *inode, struct file *file);
static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t file_write(struct file *filp, const char *buf, size_t len, loff_t *off);
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);


volatile int etx_value = 0;
dev_t dev = MKDEV(11, 0);
static struct class *my_dev_class;
static struct cdev my_cdev;
struct kobject *kobj_ref;

struct kobj_attribute etx_attr = __ATTR(etx_value, 0660, sysfs_show, sysfs_store);

/* This function will be called when we read the sysfs file */
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
  pr_info("Sysfs - Read!!!\n");
  return sprintf(buf, "%d", etx_value);
}
/* This function will be called when we write the sysfsfs file */
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count){
  pr_info("Sysfs - Write!!!\n");
  sscanf(buf,"%d",&etx_value);
  return count;
}

/* File operation sturcture */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = file_read,
    .write = file_write,
    .open = file_open,
    .release = file_release,
};

/* This function will be called when we open the Device file */
static int file_open(struct inode *inode, struct file *file){
    return 0;
}
/* This function will be called when we close the Device file */
static int file_release(struct inode *inode, struct file *file){
    return 0;
}
/* This function will be called when we read the Device file */
static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
    return 0;
}
/* This function will be called when we write the Device file */
static ssize_t file_write(struct file *filp, const char *buf, size_t len, loff_t *off){
  	return len;
}

static int __init module_start(void){
  /* Allocating Major number */
  if((alloc_chrdev_region(&dev, 0, 1, "my_dev")) <0){
    pr_info(" Cannot allocate major number \n");
    return -1;
  }
  /* Creating cdev structure */
  cdev_init(&my_cdev, &fops);
  my_cdev.owner = THIS_MODULE;
  my_cdev.ops = &fops;
    
  /* Adding character device to the system */
  if(cdev_add(&my_cdev,dev,1) < 0){
    pr_err(" Can\'t create the character device \n");
    class_destroy(my_dev_class);
    return -1;
  }
  /* Creating device */
  if(IS_ERR(my_dev_class = class_create(THIS_MODULE,"my_class"))){
    pr_err(" Can\'t create the struct class \n");
    class_destroy(my_dev_class);
    return -1;
  }
  if(IS_ERR(device_create(my_dev_class, NULL, dev, NULL, "my_dev"))){
    pr_err(" Can\'t create the device \n");
    unregister_chrdev_region(dev, 1);
    return -1;
  }
  
  /* Creating a directory in /sys/kernel/ */
  kobj_ref = kobject_create_and_add("etx_sysfs",kernel_kobj);
  /* Creating sysfs file for etx_value*/
  if(sysfs_create_file(kobj_ref,&etx_attr.attr)){
    pr_err("Cannot create sysfs file......\n");
    kobject_put(kobj_ref); 
    sysfs_remove_file(kernel_kobj, &etx_attr.attr);
    return -1;
  }
  
  
  pr_info(" start...\n");
	return 0;
}

static void __exit module_end(void){

  kobject_put(kobj_ref); 
  sysfs_remove_file(kernel_kobj, &etx_attr.attr);
        
  device_destroy(my_dev_class, dev);
  class_destroy(my_dev_class);
  cdev_del(&my_cdev);
  unregister_chrdev_region(dev,1);
  
	pr_info(" end...\n");
}

module_init(module_start);
module_exit(module_end);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_VERSION("0:0.2");
