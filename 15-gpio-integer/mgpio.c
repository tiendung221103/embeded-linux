// mgpio.c — Character device điều khiển 1 GPIO bằng gpiolib (integer-based)

#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/gpio.h>
#include <linux/string.h>

// Prefix log: "mgpio: ..."
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#define DRIVER_NAME   "mgpio"
#define DEV_NAME      "mgpio"
#define BUF_LEN       32

// Tương thích class_create kernel >= 6.4
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,4,0)
  #define CLASS_CREATE(name) class_create(name)
#else
  #define CLASS_CREATE(name) class_create(THIS_MODULE, name)
#endif

// --- THAM SỐ MODULE: số GPIO TOÀN CỤC (global number) ---
static unsigned int gpio_num = 539;  // mặc định 27 (sẽ đổi bằng tham số 'gpio=' khi insmod)
module_param(gpio_num, uint, 0444);
MODULE_PARM_DESC(gpio_num, "Global GPIO number (integer-based gpiolib)");

// ---------------------------------------------------------
static dev_t devno;
static struct cdev mgpio_cdev;
static struct class *mgpio_class;
static struct device *mgpio_dev;
static DEFINE_MUTEX(mgpio_lock);
static int gpio_state = 0;

static ssize_t mgpio_write(struct file *filp, const char __user *ubuf,
                           size_t count, loff_t *ppos)
{
    char kbuf[BUF_LEN];
    size_t len = min(count, (size_t)(BUF_LEN - 1));

    if (copy_from_user(kbuf, ubuf, len))
        return -EFAULT;
    kbuf[len] = '\0';

    while (len && (kbuf[len-1] == '\n' || kbuf[len-1] == '\r' ||
                   kbuf[len-1] == ' '  || kbuf[len-1] == '\t'))
        kbuf[--len] = '\0';

    mutex_lock(&mgpio_lock);

    if (!strncasecmp(kbuf, "on", 2)) {
        gpio_set_value(gpio_num, 1);
        gpio_state = 1;
        pr_info("GPIO%u = 1 (ON)\n", gpio_num);
    } else if (!strncasecmp(kbuf, "off", 3)) {
        gpio_set_value(gpio_num, 0);
        gpio_state = 0;
        pr_info("GPIO%u = 0 (OFF)\n", gpio_num);
    } else {
        mutex_unlock(&mgpio_lock);
        pr_info("unknown cmd \"%s\" (use \"on\"/\"off\")\n", kbuf);
        return -EINVAL;
    }

    mutex_unlock(&mgpio_lock);
    return count;
}

static ssize_t mgpio_read(struct file *filp, char __user *ubuf,
                          size_t count, loff_t *ppos)
{
    char tmp[8];
    int n = scnprintf(tmp, sizeof(tmp), "%s\n", gpio_state ? "on" : "off");
    if (*ppos > 0) return 0;
    if (count < n) return -EINVAL;
    if (copy_to_user(ubuf, tmp, n)) return -EFAULT;
    *ppos = n;
    return n;
}

static int mgpio_open(struct inode *inode, struct file *file)
{
    try_module_get(THIS_MODULE);
    return 0;
}

static int mgpio_release(struct inode *inode, struct file *file)
{
    module_put(THIS_MODULE);
    return 0;
}

static const struct file_operations mgpio_fops = {
    .owner   = THIS_MODULE,
    .write   = mgpio_write,
    .read    = mgpio_read,
    .open    = mgpio_open,
    .release = mgpio_release,
};

static int __init mgpio_init(void)
{
    int ret;

    pr_info("init\n");

    if (!gpio_is_valid(gpio_num)) {
        pr_err("invalid GPIO %u\n", gpio_num);
        return -EINVAL;
    }

    ret = gpio_request(gpio_num, DRIVER_NAME);
    if (ret) {
        pr_err("gpio_request(%u) failed: %d\n", gpio_num, ret);
        return ret;
    }

    ret = gpio_direction_output(gpio_num, 0);
    if (ret) {
        pr_err("gpio_direction_output failed: %d\n", ret);
        gpio_free(gpio_num);
        return ret;
    }
    gpio_state = 0;

    ret = alloc_chrdev_region(&devno, 0, 1, DEV_NAME);
    if (ret) {
        pr_err("alloc_chrdev_region failed: %d\n", ret);
        gpio_free(gpio_num);
        return ret;
    }

    cdev_init(&mgpio_cdev, &mgpio_fops);
    ret = cdev_add(&mgpio_cdev, devno, 1);
    if (ret) {
        pr_err("cdev_add failed: %d\n", ret);
        unregister_chrdev_region(devno, 1);
        gpio_free(gpio_num);
        return ret;
    }

    mgpio_class = CLASS_CREATE(DEV_NAME);
    if (IS_ERR(mgpio_class)) {
        ret = PTR_ERR(mgpio_class);
        pr_err("class_create failed: %d\n", ret);
        cdev_del(&mgpio_cdev);
        unregister_chrdev_region(devno, 1);
        gpio_free(gpio_num);
        return ret;
    }

    mgpio_dev = device_create(mgpio_class, NULL, devno, NULL, DEV_NAME);
    if (IS_ERR(mgpio_dev)) {
        ret = PTR_ERR(mgpio_dev);
        pr_err("device_create failed: %d\n", ret);
        class_destroy(mgpio_class);
        cdev_del(&mgpio_cdev);
        unregister_chrdev_region(devno, 1);
        gpio_free(gpio_num);
        return ret;
    }

    pr_info("created /dev/%s (major=%d minor=%d), gpio=%u\n",
            DEV_NAME, MAJOR(devno), MINOR(devno), gpio_num);
    return 0;
}

static void __exit mgpio_exit(void)
{
    pr_info("exit\n");

    if (mgpio_dev && !IS_ERR(mgpio_dev))
        device_destroy(mgpio_class, devno);
    if (mgpio_class && !IS_ERR(mgpio_class))
        class_destroy(mgpio_class);

    cdev_del(&mgpio_cdev);
    unregister_chrdev_region(devno, 1);

    gpio_set_value(gpio_num, 0);
    gpio_free(gpio_num);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("you");
MODULE_DESCRIPTION("Character device /dev/mgpio using gpiolib (integer-based)");
MODULE_VERSION("1.1");

module_init(mgpio_init);
module_exit(mgpio_exit);
