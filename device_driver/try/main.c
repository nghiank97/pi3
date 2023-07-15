
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

static int __init start_module(void);
static void __exit exit_module(void);

ssize_t proc_fos_read(struct file *, char *, size_t, loff_t *);
ssize_t proc_fos_write(struct file *, const char *, size_t, loff_t *);

struct proc_ops proc_fos = {
    .proc_read = proc_fos_read,
    .proc_write = proc_fos_write,
};

char* data_is_written;

ssize_t proc_fos_read(struct file *File, char *user, size_t count, loff_t *offs){
    printk("Data is read : %s\n",user);
    return count;
}

ssize_t proc_fos_write(struct file *File, const char *user, size_t count, loff_t *offs){
    // if(copy_from_user(data_is_written, user, count)){
    //     printk("Can't write data user\n");
    //     return -1;
    // }
    printk("Data is written : %s %d\n",user, count);
    return count;
}

static int __init start_module(void){
    printk("    start \n");   
    /* create a froc in /proc/ */
    if (proc_create("my_value", 0666, NULL, &proc_fos) == NULL){
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
