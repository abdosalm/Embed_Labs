// have some useful utility functions provided by zeyphr kernal.
#include <zephyr/kernel.h>

// include the GPIO driver
#include <zephyr/drivers/gpio.h>

// include the printk function header 
#include <zephyr/sys/printk.h>

// get the gpio pin properties from the .overlay file
// static struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(yellow_led), gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(yellow_led), gpios);


int main(void)
{
	
	// a variable to keep track of the led 
	int led_state = 0;

	// check if the pin is ready to use (the function returns 0 if everything is ok)
	if(!gpio_is_ready_dt(&led))
	{
		// end the program
		return 0;
	}

	// check if the pin is configured successfully (the function returns 0 if everything is ok)
	if(gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE) < 0)
	{
		// end the program
		return 0;
	}

	// loop infinitely
	while(1)
	{
		// print led state
		printk("LED state = %d\n\r", led_state);
		led_state = !led_state;

		// toggle the led and check if it was toggled successfully 
		if(gpio_pin_toggle_dt(&led) < 0)
		{
				return 0;
		}

		// sleep for 1 second 
		k_msleep(1000);
	}

	return 0;
}