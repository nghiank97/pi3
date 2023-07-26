
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knghia");
MODULE_DESCRIPTION("a simple i2c device");

#define CRA_REG_M       0x00
#define CRB_REG_M       0x01
#define MR_REG_M        0x02
#define OUT_X_H_M       0x03
#define OUT_X_L_M       0x04
#define OUT_Z_H_M       0x05
#define OUT_Z_L_M       0x06
#define OUT_Y_H_M       0x07
#define OUT_Y_L_M       0x08
#define SR_REG_M        0x09
#define IRA_REG_M       0x0a
#define IRB_REG_M       0x0b
#define IRC_REG_M       0x0c
#define TEMP_OUT_H_M    0x31
#define TEMP_OUT_L_M    0x32

static struct proc_dir_entry *proc_folder;
static struct proc_dir_entry *proc_value;
static struct i2c_client *lsm303dlhc_client;

static int lsm303dlhc_probe(struct i2c_client *client, const struct i2c_device_id *id);
static void lsm303dlhc_remove(struct i2c_client *client);

static struct of_device_id lsm303dlhc_ids[] = {
	{
		.compatible = "brightlight,lsm303dlhc",
	}, { /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, lsm303dlhc_ids);

static struct i2c_device_id lsm303dlhc[] = {
	{"lsm303dlhc", 0x1e},
	{ },
};

MODULE_DEVICE_TABLE(i2c, lsm303dlhc);

static struct i2c_driver lsm303dlhc_driver = {
	.probe = lsm303dlhc_probe,
	.remove = lsm303dlhc_remove,
	.driver = {
		.name = "lsm303dlhc",
		.of_match_table = of_match_ptr(lsm303dlhc_ids),
	},
};

char buff_data[255];
int16_t buff_len = 0;

uint16_t raw_x, raw_y, raw_z;
uint8_t LSM303DLHC_Data[6];

#define Gauss_LSB_XY	1100
#define Gauss_LSB_Z		980

int16_t magnetic_x, magnetic_y, magnetic_z;

static int proc_fs_open(struct inode *Inode, struct file *File) {
	i2c_smbus_read_i2c_block_data(lsm303dlhc_client, OUT_X_H_M, 6, LSM303DLHC_Data);
	raw_x = (LSM303DLHC_Data[0]<<8)|LSM303DLHC_Data[1];
	raw_z = (LSM303DLHC_Data[2]<<8)|LSM303DLHC_Data[3];
	raw_y = (LSM303DLHC_Data[4]<<8)|LSM303DLHC_Data[5];

	magnetic_x = raw_x/Gauss_LSB_XY*100;
	magnetic_y = raw_y/Gauss_LSB_XY*100;
	magnetic_z = raw_z/Gauss_LSB_Z*100;
	return 0;
}

static ssize_t proc_fs_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	buff_len = sprintf(user_buffer,"%03d %03d %03d\n", magnetic_x, magnetic_y, magnetic_z);
	return buff_len;
}

static const struct proc_ops fops = {
	.proc_open = proc_fs_open,
	.proc_read = proc_fs_read,
};

static int lsm303dlhc_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    int ret;
    uint8_t iden[4] = {0};

	if(client->addr != 0x1e) {
		printk("	Error! Wrong I2C address!\n");
		return -1;
	}

	lsm303dlhc_client = client;
    ret = i2c_smbus_write_byte_data(lsm303dlhc_client, CRA_REG_M, 0x00);
    ret = i2c_smbus_write_byte_data(lsm303dlhc_client, CRB_REG_M, 0x20);
    ret = i2c_smbus_write_byte_data(lsm303dlhc_client, MR_REG_M, 0x00);
    ret = i2c_smbus_read_i2c_block_data(lsm303dlhc_client, IRA_REG_M, 4, iden);

	proc_folder = proc_mkdir("lsm303dlhc", NULL);
	if(proc_folder == NULL) {
		printk("	Error! Can't create /proc/lsm303dlhc\n");
		return -ENOMEM;
	}

	proc_value = proc_create("value_x", 0666, proc_folder, &fops);
	if(proc_value == NULL) {
		printk("	Error! Can't create /proc/lsm303dlhc/value\n");
		proc_remove(proc_folder);
		return -ENOMEM;
	}

	printk("	Info! Inserted a device\n");
	return 0;
}

static void lsm303dlhc_remove(struct i2c_client *client) {
	proc_remove(proc_value);
	proc_remove(proc_folder);
	printk("	Info! End!\n");
}

module_i2c_driver(lsm303dlhc_driver);
