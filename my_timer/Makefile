ifneq ($(KERNELRELEASE),)
obj-m += my_timer.o
else
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
all:
	make -C $(KERNELDIR) M=$(PWD) modules
help:
	make -C $(KERNELDIR) M=$(pwd) help
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
endif
