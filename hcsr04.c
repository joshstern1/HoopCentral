//Hoop Central Project
//Device Drvier for HC-SR04 Ultrasonic Sensor

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/proc_fs.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <asm-arm/arch-pxa/pxa-regs.h>
#include <asm-arm/arch-pxa/hardware.h>
#include <asm-arm/arch-pxa/gpio.h>	// Required for the GPIO functions
#include <linux/interrupt.h>            // Required for the IRQ code
#include <linux/time.h>

MODULE_LICENSE("Dual BSD/GPL");
static int hcsr04_major = 61;

//driver fops
static int hcsr04_open(struct inode *inode, struct file *filp);
static int hcsr04_release(struct inode *inode, struct file *filp);
static ssize_t hcsr04_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t hcsr04_write(struct file *filp,	const char *buf, size_t count, loff_t *f_pos);
static int hcsr04_init(void);
static void hcsr04_exit(void);

module_init(hcsr04_init);
module_exit(hcsr04_exit);
struct file_operations hcsr04_fops = {
	write: hcsr04_write,
	read: hcsr04_read,
	open: hcsr04_open,
	release: hcsr04_release
};


//interrupt handlers
static irqreturn_t echo_rising_handler(int irq, void *dev_id);
static irqreturn_t echo_falling_handler(int irq, void *dev_id);
static void my_timer_callback(void);


//procfs for reading score
static struct proc_dir_entry *proc_entry;
static int score_read( char *page, char **start, off_t off, int count, int *eof, void *data);

//timer
static struct timer_list my_timer;

//variables used for counting
static unsigned int score; //running score of baskets made
static unsigned int counting; //1 - start detecting baskets, 0 - stop
static unsigned int TRIGGER_PIN;
static unsigned int ECHO_PIN_UP;
static unsigned int ECHO_PIN_DOWN;
static unsigned int valid; //used to ensure that the rising handler is called before the falling handler
static unsigned int got;
static unsigned int timeout_len; //used for when the sensor times out, timeouts usually mean error

//variables used for taking timestamps
static struct timeval trigger_time; //used for getting time of day
static struct timeval echo_time; //used for getting time of day

static void send_pulse(void);

