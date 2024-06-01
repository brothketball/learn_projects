obj-m += parameter.o

KDIR:=/lib/modules/$(shell uname -r)/build

PWD?=$(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -f *.ko *.mod.c *.mod.o *.order *.mod *.o *.symvers
