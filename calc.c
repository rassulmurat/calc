#include <linux/module.h>    
#include <linux/kernel.h>    
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rassul");
MODULE_DESCRIPTION("A Simple Hello World module");

int operator0 = 0;
int operator1 = 0;
char operand  = '+';
int result    = 0;

int convert(const char *buf, int count, int *res)
{
    int i = 0;
    int tmp;
    int sum = 0;
    while (i < count - 1) {
        int power = count - 2 - i;
        int rlpwr = 1;
        while (power > 0) {
            rlpwr = rlpwr * 10;
            --power;
        }
        tmp = buf[i] - '0';
        sum = sum + tmp*rlpwr;
        ++i;
    }
    *res = sum;
    return 0;
}

int calculate(void)
{
    switch(operand) {
        case '+': 
            result = operator0 + operator1;
            break;
        case '-':
            result = operator0 - operator1;
            break;
        case '*':
            result = operator0 * operator1;
            break;
        case '/':
            result = operator0 / operator1;
            break;
    }
    return 0;
}

//dev
static ssize_t write_op0 (struct file *file, const char *buf, size_t count, loff_t *llf)
{
    int res;
    convert(buf, count, &res);
    operator0 = res;
    calculate();
    return count;
}

static const struct file_operations op0 = {
        .owner                = THIS_MODULE,
        .write                = write_op0,
};

static struct miscdevice miscop0 = {
        MISC_DYNAMIC_MINOR,
        "operator0",
        &op0
};

//Operator 1
static ssize_t write_op1 (struct file *file, const char *buf, size_t count, loff_t *llf)
{
    int res;
    convert(buf, count, &res);
    operator1 = res;
    calculate();
    return count;
}

static const struct file_operations op1 = {
        .owner                = THIS_MODULE,
        .write                = write_op1,
};

static struct miscdevice miscop1 = {
        MISC_DYNAMIC_MINOR,
        "operator1",
        &op1
};

//Operand
static ssize_t write_oprnd (struct file *file, const char *buf, size_t count, loff_t *llf)
{
    operand = buf[0];
    calculate();
    return count;
}

static const struct file_operations oprnd = {
        .owner                = THIS_MODULE,
        .write                = write_oprnd,
};

static struct miscdevice miscoprnd = {
        MISC_DYNAMIC_MINOR,
        "operand",
        &oprnd
};

//proc
static ssize_t result_read_proc(struct file *file, char *buf, size_t count, loff_t *llf)
{
    sprintf( buf, "%d", result);
    return count;
}

static const struct file_operations result_fops = {
        .owner                = THIS_MODULE,
        .read                 = result_read_proc,
};

static int __init init(void)
{
    int ret;
    //DEV
    ret = misc_register(&miscop0);
    if (ret){
            printk(KERN_ERR "Unable to register \"Hello, world!\" misc device\n");
    }
    ret = misc_register(&miscop1);
    if (ret){
            printk(KERN_ERR "Unable to register \"Hello, world!\" misc device\n");
    }
    ret = misc_register(&miscoprnd);
    if (ret){
            printk(KERN_ERR "Unable to register \"Hello, world!\" misc device\n");
    }
    //PROC
    if (proc_create("result", 0, NULL, &result_fops) == 0) {
                printk(KERN_ERR
                       "Unable to register \"Hello, world!\" proc file\n");
                return -ENOMEM;
        }

    return ret;
}

static void __exit cleanup(void)
{
    misc_deregister(&miscop0);
    misc_deregister(&miscop1);
    misc_deregister(&miscoprnd);
}

module_init(init);
module_exit(cleanup);