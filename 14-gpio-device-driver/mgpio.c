// mgpio.c - Character device driver điều khiển GPIO27 cho Raspberry Pi
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/bitops.h>

#define DRIVER_NAME   "mgpio"
#define CLASS_NAME    "mgpio"
#define DEV_NAME      "mgpio"

// BCM2711 (Raspberry Pi 4). Với Pi Zero W dùng 0x20200000.
#define GPIO_BASE_PHYS_DEFAULT  0xFE200000UL

// GPIO register offsets (tính từ base)
#define GPFSEL2_OFF  0x08  // GPIO 20..29 function select
#define GPSET0_OFF   0x1C  // Set GPIO 0..31
#define GPCLR0_OFF   0x28  // Clear GPIO 0..31

#define GPIO_NUM     27
#define GPFSEL27_SHIFT 21

static unsigned long gpio_phys_base = GPIO_BASE_PHYS_DEFAULT;
module_param(gpio_phys_base, ulong, 0444);
MODULE_PARM_DESC(gpio_phys_base, "Physical base address of GPIO controller");

static void __iomem *gpio_base;
static dev_t devno;
static struct cdev mgpio_cdev;
static struct class *mgpio_class;
static struct device *mgpio_dev;
static DEFINE_MUTEX(mgpio_lock);

static inline void __iomem *GPIO_REG(unsigned int off)
{
    return (void __iomem *)((char __iomem *)gpio_base + off);
}

static inline void gpio27_as_output(void)
{
    u32 val = readl(GPIO_REG(GPFSEL2_OFF));
    val &= ~(0x7 << GPFSEL27_SHIFT);   // clear FSEL27
    val |=  (0x1 << GPFSEL27_SHIFT);   // set to 001 = output
    writel(val, GPIO_REG(GPFSEL2_OFF));
}

static inline void gpio27_on(void)
{
    writel(BIT(GPIO_NUM), GPIO_REG(GPSET0_OFF));
}

static inline void gpio27_off(void)
{
    writel(BIT(GPIO_NUM), GPIO_REG(GPCLR0_OFF));
}

static int mgpio_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int mgpio_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t mgpio_write(struct file *file, const char __user *ubuf,
                           size_t len, loff_t *ppos)
{
    char kbuf[16];
    size_t n = min(len, sizeof(kbuf) - 1);
    int i;

    if (!n) return 0;
    if (copy_from_user(kbuf, ubuf, n)) return -EFAULT;
    kbuf[n] = '\0';

    // Trim newline/space và hạ chữ thường
    strim(kbuf);
    for (i = 0; kbuf[i]; i++) kbuf[i] = tolower(kbuf[i]);

    mutex_lock(&mgpio_lock);

    if (!strcmp(kbuf, "on")) {
        gpio27_on();
        pr_info(DRIVER_NAME ": set ON\n");
    } else if (!strcmp(kbuf, "off")) {
        gpio27_off();
        pr_info(DRIVER_NAME ": set OFF\n");
    } else {
        pr_info(DRIVER_NAME ": unknown cmd '%s' (use 'on' or 'off')\n", kbuf);
        mutex_unlock(&mgpio_lock);
        return -EINVAL;
    }

    mutex_unlock(&mgpio_lock);
    return len; // báo đã tiêu thụ toàn bộ dữ liệu
}

static const struct file_operations mgpio_fops = {
    .owner   = THIS_MODULE,
    .open    = mgpio_open,
    .release = mgpio_release,
    .write   = mgpio_write,
};

static int __init mgpio_init(void)
{
    int ret;

    // 1) Map GPIO
    gpio_base = ioremap(gpio_phys_base, 0x1000);
    if (!gpio_base) {
        pr_err(DRIVER_NAME ": ioremap failed for 0x%lx\n", gpio_phys_base);
        return -ENOMEM;
    }

    // 2) Đăng ký char device -> /dev/mgpio
    ret = alloc_chrdev_region(&devno, 0, 1, DRIVER_NAME);
    if (ret) {
        pr_err(DRIVER_NAME ": alloc_chrdev_region failed: %d\n", ret);
        goto err_unmap;
    }

    cdev_init(&mgpio_cdev, &mgpio_fops);
    ret = cdev_add(&mgpio_cdev, devno, 1);
    if (ret) {
        pr_err(DRIVER_NAME ": cdev_add failed: %d\n", ret);
        goto err_unregister;
    }

    mgpio_class = class_create (CLASS_NAME);
    if (IS_ERR(mgpio_class)) {
        ret = PTR_ERR(mgpio_class);
        pr_err(DRIVER_NAME ": class_create failed: %d\n", ret);
        goto err_cdevdel;
    }

    mgpio_dev = device_create(mgpio_class, NULL, devno, NULL, DEV_NAME);
    if (IS_ERR(mgpio_dev)) {
        ret = PTR_ERR(mgpio_dev);
        pr_err(DRIVER_NAME ": device_create failed: %d\n", ret);
        goto err_class;
    }

    mutex_init(&mgpio_lock);

    // 3) Cấu hình GPIO27 làm output (không bật/tắt ở init)
    gpio27_as_output();

    pr_info(DRIVER_NAME ": loaded. /dev/%s ready (base=0x%lx)\n",
            DEV_NAME, gpio_phys_base);
    return 0;

err_class:
    class_destroy(mgpio_class);
err_cdevdel:
    cdev_del(&mgpio_cdev);
err_unregister:
    unregister_chrdev_region(devno, 1);
err_unmap:
    iounmap(gpio_base);
    return ret;
}

static void __exit mgpio_exit(void)
{
    // Theo giai đoạn 1 có thể tắt GPIO khi remove để an toàn
    gpio27_off();

    device_destroy(mgpio_class, devno);
    class_destroy(mgpio_class);
    cdev_del(&mgpio_cdev);
    unregister_chrdev_region(devno, 1);

    iounmap(gpio_base);
    pr_info(DRIVER_NAME ": unloaded\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Character device driver for Raspberry Pi GPIO27 (/dev/mgpio)");
MODULE_VERSION("1.0");

module_init(mgpio_init);
module_exit(mgpio_exit);
