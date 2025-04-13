#include <zephyr/kernel.h>

// include GPIO drivers
#include <zephyr/drivers/gpio.h>

/**
 * Documenation links of the used functions
 * ----------------------------------------
 * Function                 |   Documentation Link
 * =========================|=====================
 * PWM_DT_SPEC_GET()        |   https://docs.nordicsemi.com/bundle/zephyr-apis-latest/page/group_pwm_interface.html#ga59a79f0b77c5b71252bde126f333a84b
 * DT_NODELABEL()           |   https://docs.nordicsemi.com/bundle/zephyr-apis-latest/page/group_devicetree-generic-id.html#gab7d23294a6bf7fd44a98b48ec47d8a79 
 * GPIO_DT_SPEC_GET()       |   https://docs.nordicsemi.com/bundle/zephyr-apis-latest/page/group_gpio_interface.html#ga2fa6bb5880f46984f9fc29c70f7d503e
 * pwm_is_ready_dt()        |   https://docs.nordicsemi.com/bundle/zephyr-apis-latest/page/group_pwm_interface.html#ga70aad0d88e8041c880499e7cdaa9ae57 
 * gpio_is_ready_dt()       |   https://docs.nordicsemi.com/bundle/zephyr-apis-latest/page/group_gpio_interface.html#gaaec9ad17c08a0d527d66445fe82d8327 
 * gpio_pin_configure_dt()  |   https://docs.nordicsemi.com/bundle/zephyr-apis-latest/page/group_gpio_interface.html#ga423db4f985098ddcaa504ec430e91913 
 * pwm_set_dt()             |   https://docs.nordicsemi.com/bundle/zephyr-apis-latest/page/group_pwm_interface.html#ga225ce58ceb3de3d76df3e03439d655b9 
 * gpio_pin_get()           |   https://docs.nordicsemi.com/bundle/zephyr-apis-latest/page/group_gpio_interface.html#gaabeb2d0d98856c7ff78be36651d6bbc1 
 * k_msleep()               |   https://docs.zephyrproject.org/apidoc/latest/group__thread__apis.html#ga51307cdfe153ab3e918b18755d97c5d9 
 * 
 */

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_NODELABEL(btn0), gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led0), gpios);
static struct gpio_callback button_cb_data;


/**
 * @brief: callback function
 */
void button_pressed(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins)
{
	gpio_pin_toggle_dt(&led);
}


int main(void)
{       
    if(!gpio_is_ready_dt(&button) || !gpio_is_ready_dt(&led))
    {
        return 0;
    }    

    if(gpio_pin_configure_dt(&led, GPIO_OUTPUT) < 0 || gpio_pin_configure_dt(&button, GPIO_INPUT) < 0)
    {
        return 0;
    }

    if(gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_FALLING) < 0)
    {
        return 0;
    }
	
    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
    
    if(gpio_add_callback_dt(&button, &button_cb_data) < 0)
    {
        return 0;
    }

    while (1)
    {
        k_msleep(10 * 60 * 1000);
    }
    
}
