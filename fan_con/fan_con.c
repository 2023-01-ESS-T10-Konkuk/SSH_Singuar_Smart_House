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

MODULE_LICENSE("GPL");

#define MAX_TIMING 85
#define DHT11 20
#define FAN 21

#define CYCLE 10
#define CHECKING 500

static int dht11_data[5] = {0, };

int activation = 0;

static void dht11_read(void) {
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
		
		if( (i >= 4) && (i % 2 == 0) ) {
			dht11_data[j/8] <<= 1;
			
			if(counter > 16) {
				dht11_data[j / 8] |= 1;
			}
			
			j++;
		}
	}
	
	if( (j >= 40) && (dht11_data[4] == ( (dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3]) & 0xFF) ) ) {
		printk("Humidity: %d.%d Temperature = %d.%d C\n" , dht11_data[0] , dht11_data[1] , dht11_data[2] , dht11_data[3]);
		
		if(dht11_data[0] > 41) {				
			activation = 1;
		}
		else {
			activation = 0;
		}
	}
	else {
		printk("Data not good, skip\n");
	}
}

static int __init fan_con_init(void) {
	printk("fan_con: Init Module\n");

	gpio_request_one(FAN, GPIOF_OUT_INIT_LOW, "FAN");
	gpio_request_one(DHT11, GPIOF_IN, "DHT11");
    
    //must use another way
	/*while(1) {
    		dht11_read();
		gpio_set_value(FAN, activation);
    		mdelay(CHECKING);
	}*/

	return 0;
}

static void __exit fan_con_exit(void) {
	printk("fan_con: exit modeul\n");
	
	gpio_set_value(DHT11, 0);
	gpio_set_value(FAN, 0);
	gpio_free(FAN);
	gpio_free(DHT11);
}

module_init(fan_con_init);
module_exit(fan_con_exit);
