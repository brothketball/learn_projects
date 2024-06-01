#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

void led_release(struct device *dev)
{
	printk("led_release\r\n");
}

struct resource led_res[] = {
	[0] = {
		.start = 0xfe200008,
		.end   = 0xfe20000b,
		.name  = "GPSEL2",
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = 0xfe20001c,
		.end   = 0xfe20001f,
		.name  = "GPSET0",
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = 0xfe200028,
		.end   = 0xfe20002b,
		.name  = "GPCLR0",
		.flags = IORESOURCE_MEM,
	}
};

struct platform_device led_device = {
	.name = "led_test", // insmod后，在/sys/bus/platform/devices/目录下会创建平台设备，以.name命名
	.id = -1,
	.resource = led_res,
	.num_resources = ARRAY_SIZE(led_res),
	.dev = {
		.release = led_release
	}
};

static int led_init(void)
{
	platform_device_register(&led_device);
    printk("platform_device_register\r\n");
    return 0;
}

static void led_exit(void)
{
	platform_device_unregister(&led_device);
    printk("platform_device_unregister\r\n");
}

module_init(led_init);

module_exit(led_exit);

MODULE_LICENSE("GPL");
