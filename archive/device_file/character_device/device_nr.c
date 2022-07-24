#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Htt");
MODULE_DESCRIPTION("testing");


static int driver_open(struct inode* device_file, struct file* instance)
{
    printk("file opened\n");
    return 0;
}

static int driver_close(struct inode* device_file, struct file* instance)
{
    printk("file closed\n");
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close
};

#define MAJOR_NUMBER 90

static int __init ModuleInit(void)
{
    printk("hello\n");
    int res = register_chrdev(MAJOR_NUMBER, "my_dev_nr", &fops);
    if (res == 0)
    {
        printk("dev_nr - registered major number: %d, minor number: %d\n", MAJOR_NUMBER, 0);
    }else if(res == 0)
    {
        printk("dev_nr - registered major number: %d, minor number: %d\n", res>>20, res&0xfffff);
    }
    else{
        printk("fail to register device\n");
        return -1;
    }
    ioread32()
    return 0;
}

static void __exit ModuleExit(void)
{
    unregister_chrdev(MAJOR_NUMBER, "my_dev_nr");
    printk("exit\n");
}

module_init(ModuleInit);

module_exit(ModuleExit);