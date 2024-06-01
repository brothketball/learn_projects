#include<linux/init.h>
#include<linux/module.h>

//static int a;
static int b[10];
static int b_cnt = 5;

// module_param(a, int, S_IRUSR); //a 将会通过 insmod [driver.ko] a=xxx 传入
							   //a 作为文件存储在 /sys/module/parameter/parameters/ 目录下
							   //a 文件的权限将会是 S_IRUSR 即定义的0x400
							   //a 文件的文件名就是a，内容就是传入的值

module_param_array(b, int, &b_cnt, S_IRUSR); //b 将会通过 inmods [driver.ko] b=1,2,3,4,5 传入
							   				 //b 文件的文件名就是b，内容就是 1,2,3,4,5

static int hello_init(void)
{
    printk("hello world\r\n");
	// printk("a = %d\r\n", a);
	int i;
   	for (i = 0; i < b_cnt; i++) {
		printk("b[%d] = %d\r\n", i, b[i]);
	}
   	return 0;
}

static void hello_exit(void)
{
    printk("bye bye\r\n");
}

module_init(hello_init);

module_exit(hello_exit);

MODULE_LICENSE("GPL");
