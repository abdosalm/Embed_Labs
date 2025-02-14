#include <zephyr/kernel.h>

#include "gpio.h"

int main(void)
{       
        gpio_config(7, OUTPUT);

        while (1)
        {
                gpio_set(7);
                k_msleep(1000);
                gpio_clear(7);
                k_msleep(1000);
        }
        
        return 0;
}
