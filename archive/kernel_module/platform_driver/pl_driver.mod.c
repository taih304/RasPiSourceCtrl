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
	{ 0xf045287f, "platform_driver_unregister" },
	{ 0xa25d9187, "__platform_driver_register" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0x9d4cb3b3, "gpiod_direction_output_raw" },
	{ 0xa32498f0, "gpio_to_desc" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0xe50f8328, "cdev_add" },
	{ 0x322b4fd3, "cdev_init" },
	{ 0x2ea9c462, "device_create" },
	{ 0x75d7cdf1, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x517e9d4f, "of_property_read_variable_u32_array" },
	{ 0x92997ed8, "_printk" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xc69506dc, "class_destroy" },
	{ 0xe3a719e1, "device_destroy" },
	{ 0xe247e3c3, "cdev_del" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Csample-gpio-led");
MODULE_ALIAS("of:N*T*Csample-gpio-ledC*");

MODULE_INFO(srcversion, "415FDCFC45B261FCC4FA198");
