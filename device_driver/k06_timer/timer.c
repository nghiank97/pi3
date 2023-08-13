#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/err.h>

#define TIME_CYCLE 2000
static struct timer_list timer_0;
static unsigned int count = 0;

void timer_callback(struct timer_list* data){
	pr_info("\t count %d \n",count++);
	mod_timer(&timer_0, jiffies + msecs_to_jiffies(TIME_CYCLE));
}

static int __init module_start(void){
	pr_info("\t start \n");
	timer_setup(&timer_0, timer_callback, 0);
	mod_timer(&timer_0, jiffies + msecs_to_jiffies(TIME_CYCLE));
	return 0;
}

static void __exit module_end(void){
	del_timer(&timer_0);
	pr_info("\t end \n");
}

module_init(module_start);
module_exit(module_end);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_VERSION("0:0.1");
