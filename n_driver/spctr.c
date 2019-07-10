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

#define uchar unsigned char 
#define uint unsigned int 
#define UINT_32 unsigned long 
#define UINT_64 unsigned long long
#define INT_32 long
#define INT_64 long long   

struct cdev scdev;
dev_t devid;
int major;
struct class* sc_class;
struct class_device* sc_class_dev;
DEFINE_SPINLOCK(pos_lock);

/*方向 */
#define H_D(x) gpio_set_value(PAD_GPIO_B+30,x)
#define V_D(x) gpio_set_value(PAD_GPIO_B+31,x) 
/*限位 */
#define H_L gpio_get_value(PAD_GPIO_C+8)
#define V_L gpio_get_value(PAD_GPIO_C+9) 
/*running */
#define H_R_H gpio_set_value(PAd_GPIO_B+26，1)
#define H_R_L gpio_set_value(PAd_GPIO_B+26，0)
#define V_R_H gpio_set_value(PAd_GPIO_B+27，1)
#define V_R_L gpio_set_value(PAd_GPIO_B+27，0)
/*ioctl CMD */
#define SC_RESET 1
#define SC_RUNNING 2

struct sc_point{
    INT_64 x;
    INT_64 y;    
};
struct sc_point cur_pos;
struct sc_arg{
    struct sc_point current_pos;
    struct sc_point aim_pos;
    uint speed;
    uint sc_res;
};
static int sc_step(struct sc_point aim_p)
{
    while()
}

static int sc_runing(unsigned long arg)
{
    struct sc_arg data_arg;
    if(copy_from_user(&data_arg,(struct sc_arg *) arg, sizeof(struct sc_arg)))
    {
        return -EFAULT;
    }
    if(data_arg.current_pos.x != data_arg.aim_pos.x || data_arg.current_pos.y != data_arg.aim_pos.y)
    {
        sc_step(data_arg.aim_pos);
    }
}


static int sc_gpio_init()
{
    gpio_direction_output(PAD_GPIO_B+30,0);
    gpio_direction_output(PAD_GPIO_B+31,0);
    gpio_direction_output(PAD_GPIO_B+26,0);
    gpio_direction_output(PAD_GPIO_B+27,0);

    gpio_direction_input(PAD_GPIO_C+8,0);
    gpio_direction_input(PAD_GPIO_C+9,0);
    printk("<0>sc finish init");


    return 0;
}

static ssize_t sc_read(struct file *filp, char *buffer, size_t count, loff_t *ppos)
{
    spin_lock(&pos_lock);
    if(copy_to_user(buffer,(struct sc_point *)&cur_pos,sizeof(struct sc_point)))
    {
        spin_unlock(&pos_lock);
        return -EFAULT;
    }
    spin_unlock(&pos_lock);
    return 1;
}

static size_t sc_open(struct inode *node, struct file *file)
{
    sc_gpio_init();
}

static size_t sc_write(struct file *filp, char *buffer, size_t count, loff_t *ppos)
{

}
static size_t sc_ioctl(struct file *filp, unsigned char cmd, unsigned long arg)
{
    switch (cmd)
    {
    case SC_RESET:
        if(sc_reset() < 0)
        {
            printk("<0> fail resrt!!!");
            return -1;
        }
        else return 1;
        break;
    case SC_RUNNING:
        return i2c_running( arg );
        break;
    default:
        break;
    }
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

    return 1;
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
