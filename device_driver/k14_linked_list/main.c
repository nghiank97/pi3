
#include <linux/kernel.h>               /* We're doing kernel work */
#include <linux/module.h>               /* Specifically, a module */
#include <linux/slab.h>                 /* kmalloc, kfee */
#include <linux/list.h>                 /* list implementation */

static struct tmp_list {
        struct list_head list;
        int id;
} *my_list;

int __init tmp_init(void) {

        // initialize parameter
        int ids[] = {9, 7, 8};
        struct tmp_list *ptr;
        struct list_head *current_list;

        // create node in linked list
        my_list = kmalloc(sizeof(struct tmp_list), GFP_KERNEL);
        INIT_LIST_HEAD(&my_list->list);
        current_list = &my_list->list;

	// Populate list with IDs
        for (int i = 0; i < 3; ++i) {
                struct tmp_list *buff = kmalloc(sizeof(struct tmp_list), GFP_KERNEL);
                buff->id = ids[i];
                printk("\t in = %d\n", buff->id);
                // add new list
                list_add(&buff->list, current_list);
                current_list = current_list->next;
        }

redo:
	// Print ID and clean up list
        list_for_each_entry(ptr, &my_list->list, list) {
                int t = ptr->id;
                printk("\t out = %d\n", t);
                list_del(&ptr->list);
                kfree(ptr);
                goto redo; // Not that clean, but fast.
        }

        kfree(my_list);
        return 0;
}

void tmp_exit(void) { }

module_init(tmp_init);
module_exit(tmp_exit);

MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple device");
MODULE_LICENSE("GPL");