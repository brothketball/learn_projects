#ifndef MY_PRINTK_H
#define MY_PRINTK_H

#define myprintk(fmt, ...) printk("[my printk]:"fmt, ##__VA_ARGS__);

#endif
