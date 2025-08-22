// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio/machine.h>    // gpiod_lookup_table
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>
#include <linux/version.h>
#include <linux/string.h>

#define DRV_NAME   "mgpio_desc"
#define DEV_NAME   "mgpio_desc"

struct mgpio_dev {
    struct device *dev;
    struct gpio_desc *gpiod;
    struct mutex lock;
    int state;
    struct cdev cdev;      // cdev nhúng
    dev_t devt;
    struct gpiod_lookup_table *lt;  // lookup table (PC test)
};

static struct class *mgpio_class;

/* --- Module params để test trên PC với gpio-sim --- */
static bool use_lookup = true;           // bật lookup (no-DT)
module_param(use_lookup, bool, 0644);
MODULE_PARM_DESC(use_lookup, "Use gpiod_lookup_table + self platform_device (no DT)");

static char *chip_label = (char *)"sim0"; // label của gpiochip do gpio-sim tạo
module_param(chip_label, charp, 0644);
MODULE_PARM_DESC(chip_label, "gpiochip label (gpio-sim)");

static int line = 27;                    // line offset trên chip
module_param(line, int, 0644);
MODULE_PARM_DESC(line, "GPIO line offset on chip");

static struct platform_device *pdev_self;

static const struct of_device_id mgpio_of_match[] = {
    { .compatible = "mygpio-led" },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mgpio_of_match);

/* ------------ file ops ------------ */

static int mgpio_open(struct inode *inode, struct file *filp)
{
    struct mgpio_dev *mdev = container_of(inode->i_cdev, struct mgpio_dev, cdev);
    filp->private_data = mdev;
    return 0;
}

static ssize_t mgpio_read(struct file *filp, char __user *buf, size_t len, loff_t *ppos)
{
    struct mgpio_dev *mdev = filp->private_data;
    char tmp[4];
    int v;

    if (*ppos) return 0;
    v = gpiod_get_value_cansleep(mdev->gpiod);
    mdev->state = !!v;
    snprintf(tmp, sizeof(tmp), "%d\n", v);
    if (copy_to_user(buf, tmp, strlen(tmp))) return -EFAULT;
    *ppos = strlen(tmp);
    return strlen(tmp);
}

static ssize_t mgpio_write(struct file *filp, const char __user *buf, size_t len, loff_t *ppos)
{
    struct mgpio_dev *mdev = filp->private_data;
    char kbuf[8] = {0};
    int v;

    if (len == 0) return 0;
    if (len > sizeof(kbuf)-1) len = sizeof(kbuf)-1;
    if (copy_from_user(kbuf, buf, len)) return -EFAULT;

    if (!strncmp(kbuf, "1", 1) || !strncasecmp(kbuf, "on", 2))
        v = 1;
    else if (!strncmp(kbuf, "0", 1) || !strncasecmp(kbuf, "off", 3))
        v = 0;
    else
        return -EINVAL;

    mutex_lock(&mdev->lock);
    gpiod_set_value_cansleep(mdev->gpiod, v);
    mdev->state = v;
    mutex_unlock(&mdev->lock);

    return len;
}

static int mgpio_release(struct inode *inode, struct file *filp) { return 0; }

static const struct file_operations mgpio_fops = {
    .owner   = THIS_MODULE,
    .open    = mgpio_open,
    .read    = mgpio_read,
    .write   = mgpio_write,
    .release = mgpio_release,
    .llseek  = no_llseek,
};

/* ------------ platform driver ------------ */