static void my_timer_callback() {
	//timer has gone off
	//if got == 0, it means the sensor timed out
	if ((got == 0) && (counting == 1)){
		//this likely means the sensor is not working and should be debugged
		printk(KERN_INFO "Timeout\n");
	}
	got = 0;
	//
	send_pulse();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//echo pin interrupt handlers
//once we send a pulse from the trigger pin, we wait on the echo to receive a high signal
//the duration of this high signal tells us how far away an object is

//the triggered ultrasonic signal has returned
static irqreturn_t echo_rising_handler(int irq, void *dev_id) {

	if((counting == 1) && (valid == 1)){
		//record the time of the rising handler being called
		do_gettimeofday(&trigger_time);
		valid = 0;
	}

	return IRQ_HANDLED;
}

//the triggered ultrasonic signal has completed its return and has dropped back to a low signal
static irqreturn_t echo_falling_handler(int irq, void *dev_id) {
	unsigned long duration;

	if((counting == 1) && (valid == 0)){
		do_gettimeofday(&echo_time);
		//caluclate how much time in milliseconds occurred between the rising handler and falling handler being called
		duration = (echo_time.tv_sec - trigger_time.tv_sec) * 1000000 + (echo_time.tv_usec - trigger_time.tv_usec);
		if(duration < 1500){
			//for debugging
			printk(KERN_INFO "duration = %lu\n", duration);
		}
		got = 1; //sensor detected an object before the timeout occurred
		if(duration < 1200){
			//if basket was made, increment score
			score++;
			//start a counter for 0.5 seconds second to repeat the distance detection
			mod_timer(&my_timer, jiffies + msecs_to_jiffies(500));
		}
		else{
			//start a counter for 50 milliseconds to repeat the distance detection
			mod_timer(&my_timer, jiffies + msecs_to_jiffies(50));
		}
	}

	return IRQ_HANDLED;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

//Register device and allocate memory
static int hcsr04_init(void) {
	int result, ret;
	result = register_chrdev(hcsr04_major, "hcsr04", &hcsr04_fops);

	score = -2; //start at -2 so that the speaker program knows the game is at the starting screen
	/*
	score = -2 -> start screen
	score = -1 -> end screen
	score >= 0 -> number of baskets that have been made
	*/

	counting = 0; //0 - reset score to 0, 1 - count score, should start as 0
	valid = 1; //1 - sensor ready to detect distance, 0 - sensor in the middle of a distance collection
	got = 0;
	timeout_len = 10000;// timeout = 10 seconds, this should be shortened for production

	//create a /procfs/hcsr04 file for storing the current score
	proc_entry = create_proc_entry( "hcsr04", 0644, NULL );
	proc_entry->read_proc = score_read;
	proc_entry->owner = THIS_MODULE;

	ECHO_PIN_UP = 30; //AC5
	ECHO_PIN_DOWN = 31; //AC8
	TRIGGER_PIN = 29;  //AC3
	gpio_direction_input(ECHO_PIN_UP);
	gpio_direction_input(ECHO_PIN_DOWN);
	gpio_direction_output(TRIGGER_PIN, 0);

	//set interrupt handlers for the input echo signal
	ret = request_irq(gpio_to_irq(ECHO_PIN_UP), echo_rising_handler, IRQF_TRIGGER_RISING, "echo_rising_handler", NULL);
  ret = request_irq(gpio_to_irq(ECHO_PIN_DOWN), echo_falling_handler, IRQF_TRIGGER_FALLING, "echo_falling_handler", NULL);

	setup_timer(&my_timer, my_timer_callback, 0);
	//start timer
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(500));

	return 0;
}


static void hcsr04_exit(void) {
	del_timer(&my_timer);
	unregister_chrdev(hcsr04_major, "hcsr04");
	remove_proc_entry("hcsr04", &proc_root);
}

static int hcsr04_open(struct inode *inode, struct file *filp) {
	return 0;
}
static int hcsr04_release(struct inode *inode, struct file *filp) {
	return 0;
}
static ssize_t hcsr04_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) {
	//don't need to have any read functionality, as we have our lcd and speaker programs read from /proc/hcsr04
	return count;
}


static ssize_t hcsr04_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
	//0 - stop counting, 1 - start counting

	score = 0;
	if((count != 1) || ((buf[0] < '1') && (buf[0] > '4'))){
		//invalid input
		printk(KERN_INFO "invalid input\n");
		return count;
	}

	if (buf[0] == '1'){
		//reset score, start counting
		score = 0;
		counting = 1;
	}
	else if (buf[0] == '2'){
		//stop counting, reset score
		score = 0;
		counting = 0;
	}
	else if (buf[0] == '3'){
		score = -1;	//end screen
		counting = 0;
	}
	else if (buf[0] == '4'){
		score = -2;	//start screen
		counting = 0;
	}
	//printk(KERN_INFO "counting = %d, score = %d\n", counting, score);
	return count;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


static void send_pulse(){
	//send a pulse from the trigger pin for 10ms, also set timeout timer for 10s
	if(counting == 1){
		valid = 1;
		mod_timer(&my_timer, jiffies + msecs_to_jiffies(timeout_len));
		pxa_gpio_set_value(TRIGGER_PIN, 1);
		udelay(10);
		pxa_gpio_set_value(TRIGGER_PIN, 0);
	}
	else{
		//else if we're not counting, keep timer going at 500 milliseconds so that it will still be ready for when the driver needs to start counting again
		mod_timer(&my_timer, jiffies + msecs_to_jiffies(500));
	}
}

static int score_read( char *page, char **start, off_t off, int count, int *eof, void *data) {
	//store the current score in /proc/hcsr04
	int len;
	len = sprintf(page, "%d\n", score);
	return len;
}
