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

static struct platform_driver mydrv = {
    .probe      = my_pdrv_probe,
    .remove     = my_pdrv_remove,
    .driver     = {
        .name = "oni_pdev",
        .owner = THIS_MODULE,
    },
};

module_platform_driver(mydrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("crocodile");
