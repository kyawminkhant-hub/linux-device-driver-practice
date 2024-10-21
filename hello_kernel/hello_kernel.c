#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kmk");
MODULE_DESCRIPTION("Hello kernel module");

/**
 * @breif This function is called, when the module is loaded into the kernel
 */
static int __init hello_init(void) {
        printk("Hello Kernel!\n");
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

