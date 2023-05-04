#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>

// Declare major & minor number
dev_t dev = MKDEV(7, 0);
/*
* module function start
*/
static int __init start_func(void){
    // set a particular
    register_chrdev_region(dev, 1, "k21_dev");
    // get value
    printk(KERNEL_INFO "Major %d \r\n", MAJOR(dev));
    printk(KERNEL_INFO "Minor %d \r\n", MINOR(dev));
    printk("Starting !...");
    return 0;
}
/*
* module function end
*/
static void __exit end_func(void){
    printk(KERNEL_INFO "End !...");
}

module_init(start_func);
module_exit(end_func);

MODULE_AUTHOR("KN");
MODULE_LICENSE("GLP");
MODULE_DESCRIPTION("Major & minor");
MODULE_VERSION("0.2");

