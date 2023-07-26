
#include <linux/kernel.h>
#include <linux/init.h>
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

#include <linux/module.h>
#include <linux/moduleparam.h>

#define PROC_DIR_NAME   "encoder"
#define PROC_VALUE      "value"

#define GPIO_A_IN      17
#define GPIO_B_IN      27
#define GPIO_Z_IN      22

static struct proc_dir_entry* encoder_dir = NULL;
unsigned int encoder_a_irq;
unsigned int encoder_b_irq;
unsigned int encoder_z_irq;

int64_t rotion = 0;
int64_t pulse = 0;

static irqreturn_t encoder_a_irq_handler(int irq,void *dev_id) {
  int state = gpio_get_value(GPIO_B_IN);
  if (state == 1){
    pulse +=1;
  }
  else{
    pulse -=1;
  }
  return IRQ_HANDLED;
}

static irqreturn_t encoder_b_irq_handler(int irq,void *dev_id) {
  int state = gpio_get_value(GPIO_A_IN);
  if (state == 0){
    pulse +=1;
  }
  else{
    pulse -=1;
  }
  return IRQ_HANDLED;
}

static irqreturn_t encoder_z_irq_handler(int irq,void *dev_id) {
  static int64_t p = 0;
  static int64_t p_p = 0;
  
  p = pulse;
  if (p >= p_p)
    rotion+=1;
  else{
    rotion-=1;
  }
  return IRQ_HANDLED;
}

ssize_t proc_fs_read(struct file* filep, char* __user u_buffer, size_t len, loff_t* offset){
  ssize_t len_msg = 0;
  len_msg = sprintf(u_buffer,"%lld %lld\n",pulse,rotion);
  if(len_msg > 0){
    return len_msg;
  }
  return 0;
}
 
struct proc_ops proc_fs = {
  .proc_read = proc_fs_read
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

  //  Encoder a pin configuration
  if(gpio_is_valid(GPIO_A_IN) == false){
    pr_err("    GPIO %d is not valid\n", GPIO_A_IN);
    goto r_device;
  }
  if(gpio_request(GPIO_A_IN,"GPIO_A_IN") < 0){
    pr_err("    ERROR: GPIO %d request\n", GPIO_A_IN);
    goto r_gpio_in;
  }
  gpio_direction_input(GPIO_A_IN);

  //  Encoder b pin configuration
  if(gpio_is_valid(GPIO_B_IN) == false){
    pr_err("    GPIO %d is not valid\n", GPIO_B_IN);
    goto r_gpio_in;
  }
  if(gpio_request(GPIO_B_IN,"GPIO_B_IN") < 0){
    pr_err("    ERROR: GPIO %d request\n", GPIO_B_IN);
    goto r_gpio_in;
  }
  gpio_direction_input(GPIO_B_IN);
  
  //  Encoder z pin configuration
  if(gpio_is_valid(GPIO_Z_IN) == false){
    pr_err("    GPIO %d is not valid\n", GPIO_Z_IN);
    goto r_gpio_in;
  }
  if(gpio_request(GPIO_Z_IN,"GPIO_B_IN") < 0){
    pr_err("    ERROR: GPIO %d request\n", GPIO_Z_IN);
    goto r_gpio_in;
  }
  gpio_direction_input(GPIO_Z_IN);

  //Get the IRQ number for our GPIO
  encoder_a_irq = gpio_to_irq(GPIO_A_IN);
  encoder_b_irq = gpio_to_irq(GPIO_B_IN);
  encoder_z_irq = gpio_to_irq(GPIO_Z_IN);
  pr_info("    encoder_a_irq = %d\n", encoder_a_irq);
  pr_info("    encoder_b_irq = %d\n", encoder_b_irq);
  pr_info("    encoder_z_irq = %d\n", encoder_z_irq);
  
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
  if (request_irq(encoder_z_irq,
                  (void *)encoder_z_irq_handler,
                  IRQF_TRIGGER_FALLING, 
                  "etx_device", 
                  NULL)) {
    pr_err("    my_device: cannot register IRQ ");
    goto r_gpio_in;
  }
  

  encoder_dir = proc_mkdir(PROC_DIR_NAME, NULL);
  if(!encoder_dir){
    proc_remove(encoder_dir);  
    return -1;
  }

  if (!proc_create(PROC_VALUE, 0666, encoder_dir, &proc_fs)){
    printk("    Can't create a proc in file\n");
    remove_proc_entry(PROC_VALUE, NULL);
    return -1;
  }
  
  pr_info("    Done!!!\n");
  return 0;

r_gpio_in:
  gpio_free(GPIO_A_IN);
  gpio_free(GPIO_B_IN);
  gpio_free(GPIO_Z_IN);
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
static void __exit etx_driver_exit(void){
  remove_proc_entry(PROC_VALUE, NULL);
  proc_remove(encoder_dir);
  free_irq(encoder_a_irq, NULL);
  free_irq(encoder_b_irq, NULL);
  gpio_free(GPIO_A_IN);
  gpio_free(GPIO_B_IN);
  gpio_free(GPIO_Z_IN);
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&etx_cdev);
  unregister_chrdev_region(dev, 1);
  pr_info("Device Driver Remove...Done!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("read encoder");
MODULE_VERSION("0.1");
