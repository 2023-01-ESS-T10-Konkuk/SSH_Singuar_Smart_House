//dht11
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>

//motor
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>

#include "fan_con.h"

MODULE_LICENSE("GPL");

#ifdef MY_DEBUG
	#define DEBUG_MSG(fmt, args...)\
		printk(KERN_DEBUG "fan_con: " fmt, ##args);
#else
	#define DEBUG_MSG(fmt, args...)
#endif

#define MAX_TIMING 85
#define DHT11 20
#define FAN 21

#define CYCLE 10
#define CHECKING 500

#define DEV_NAME "fan_con_dev"

static int dht11_data[5] = {0, };

int activation = 0;

static int dht11_read(void) {
	int last_state = 1;
	int counter = 0;
	int i = 0, j = 0;
	
	dht11_data[0] = dht11_data[1] = dht11_data[2] = dht11_data[3] = dht11_data[4] = 0;
	
	gpio_direction_output(DHT11, 0);
	gpio_set_value(DHT11, 0);
	mdelay(18);
	gpio_set_value(DHT11, 1);
	udelay(40);
	gpio_direction_input(DHT11);
	
	for(i = 0; i< MAX_TIMING; i++) {
		counter = 0;
		
		while(gpio_get_value(DHT11) == last_state) {
			counter++;
			udelay(1);
			if(counter == 255) {
				break;
			}
		}
		
		last_state = gpio_get_value(DHT11);
		
		if(counter == 255) {
			break;
		}
		
		// Ignore first 3 transitions
		if( (i >= 4) && (i % 2 == 0) ) {
			dht11_data[j/8] <<= 1;

			// Shove each bit into the storage bytes
			if(counter > 16) {
				dht11_data[j / 8] |= 1;
			}
			
			j++;
		}
	}
	
	// Verify checksum
	if( (j >= 40) && (dht11_data[4] == ( (dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3]) & 0xFF) ) ) {
		// Valid data, return it
		DEBUG_MSG("Humidity: %d.%d Temperature = %d.%d C\n" , dht11_data[0] , dht11_data[1] , dht11_data[2] , dht11_data[3]);
		
		if(dht11_data[0] > 40) {				
			activation = 1;
		}
		else {
			activation = 0;
		}
	}
	else {
		DEBUG_MSG("Data not good, skip\n");
	}
	// Invalid data, return prevous activation
	return activation;
}

static int fan_con_open(struct inode *inode, struct file *file) {
	DEBUG_MSG("open\n");
	return 0;
}

static int fan_con_release(struct inode *inode, struct file *file) {
	DEBUG_MSG("release\n");
	return 0;
}

static long fan_con_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	int ret = 0;
	
	switch(cmd) {
		case FAN_ACTIVE:
			gpio_set_value(FAN, arg);
			break;
		case DHT11_GET:
			ret = dht11_read();
			return ret;
		default:
			return -1;
	}
	return 0;
}

struct file_operations fan_con_ioctl_fops = {
	.open = fan_con_open,
	.release = fan_con_release,
	.unlocked_ioctl = fan_con_ioctl,
};

static dev_t dev_num;
static struct cdev *cd_cdev;

static int __init fan_con_init(void) {
	gpio_request_one(FAN, GPIOF_OUT_INIT_LOW, "FAN");
	gpio_request_one(DHT11, GPIOF_IN, "DHT11");
    
	DEBUG_MSG("Init Module\n");

    alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
	cd_cdev = cdev_alloc();
	cdev_init(cd_cdev, &fan_con_ioctl_fops);
	cdev_add(cd_cdev, dev_num, 1);
    
	return 0;
}

static void __exit fan_con_exit(void) {
	DEBUG_MSG("exit modeul\n");
	
	gpio_set_value(DHT11, 0);
	gpio_set_value(FAN, 0);
	gpio_free(FAN);
	gpio_free(DHT11);
	
	cdev_del(cd_cdev);
	unregister_chrdev_region(dev_num, 1);
}

module_init(fan_con_init);
module_exit(fan_con_exit);
