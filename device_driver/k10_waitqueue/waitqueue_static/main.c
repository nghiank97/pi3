
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/kthread.h>
#include <linux/wait.h>                 // Required for the wait queues


uint32_t read_count = 0;
static struct task_struct *wait_thread;
DECLARE_WAIT_QUEUE_HEAD(wait_queue_etx);
int wait_queue_flag = 0;

dev_t dev = MKDEV(11, 0);
static struct class *my_dev_class;
static struct cdev my_cdev;

/* function prototypes */
static int __init start_func(void);
static void __exit end_func(void);
static int file_open(struct inode *inode, struct file *file);
static int file_release(struct inode *inode, struct file *file);
static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t file_write(struct file *filp, const char *buf, size_t len, loff_t *off);


/* File operation sturcture */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = file_read,
    .write = file_write,
    .open = file_open,
    .release = file_release,
};

/* Thread function */
static int wait_function(void *unused){
  while(1) {
    pr_info(" Waiting For Event...\n");
    wait_event_interruptible(wait_queue_etx, wait_queue_flag != 0 );
    switch (wait_queue_flag){
      case 1:{
        pr_info(" Event Came From Read Function : %d\n", ++read_count);
        wait_queue_flag = 0;
        break;
      }
      case 2:{
        pr_info(" Event Came From Write Function : %d\n", ++read_count);
        wait_queue_flag = 0;
        break;
      }
      case 3:{
        pr_info(" Event Came From Exit Function\n");
        return 0;
      }
    }
  }
  do_exit(0);
  return 0;
}
/* This function will be called when we open the Device file */
static int file_open(struct inode *inode, struct file *file){
    pr_info(" Drive open function called \n");
    return 0;
}
/* This function will be called when we close the Device file */
static int file_release(struct inode *inode, struct file *file){
    pr_info(" Drive release function call \n");
    return 0;
}
/* This function will be called when we read the Device file */
static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off){

    pr_info(" Read Function\n");
    wait_queue_flag = 1;
    wake_up_interruptible(&wait_queue_etx);

    pr_info(" Drive read function call \n");
    return 0;
}
/* This function will be called when we write the Device file */
static ssize_t file_write(struct file *filp, const char *buf, size_t len, loff_t *off){

    pr_info(" Write Function\n");
    wait_queue_flag = 2;
    wake_up_interruptible(&wait_queue_etx);

    pr_info(" Drive write function call \n");
    return len;
}
/* Module function start */
static int __init start_func(void){
    pr_info(" Starting \n");
    /* Allocating Major number */
    if((alloc_chrdev_region(&dev, 0, 1, "my_dev")) <0){
      pr_info(" Cannot allocate major number \n");
      return -1;
    }
    pr_info(" Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
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
    
    wait_thread = kthread_create(wait_function, NULL, "WaitThread");
    if (wait_thread) {
      pr_info(" Thread Created successfully\n");
      wake_up_process(wait_thread);
    } else
      pr_info(" Thread creation failed\n");
      
    pr_info(" Device Driver Insert...Done!!!\n");
    return 0;
}
/* Module function end */
static void __exit end_func(void){
  wait_queue_flag = 3;
  wake_up_interruptible(&wait_queue_etx);
  
  device_destroy(my_dev_class, dev);
  class_destroy(my_dev_class);
  cdev_del(&my_cdev);
  unregister_chrdev_region(dev,1);
  pr_info(" End !... \n");
}

module_init(start_func);
module_exit(end_func);

MODULE_AUTHOR("nkn");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Waitqueue");
MODULE_VERSION("0:0.5");