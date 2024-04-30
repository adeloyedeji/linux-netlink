obj-m += netlink_kernel.o

CURRENT_PATH := $(shell pwd)

LINUX_KERNEL_PATH := /lib/modules/$(shell uname -r)/build

all:
	@echo $(LINUX_KERNEL_PATH)
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules

client:
	gcc netlink_client.c -o netlink_client -g

clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean
	rm netlink_client