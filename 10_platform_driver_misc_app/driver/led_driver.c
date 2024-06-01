#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>
#include<linux/mod_devicetable.h>
#include<linux/ioport.h>
#include<linux/miscdevice.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/io.h>

static int *vir_addr_gpio18_sel;
static int *vir_addr_gpio18_set;
static int *vir_addr_gpio18_clr;

struct resource *led_mem;
struct resource *led_mem_temp;

static int misc_open(struct inode *inode, struct file *file) {
	printk("misc_open\r\n");
	return 0;
}

static int misc_release(struct inode *inode, struct file *file) {
	printk("misc_release\r\n");
	return 0;
}

static ssize_t misc_read(struct file *file, char __user *ubuf, size_t size, loff_t *loff_t) {
	printk("misc_read\r\n");
	return 0;
}

static ssize_t misc_write(struct file *file, const char __user *ubuf, size_t size, loff_t *loff_t) {
	char kbuf[64] = {0};
	printk("misc_write\r\n");
	if (raw_copy_from_user(kbuf, ubuf, size) != 0) {
		printk("raw_copy_from_user is error\r\n");
		return -1;
	}
	
	*vir_addr_gpio18_sel = (1 << 24);
	if (kbuf[0] = 0) {
		*vir_addr_gpio18_set &= ~(1 << 18);
		*vir_addr_gpio18_clr |= (1 << 18);
	} else if (kbuf[0] = 1) {
		*vir_addr_gpio18_set |= (1 << 18);
		*vir_addr_gpio18_clr &= ~(1 << 18);
	} else {
		printk("kbuf[0] is %d, error", kbuf[0]);
	}

	return 0;
}

struct file_operations mymisc_fops = {
	.owner = THIS_MODULE,
	.open = misc_open,
	.release = misc_release,
	.read = misc_read,
	.write = misc_write
};

struct miscdevice mymisc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "gpio_misc",
	.fops = &mymisc_fops
};

int led_probe(struct platform_device *pdev)
{
	printk("led_probe\r\n");
	// 获取device资源方法一：直接访问device成员
	// printk("beep_res is %s", pdev->resource[0].name);
	// 获取device资源方法二：通过接口获取
	led_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (led_mem == NULL) {
		printk("platform_get_resource is error\r\n");
		return -EBUSY;
	}

	/* 因为该处的内存地址空间系统已经申请过了，在IO内存地址空间中，所以不需要再申请
	 * led_mem_temp = request_mem_region(led_mem->start, led_mem->end - led_mem->start + 1, "my_led");
	 * if (led_mem_temp == NULL) {
	 * 		printk("request_mem_region is error\r\n");
	 * 		goto error_region;
	 * error_region:
	 * 		release_mem_region(led_mem->start, led_mem->end - led_mem->start + 1);
	 * 		return -EBUSY;
	 * }
	*/
	vir_addr_gpio18_sel = ioremap(led_mem[0].start, 4);
	vir_addr_gpio18_set = ioremap(led_mem[1].start, 4);
	vir_addr_gpio18_clr = ioremap(led_mem[2].start, 4);

	if (vir_addr_gpio18_sel == NULL || vir_addr_gpio18_set == NULL || vir_addr_gpio18_clr == NULL) {
		printk("ioremap is error\r\n");
		return -EBUSY;
	}
	printk("ioremp is OK\r\n");
	
	int ret = misc_register(&mymisc_dev);
	if (ret < 0) {
		printk("misc_register error ret:%d\r\n", ret);
	}
	printk("misc_register ok\r\n");

	return 0;
}

int led_remove(struct platform_device *pdev)
{
	printk("led_remove\r\n");
	return 0;
}

struct platform_device_id led_idtable[] = {
	[0] = {
		.name = "led_test",
	}
};

struct platform_driver led_driver = {
	.probe = led_probe,
	.remove = led_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "led_test"
	},
	.id_table = led_idtable //id_table成员的name成员与device的name成员匹配的优先级高于driver成员的name成员
};

static int led_init(void)
{
	int ret = 0;
	ret = platform_driver_register(&led_driver);
    if (ret != 0) {
		printk("platform_driver_register error ret:%d\r\n", ret);
	}
	printk("platform_driver_register ok\r\n");
    return ret;
}

static void led_exit(void)
{
	platform_driver_unregister(&led_driver);
	misc_deregister(&mymisc_dev);
	iounmap(led_mem[0].start);
	iounmap(led_mem[1].start);
	iounmap(led_mem[2].start);
    printk("platfrm_driver_unregister\r\n");
}

module_init(led_init);

module_exit(led_exit);

MODULE_LICENSE("GPL");
