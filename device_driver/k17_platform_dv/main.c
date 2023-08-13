/*File oni_pdriver.c*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

static int my_pdrv_probe(struct platform_device *pdev){
    pr_info("Hello! device probed! \n");
    return 0;
}

static int my_pdrv_remove(struct platform_device *pdev){
    pr_info("Good bye!!!!!!!!!");
    return 0;
}

static struct platform_driver mypdrv = {
    .probe      = my_pdrv_probe,
    .remove     = my_pdrv_remove,
    .driver     = {
        .name = "oni_pdev",
        .owner = THIS_MODULE,
    },
};

static int __init start_func(void){
    pr_info("Hello Guy\n");

    /*Registering with Kernel */
    platform_driver_register(&mypdrv);
    return 0;
}

static void __exit end_func(void){
    pr_info("Good bye Guy\n");
    /*Unregistering from Kernel*/
    platform_driver_unregister(&mypdrv);
}

module_init(start_func);
module_exit(end_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Phi Nguyen");
MODULE_DESCRIPTION("HIHI");