obj-m := spctr.o
KDIR := /home/fa/linux

all:
	make -C $(KDIR) M = $(PWD) modules CROSS_COMPILE = arm-linux-gnueabihf- ARCH = arm
clean:
	rm -rf 