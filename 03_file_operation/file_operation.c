#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>

int mymisc_open(struct inode *inode, struct file *file)
{
	printk("hello mymisc open\r\n");
	return 0;
}

int mymisc_release(struct inode *inode, struct file *file)
{
	printk("hello mymisc release\r\n");
	return 0;
}

static char kbuf[64] = {0};

ssize_t mymisc_read(struct file *file, char __user *ubuf, size_t size, loff_t *loff_t)
{
	// char kbuf[64] = "read something";
	printk("hello mymisc read\r\n");
	if(raw_copy_to_user(ubuf, kbuf, strlen(kbuf)) != 0) {
		printk("raw_copy_to_user error\r\n");
		return -1;
	}
	return 0;
}

ssize_t mymisc_write(struct file *file, const char __user *ubuf, size_t size, loff_t *loff_t)
{
	// char kbuf[64] = {0};
	printk("hello mymisc write\r\n");
	if(raw_copy_from_user(kbuf, ubuf, size) != 0) {
		printk("raw_copy_from_user error\r\n");
		return -1;
	}
	return 0;
}

struct file_operations mymisc_fops = {
	.owner = THIS_MODULE,
	.open = mymisc_open,
	.release = mymisc_release,
	.read = mymisc_read,
	.write = mymisc_write
};

struct miscdevice mymisc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "file_operation", // 映射到/dev/目录下，作为设备节点被应用层访问
					  // 而insmod对应的是驱动文件名.ko，rmmod对应的驱动文件名，lsmod也是驱动文件名
					  // 而 /proc/devices 文件下的名字是主设备的名字，即misc
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
