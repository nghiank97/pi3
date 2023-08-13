#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/spi/spi.h>

#define SPI_CLASS_NAME              "mySPIClass"
#define MY_DEVICE_NAME_0            "mySPI_slave-0"
#define MY_DEVICE_MAX_DEVICES       (2)
#define MY_DEVICE_USER_BUF_LENGTH   (32)

static dev_t my_device_dev_t;
static struct device *my_device_dev;
static struct cdev *my_device_cdev;
static struct class *my_device_class;
static struct spi_device *my_device_spi;

struct spi_transfer spi_element[] = {
    {
        .len = 2,
        .cs_change = 0,
    }, {
        .len = 2,
    },
};

static const struct of_device_id my_device_of_match[] = {
    { 
      .compatible = "vend,mySPI_slave_0", 
      .data = (void *) 0,
    },
    { }
};
MODULE_DEVICE_TABLE(of, my_device_of_match);

// (2) finding a match from stripped device-tree (no vendor part)
static const struct spi_device_id my_device_id[] = {
    { "mySPI_slave", 0 },
    { }
};

MODULE_DEVICE_TABLE(spi, my_device_id);

static int my_device_open(struct inode *inode, struct file *filp)
{    
    printk("mySPI_slave::my_device_open called.\n");
    return 0;
}

static int my_device_release(struct inode *inode, struct file *file)
{
    printk("mySPI_slave::my_device_release called.\n");
    return 0;
}

static ssize_t my_device_read(struct file *filp, char __user * buf, size_t lbuf, loff_t * ppos)
{
    printk("mySPI_slave::my_device_read called.\n");
    return lbuf;
}

static ssize_t my_device_write(struct file *filep, const char __user * buf, size_t lbuf, loff_t * ppos)
{
    printk("mySPI_slave::my_device_write called.\n");
    return lbuf;
}

static const struct file_operations my_device_fops = {
    .owner =    THIS_MODULE,
    .write =    my_device_write,
    .read =        my_device_read,
    .open =        my_device_open,
    .release =    my_device_release
};


// Initialize SPI interface...
static int my_device_probe(struct spi_device *spi)
{
    int err;
    unsigned char ch16[] = {0x5A, 0x5A};    
    unsigned char rx16[] = {0x00, 0x00};
    int devData = 0;
    printk("mySPI_slave::my_device_probe called.\n");

    spi->max_speed_hz = 10000000; // ?
    spi->bits_per_word = 16;
    spi->mode = (0);
    
    err = spi_setup(spi);
    if (err < 0) {
        printk("mySPI_slave::my_device_probe spi_setup failed!\n");
        return err;
    }

    printk("spi_setup ok, cs: %d\n", spi->chip_select);
    printk("start data transfer...\n");

    spi_element[0].tx_buf = ch16;
    spi_element[1].rx_buf = rx16;

    err = spi_sync_transfer(spi, spi_element, ARRAY_SIZE(spi_element));
    printk("data size: %d\n", sizeof(rx16));
    if (err < 0) {
        printk("mySPI_slave::my_device_probe spi_sync_transfer failed!\n");
        return err;
    }

    my_device_spi = spi;
    printk("transfer ok\n");
    

    // define a device class
    my_device_class = class_create(THIS_MODULE, SPI_CLASS_NAME);
    if (my_device_class == NULL) {
        printk("mySPI_slave::my_device_probe class_create failed!\n");
        return -1;
    }

    err = alloc_chrdev_region(&my_device_dev_t, 0, MY_DEVICE_MAX_DEVICES, MY_DEVICE_NAME_0);

    if (err < 0) {
        printk("mySPI_slave::my_device_probe alloc_chrdev_region failed!\n");
        class_destroy(my_device_class);
        return err;
    }
    
    my_device_cdev = cdev_alloc();
    if (!(my_device_cdev)) {
        printk("mySPI_slave::my_device_probe cdev_alloc failed!\n");
        unregister_chrdev_region(my_device_dev_t, MY_DEVICE_MAX_DEVICES);
        class_destroy(my_device_class);
        return -1;
    }

    cdev_init(my_device_cdev, &my_device_fops);

    err = cdev_add(my_device_cdev, my_device_dev_t, MY_DEVICE_MAX_DEVICES);
    if(err < 0) {
        printk("mySPI_slave::my_device_probe cdev_add failed!\n");
        cdev_del(my_device_cdev);
        unregister_chrdev_region(my_device_dev_t, MY_DEVICE_MAX_DEVICES);
        class_destroy(my_device_class);
        return err;
    }

    my_device_dev = device_create(my_device_class, NULL, my_device_dev_t, NULL, "%s", MY_DEVICE_NAME_0);
    
    return 0;
}

static void my_device_remove(struct spi_device *spi)
{
    printk("my_device_remove() called.\n");
    device_destroy(my_device_class, my_device_dev_t);
    if(my_device_cdev) {
        cdev_del(my_device_cdev);
    }
    unregister_chrdev_region(my_device_dev_t, MY_DEVICE_MAX_DEVICES);
    class_destroy(my_device_class);
}

static struct spi_driver my_device_spi_driver = {
    .driver = {
        .owner =    THIS_MODULE,
        .name =        "mySPI_slave",
        .of_match_table = of_match_ptr(my_device_of_match),
    },
    .id_table =    my_device_id,
    .probe =    my_device_probe,
    .remove =    my_device_remove
};
module_spi_driver(my_device_spi_driver);

MODULE_LICENSE("GPL");
