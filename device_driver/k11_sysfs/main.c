#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/sysfs.h> 
#include <linux/kobject.h> 

/*************** Sysfs functions **********************/
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);

static int value = 0;
static struct kobject *kobj_ref;
static struct kobj_attribute etx_attr = __ATTR(etx_value, 0660, sysfs_show, sysfs_store);

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
  pr_info("	Callback! Read!!!\n");
  return sprintf(buf, "%d", value);
}

static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count){
  pr_info("	Callback! Write!!!\n");
  sscanf(buf,"%d",&value);
  return count;
}

static int __init init_func(void){
  /* Creating a directory in /sys/kernel/ */
  kobj_ref = kobject_create_and_add("etx_sysfs",kernel_kobj);

  /* Creating sysfs file for etx_value*/
  if(sysfs_create_file(kobj_ref, &etx_attr.attr)){
    kobject_put(kobj_ref); 
    sysfs_remove_file(kernel_kobj, &etx_attr.attr);
    pr_err("	Error! Cannot create sysfs file......\n");
    return -1;
  }
  printk("	Info! Inserted a device\n");
  return 0;
}

static void __exit exit_func(void){

  kobject_put(kobj_ref); 
  sysfs_remove_file(kernel_kobj, &etx_attr.attr);
  printk("	Info! Removed the device\n");
}

module_init(init_func);
module_exit(exit_func);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple sys_fs");
