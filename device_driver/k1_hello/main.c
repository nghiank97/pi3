/*
 * kn
 * hello.c
 * 1218
*/

#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/init.h>

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "bye bye world\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("KNghia");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello");
