#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>

#define DEVICE_NUM 1
#define DEVICE_MINOR_NO 0
#define SDEVICE_NAME "schrdev"
#define ADEVICE_NAME "achrdev"

// 主设备号存在 /proc/devices 文件中，其中 misc 对应的主设备号为10
static int major_no, minor_no;

module_param(major_no, int, S_IRUSR);
module_param(minor_no, int, S_IRUSR);

static int hello_init(void)
{
	dev_t dev_no;
	int ret;
	if (major_no) {
		printk("major_no:%d, minor_no:%d\r\n", major_no, minor_no);
		dev_no = MKDEV(major_no, major_no);

		ret = register_chrdev_region(dev_no, DEVICE_NUM, SDEVICE_NAME);
		if (ret != 0) {
			printk("register_chrdev_region error\r\n");
			return ret;
		}
		printk("register_chrdev_region success\r\n");
	} else {
		// 优先注册255~234之间的主设备号
		ret = alloc_chrdev_region(&dev_no, DEVICE_MINOR_NO, DEVICE_NUM, ADEVICE_NAME);
		if (ret != 0) {
			printk("alloc_chrdev_region error\r\n");
			return ret;
		}
		printk("alloc_chrdev_region success\r\n");

		major_no = MAJOR(dev_no);
		minor_no = MINOR(dev_no);
		printk("major_no:%d, minor_no:%d\r\n", major_no, minor_no);
	}
	
   	return 0;
}

static void hello_exit(void)
{
	unregister_chrdev_region(MKDEV(major_no, major_no), DEVICE_NUM);
    printk("bye bye\r\n");
}

module_init(hello_init);

module_exit(hello_exit);

MODULE_LICENSE("GPL");
