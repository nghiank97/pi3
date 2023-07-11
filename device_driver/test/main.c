
#include <linux/module.h>
#include <linux/init.h>
#include <linux/property.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple project");

/* Declate the probe and remove functions */
static int dt_probe(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);

static struct of_device_id my_driver_ids[] = {
	{
		.compatible = "brightlight,mydev",
	}, { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, my_driver_ids);

static struct platform_driver my_driver = {
	.probe = dt_probe,
	.remove = dt_remove,
	.driver = {
		.name = "my_device_driver",
		.of_match_table = my_driver_ids,
	},
};
/**
 * @brief This function is called on loading the driver 
 */
static int dt_probe(struct platform_device *pdev) {
    struct device *my_dev = &pdev->dev;
	const char *label;
	int my_value, ret;

	printk("dt_probe - Now I am in the probe function!\n");
    printk("    Check\n");

    if (!device_property_present(my_dev, "compatible")){
        printk("    Device loss property\n");
        return 0;
    }
    if (!device_property_present(my_dev, "status")){
        printk("    Device loss property\n");
        return 0;
    }
    if (!device_property_present(my_dev, "label")){
        printk("    Device loss property\n");
        return 0;
    }
    if (!device_property_present(my_dev, "my_value")){
        printk("    Device loss property\n");
        return 0;
    }

    ret = device_property_read_string(my_dev, "compatible", &label);
    if (ret){
        printk("    Device loss property value\n");
        return 0;
    }
    printk("    my_dev compatible: %s\n",  label);

    ret = device_property_read_string(my_dev, "status", &label);
    if (ret){
        printk("    Device loss property value\n");
        return 0;
    }
    printk("    my_dev status: %s\n",  label);

    ret = device_property_read_string(my_dev, "label", &label);
    if (ret){
        printk("    Device loss property value\n");
        return 0;
    }
    printk("    my_dev label: %s\n",  label);

    ret = device_property_read_u32(my_dev, "my_value", &my_value);
    if (ret){
        printk("    Device loss property value\n");
        return 0;
    }
    printk("    my_dev my_value : %d\n",  my_value);
    
	return 0;
}
/**
 * @brief This function is called on unloading the driver 
 */
static int dt_remove(struct platform_device *pdev) {
	printk("dt_probe - Now I am in the remove function\n");
	return 0;
}
/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init my_init(void) {
	printk("dt_probe - Loading the driver...\n");
	if(platform_driver_register(&my_driver)) {
		printk("dt_probe - Error! Could not load driver\n");
		return -1;
	}
	return 0;
}
/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit my_exit(void) {
	printk("dt_probe - Unload driver\n");
	platform_driver_unregister(&my_driver);
}

module_init(my_init);
module_exit(my_exit);
