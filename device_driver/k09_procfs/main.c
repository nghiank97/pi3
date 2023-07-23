#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

/* global variables for procfs folder and file */
static struct proc_dir_entry *proc_folder;
static struct proc_dir_entry *proc_file;

char buff_data[255];
int16_t buff_len = 0;

/**
 * @brief Read data out of the buffer
 */
static ssize_t proc_fs_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta;
	/* Get amount of data to copy */
	to_copy = min(count, sizeof(buff_data));
	/* Copy data to user */
	not_copied = copy_to_user(user_buffer, buff_data, to_copy);
	/* Calculate data */
	delta = to_copy - not_copied;
	return delta;
}
/**
 * @brief Write data to buffer
 */
static ssize_t proc_fs_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta;
	/* Clear text */
	memset(buff_data, 0, sizeof(buff_data));
	/* Get amount of data to copy */
	to_copy = min(count, sizeof(buff_data));
	/* Copy data to user */
	not_copied = copy_from_user(buff_data, user_buffer, to_copy);
	/* Calculate data */
	delta = to_copy - not_copied;
	return delta;
}

static const struct proc_ops fops = {
	.proc_read = proc_fs_read,
	.proc_write = proc_fs_write,
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init init_func(void) {
	/* /proc/hello/dummy */

	proc_folder = proc_mkdir("my_value", NULL);
	if(proc_folder == NULL) {
		printk("procfs_test - Error creating /proc/my_value\n");
		return -ENOMEM;
	}

	proc_file = proc_create("value", 0666, proc_folder, &fops);
	if(proc_file == NULL) {
		printk("procfs_test - Error creating /proc/my_value/value\n");
		proc_remove(proc_folder);
		return -ENOMEM;
	}

	printk("	Info! Inserted a device\n");
	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit exit_func(void) {
	printk("	Info! Removed the device\n");
	proc_remove(proc_file);
	proc_remove(proc_folder);
}

module_init(init_func);
module_exit(exit_func);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple device");
