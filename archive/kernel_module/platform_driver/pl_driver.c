#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/cdev.h>


MODULE_LICENSE("GPL");

#define DRIVER_NAME "sample"
static unsigned int gpio_number;

dev_t dev_id;
struct cdev ch_dev;
struct class* dev_class;

int init_result;


ssize_t gpio_read(struct file* F, char* buff, size_t count, loff_t* offset)
{
    return count;
}
ssize_t gpio_write(struct file* F, const char* buff, size_t count, loff_t* offset)
{
    char temp;
    if(copy_from_user(&temp, buff, count)){
        return -EFAULT;
    }
    printk("receive write command\n");
    switch (temp)
    {
    case '0':
        printk("write 0 \n");
        gpio_direction_output(gpio_number, 0);
        break;
    case '1':
        printk("write 1 \n");
        gpio_direction_output(gpio_number, 1);
        break;
    
    default:
        break;
    }
    return count;
}
int gpio_open(struct inode* inode, struct file* F)
{
    return 0;
}
int gpio_close(struct inode* inode, struct file* F)
{
    return 0;
}
struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = gpio_open,
    .release = gpio_close,
    .write = gpio_write,
    .read = gpio_read
};

int sample_drv_probe(struct platform_device* pdev)
{
    printk("enter sample_drv_probe\n");
    struct device_node* np = pdev->dev.of_node;
    of_property_read_u32(np, "led-number", &gpio_number);
    printk("value read from device node: %d\n", gpio_number);
    int res = alloc_chrdev_region(&dev_id, 0, 1, "gpio_drv");
    if(res < 0)
    {
        printk("Registration Failed\n");
        return -1;
    }
    printk("Major nr: %d\n", MAJOR(res));
    
    dev_class = class_create(THIS_MODULE, "gpio drv");
    device_create(dev_class, NULL, dev_id, NULL, "gpio_driver");
    cdev_init(&ch_dev, &fops);
    cdev_add(&ch_dev, dev_id, 1);
    return 0;
}

int sample_drv_remove(struct platform_device* pdev)
{
    cdev_del(&ch_dev);
    device_destroy(dev_class, dev_id);
    class_destroy(dev_class);
    unregister_chrdev_region(dev_id, 1);
    printk("Device unregisterd\n");
    return 0;
}
static const struct of_device_id my_of_ids[] =
{
    {.compatible = "sample-gpio-led"},
    {},
};
MODULE_DEVICE_TABLE(of, my_of_ids);
static struct platform_driver sample_pldriver = 
{
    .probe = sample_drv_probe,
    .remove = sample_drv_remove,
    .driver = {
        .name = "my-leds",
        .of_match_table = of_match_ptr(my_of_ids),
        .owner = THIS_MODULE
    }
};

int _module_init(void)
{
    printk("Init module\n");

    int st = platform_driver_register(&sample_pldriver);
    printk("platform init status: %d\n", st);
    return 0;
}
void _module_exit(void)
{
    platform_driver_unregister(&sample_pldriver);
    printk("Module exit\n");
}
module_init(_module_init);
module_exit(_module_exit);
