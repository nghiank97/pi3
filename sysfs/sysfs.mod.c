#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
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
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x92997ed8, "_printk" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x9778f277, "kernel_kobj" },
	{ 0xb3634719, "kobject_create_and_add" },
	{ 0xebc01eb5, "sysfs_create_file_ns" },
	{ 0xa197e303, "kobject_put" },
	{ 0x48c620ba, "sysfs_remove_file_ns" },
	{ 0xc84d16dc, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "9021506DD9A0740CD1E0037");
