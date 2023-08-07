
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sysfs.h> 
#include <linux/kobject.h> 
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/err.h>

// #define IRQ_NO          11
#define DEV_NAME        "etx_dev"
#define CLASS_NAME      "etx_class"
#define DEVICE_NAME     "etx_device"

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);

static struct file_operations fops ={
  .owner      = THIS_MODULE,
  .read       = etx_read,
  .write      = etx_write,
  .open       = etx_open,
  .release    = etx_release,
};

// Interrupt handler for IRQ 11. 
// static irqreturn_t irq_handler(int irq,void *dev_id) {
//   pr_info("\t Interrupt Occurred\n");
//   return IRQ_HANDLED;
// }
/*
** This function will be called when we open the Device file
*/
static int etx_open(struct inode *inode, struct file *file){
  pr_info("\t Driver Open Function Called...!!!\n");
  return 0;
}
/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file){
  pr_info("\t Driver Release Function Called...!!!\n");
  return 0;
}
/*
** This function will be called when we read the Device file
*/
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
  pr_info("\t Driver Read Function Called...!!!\n");
  return 0;
}
/*
** This function will be called when we write the Device file
*/
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off){
  pr_info("\t Driver Write Function Called...!!!\n");
  return len;
}
/*
** Module Init function
*/
static int __init etx_driver_init(void){
  /* Allocating Major number */
  if((alloc_chrdev_region(&dev, 0, 1, DEV_NAME)) <0){
    pr_err("\t Can\'t allocate major number\n");
    return -1;
  }
  
  /* Creating cdev structure */
  cdev_init(&etx_cdev,&fops);
  
  /* Adding character device to the system */
  if((cdev_add(&etx_cdev,dev,1)) < 0){
    pr_err("\t Can\'t add the device to the system\n");
    goto r_class;
  }
  /* Creating struct class */
  if(IS_ERR(dev_class = class_create(THIS_MODULE,CLASS_NAME))){
    pr_err("\t Can\'t create the struct class\n");
    goto r_class;
  }
  /* Creating device */
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,DEVICE_NAME))){
    pr_err("\t Can\'t create the Device\n");
    goto r_device;
  }
  
  // if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, DEVICE_NAME, (void *)(irq_handler))) {
  //   pr_info("\t Can\'t register IRQ ");
  //   goto irq;
  // }

  pr_info("\t Device Driver Insert...Done!!!\n");
  return 0;

// irq:
//   free_irq(IRQ_NO,(void *)(irq_handler));
r_device:
  class_destroy(dev_class);
r_class:
  unregister_chrdev_region(dev,1);
  return -1;
}
/*
** Module exit function
*/
static void __exit etx_driver_exit(void){
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&etx_cdev);
  unregister_chrdev_region(dev, 1);
  // free_irq(IRQ_NO,(void *)(irq_handler));
  pr_info("\t Device Driver Remove...Done!!!\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple device");
MODULE_VERSION("1.3");
