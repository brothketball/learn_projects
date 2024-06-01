#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>

struct file_operations mymisc_fops = {
	.owner = THIS_MODULE
};

struct miscdevice mymisc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "mymisc",
	.fops = &mymisc_fops
};

static int mymisc_init(void)
{
	int ret;
	ret = misc_register(&mymisc_dev);
	if (ret) {
		printk("register mymisc_dev failed\r\n");
		return -1;
	}
	printk("register mymisc_dev success\r\n");
	return 0;
}

void mymisc_exit(void)
{
	misc_deregister(&mymisc_dev);
	printk("deregister mymisc_dev\r\n");
}

module_init(mymisc_init);
module_exit(mymisc_exit);

MODULE_LICENSE("GPL");
