#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define TOTAL_MINOR                1
#define BASE_MINOR				   0

#define DEVICE_NAME  			"SHT30_DRIVER" //Character device to handle PCF8574
#define DEVICE_CLASS 			"SHT30_CLASS"

#define I2C_BUS_AVAILABLE		1			/* /dev/i2c-1 */
#define SLAVE_DEVICE_NAME		"SHT30"	/* Device and Driver Name */
#define SHT30_SLAVE_ADDRESS	0x44		/* PCF8574 I2C address */

MODULE_LICENSE("GPL");

static struct i2c_adapter *sht30_i2c_adapter = NULL;
static struct i2c_client  *sht30_i2c_client = NULL;

static const struct i2c_device_id pcf8574_id[] = {
		{ SLAVE_DEVICE_NAME, 0 }, 
		{ }
};

int i2c_prop_new(struct i2c_client *sth)
{
    printk("new prop function\n");
    return 0;
}
void i2c_alert(struct i2c_client * i2c_str, enum i2c_alert_protocol protocol, unsigned int data)
{
    printk("new alert function\n");
}

static struct i2c_driver sht30_driver = {
	.driver = {
		.name = SLAVE_DEVICE_NAME,
		.owner = THIS_MODULE,
	},
    .probe_new = i2c_prop_new,
	.alert = i2c_alert
};

static struct i2c_board_info sht30_i2c_board_info = {
	I2C_BOARD_INFO(SLAVE_DEVICE_NAME, SHT30_SLAVE_ADDRESS)
};

dev_t dev_id;

struct cdev *character_device;
struct class *device_class;
struct device *device;

int dev_open(struct inode *, struct file *);
int dev_close(struct inode *, struct file *);
ssize_t dev_read(struct file*, char __user *, size_t, loff_t *);
ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);

struct file_operations fops = {
	.open = dev_open,
	.release = dev_close,
	.read = dev_read,
	.write = dev_write,
};

int dev_open(struct inode *inodep, struct file *filep)
{
	printk("open\n");
	return 0;
}

int dev_close(struct inode *inodep, struct file *filep)
{
	printk("close\n");
	return 0;
}

ssize_t dev_read(struct file*filep, char __user *buf, size_t len, loff_t *offset)
{
	printk("read\n");
	return 0;
}

char data[100];
ssize_t dev_write(struct file*filep, const char __user *buf, size_t len, loff_t *offset)
{
	memset(data, 0, sizeof(data));
	copy_from_user(data, buf, len);
	printk("write data: %s\n", data);
	return sizeof(data);
}
int pressed_times = 0;
int irq_result = 0;
irq_handler_t irq_1_handler(unsigned int irq, void* dev_id, struct pt_regs *regs){
	printk("Device ID %s; Keyboard interrupt occured %d\n", (char*)dev_id, pressed_times);
	pressed_times += 1;
    return (irq_handler_t) IRQ_HANDLED;
}

int irq_number;
struct 	task_struct *kthread_1;

int thread_function(void *kernel_data) {
	msleep(500);
	while(!kthread_should_stop()){
                char write_value[2] = {0xe0, 0x00};
				int ret1 = i2c_master_send(sht30_i2c_client, write_value, 2);
                msleep(5);
				printk("write result: %d\n", ret1);
				char temp[6];
				int nums = i2c_master_recv(sht30_i2c_client, temp, 6);
				printk("read result: %d\n", nums);
				printk("all bytes: ");
				int i = 0;
				while(i != 6)
				{
					printk("%d ", temp[i++]);
				}
				printk("\n");
                msleep(5000);
	}
	return 0;
}

int device_init(void)
{
	int ret;

	ret = alloc_chrdev_region(&dev_id, BASE_MINOR, TOTAL_MINOR, "fops_alloc_chrdev_region");
	if(ret)
	{
		printk("can not register major no\n");
		return ret;
	}
	printk(KERN_INFO "register successfully major now is: %d\n", MAJOR(dev_id));
	character_device = cdev_alloc();
	character_device->owner = THIS_MODULE;
	character_device->ops = &fops;
	character_device->dev = dev_id;

	cdev_add(character_device, dev_id, 1);
	device_class = class_create(THIS_MODULE, DEVICE_CLASS);
	device = device_create(device_class, NULL, dev_id, NULL, DEVICE_NAME);

	//Add driver for PCF8574
	sht30_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

	if(sht30_i2c_adapter != NULL) {
		// sht30_i2c_board_info.irq = 200;
		sht30_i2c_client = i2c_new_client_device(sht30_i2c_adapter, &sht30_i2c_board_info);
		if(sht30_i2c_client != NULL) {
			if(i2c_add_driver(&sht30_driver) != -1) ret = 0;
			else printk("Can't add driver...\n");
		}
		i2c_put_adapter(sht30_i2c_adapter);
	}
	printk("sht30 driver added!\n");
	//Then turn on a specifc GPIO when insmod successfully
	

// if(gpio_direction_input(2)){
//         printk("Unable to set GPIO %d to input\n", 2);
//     }
// 	#define GPIO		2
// 	irq_number = gpio_to_irq(GPIO);
// 	// #define IRQ_1 200
// 	if (request_irq(irq_number, (irq_handler_t) irq_1_handler, IRQF_SHARED, "DEV_NAME IRQ_1", "DEV_ID") != 0){
//         printk("Can't request interrupt number %d\n", irq_number);
//     } else {
// 		irq_result = 1;
// 		printk("Request interrupt number %d successfully\n", irq_number);
// 	}

#define WRITE_SIZE	2
	char write_value[WRITE_SIZE] = {0x22, 0x20};
	int ret1 = i2c_master_send(sht30_i2c_client, write_value, WRITE_SIZE);
    printk("write result: %d\n", sht30_i2c_client->irq);

	kthread_1 = kthread_create(thread_function, NULL, "kthread_1");
	if(kthread_1 != NULL){
		wake_up_process(kthread_1); //Start kthread_1
		printk("kthread_1 was created and is running now!\n");
	}
	else {
		printk("kthread_1 could not be created!\n");
		return -1;
	}
	return 0;
}

void device_exit(void)
{
	printk("Device remove\n");
	unregister_chrdev_region(dev_id, TOTAL_MINOR); 
	cdev_del(character_device);
	device_destroy(device_class, dev_id);
	class_destroy(device_class);
	kthread_stop(kthread_1);

	i2c_unregister_device(sht30_i2c_client);
	i2c_del_driver(&sht30_driver);

	if(irq_result == 1) free_irq(irq_number, "DEV_ID");
}

module_init(device_init);
module_exit(device_exit);