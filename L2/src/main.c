#include <zephyr/kernel.h>

// include PWM drivers
#include <zephyr/drivers/pwm.h>

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

// list all the possible state
typedef enum {
    STATE_INIT,
    BRIGHTNESS_20,
    BRIGHTNESS_50,
    BRIGHTNESS_100,
} states_t;

// list all possible inputs values for ON and OFF buttons 
typedef enum {
    OFF_PRESENT,
    ON_PRESENT,
    ABSENT,
} button_in_vals_t;

// struct containing possible values for the inputs
typedef struct {
    uint8_t ON; /** for possible values, refer to @button_in_vals_t */
    uint8_t OFF;/** for possible values, refer to @button_in_vals_t */
} inputs_t;

// function declrations 
void stateMachine(inputs_t* events);


// read PWM configurations of the led from the '.overlay' file
const static struct pwm_dt_spec led_pwm = PWM_DT_SPEC_GET(DT_NODELABEL(led));

// read the gpio configurations of the buttons from the '.overlay' file
const static struct gpio_dt_spec ON_button = GPIO_DT_SPEC_GET(DT_NODELABEL(on), gpios);
const static struct gpio_dt_spec OFF_button = GPIO_DT_SPEC_GET(DT_NODELABEL(off), gpios);


/**
 * @brief: a function returns the new events to the system 
 * @param events [OUT]:  a struct containing all the read inputs, refer to @inputs_t
 */
void get_inputs(inputs_t* events)
{   
    // variables to store previous values
    static uint8_t on_prev_val = 0;
    static uint8_t off_prev_val = 0;
    
    // read hardware buttons
    int on_val = gpio_pin_get_dt(&ON_button);
    int off_val = gpio_pin_get_dt(&OFF_button);
    
    // check if there is a change between current and previous readings
    if(on_prev_val == 0 && on_val == 1)
    {
        events->ON = ON_PRESENT;
    }
    else
    {
        // there is no event
        events->ON = ABSENT;
    }

    if(off_prev_val == 0 && off_val == 1)
    {
        events->OFF = OFF_PRESENT;
    }
    else
    {
        // there is no event
        events->OFF = ABSENT;
    }

    // store the values into previous values for next iteration
    on_prev_val = on_val;
    off_prev_val = off_val;
    
}



int main(void)
{
    // variable to hold the read variables
    inputs_t inputs = {.OFF = ABSENT, .ON = ABSENT};


    // validate if the led connected to the pwm is ready to use
    if(!pwm_is_ready_dt(&led_pwm)){
        return 0;
    }

    // valide if both buttons are ready to use
    if(!gpio_is_ready_dt(&ON_button) || !gpio_is_ready_dt(&OFF_button)){
        return 0;
    }
    
    //configure the gpios conencted to the buttons with the configuration set in the '.overlay' file
    if(gpio_pin_configure_dt(&ON_button, GPIO_INPUT) < 0 || gpio_pin_configure_dt(&OFF_button, GPIO_INPUT) < 0)
    {
        return 0;
    }

    // infinite loop (we don't want the program to terminate)
    while(1)
    {
        // get the inputs 
        get_inputs(&inputs);

        // delay for debouncing of the buttons
        k_msleep(40);

        // repeatedly call the state machine function with the detected inputs
        stateMachine(&inputs);

        
    }
    return 0;
}

void changeLedBrightness(uint8_t newValue)
{   
    // set PWM period to be 10 ms with the required pulse width value all in nano-seconds range
    if(pwm_set_dt(&led_pwm, 10000, newValue * 1000 / 10) != 0)
    {
        return;
    }
}

/**
 * @brief: this function will behave as our Mealy state machine reacting properly to the inputs 
 *          and calling the intended functions to produce the required outputs
 * @param events [IN]: inputs to which the state machine shall react 
 */
void stateMachine(inputs_t* events){

    // a variable to keep track of the current state initialized at the init state
    // Notice the need for the 'static' keyword
    states_t static current_state = STATE_INIT;

    // since Mealy state machine depends on both inputs and current state, we need to check for both
    // let's check for the current state first
    switch (current_state)
    {
        // check for all transistions going out of state 'STATE_INIT'
        case STATE_INIT:
            
            // check for transition that takes us from 'init' state to 'brightness_20' state
            if(events->ON == ON_PRESENT && events->OFF == ABSENT)
            {   
                // produce an output and change the current state
                changeLedBrightness(20);
                current_state = BRIGHTNESS_20;
            }
            
            break;

        // check for all transistions going out of state 'BRIGHTNESS_20'
        case BRIGHTNESS_20:

            // check for transition that takes us from 'brightness_20' state to 'init' state
            if(events->ON == ABSENT && events->OFF == OFF_PRESENT)
            {   
                // produce an output and change the current state
                changeLedBrightness(0);
                current_state = STATE_INIT;
            }
            // check for transition that takes us from 'brightness_20' state to 'brightness_50' state
            else if(events->ON == ON_PRESENT && events->OFF == ABSENT)
            {
                // produce an output and change the current state
                changeLedBrightness(50);
                current_state = BRIGHTNESS_50;
            }

            break;

        // check for all transistions going out of state 'BRIGHTNESS_50'
        case BRIGHTNESS_50:

            // check for transition that takes us from 'brightness_50' state to 'init' state
            if(events->ON == ABSENT && events->OFF == OFF_PRESENT)
            {   
                // produce an output and change the current state
                changeLedBrightness(0);
                current_state = STATE_INIT;
            }
            // check for transition that takes us from 'brightness_50' state to 'brightness_100' state
            else if(events->ON == ON_PRESENT && events->OFF == ABSENT)
            {
                // produce an output and change the current state
                changeLedBrightness(100);
                current_state = BRIGHTNESS_100;
            }

            break;
        
        // check for all transistions going out of state 'BRIGHTNESS_100'
        case BRIGHTNESS_100:

            // check for transition that takes us from 'brightness_100' state to 'init' state
            if(events->ON == ABSENT && events->OFF == OFF_PRESENT)
            {   
                // produce an output and change the current state
                changeLedBrightness(0);
                current_state = STATE_INIT;
            }
            // check for transition that takes us from 'brightness_100' state to 'brightness_20' state
            else if(events->ON == ON_PRESENT && events->OFF == ABSENT)
            {
                // produce an output and change the current state
                changeLedBrightness(20);
                current_state = BRIGHTNESS_20;
            }
            break;

        default:
            break;
    }
}
