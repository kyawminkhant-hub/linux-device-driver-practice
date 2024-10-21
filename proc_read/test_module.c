#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

/* Module Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kmk");
MODULE_DESCRIPTION("Basic read loadable kernel module");

#define PROCFS_NAME		"test_module"
#define PROCFS_MAX_SIZE		1024

/* kernel space buffer */
static char procfs_buffer[PROCFS_MAX_SIZE];

/* size of the buffer*/
static unsigned long procfs_buffer_size = 0;

/* Create /proc file */
static struct proc_dir_entry *custom_proc_node;

static ssize_t test_module_read(struct file* file_pointer, 
		  char *userspace_buffer, 
		  size_t count, 
		  loff_t* offset) {
	
	static int count_call = 1; 

	/* snprintf is used to concatenate two strings */	
       	procfs_buffer_size = snprintf(procfs_buffer, PROCFS_MAX_SIZE, "Read operation: %d times called\n", count_call) + 1; 
	
	int result;
	
	printk("%s_read: %d times called\n", PROCFS_NAME, count_call);

	if (*offset >= procfs_buffer_size)
		return 0;

	result = copy_to_user(userspace_buffer, procfs_buffer, procfs_buffer_size);
	
	*offset += procfs_buffer_size; 
	count_call ++;

	return procfs_buffer_size;
}

struct proc_ops driver_proc_ops = {
	.proc_read = test_module_read
};

/**
 * @breif This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("%s __init: entry\n", PROCFS_NAME);

	custom_proc_node = proc_create(PROCFS_NAME,
		       	               0, 
				       NULL,
				       &driver_proc_ops);
                                       
	printk("%s __init: exit\n", PROCFS_NAME);
	return 0;
}

/**
 * @breif This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("%s __exit: entry\n", PROCFS_NAME);
	
	proc_remove(custom_proc_node);

	printk("%s __exit: exit\n", PROCFS_NAME);
}

module_init(ModuleInit);
module_exit(ModuleExit);
