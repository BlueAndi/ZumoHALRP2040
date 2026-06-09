/* MIT License
 *
 * Copyright (c) 2026 - 2026 Andreas Merkle <web@blue-andi.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
 * @brief  Power interface realization
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Power.h"
#include "Zumo2040Pin.h"
#include <pico/stdlib.h>
#include <hardware/adc.h>
/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and classes
 *****************************************************************************/

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/** ADC channel which is used to read the battery voltage. */
static const uint ADC_CHANNEL_BATTERY = 0u;

/** Resolution of the ADC. */
static const uint32_t ADC_RES = 4096u;

/** Reference voltage of the ADC in millivolts. */
static const uint32_t ADC_REFERENCE_MV = 3300u;

/**
 * Factor to convert the ADC pin voltage back to the battery voltage.
 *
 * The Zumo 2040 battery voltage is divided by 11 before it reaches the ADC pin.
 */
static const uint32_t BATTERY_VOLTAGE_DIVIDER_FACTOR = 11u;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

uint16_t Power::getBatteryVoltage()
{
    const uint pin = Zumo2040::Pins::LINE_SENSOR_EMITTER_PIN;

    /* Save the old pin configuration, because this pin is also used
     * for line sensor emitter control.
     */
    gpio_function_t oldFunc     = gpio_get_function(pin);
    bool            oldState    = gpio_get_out_level(pin);
    bool            oldPullUp   = gpio_is_pulled_up(pin);
    bool            oldPullDown = gpio_is_pulled_down(pin);
    uint16_t        adcValue    = 0u;
    uint16_t        voltage     = 0u;

    /* Initialize the ADC only if it is not already enabled. */
    if (0u == (adc_hw->cs & ADC_CS_EN_BITS))
    {
        adc_init();
    }

    adc_gpio_init(pin);
    adc_select_input(ADC_CHANNEL_BATTERY);

    adcValue = adc_read();

    /* Restore the previous pin configuration. */
    gpio_set_function(pin, oldFunc);
    gpio_put(pin, oldState);
    gpio_set_pulls(pin, oldPullUp, oldPullDown);

    /* The ADC measures the divided battery voltage on GPIO26.
     * The voltage divider scales the battery voltage down by a factor of 11.
     * Convert the 12-bit ADC value to millivolts using the 3.3 V reference,
     * then multiply by 11 to get the actual battery voltage.
     */
    voltage = static_cast<uint16_t>((adcValue * BATTERY_VOLTAGE_DIVIDER_FACTOR * ADC_REFERENCE_MV) / ADC_RES);

    return voltage;
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/
