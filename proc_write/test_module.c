#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kmk");
MODULE_DESCRIPTION("Basic read loadable kernel module");

#define PROCFS_NAME		"test_module"
#define PROCFS_MAX_SIZE		1024

/* create the /proc file */
static struct proc_dir_entry *custom_proc_node;

/* kernel space buffer */
static char procfs_buffer[PROCFS_MAX_SIZE];

/* size of the kernel space buffer */
static unsigned long procfs_buffer_size = 0;

/**
 * @breif This function is called, when /proc file is written
*/
static ssize_t test_module_write(struct file* file_pointer, 
				 const char *userspace_buffer, 
				 size_t count, 
				 loff_t* offset) {
        
	/* prevent kernelspace buffer overflow */
	procfs_buffer_size = count;
	if (procfs_buffer_size > PROCFS_MAX_SIZE) {
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}

        /* write data from userspace buffer to kernelspace buffer */
        if (copy_from_user(procfs_buffer, userspace_buffer, procfs_buffer_size)) {
    		return -EFAULT; //Bad Address
	}

        printk("%s_write\n", PROCFS_NAME);
	return procfs_buffer_size;
}

/**
 * @breif This function is called, when /proc file is read
*/
static ssize_t test_module_read(struct file* file_pointer, 
		  		char *userspace_buffer, 
		  		size_t count, 
		  		loff_t* offset) {
	int result;
	printk("%s_read\n", PROCFS_NAME);

	if (*offset >= procfs_buffer_size)
		return 0;

	result = copy_to_user(userspace_buffer, procfs_buffer, procfs_buffer_size);
	*offset += procfs_buffer_size; 

	return procfs_buffer_size;
}

struct proc_ops driver_proc_ops = {
	.proc_read = test_module_read,	
	.proc_write = test_module_write,
};

/**
 * @breif This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
		
	printk("%s_init: entry\n", PROCFS_NAME);

	custom_proc_node = proc_create(PROCFS_NAME,
		       	               0666, 
				       NULL,
				       &driver_proc_ops);
                                       
	printk("%s_init: exit\n", PROCFS_NAME);
	return 0;
}

/**
 * @breif This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("%s_exit: entry\n", PROCFS_NAME);
	
	proc_remove(custom_proc_node);

	printk("%s_exit: exit\n", PROCFS_NAME);
}

module_init(ModuleInit);
module_exit(ModuleExit);
