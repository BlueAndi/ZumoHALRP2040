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
 * @brief Low-level line sensors driver.
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "RP2040Linesensors.h"
#include "RP2040Linesensors.pio.h"
#include "Zumo2040Pin.h"
#include <pico/stdlib.h>
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

/** Prevents a panic if no free state machine can be claimed. */
constexpr bool DONT_PANIC = false;

/** Configures pins as outputs for PIO usage. */
constexpr bool OUTPUT = true;

/** Enables the PIO state machine. */
constexpr bool ENABLE = true;

/** Disables the PIO state machine. */
constexpr bool DISABLE = false;

/** Integer value zero. */
constexpr uint8_t U_INTEGER_ZERO = 0u;

/** Integer value one. */
constexpr uint8_t U_INTEGER_ONE = 1u;

/** Configures ISR shifting to the left. */
constexpr bool SHIFT_LEFT_ISR = false;

/** Configures OSR shifting to the right. */
constexpr bool SHIFT_RIGHT_OSR = true;

/** Enable ISR autopush. */
constexpr bool ENABLE_AUTOPUSH = true;

/** Disable OSR autopull. */
constexpr bool DISABLE_AUTOPULL = false;

/** Bit threshold for ISR autopush. */
constexpr uint8_t PUSH_THRESHOLD_ISR = 20u;

/** Bit threshold for OSR autopull.
 *  Autopull is disabled in this program, but a threshold value is still required.
 */
constexpr uint8_t PULL_THRESHOLD_OSR = 32u;

/** Initial value for the pin comparison. */
constexpr uint32_t INITIAL_PIN_VALUE = 0b11111u;

/** Value sent by the state machine to signal the end of the measurement. */
constexpr uint32_t SM_END_CONDITION = 0xFFFFFFFFu;

/** Mask for the pin values in the data word from the state machine. */
constexpr uint8_t VALUES_MASK = 0b11111u;

/** Mask for the pin value, used in the pin value comparison. */
constexpr uint8_t VALUE_MASK = 0b1u;

/** Range of the timer used by the state machine
 *  to measure the discharge time.
 */
constexpr uint32_t TIMER_RANGE = 32767u;

/** Mask for the timer value in the data word from the state machine. */
constexpr uint32_t TIMER_MASK = 0b111111111111111;

/** Count of the timer bits in the data word from the state machine. */
constexpr uint8_t TIMER_BIT_COUNT = 15u;

/** Constant for the measurement loop condition. */
constexpr bool MEASUREMENT = true;

/** Delay which is used when emitter state is changed. */
constexpr uint8_t EMITTER_DELAY_US = 30u;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

LinesensorsCore::LinesensorsCore() : m_errorCode(NONE),
                                     m_pio(pio1),
                                     m_sm(PICO_ERROR_GENERIC),
                                     m_debugInformation{INITIAL_PIN_VALUE, U_INTEGER_ZERO}
{}

LinesensorsCore::~LinesensorsCore()
{
    /* Unclaim the state machine used by the line sensors. */
    pio_sm_unclaim(m_pio, m_sm);

    /* Deactivate the line sensor emitter. */
    gpio_put(Zumo2040Pins::LINE_SENSOR_EMITTER_PIN, PinLevel::LOW);
}

