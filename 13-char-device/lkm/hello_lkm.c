#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
    pr_info("Hello, Kernel World!\n");
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("Goodbye, Kernel World!\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dũng");       // đổi tên nếu muốn
MODULE_DESCRIPTION("Simple Hello World Linux Kernel Module");
MODULE_VERSION("1.0");

module_init(hello_init);
module_exit(hello_exit);
