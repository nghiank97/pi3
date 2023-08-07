#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#define USE_IRQ 11

//Interrupt handler for IRQ 11.
irqreturn_t irq_handler(int irq,void *dev_id) {
  printk(KERN_INFO "Shared IRQ: Interrupt Occurred");
  return 0;
}

static int __init sharedirqmodule_init(void) {
  if (request_irq(USE_IRQ, irq_handler, IRQF_SHARED, "my_device", (void *)(irq_handler))) {
    printk(KERN_INFO "my_device: cannot register IRQ ");
    return -1;
  }
  return 0;
}

static void __exit sharedirqmodule_exit(void) {
  free_irq(USE_IRQ,(void *)(irq_handler));
}

module_init(sharedirqmodule_init);
module_exit(sharedirqmodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");