
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()

static int __init start_module(void);
static void __exit exit_module(void);

static ssize_t proc_fos_read(struct file *, char *, size_t, loff_t *);
static ssize_t proc_fos_write(struct file *, const char *, size_t, loff_t *);

struct proc_ops proc_fos = {
    .proc_read = proc_fos_read,
    .proc_write = proc_fos_write,
};

char* stored;
size_t store_len;

static ssize_t proc_fos_read(struct file *File, char *user, size_t count, loff_t *offs){
    if(copy_to_user(user, stored, store_len)){
        printk("Can't read data user\n");
        return 0;
    }
    return store_len;
}

static ssize_t proc_fos_write(struct file *File, const char *user, size_t count, loff_t *offs){
    if(copy_from_user(stored, user, count)){
        printk("Can't write data user\n");
        return 0;
    }
    store_len = count;
    return count;
}

static int __init start_module(void){
    printk("    start \n");   
    /* create a froc in /proc/ */
    if (proc_create("my_value", 0666, NULL, &proc_fos) == NULL){
        printk("    Can't create a proc in file\n");
        return -1;
    }
    // allocate monory
    if ((stored = kmalloc(256, GFP_KERNEL)) == NULL){
        printk("    Can't create a proc in file\n");
        return -1;
    }
    return 0;
}

static void __exit exit_module(void){
    remove_proc_entry("my_value",NULL);
    printk("    end \n");
}

module_init(start_module);
module_exit(exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple drive");
