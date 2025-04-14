KERNELDIR := /home/alientek/IMX6/4.1.15/linux-imx-4.1.15-2.1.0
CURRENT_PATH := $(shell pwd)

obj-m := dht11.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
