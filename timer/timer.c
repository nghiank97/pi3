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

#define TIMEOUT 1000
static struct timer_list etx_timer;
static unsigned int count = 0;

void timer_callback(struct timer_list* data){
	pr_info(": count %d",count++);
	mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}

static int __init module_start(void){
	pr_info(": start");
	timer_setup(&etx_timer, timer_callback, 0);
	mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));
	return 0;
}

static void __exit module_end(void){
	del_timer(&etx_timer);
	pr_info(": end");
}

module_init(module_start);
module_exit(module_end);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_VERSION("0:0.1");
