#include <stdint.h>

#include "gpio.h"


// --------------------------------------------
// some defines
// --------------------------------------------
#define GPIO_BASE_ADDRESS   0x50000000 // can be in the 'product specification' page ''


// --------------------------------------------
// some defines
// --------------------------------------------

/**
 * gpio structure in the memory, this can be found in the 'product specification' page '117'
 */
typedef struct {
 uint32_t RESERVED1[321]; // address 0 to 0x503   
 uint32_t OUT;  // address = 0x504
 uint32_t OUTSET;  // address = 0x508
 uint32_t OUTCLR;  // address = 0x50c
 uint32_t IN;  // address = 0x510
 uint32_t INDIR;  // address = 0x514
 uint32_t DIRSET;  // address = 0x518
 uint32_t DIRCLR;  // address = 0x51C
 uint32_t LATCH;  // address = 0x520
 uint32_t DETECTMODE;  // address = 0x524
 uint32_t RESERVED2[118];  // addresses from 0x528 to 0x6FF
 uint32_t PIN_CNF[32]; // this will occupies the adresses from 0x700 to 0x77F
 } gpio_reg_t;


 gpio_reg_t* global_gpio_reg = (gpio_reg_t*) GPIO_BASE_ADDRESS;

uint32_t dummy = 0;

// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
    // configure pin direction as input, refer to 'page 138' in the 'product specification' page 
    global_gpio_reg->PIN_CNF[gpio_num] &= ~(0b1 << 0);
    global_gpio_reg->PIN_CNF[gpio_num] |= (dir << 0);
    // conenct the pin direction input buffer, refer to 'page 138' and 'page 115' in the 'product specification' page 
    global_gpio_reg->PIN_CNF[gpio_num] &= ~(0b1 << 1);
    global_gpio_reg->PIN_CNF[gpio_num] |= (dir << 1);

    dummy = global_gpio_reg->PIN_CNF[gpio_num];
}

// Set gpio_num high
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num) {
    // write 1 to the out register, refer to 'page 138', 'page 115' and 'page 117' in the 'product specification' page 
    global_gpio_reg->OUT |= (1 << gpio_num);
    dummy = global_gpio_reg->OUT;
}

// Set gpio_num low
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num) {
    // write 0 to the out register, refer to 'page 138', 'page 115' and 'page 117' in the 'product specification' page 
    global_gpio_reg->OUT &= ~(1 << gpio_num);
    dummy = global_gpio_reg->OUT;
}
