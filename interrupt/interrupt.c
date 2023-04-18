#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/module.h>

#define IRQ_NO 11

static int __init start_module(void){
	pr_info(": start\n");
	return 0;
}

static void __exit exit_module(void){
	pr_info(": end\n");
}

module_init(start_module);
module_exit(exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_VERSION("0:0.1");
