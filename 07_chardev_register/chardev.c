#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>

#define DEVICE_NUM 1
#define DEVICE_MINOR_NO 0
// 注册设备的名字，存在 /proc/devices 文件中
// 字符设备注册后不会自动生成设备节点，即在 /dev 目录下不存在设备节点供应用层使用、
// 需要使用 mknod命令 生成设备节点，格式为 mknod [/dev/xxx] [类型] [主设备号] [次设备号]，字符设备的类型为c
#define SDEVICE_NAME "schrdev"
#define ADEVICE_NAME "achrdev"

// 主设备号存在 /proc/devices 文件中，其中 misc 对应的主设备号为10
static int major_no, minor_no;
struct cdev chrdev;
static dev_t dev_no;

module_param(major_no, int, S_IRUSR);
module_param(minor_no, int, S_IRUSR);

int chrdev_open(struct inode *inode, struct file *file)
{
	printk("hello chrdev open\r\n");
	return 0;
};

struct file_operations chrdev_ops = {
	.owner = THIS_MODULE,
	.open = chrdev_open
};

struct class *class_test;

static int hello_init(void)
{
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

	// 初始化设备
	cdev_init(&chrdev, &chrdev_ops);
	chrdev.owner = THIS_MODULE;
	printk("cdev_init\r\n");
	// 添加设备
	ret = cdev_add(&chrdev, dev_no, DEVICE_NUM);
	if (ret < 0) {
		printk("cdev_add is error, ret:%d\r\n", ret);
	}
	printk("cdev_add ok\r\n");
	class_test = class_create(THIS_MODULE, "class_test"); // 创建类后，在/sys/class/目录下创建一个目录，名字为第二个参数
	device_create(class_test, NULL, dev_no, NULL, "device_test"); // 创建设备节点后，在/sys/class/class_test/下会创建一个第五个参数为名字的文件
																// udev根据该文件自动在/dev/目录下创建设备节点（设备文件 - app和driver之间的桥梁）
																// 自此，就可以对该字符设备的设备文件做open/release/read/write操作了

   	return 0;
}

static void hello_exit(void)
{
	unregister_chrdev_region(MKDEV(major_no, major_no), DEVICE_NUM);
	// 删除设备，必须要在注销设备号之后
	cdev_del(&chrdev);
	device_destroy(class_test, dev_no);
	class_destroy(class_test);
    printk("bye bye\r\n");
}

module_init(hello_init);

module_exit(hello_exit);

MODULE_LICENSE("GPL");
