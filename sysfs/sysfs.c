#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/err.h>

volatile int etx_value = 0;
struct kobject *kobj_ref;

/* sysfs funtion */
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count);

struct kobj_attribute etx_attr = __ATTR(etx_value, 0660, sysfs_show, sysfs_store);

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff){
        pr_info("Sysfs - Read ");
        return sprintf(buff, "%d", etx_value);
}

static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count){
        pr_info("Sysfs - Write ");
        sscanf(buff,"%d",&etx_value);
        return count;
}

static int __init start_device(void){
	pr_info(": start\n");
	/* creating a directory in /sys/kernel/ */
	kobj_ref = kobject_create_and_add("etx_sysfs", kernel_kobj);
	/* creating sysfs file for etx_value */
	if(sysfs_create_file(kobj_ref,&etx_attr.attr)){
		pr_err("Can't create sysfs file");
		kobject_put(kobj_ref);
		sysfs_remove_file(kernel_kobj, &etx_attr.attr);
		return -1;
	}
	return 0;
}

static void __exit stop_device(void){
        kobject_put(kobj_ref);
        sysfs_remove_file(kernel_kobj, &etx_attr.attr);
	pr_info(": end\n");
}


module_init(start_device);
module_exit(stop_device);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_VERSION("1.8");
