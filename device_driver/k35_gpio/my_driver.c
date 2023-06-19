
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
#include <asm/io.h>

/* function prototypes */
static int __init module_start(void);
static void __exit module_end(void);
static int file_open(struct inode *inode, struct file *file);
static int file_release(struct inode *inode, struct file *file);
static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t file_write(struct file *filp, const char *buf, size_t len, loff_t *off);

#define BCM2835_PERI_BASE       (0x3F000000)
#define GPIO_BASE               (BCM2835_PERI_BASE + 0x200000)

#define _BLOCK_SIZE 	0xB0  // BLOCK_SIZE is defined in fs.h
#define GPFSEL0    	0x00
#define GPFSEL1    	0x04
#define GPFSEL2    	0x08
#define GPSET0     	0x1C
#define GPSET1		0x20
#define GPCLR0     	0x28
#define GPCLR1		0x2C

#define GPPUD		0x94
#define GPPUDCLK0	0x98
#define GPPUDCLK1	0x9C

#define LED_ON()     {gpio[GPSET0/4] |= (1 << 6);}
#define LED_OFF()    {gpio[GPCLR0/4] |= (1 << 6);}

volatile uint32_t *gpio;

static int gpio_int(void){
	gpio = ioremap(GPIO_BASE, _BLOCK_SIZE);
	if (gpio == NULL) {
		iounmap(gpio);
		return 0;
	}
	// config gpio 6
	gpio[GPFSEL0/4] &=~ (7 << 18);
	gpio[GPFSEL0/4] |= (1 << 18);

	// config pull up
	// gpio[GPPUD/4] &=~ (3<<12);
	// gpio[GPPUD/4] |= (2<<12);
	// gpio[GPPUDCLK0/4] |= (1<<6);

  return 1;
}

dev_t dev = MKDEV(11, 0);
static struct class *my_dev_class;
static struct cdev my_cdev;

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
  	switch (buf[0]) {
  		case '0':
        LED_OFF();
        break;
  		case '1':
        LED_ON();
        break;
  		default:
  			break;
  	}
  	return len;
}

static int __init module_start(void){
  if (gpio_int() == 0){
    pr_info(" GPIO remap failed\n");
    iounmap(gpio);
    return 0;
  }
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
  pr_info(" GPIO LED driver led start...\n");
	return 0;
}

static void __exit module_end(void){
	iounmap(gpio);
  device_destroy(my_dev_class, dev);
  class_destroy(my_dev_class);
  cdev_del(&my_cdev);
  unregister_chrdev_region(dev,1);
	pr_info(" GPIO LED driver led end...\n");
}

module_init(module_start);
module_exit(module_end);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_VERSION("0:0.2");
