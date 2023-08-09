

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/workqueue.h>

#define printd(msg) printk("\t %s: %s:%d \n", msg, __func__, __LINE__);

#define DEV_NAME        "etx_dev"
#define CLASS_NAME      "etx_class"
#define DEVICE_NAME     "etx_device"

/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);
void workqueue_fn(struct work_struct *work); 

#define GPIO_20_IN      (20)
#define GPIO_21_IN      (21)
unsigned int gpio_irq_number[2];

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
static struct file_operations fops = {
  .owner          = THIS_MODULE,
  .read           = etx_read,
  .write          = etx_write,
  .open           = etx_open,
  .release        = etx_release,
};

/*Creating work by Static Method */
DECLARE_WORK(workqueue,workqueue_fn);

int64_t _position = 0;
int _oldState = 0;
static int sig1,sig2;

const int8_t KNOBDIR[] = {
  0, -1,  1,  0,
  1,  0,  0, -1,
  -1,  0,  0,  1,
   0,  1, -1,  0
};
/*Workqueue Function*/
void workqueue_fn(struct work_struct *work){
  int thisState;
  sig1 = gpio_get_value(GPIO_20_IN);
  sig2 = gpio_get_value(GPIO_21_IN);
  thisState = sig1 + sig2*2;
  if (_oldState != thisState) {
    _position += KNOBDIR[thisState | (_oldState << 2)];
    _oldState = thisState;
  }
}

static irqreturn_t irq_handler(int irq,void *dev_id) {
  schedule_work(&workqueue);
  return IRQ_HANDLED;
}

static int etx_open(struct inode *inode, struct file *file){
  return 0;
}

static int etx_release(struct inode *inode, struct file *file){
  return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
  uint16_t mem_size = sprintf(buf, "%lld", _position);
  return mem_size;
}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off){
  return len;
}
/*
** Module Init function
*/
static int __init etx_driver_init(void){
  /* Allocating Major number */
  if((alloc_chrdev_region(&dev, 0, 1, DEV_NAME)) <0){
    printd("error");
    return -1;
  }
  
  /* Creating cdev structure */
  cdev_init(&etx_cdev,&fops);
  
  /* Adding character device to the system */
  if((cdev_add(&etx_cdev,dev,1)) < 0){
    printd("error");
    goto r_class;
  }
  /* Creating struct class */
  if(IS_ERR(dev_class = class_create(THIS_MODULE,CLASS_NAME))){
    printd("error");
    goto r_class;
  }
  /* Creating device */
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,DEVICE_NAME))){
    printd("error");
    goto r_device;
  }

  //Input GPIO configuratioin
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_20_IN) == false){
    printd("error");
    goto r_device;
  }
  //Requesting the GPIO
  if(gpio_request(GPIO_20_IN,"GPIO_20_IN") < 0){
    printd("error");
    goto r_gpio_20in;
  }
  //configure the GPIO as input
  gpio_direction_input(GPIO_20_IN);
  //Get the IRQ number for our GPIO
  gpio_irq_number[0] = gpio_to_irq(GPIO_20_IN);
  if (request_irq(gpio_irq_number[0],            // IRQ number
                  (void *)irq_handler,   // IRQ handler
                  IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING,        // Handler will be called in raising edge
                  DEVICE_NAME,                // used to identify the device name using this IRQ
                  NULL)) {                    // device id for shared IRQ
    printd("error");
    goto r_gpio_20in;
  }
  //Input GPIO configuratioin
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_21_IN) == false){
    printd("error");
    goto r_gpio_20in;
  }
  //Requesting the GPIO
  if(gpio_request(GPIO_21_IN,"GPIO_21_IN") < 0){
    printd("error");
    goto r_gpio_21in;
  }
  //configure the GPIO as input
  gpio_direction_input(GPIO_21_IN);
  //Get the IRQ number for our GPIO
  gpio_irq_number[1] = gpio_to_irq(GPIO_21_IN);
  if (request_irq(gpio_irq_number[1],            // IRQ number
                  (void *)irq_handler,   // IRQ handler
                  IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING,        // Handler will be called in raising edge
                  DEVICE_NAME,                // used to identify the device name using this IRQ
                  NULL)) {                    // device id for shared IRQ
    printd("error");
    goto r_gpio_21in;
  }
  sig1 = gpio_get_value(GPIO_20_IN);
  sig2 = gpio_get_value(GPIO_21_IN);
  _oldState = sig1 | (sig2 << 1);
  
  printd("init");
  return 0;

r_gpio_21in:
  gpio_free(GPIO_21_IN);
r_gpio_20in:
  gpio_free(GPIO_20_IN);
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
  free_irq(gpio_irq_number[0],NULL);
  free_irq(gpio_irq_number[1],NULL);
  gpio_free(GPIO_20_IN);
  gpio_free(GPIO_21_IN);
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&etx_cdev);
  unregister_chrdev_region(dev, 1);
  printd("end");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple device");
MODULE_VERSION("1.3");
