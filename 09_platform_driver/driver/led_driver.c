#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>
#include<linux/mod_devicetable.h>

struct platform_device_id led_idtable[] = {
	[0] = {
		.name = "led_test",
	}
};

int led_probe(struct platform_device *pdev)
{
	printk("led_probe\r\n");
	// 获取device资源方法一：直接访问device成员
	// printk("beep_res is %s", pdev->resource[0].name);
	// 获取device资源方法二：通过接口获取，其中第三个参数为resource下标
	struct resource *led_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (led_mem == NULL) {
		printk("platform_get_resource is error\r\n");
		return -EBUSY;
	}
	printk("led_res 0 start is 0x%llx\r\n", led_mem[0].start);
	printk("led_res 1 start is 0x%llx\r\n", led_mem[1].start);
	printk("led_res 2 start is 0x%llx\r\n", led_mem[2].start);
	return 0;
}

int led_remove(struct platform_device *pdev)
{
	printk("led_remove\r\n");
	return 0;
}

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
    printk("platfrm_driver_unrgister\r\n");
}

module_init(led_init);

module_exit(led_exit);

MODULE_LICENSE("GPL");
