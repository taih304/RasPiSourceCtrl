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
	{ 0xc906d186, "param_ops_ulong" },
	{ 0x649a4c20, "param_array_ops" },
	{ 0xd1581d71, "param_ops_ushort" },
	{ 0xaba3dd23, "param_ops_byte" },
	{ 0xfc1a2bf9, "i2c_del_adapter" },
	{ 0x638c211d, "i2c_add_adapter" },
	{ 0x92997ed8, "_printk" },
	{ 0x2d6fcc06, "__kmalloc" },
	{ 0x37a0cba, "kfree" },
	{ 0xaf291ea1, "devm_kmalloc" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "0973A8A3D0F4279FABD1945");
