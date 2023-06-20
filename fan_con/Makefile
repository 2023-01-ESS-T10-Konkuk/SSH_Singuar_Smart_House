obj-m:= fan_con.o

KDIR:=~/linux-rpi/
ARM:= ARCH=arm CROSS_COMPILE=/usr/bin/arm-linux-gnueabi-
PWD:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) $(ARM) modules
	arm-linux-gnueabi-gcc fan_con_app.c -o fan_con_app
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) $(ARM) clean
	rm fan_con_app
scp:
	scp $(obj-m:.o=.ko) mknod_fan_con.sh fan_con_app pi@10.10.10.12:~/
