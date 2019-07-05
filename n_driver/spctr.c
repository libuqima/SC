#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>
#include <linux/gpio.h>
#include <linux/pwm.h>
#include <linux/io.h>
#include <linux/delay.h>
MODUEL_LICENSE("GPL");
MODUEL_AUTHOR("MW");

#define DEV_NAME "SpCtr"

struct cdev scdev;
dev_t devid;
int major;
struct class* sc_class;
struct class_device* sc_class_dev;


static ssize_t sc_read(struct file *filp, char *buffer, size_t count, loff_t *ppos)
{

}
static size_t sc_open(struct inode *node, struct file *file)
{
    gpio_init();
}

static size_t sc_write(struct file *filp, char *buffer, size_t count, loff_t *ppos)
{

}
static size_t sc_ioctl(struct file *filp, unsigned char cmd, unsigned long arg)
{

}
struct file_operations sc_fops = 
{
    .read = sc_read,
    .write = sc_write,
    .open = sc_open,
    .unlocked_ioctl = sc_ioctl,
};

static int sc_init(void)
{
    cdev_init(&scdev,&sc_fops);
    alloc_chrdev_regioon(&devid,0,1,DEV_NAME);
    cdev_add(&scdev,devid,1);

    sc_class = class_creat(THIS_MOUDLE,DEV_NAME);
    sc_class_dev = class_creat_dev(sc_class,NULL,devid,0,DEV_NAME);

    return 0;
}


struct void sc_exit(void)
{
    cdev_del(&scdev);
    unregister_chdev_region(devid,1);
    device_unregister(sc_class_dev);
    class_destory(sc_class);
}

module_init(sc_init);
module_exit(sc_exit);
