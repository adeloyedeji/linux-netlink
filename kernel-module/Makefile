obj-m += ikserver.o

ikserver-y := kernserver.o

KDIR ?= /lib/modules/$(shell uname -r)/build
MDIR ?= /lib/modules/$(shell uname -r)
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean