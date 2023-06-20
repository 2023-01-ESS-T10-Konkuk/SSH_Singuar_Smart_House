#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "fan_con.h"

//#define MCP_DATA_TRANSFER _IOWR('x', 0x80, unsigned long*)

int main(int argc, char *argv[]) {
	int ret = 0, light_value = 0;
	int dev;
	int activation;
	
	dev = open("/dev/fan_con_dev", O_RDWR);
	
	while(1) {
		activation = ioctl(dev, DHT11_GET, (unsigned long)0);
		ioctl(dev, FAN_ACTIVE, (unsigned long)activation);
		
		sleep(1);
	}
	
	return 0;
}
