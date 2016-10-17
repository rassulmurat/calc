#include <linux/module.h>    
#include <linux/kernel.h>    
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rassul");
MODULE_DESCRIPTION("A Simple Hello World module");

int operator0 = 0;
int operator1 = 0;
char operand  = '+';

int calculate()
{

}

static ssize_t hello_read(struct file * file, char * buf, 
                          size_t count, loff_t *ppos)
{
        char *hello_str = "Hello, world!\n";
        int len = strlen(hello_str); /* Don't include the null byte. */
        /*
         * We only support reading the whole string at once.
         */
        if (count < len)
                return -EINVAL;
        /*
         * If file position is non-zero, then assume the string has
         * been read and indicate there is no more data to be read.
         */
        if (*ppos != 0)
                return 0;
        /*
         * Besides copying the string to the user provided buffer,
         * this function also checks that the user has permission to
         * write to the buffer, that it is mapped, etc.
         */
        if (copy_to_user(buf, hello_str, len))
                return -EINVAL;
        /*
         * Tell the user how much data we wrote.
         */
        *ppos = len;

        return len;
}

static const struct file_operations oper_fops = {
        .owner                = THIS_MODULE,
        .read                = hello_read,
};



static struct miscdevice hello_dev = {
        /*
         * We don't care what minor number we end up with, so tell the
         * kernel to just pick one.
         */
        MISC_DYNAMIC_MINOR,
        /*
         * Name ourselves /dev/hello.
         */
        "operator0",
        /*
         * What functions to call when a program performs file
         * operations on the device.
         */
        &oper_fops
};

static int __init init(void)
{
    int ret;
    ret = misc_register(&hello_dev);
	if (ret)
                printk(KERN_ERR
                       "Unable to register \"Hello, world!\" misc device\n");

        return ret;
}

static void __exit cleanup(void)
{
    misc_deregister(&hello_dev);
}

module_init(init);
module_exit(cleanup);