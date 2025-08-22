#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "mydevice"
#define CLASS_NAME  "mydevice_class"

static dev_t devno;
static struct cdev my_cdev;
static struct class *my_class;

static const char greet[] = "Hello from mydevice!\n";

static int my_open(struct inode *inode, struct file *file)
{
    pr_info(DRIVER_NAME ": open\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    pr_info(DRIVER_NAME ": release\n");
    return 0;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
    size_t msg_len = sizeof(greet) - 1;
    size_t remaining, to_copy;

    if (*ppos >= msg_len)
        return 0; // EOF

    remaining = msg_len - *ppos;
    to_copy = (len < remaining) ? len : remaining;

    if (copy_to_user(buf, greet + *ppos, to_copy))
        return -EFAULT;

    *ppos += to_copy;
    return to_copy;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
    char kbuf[256];
    size_t to_copy = (len < sizeof(kbuf) - 1) ? len : (sizeof(kbuf) - 1);

    if (copy_from_user(kbuf, buf, to_copy))
        return -EFAULT;

    kbuf[to_copy] = '\0';
    pr_info(DRIVER_NAME ": write(%zu bytes): %s\n", to_copy, kbuf);

    return to_copy; // số byte đã nhận
}

static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_release,
    .read    = my_read,
    .write   = my_write,
};

static int __init my_init(void)
{
    int ret;

    // 1) Cấp phát số thiết bị (major:minor)
    ret = alloc_chrdev_region(&devno, 0, 1, DRIVER_NAME);
    if (ret) {
        pr_err(DRIVER_NAME ": alloc_chrdev_region failed (%d)\n", ret);
        return ret;
    }

    // 2) Khởi tạo & đăng ký cdev
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;
    ret = cdev_add(&my_cdev, devno, 1);
    if (ret) {
        pr_err(DRIVER_NAME ": cdev_add failed (%d)\n", ret);
        unregister_chrdev_region(devno, 1);
        return ret;
    }

    // 3) Tạo class & node /dev/mydevice
    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        pr_err(DRIVER_NAME ": class_create failed\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(devno, 1);
        return PTR_ERR(my_class);
    }

    if (IS_ERR(device_create(my_class, NULL, devno, NULL, DRIVER_NAME))) {
        pr_err(DRIVER_NAME ": device_create failed\n");
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(devno, 1);
        return -ENOMEM;
    }

    pr_info(DRIVER_NAME ": initialized. major=%d minor=%d\n", MAJOR(devno), MINOR(devno));
    return 0;
}

static void __exit my_exit(void)
{
    device_destroy(my_class, devno);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(devno, 1);
    pr_info(DRIVER_NAME ": exited\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dung~");
MODULE_DESCRIPTION("Simple character device driver example");
MODULE_VERSION("1.0");

module_init(my_init);
module_exit(my_exit);