static int mgpio_probe(struct platform_device *pdev)
{
    int ret;
    struct mgpio_dev *mdev;

    mdev = devm_kzalloc(&pdev->dev, sizeof(*mdev), GFP_KERNEL);
    if (!mdev) return -ENOMEM;

    mdev->dev = &pdev->dev;
    mutex_init(&mdev->lock);

    /* Nếu có DT thì lấy từ DT, ngược lại dùng lookup table (PC test) */
    if (pdev->dev.of_node) {
        mdev->gpiod = devm_gpiod_get(&pdev->dev, NULL, GPIOD_OUT_LOW);
    } else if (use_lookup) {
        /* Cấp phát lookup table 1 entry + terminator */
        mdev->lt = devm_kzalloc(&pdev->dev,
                    sizeof(*mdev->lt) + sizeof(struct gpiod_lookup)*2,
                    GFP_KERNEL);
        if (!mdev->lt) return -ENOMEM;

        mdev->lt->dev_id = dev_name(&pdev->dev);
        mdev->lt->table[0] = GPIO_LOOKUP(chip_label, line, "led", 0);
        mdev->lt->table[1] = (struct gpiod_lookup){};
        gpiod_add_lookup_table(mdev->lt);

        mdev->gpiod = devm_gpiod_get(&pdev->dev, "led", GPIOD_OUT_LOW);
    } else {
        return -ENODEV;
    }

    if (IS_ERR(mdev->gpiod)) {
        dev_err(&pdev->dev, "Failed to get GPIO\n");
        if (mdev->lt) gpiod_remove_lookup_table(mdev->lt);
        return PTR_ERR(mdev->gpiod);
    }
    mdev->state = 0;

    /* Tạo char device */
    ret = alloc_chrdev_region(&mdev->devt, 0, 1, DEV_NAME);
    if (ret) {
        if (mdev->lt) gpiod_remove_lookup_table(mdev->lt);
        return ret;
    }

    cdev_init(&mdev->cdev, &mgpio_fops);
    mdev->cdev.owner = THIS_MODULE;
    ret = cdev_add(&mdev->cdev, mdev->devt, 1);
    if (ret) {
        unregister_chrdev_region(mdev->devt, 1);
        if (mdev->lt) gpiod_remove_lookup_table(mdev->lt);
        return ret;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,4,0)
    if (!mgpio_class) mgpio_class = class_create(DEV_NAME);
#else
    if (!mgpio_class) mgpio_class = class_create(THIS_MODULE, DEV_NAME);
#endif
    if (IS_ERR(mgpio_class)) {
        ret = PTR_ERR(mgpio_class);
        mgpio_class = NULL;
        cdev_del(&mdev->cdev);
        unregister_chrdev_region(mdev->devt, 1);
        if (mdev->lt) gpiod_remove_lookup_table(mdev->lt);
        return ret;
    }

    device_create(mgpio_class, NULL, mdev->devt, mdev, DEV_NAME);
    platform_set_drvdata(pdev, mdev);

    dev_info(&pdev->dev, "loaded, /dev/%s ready (descriptor)\n", DEV_NAME);
    return 0;
}

static int mgpio_remove(struct platform_device *pdev)
{
    struct mgpio_dev *mdev = platform_get_drvdata(pdev);

    if (mdev->lt)
        gpiod_remove_lookup_table(mdev->lt);

    device_destroy(mgpio_class, mdev->devt);
    cdev_del(&mdev->cdev);
    unregister_chrdev_region(mdev->devt, 1);

    class_destroy(mgpio_class);
    mgpio_class = NULL;
    return 0;
}

static struct platform_driver mgpio_driver = {
    .driver = {
        .name = DRV_NAME,
        .of_match_table = mgpio_of_match,
    },
    .probe  = mgpio_probe,
    .remove = mgpio_remove,
};

/* Tự đăng ký driver + 1 thiết bị “ảo” khi không có DT */
static int __init mgpio_init(void)
{
    int ret = platform_driver_register(&mgpio_driver);
    if (ret) return ret;

    if (use_lookup) {
        pdev_self = platform_device_register_simple(DRV_NAME, -1, NULL, 0);
        if (IS_ERR(pdev_self)) {
            ret = PTR_ERR(pdev_self);
            platform_driver_unregister(&mgpio_driver);
            return ret;
        }
    }
    return 0;
}

static void __exit mgpio_exit(void)
{
    if (use_lookup && pdev_self && !IS_ERR(pdev_self))
        platform_device_unregister(pdev_self);
    platform_driver_unregister(&mgpio_driver);
}

module_init(mgpio_init);
module_exit(mgpio_exit);

MODULE_AUTHOR("You");
MODULE_DESCRIPTION("GPIO char driver (descriptor + DT or gpio-sim lookup)");
MODULE_LICENSE("GPL");
