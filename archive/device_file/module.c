#include <linux/module.h>
#include <linux/init.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Htt");
MODULE_DESCRIPTION("testing");


static int __init ModuleInit(void)
{
    printk("hello\n");
    return 0;
}

static void __exit ModuleExit(void)
{
    printk("exit\n");
}

module_init(ModuleInit);

module_exit(ModuleExit);