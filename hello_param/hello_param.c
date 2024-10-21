#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kmk");
MODULE_DESCRIPTION("A hello kernel module using parameter");

static char *what = "world";
module_param(what, charp, 0644);
MODULE_PARM_DESC(what, "What comes after hello message");

static int howmany = 1;
module_param(howmany, int, 0644);
MODULE_PARM_DESC(howmany, "Number of message calls");

/**
 * @breif This function is called, when the module is loaded into the kernel
 */
static int __init hello_init(void) {
	int i;
	for (i = 0; i < howmany; i++)
		printk("(%d) Hello, %s\n", i, what);
	return 0;
}

/**
 * @breif This function is called, when the module is removed from the kernel
 */
static void __exit hello_exit(void) {
        printk("Goodbye, Kernel\n");
        //pr_alert("Goodbye, Kernel\n");
}

module_init(hello_init);
module_exit(hello_exit);

