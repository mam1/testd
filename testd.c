#include <stdio.h>
#include <unistd.h>		//sleep
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <ctype.h> 		//isalnum, tolower
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "bitlit.h"

#define SYSFS_GPIO_DIR "/sys/class/gpio"
// #define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
 #define MAX_BUF 100

#define ON 			1
#define OFF 		0


// /****************************************************************
//  * gpio_export
//  ****************************************************************/
int gpio_export(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];
 
	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}
 
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
 
	return 0;
}

// /****************************************************************
//  * gpio_unexport
//  ****************************************************************/
// int gpio_unexport(unsigned int gpio)
// {
// 	int fd, len;
// 	char buf[MAX_BUF];
 
// 	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
// 	if (fd < 0) {
// 		perror("gpio/export");
// 		return fd;
// 	}
 
// 	len = snprintf(buf, sizeof(buf), "%d", gpio);
// 	write(fd, buf, len);
// 	close(fd);
// 	return 0;
// }

// /****************************************************************
//  * gpio_set_dir
//  ****************************************************************/
int gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
	int fd;
	char buf[MAX_BUF];
 
	// len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);
	snprintf(buf,sizeof(buf),"/sys/class/gpio/gpio%d/direction",gpio);
 
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/direction");
		return fd;
	}
 
	if (out_flag)
		write(fd, "out", 4);
	else
		write(fd, "in", 3);
 
	close(fd);
	return 0;
}

void led_set(int led_num, int state){

	char					set_state[2];
	unsigned int 			gpio[4] = {66,69,68,67}; //map gpio to led
	char					filename[100];
	int 					gpio_fd;

	set_state[1] = 0;
	if(state)
		set_state[0] = '0';
	else
		set_state[0] = '1';

	gpio_export(gpio[led_num]);
	gpio_set_dir(gpio[led_num], 1);
	snprintf(filename,sizeof(filename),"/sys/class/gpio/gpio%d/value",gpio[led_num]);
	gpio_fd = open(filename,O_WRONLY);
	write(gpio_fd,set_state,2);
	close(gpio_fd);
		
	return;
}


/****************************************************************
 * Main
 ****************************************************************/

int main(void){

	int 		on_led;

	printf("\n\n**** testd version 0.0 ****\n");
	printf("  startng loop\n");


	on_led = 0;
	led_set(0,1);
	led_set(1,0);
	led_set(2,0);
	led_set(3,0);
	sleep(1);
	while(1){
		led_set(on_led,0);
		on_led += 1;
		if(on_led > 3) on_led = 0;
		led_set(on_led,1);
		sleep(1);
		// if(test_alm(rtc)){
		// 	reset_alm(rtc);
		// 	update();
	}

	printf("normal termination\n");
	return 1;
}