ErrorCode LinesensorsCore::init()
{
    /* Initialize the line sensor emitter. */
    gpio_init(Zumo2040Pins::LINE_SENSOR_EMITTER_PIN);
    gpio_put(Zumo2040Pins::LINE_SENSOR_EMITTER_PIN, PinLevel::HIGH);
    gpio_set_dir(Zumo2040Pins::LINE_SENSOR_EMITTER_PIN, GPIO_OUT);

    /* State machine initialization. */
    if (!pio_can_add_program(m_pio, &rp2040linesensor_program))
    {
        m_errorCode = CANT_ADD_PROGRAM;
        return m_errorCode;
    }

    m_programEntry = pio_add_program(m_pio, &rp2040linesensor_program);
    m_sm = pio_claim_unused_sm(m_pio, DONT_PANIC);

    if (PICO_ERROR_GENERIC == m_sm)
    {
        m_errorCode = CANT_CLAIM_SM;
        return m_errorCode;
    }

    /* Initialize the pin directions for the state machine. */
    pio_sm_set_consecutive_pindirs(m_pio,
                                   m_sm,
                                   Zumo2040Pins::LINE_SENSOR_5_PIN,
                                   SENSOR_COUNT,
                                   OUTPUT);

    /* Initialize the line sensor pins for the state machine. */
    pio_gpio_init(m_pio, Zumo2040Pins::LINE_SENSOR_5_PIN);
    pio_gpio_init(m_pio, Zumo2040Pins::LINE_SENSOR_4_PIN);
    pio_gpio_init(m_pio, Zumo2040Pins::LINE_SENSOR_3_PIN);
    pio_gpio_init(m_pio, Zumo2040Pins::LINE_SENSOR_2_PIN);
    pio_gpio_init(m_pio, Zumo2040Pins::LINE_SENSOR_1_PIN);

    /* Disable the pulls of the line sensors pins, to guarantee
    *  that the capacitor is only drained by the phototransistors.
    */
    gpio_disable_pulls(Zumo2040Pins::LINE_SENSOR_5_PIN);
    gpio_disable_pulls(Zumo2040Pins::LINE_SENSOR_4_PIN);
    gpio_disable_pulls(Zumo2040Pins::LINE_SENSOR_3_PIN);
    gpio_disable_pulls(Zumo2040Pins::LINE_SENSOR_2_PIN);
    gpio_disable_pulls(Zumo2040Pins::LINE_SENSOR_1_PIN);


    m_config = rp2040linesensor_program_get_default_config(m_programEntry);

    /* Configure which pins are used for the "in pins" command in the state machine. */
    sm_config_set_in_pin_base(&m_config,  Zumo2040Pins::LINE_SENSOR_5_PIN);
    sm_config_set_in_pin_count(&m_config, SENSOR_COUNT);

    /* Configure which pins are used for the "out pins" command in the state machine. */
    sm_config_set_out_pin_base(&m_config, Zumo2040Pins::LINE_SENSOR_5_PIN);
    sm_config_set_out_pin_count(&m_config, SENSOR_COUNT);

    /* Configure which pins are used for the "set pins" command in the state machine. */
    sm_config_set_set_pin_base(&m_config, Zumo2040Pins::LINE_SENSOR_5_PIN);
    sm_config_set_set_pin_count(&m_config, SENSOR_COUNT);

    /* Configure the ISR behavior for the state machine. */
    sm_config_set_in_shift(&m_config,
                            SHIFT_LEFT_ISR,
                            ENABLE_AUTOPUSH,
                            PUSH_THRESHOLD_ISR);
    /* Configure the OSR behavior for the state machine. */
    sm_config_set_out_shift(&m_config,
                            SHIFT_RIGHT_OSR,
                            DISABLE_AUTOPULL,
                            PULL_THRESHOLD_OSR);

    /* Join both FIFO registers to double the RX FIFO capacity to 8 words. */
    sm_config_set_fifo_join(&m_config, PIO_FIFO_JOIN_RX);

    /* Initialize the state machine. */
    pio_sm_init(m_pio, m_sm, m_programEntry, &m_config);

    return NONE;
}

void LinesensorsCore::read(uint32_t sensorValues[SENSOR_COUNT])
{
    /* Enable the state machine to start the measurement. */
    pio_sm_set_enabled(m_pio, m_sm, ENABLE);

    uint32_t oldPinValues = INITIAL_PIN_VALUE;
    uint32_t newPinValues = INITIAL_PIN_VALUE;
    uint32_t data = U_INTEGER_ZERO;
    uint8_t idx = U_INTEGER_ZERO;
    uint8_t newValueCount = U_INTEGER_ZERO;

    for (int8_t sensorIdx = U_INTEGER_ZERO; idx < SENSOR_COUNT; idx++)
    {
        sensorValues[sensorIdx] = TIMER_RANGE;
    }

    while (MEASUREMENT)
    {
        /* Get the next result from the state machine.
         * Data format: 5-bit pin value + 15-bit timer value.
         */
        data = pio_sm_get_blocking(m_pio, m_sm);
        idx = U_INTEGER_ZERO;

        if (SM_END_CONDITION == data)
        {
            break;
        }

        newPinValues = data >> TIMER_BIT_COUNT & VALUES_MASK;

        /* Compare the old and new pin values. */
        while (SENSOR_COUNT > idx)
        {
            if ((newPinValues >> idx & VALUE_MASK) != (oldPinValues >> idx & VALUE_MASK))
            {
                sensorValues[idx] = (TIMER_RANGE - (data & TIMER_MASK));
                newValueCount++ ;
            }
            idx++ ;
        }
        oldPinValues = newPinValues;

    }

    m_debugInformation[U_INTEGER_ZERO] = newPinValues;
    m_debugInformation[U_INTEGER_ONE] = newValueCount;

    /* Disable the state machine and prepare it for the next measurement. */
    pio_sm_set_enabled(m_pio, m_sm, DISABLE);
    pio_sm_clear_fifos(m_pio, m_sm);
    pio_sm_restart(m_pio, m_sm);
}

const uint32_t* LinesensorsCore::getDebugInformation() const
{
    return m_debugInformation;
}

void LinesensorsCore::emitterControl(EmitterStates state)
{
    if (EMITTER_OFF == state)
    {
        gpio_put(Zumo2040Pins::LINE_SENSOR_EMITTER_PIN, PinLevel::LOW);
        sleep_us(EMITTER_DELAY_US);
    }
    else
    {
        gpio_put(Zumo2040Pins::LINE_SENSOR_EMITTER_PIN, PinLevel::HIGH);
        sleep_us(EMITTER_DELAY_US);
    }
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
