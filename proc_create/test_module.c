#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kmk");
MODULE_DESCRIPTION("Basic read loadable kernel module");

static struct proc_dir_entry *custom_proc_node;

static ssize_t test_module_read(struct file* file_pointer, 
		  char *userspace_buffer, 
		  size_t count, 
		  loff_t* offset) {
	printk("test_module_read\n");
	return 0;
}

struct proc_ops driver_proc_ops = {
	.proc_read = test_module_read
};

/**
 * @breif This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("test_module_init: entry\n");

	custom_proc_node = proc_create("test_module",
		       	               0, 
				       NULL,
				       &driver_proc_ops);
                                       
	printk("test_module_init: exit\n");
	return 0;
}

/**
 * @breif This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("test_module_exit: entry\n");
	
	proc_remove(custom_proc_node);

	printk("test_module_exit: exit\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
