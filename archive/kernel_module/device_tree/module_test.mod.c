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
	{ 0x7fa4a79e, "kthread_stop" },
	{ 0xedc03953, "iounmap" },
	{ 0x92997ed8, "_printk" },
	{ 0x19dbe92b, "wake_up_process" },
	{ 0x2565b53, "kthread_create_on_node" },
	{ 0xe97c4103, "ioremap" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0xf9a482f9, "msleep" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "DF259B318F1D8ED2A8B5DCD");
