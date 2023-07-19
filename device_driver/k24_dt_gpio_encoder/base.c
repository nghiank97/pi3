
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h>  //copy_to/from_user()
#include <linux/gpio.h>     //GPIO
#include <linux/interrupt.h>
#include <linux/err.h>
#include <asm/uaccess.h> 
#include <linux/proc_fs.h>
#include <linux/slab.h> // kmalloc

#define GPIO_16_IN      16
#define GPIO_20_IN      20
#define GPIO_21_OUT     21
#define GPIO_26_OUT     26

unsigned int encoder_a_irq;
unsigned int encoder_b_irq;
int64_t pulse = 0;

static irqreturn_t encoder_a_irq_handler(int irq,void *dev_id) {
  static int a = 1;
  int state = gpio_get_value(GPIO_20_IN);

  a ^= 1;
  gpio_set_value(GPIO_21_OUT,a);

  if (state == 1){
    pulse +=1;
  }
  else{
    pulse -=1;
  }
  return IRQ_HANDLED;
}

static irqreturn_t encoder_b_irq_handler(int irq,void *dev_id) {
  static int b = 1;
  int state = gpio_get_value(GPIO_16_IN);
  
  b ^= 1;
  gpio_set_value(GPIO_26_OUT,b);
  if (state == 0){
    pulse +=1;
  }
  else{
    pulse -=1;
  }
  return IRQ_HANDLED;
}

char* rx_data;

ssize_t proc_fos_read(struct file *File, char *user, size_t count, loff_t *offs){
  int ret;
  size_t len = 0;
  
  len = sprintf(rx_data,"%lld",pulse);
  ret = copy_to_user(user, rx_data, len);
  printk("  pulse %s \n",user);
  return ret;
}

ssize_t proc_fos_write(struct file *File, const char *user, size_t count, loff_t *offs){
  return count;
}
 
struct proc_ops proc_fos = {
  .proc_read = proc_fos_read,
  .proc_write = proc_fos_write,
};

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

static int __init etx_driver_init(void){

  /*Allocating Major number*/
  if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
    pr_err("    Cannot allocate major number\n");
    goto r_unreg;
  }
  
  /*Adding character device to the system*/
  if((cdev_add(&etx_cdev,dev,1)) < 0){
    pr_err("    Cannot add the device to the system\n");
    goto r_del;
  }

  /*Creating struct class*/
  if(IS_ERR(dev_class = class_create(THIS_MODULE,"etx_class"))){
    pr_err("    Cannot create the struct class\n");
    goto r_class;
  }

  /*Creating device*/
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))){
    pr_err("    Cannot create the Device \n");
    goto r_device;
  }

  //Output GPIO configuration
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_21_OUT) == false){
    pr_err("    GPIO %d is not valid\n", GPIO_21_OUT);
    goto r_device;
  }
  if(gpio_request(GPIO_21_OUT,"GPIO_20_IN") < 0){
    pr_err("    ERROR: GPIO %d request\n", GPIO_21_OUT);
    goto r_gpio_in;
  }
  gpio_direction_output(GPIO_21_OUT,1);

  if(gpio_is_valid(GPIO_26_OUT) == false){
    pr_err("    GPIO %d is not valid\n", GPIO_26_OUT);
    goto r_device;
  }
  if(gpio_request(GPIO_26_OUT,"GPIO_26_OUT") < 0){
    pr_err("    ERROR: GPIO %d request\n", GPIO_26_OUT);
    goto r_gpio_in;
  }
  gpio_direction_output(GPIO_26_OUT,1);

  //Output GPIO configuration
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_20_IN) == false){
    pr_err("    GPIO %d is not valid\n", GPIO_20_IN);
    goto r_device;
  }
  if(gpio_request(GPIO_20_IN,"GPIO_20_IN") < 0){
    pr_err("    ERROR: GPIO %d request\n", GPIO_20_IN);
    goto r_gpio_in;
  }
  gpio_direction_input(GPIO_20_IN);

  //Input GPIO configuratioin
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_16_IN) == false){
    pr_err("    GPIO %d is not valid\n", GPIO_16_IN);
    goto r_gpio_in;
  }
  if(gpio_request(GPIO_16_IN,"GPIO_16_IN") < 0){
    pr_err("    ERROR: GPIO %d request\n", GPIO_16_IN);
    goto r_gpio_in;
  }
  gpio_direction_input(GPIO_16_IN);

  //Get the IRQ number for our GPIO
  encoder_a_irq = gpio_to_irq(GPIO_16_IN);
  encoder_b_irq = gpio_to_irq(GPIO_20_IN);
  pr_info("    encoder_a_irq = %d\n", encoder_a_irq);
  pr_info("    encoder_b_irq = %d\n", encoder_b_irq);
  
  if (request_irq(encoder_a_irq,
                  (void *)encoder_a_irq_handler,
                  IRQF_TRIGGER_FALLING, 
                  "etx_device", 
                  NULL)) {
    pr_err("    my_device: cannot register IRQ ");
    goto r_gpio_in;
  }
  if (request_irq(encoder_b_irq,
                  (void *)encoder_b_irq_handler,
                  IRQF_TRIGGER_FALLING, 
                  "etx_device", 
                  NULL)) {
    pr_err("    my_device: cannot register IRQ ");
    goto r_gpio_in;
  }
  
  if (proc_create("my_value", 0666, NULL, &proc_fos) == NULL){
    printk("    Can't create a proc in file\n");
    return -1;
  }

  rx_data = (char*)kmalloc(256, GFP_KERNEL);

  pr_info("    Done!!!\n");
  return 0;

r_gpio_in:
  gpio_free(GPIO_16_IN);
  gpio_free(GPIO_20_IN);
r_device:
  device_destroy(dev_class,dev);
r_class:
  class_destroy(dev_class);
r_del:
  cdev_del(&etx_cdev);
r_unreg:
  unregister_chrdev_region(dev,1);
  return -1;
}

/*
** Module exit function
*/
static void __exit etx_driver_exit(void)
{
  kfree(rx_data);
  remove_proc_entry("my_value",NULL);
  free_irq(encoder_a_irq, NULL);
  free_irq(encoder_b_irq, NULL);
  gpio_free(GPIO_16_IN);
  gpio_free(GPIO_20_IN);
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&etx_cdev);
  unregister_chrdev_region(dev, 1);
  pr_info("Device Driver Remove...Done!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - GPIO Driver (GPIO Interrupt) ");
MODULE_VERSION("1.33");