#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define GPIO_BASE_ADDR 0xfe200000 //该处为io内存地址，芯片手册中的物理地址与IO内存地址相互映射，映射关系在/proc/iomem文件中
								//而ioremap是将IO内存地址映射为虚拟地址
#define GPFSEL1_REG_OFFSET 0x04
#define GPSET0_REF_OFFSET 0x1c
#define GPCLR0_REF_OFFSET 0x28

static int *vir_addr_gpio18_sel; // BCM编号为18的pin，即手册中的GPIO pin18
static int *vir_addr_gpio18_set;
static int *vir_addr_gpio18_clr;

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

	*vir_addr_gpio18_sel = (1 << 24);

	if (kbuf[0] == 1) {
		*vir_addr_gpio18_set |= (1 << 18);
		*vir_addr_gpio18_clr &= ~(1 << 18);
	} else if (kbuf[0] == 0) {
		*vir_addr_gpio18_set &= ~(1 << 18);
		*vir_addr_gpio18_clr |= (1 << 18);
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
	.name = "gpio_driver", // 映射到/dev/目录下，作为设备节点被应用层访问
					  // 而insmod对应的是驱动文件名.ko，rmmod对应的驱动文件名
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
	vir_addr_gpio18_sel = ioremap(GPIO_BASE_ADDR | GPFSEL1_REG_OFFSET, 4);
	vir_addr_gpio18_set = ioremap(GPIO_BASE_ADDR | GPSET0_REF_OFFSET, 4);
	vir_addr_gpio18_clr = ioremap(GPIO_BASE_ADDR | GPCLR0_REF_OFFSET, 4);
	if (vir_addr_gpio18_sel == NULL || vir_addr_gpio18_set == NULL || vir_addr_gpio18_clr == NULL) {
		printk("ioremap error!\r\n");
		return -EBUSY;
	}
	printk("ioremap OK!\r\n");

	return 0;
}

void mymisc_exit(void)
{
	misc_deregister(&mymisc_dev);

	iounmap(vir_addr_gpio18_sel);
	iounmap(vir_addr_gpio18_set);
	iounmap(vir_addr_gpio18_clr);

	printk("deregister mymisc_dev\r\n");
}

module_init(mymisc_init);
module_exit(mymisc_exit);

MODULE_LICENSE("GPL");
