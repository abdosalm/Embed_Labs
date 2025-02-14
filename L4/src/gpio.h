
/**
 * --------------------------------------------------------------------------------------------------------------------------------------
 * |    @title          :   configuration file for the gpio peripheral of nRF52832                                                      |
 * |    @file           :   MCAL_config.h                                                                                               |
 * |    @author         :   Abdelrahman Mohamed Salem                                                                                   |
 * |    @origin_date    :   13/02/2025                                                                                                  |
 * |    @version        :   1.0.0                                                                                                       |
 * |    @tool_chain     :   nRF Connect SDK                                                                                             |
 * |    @compiler       :   zephyr SDK toolchain                                                                                        |
 * |    @C_standard     :   ISO C99 (-std=c99)                                                                                          |
 * |    @target         :   nRF52832                                                                                                    |
 * |    @notes          :   None                                                                                                        |
 * |    @license        :   MIT License                                                                                                 |
 * |    @brief          :   this file is the main entry to our application code that will run                                           |
 * --------------------------------------------------------------------------------------------------------------------------------------
 * |    MIT License                                                                                                                     |
 * |                                                                                                                                    |
 * |    Copyright (c) - 2024 - Abdelrahman Mohamed Salem - All Rights Reserved                                                          |
 * |                                                                                                                                    |
 * |    Permission is hereby granted, free of charge, to any person obtaining a copy                                                    |
 * |    of this software and associated documentation files (the "Software"), to deal                                                   |
 * |    in the Software without restriction, including without limitation the rights                                                    |
 * |    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell                                                       |
 * |    copies of the Software, and to permit persons to whom the Software is                                                           |
 * |    furnished to do so, subject to the following conditions:                                                                        |
 * |                                                                                                                                    |
 * |    The above copyright notice and this permission notice shall be included in all                                                  |
 * |    copies or substantial portions of the Software.                                                                                 |
 * |                                                                                                                                    |
 * |    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                                                      |
 * |    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                                                        |
 * |    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE                                                     |
 * |    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                                                          |
 * |    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,                                                   |
 * |    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE                                                   |
 * |    SOFTWARE.                                                                                                                       |
 * --------------------------------------------------------------------------------------------------------------------------------------
 * |    @history_change_list                                                                                                            |
 * |    ====================                                                                                                            |
 * |    Date            Version         Author                          Description                                                     |
 * |    13/02/2025      1.0.0           Abdelrahman Mohamed Salem       file Created.                                                   |
 * --------------------------------------------------------------------------------------------------------------------------------------
 */

// header guards, imagine the case when this file is included using #include in multiple different files
// the fuction decelrations will appear multiple times in the final linked file, which will cause confusion 
// to compiler becuase multiple same function declerations appeared in the same file. what the header guards 
// ensures is that the content of this file appears only once
#ifndef GPIO_H_
#define GPIO_H_

/******************************************************************************
 * Includes
 *******************************************************************************/

/**
 * @reason: provide the'uint8_t' type-defined data-types 
 */
#include <stdint.h>

/**
 * @reason: provide the 'bool' data-type 
 */
#include <stdbool.h>

/******************************************************************************
 * Preprocessor Constants
 *******************************************************************************/

/******************************************************************************
 * Configuration Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/

/**
 * @enum: gpio_direction_t
 * @brief: this enum is used to define the configuration of the pin (output or input)
 */
typedef enum {
    INPUT = 0,  /**< pin is configured to be input */
    OUTPUT = 1, /**< pin is configured to be output */    
} gpio_direction_t;


/******************************************************************************
 * Variables
 *******************************************************************************/

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/**
 *  \b function                                 :       void gpio_config(uint8_t gpio_num, gpio_direction_t dir);
 *  \b Description                              :       configure a gpio pin whether it's output or input.
 *  @param  gpio_num [IN]                       :       number of gpio pin to be configured, possible values are numbers between 0 and 31
 *  @param  dir [IN]                            :       configure whether the pin is output or input, refer to @gpio_direction_t in 'main.h'.
 *  @note                                       :       None.
 *  \b PRE-CONDITION                            :       None.
 *  \b POST-CONDITION                           :       None.
 *  @return                                     :       None
 *  @see                                        :       void gpio_set(uint8_t gpio_num);
 *  @see                                        :       void gpio_clear(uint8_t gpio_num);
 *
 *  \b Example:
 * @code
 * 
 * #include "gpio.h"
 * 
 * 
 * int main() {
 *      gpio_config(7, INPUT); // pin 0.0 is configured to be input
 * }
 * 
 * @endcode
 *
 * <br><b> - HISTORY OF CHANGES - </b>
 * <table align="left" style="width:800px">
 * <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
 * <tr><td> 13/02/2025 </td><td> 1.0.0            </td><td> AMS      </td><td> Interface Created </td></tr>
 * </table><br><br>
 * <hr>
 */
void gpio_config(uint8_t gpio_num, gpio_direction_t dir);


/**
 *  \b function                                 :       void gpio_set(uint8_t gpio_num);
 *  \b Description                              :       set the logic value of an output pin to high.
 *  @param  gpio_num [IN]                       :       number of gpio pin to be configured, possible values are numbers between 0 and 31
 *  @note                                       :       None.
 *  \b PRE-CONDITION                            :       the pin has to be configured as output before-hand.
 *  \b POST-CONDITION                           :       None.
 *  @return                                     :       None
 *  @see                                        :       void gpio_config(uint8_t gpio_num, gpio_direction_t dir);
 *  @see                                        :       void gpio_clear(uint8_t gpio_num);
 *
 *  \b Example:
 * @code
 * 
 * #include "gpio.h"
 * 
 * 
 * int main() {
 *      gpio_config(7, OUTPUT); // pin 0.0 is configured to be output
 *      gpio_set(7);    // the output value of the pin is set to be high
 * }
 * 
 * @endcode
 *
 * <br><b> - HISTORY OF CHANGES - </b>
 * <table align="left" style="width:800px">
 * <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
 * <tr><td> 13/02/2025 </td><td> 1.0.0            </td><td> AMS      </td><td> Interface Created </td></tr>
 * </table><br><br>
 * <hr>
 */
void gpio_set(uint8_t gpio_num);


/**
 *  \b function                                 :       void gpio_clear(uint8_t gpio_num);
 *  \b Description                              :       set the logic value of an output pin to low.
 *  @param  gpio_num [IN]                       :       number of gpio pin to be configured, possible values are numbers between 0 and 31
 *  @note                                       :       None.
 *  \b PRE-CONDITION                            :       the pin has to be configured as output before-hand.
 *  \b POST-CONDITION                           :       None.
 *  @return                                     :       None
 *  @see                                        :       void gpio_config(uint8_t gpio_num, gpio_direction_t dir);
 *  @see                                        :       void gpio_set(uint8_t gpio_num);
 *
 *  \b Example:
 * @code
 * 
 * #include "gpio.h"
 * 
 * 
 * int main() {
 *      gpio_config(7, OUTPUT); // pin 0.0 is configured to be output
 *      gpio_clear(7);    // the output value of the pin is set to be low
 * }
 * 
 * @endcode
 *
 * <br><b> - HISTORY OF CHANGES - </b>
 * <table align="left" style="width:800px">
 * <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
 * <tr><td> 13/02/2025 </td><td> 1.0.0            </td><td> AMS      </td><td> Interface Created </td></tr>
 * </table><br><br>
 * <hr>
 */
void gpio_clear(uint8_t gpio_num);


/*** End of File **************************************************************/

#endif /*GPIO_H_*/