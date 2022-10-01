#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x7c30cdd7, "module_layout" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xd7b9544f, "i2c_del_driver" },
	{ 0x7586f234, "i2c_unregister_device" },
	{ 0x7fa4a79e, "kthread_stop" },
	{ 0xc69506dc, "class_destroy" },
	{ 0xe3a719e1, "device_destroy" },
	{ 0xe247e3c3, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x19dbe92b, "wake_up_process" },
	{ 0x2565b53, "kthread_create_on_node" },
	{ 0xaa9cdc86, "i2c_put_adapter" },
	{ 0x2009c6a2, "i2c_register_driver" },
	{ 0x4fee8c20, "i2c_new_client_device" },
	{ 0x6a758a2d, "i2c_get_adapter" },
	{ 0x2ea9c462, "device_create" },
	{ 0x75d7cdf1, "__class_create" },
	{ 0xe50f8328, "cdev_add" },
	{ 0x7137c132, "cdev_alloc" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x5f754e5a, "memset" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x4679640a, "i2c_transfer_buffer_flags" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0xf9a482f9, "msleep" },
	{ 0x92997ed8, "_printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "9F904E4EC6A761AC406E2CA");
