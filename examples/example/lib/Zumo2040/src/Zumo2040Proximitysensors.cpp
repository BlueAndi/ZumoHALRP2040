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
 * @brief  Proximity sensors
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Zumo2040Proximitysensors.h"
#include <hardware/pwm.h>
#include <hardware/gpio.h>
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

/** Unsigned 16-bit zero constant. */
constexpr uint16_t U_INTEGER_16_ZERO = 0u;
/** Unsigned 32-bit zero constant. */
constexpr uint32_t U_INTEGER_32_ZERO = 0u;
/** Invalid pin value for uninitialized sensor pins. */
constexpr uint8_t UNINITIALIZED_PIN = 255u;
/** PWM slice used by the proximity emitters. */
constexpr uint8_t PWM_SLICE_PROX_EMITTERS = 0u;
/** 8-bit integer part of the clock divider for the PWM. */
constexpr uint8_t PWM_CLK_DIV_INT = 3u;
/** 4-bit fractional part of the clock divider for the PWM. */
constexpr uint8_t PWM_CLK_DIV_FRAC4 = 3u;
/** Counter wrap value for the emitter PWM. */
constexpr uint32_t PWM_RANGE = 699u;
/** PWM channel used by the left emitters. */
constexpr uint LEFT_EMITTER_PWM_CHAN = 1u;
/** PWM channel used by the right emitters. */
constexpr uint RIGHT_EMITTER_PWM_CHAN = 0u;
/** The TSSP77056 datasheet specifies that the sensor output pulse can start
 *  up to 13 carrier periods after the IR burst starts. Use 14 / 56 kHz ~= 250 us
 *  before sampling the sensor output.
 */
constexpr uint64_t PULSE_ON_DELAY_US = 250u;
/** The TSSP77056 datasheet specifies that the sensor output pulse can remain
 *  active up to 17 carrier periods after the IR burst ends. Use 18 / 56 kHz ~= 321 us
 *  before starting the next burst.
 */
constexpr uint64_t PULSE_OFF_DELAY_US = 321u;
/** Number of brightness levels which are used for the read process. */
constexpr uint32_t NUM_BRIGHTNESS_LEVELS = 6u;
/** PWM compare levels used as emitter brightness steps during proximity reads.
 *  Derived from the Pololu Zumo 2040 proximity sensor example and scaled to
 *  the configured PWM range.
 */
constexpr uint32_t BRIGHTNESS_LEVELS[NUM_BRIGHTNESS_LEVELS] = {12u, 39u, 81u, 137u, 210u, 296u};

/******************************************************************************
 * Public Methods
 *****************************************************************************/

Zumo2040ProximitySensors::Zumo2040ProximitySensors() : m_lineSensorEmitterPin(Zumo2040Pins::LINE_SENSOR_EMITTER_PIN),
                                                       m_numSensors(U_INTEGER_32_ZERO)
{
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < Zumo2040Pins::PROXIMITY_SENSOR_COUNT; idx++)
    {
        m_data[idx].pin = UNINITIALIZED_PIN;
        m_data[idx].withLeftLeds = U_INTEGER_32_ZERO;
        m_data[idx].withRightLeds = U_INTEGER_32_ZERO;
    }

    /* Set the GPIO function of the proximity emitter pins to PWM. */
    gpio_set_function(Zumo2040Pins::LEFT_PROXIMITY_EMITTERS_PIN, GPIO_FUNC_PWM);
    gpio_set_function(Zumo2040Pins::RIGHT_PROXIMITY_EMITTERS_PIN, GPIO_FUNC_PWM);

    /* Configure the PWM frequency to 125 MHz / ((3 + 3 / 16) * (699 + 1)) ~= 56.022 kHz,
     * because the sensors detect carrier frequencies near 56 kHz.
     */
    pwm_set_clkdiv_int_frac4(PWM_SLICE_PROX_EMITTERS, PWM_CLK_DIV_INT, PWM_CLK_DIV_FRAC4);
    pwm_set_wrap(PWM_SLICE_PROX_EMITTERS, PWM_RANGE);

    /* Set both PWM compare levels to zero to keep both emitters off. */
    pwm_set_both_levels(PWM_SLICE_PROX_EMITTERS,
                        U_INTEGER_16_ZERO,
                        U_INTEGER_16_ZERO);

    pwm_set_counter(PWM_SLICE_PROX_EMITTERS, U_INTEGER_16_ZERO);

    pwm_set_enabled(PWM_SLICE_PROX_EMITTERS, Status::ENABLED);
}

Zumo2040ProximitySensors::~Zumo2040ProximitySensors()
{
    pwm_set_enabled(PWM_SLICE_PROX_EMITTERS, Status::DISABLED);
}

void Zumo2040ProximitySensors::initLeftSensor()
{
    /* The state sequence is the same as in the PROXIMITY_SENSOR_PINS array (left, front, right). */
    ProximitySensorState states[Zumo2040Pins::PROXIMITY_SENSOR_COUNT] = {ProximitySensorState::ACTIVATE,
                                                           ProximitySensorState::DEACTIVATE,
                                                           ProximitySensorState::DEACTIVATE};
    init(states);
}

void Zumo2040ProximitySensors::initFrontSensor()
{
    /* The state sequence is the same as in the PROXIMITY_SENSOR_PINS array (left, front, right). */
    ProximitySensorState states[Zumo2040Pins::PROXIMITY_SENSOR_COUNT] = {ProximitySensorState::DEACTIVATE,
                                                           ProximitySensorState::ACTIVATE,
                                                           ProximitySensorState::DEACTIVATE};

    init(states);
}

void Zumo2040ProximitySensors::initRightSensor()
{
    /* The state sequence is the same as in the PROXIMITY_SENSOR_PINS array (left, front, right). */
    ProximitySensorState states[Zumo2040Pins::PROXIMITY_SENSOR_COUNT] = {ProximitySensorState::DEACTIVATE,
                                                           ProximitySensorState::DEACTIVATE,
                                                           ProximitySensorState::ACTIVATE};
    init(states);
}

void Zumo2040ProximitySensors::initAllSensors()
{
    /* The state sequence is the same as in the PROXIMITY_SENSOR_PINS array (left, front, right). */
    ProximitySensorState states[Zumo2040Pins::PROXIMITY_SENSOR_COUNT] = {ProximitySensorState::ACTIVATE,
                                                           ProximitySensorState::ACTIVATE,
                                                           ProximitySensorState::ACTIVATE};
    init(states);
}

uint8_t Zumo2040ProximitySensors::getNumSensors() const
{
    return m_numSensors;
}

uint32_t Zumo2040ProximitySensors::getNumBrightnessLevels()
{
    return NUM_BRIGHTNESS_LEVELS;
}

void Zumo2040ProximitySensors::read()
{
    /* Ensure that all initialized sensor pins use pull-ups and reset their counts. */
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < Zumo2040Pins::PROXIMITY_SENSOR_COUNT; idx++)
    {
        if (m_data[idx].pin != UNINITIALIZED_PIN)
        {
            if (!gpio_is_pulled_up(m_data[idx].pin))
            {
                gpio_pull_up(m_data[idx].pin);
            }

            m_data[idx].withLeftLeds = U_INTEGER_32_ZERO;
            m_data[idx].withRightLeds = U_INTEGER_32_ZERO;
        }
    }
    /* Ensure that the line sensor emitter is deactivated during proximity measurement. */
    if ((gpio_get((m_lineSensorEmitterPin)) == PinLevel::HIGH) &&
        (gpio_get_dir((m_lineSensorEmitterPin)) == PinDirection::OUT))
    {
        gpio_put(m_lineSensorEmitterPin, PinLevel::LOW);
    }

    sleep_us(PULSE_OFF_DELAY_US);

    for (uint32_t idx = U_INTEGER_32_ZERO; idx < NUM_BRIGHTNESS_LEVELS; idx++)
    {
        startIRPuls(EmitterSide::LEFT, BRIGHTNESS_LEVELS[idx]);
        sleep_us(PULSE_ON_DELAY_US);

        for (uint32_t pinIDX = U_INTEGER_32_ZERO; pinIDX < Zumo2040Pins::PROXIMITY_SENSOR_COUNT; pinIDX++)
        {
            if (m_data[pinIDX].pin != UNINITIALIZED_PIN)
            {
                /* Count the sensor if its output is pulled low while the emitter is active. */
                if (gpio_get(m_data[pinIDX].pin) == PinLevel::LOW)
                {
                    m_data[pinIDX].withLeftLeds++;
                }
            }
        }

        stopIRPuls();
        sleep_us(PULSE_OFF_DELAY_US);

        startIRPuls(EmitterSide::RIGHT, BRIGHTNESS_LEVELS[idx]);
        sleep_us(PULSE_ON_DELAY_US);

        for (uint32_t pinIDX = U_INTEGER_32_ZERO; pinIDX < Zumo2040Pins::PROXIMITY_SENSOR_COUNT; pinIDX++)
        {
            if (m_data[pinIDX].pin != UNINITIALIZED_PIN)
            {
                /* Count the sensor if its output is pulled low while the emitter is active. */
                if (gpio_get(m_data[pinIDX].pin) == PinLevel::LOW)
                {
                    m_data[pinIDX].withRightLeds++;
                }
            }
        }

        stopIRPuls();
        sleep_us(PULSE_OFF_DELAY_US);
    }
}

uint32_t Zumo2040ProximitySensors::getCountsWithLeftLeds(ProximitySensor sensor) const
{
    return m_data[static_cast<uint8_t>(sensor)].withLeftLeds;
}

uint32_t Zumo2040ProximitySensors::getCountsWithRightLeds(ProximitySensor sensor) const
{
    return m_data[static_cast<uint8_t>(sensor)].withRightLeds;
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

void Zumo2040ProximitySensors::init(ProximitySensorState (&states)[Zumo2040Pins::PROXIMITY_SENSOR_COUNT])
{
    m_numSensors = U_INTEGER_32_ZERO;

    for (uint32_t idx = U_INTEGER_32_ZERO; idx < Zumo2040Pins::PROXIMITY_SENSOR_COUNT; idx++)
    {
        m_data[idx].pin = UNINITIALIZED_PIN;
        m_data[idx].withLeftLeds = U_INTEGER_32_ZERO;
        m_data[idx].withRightLeds = U_INTEGER_32_ZERO;

        /* Initialize selected sensor pins as inputs with pull-ups. The sensor output pulls
         * the pin low when reflected IR is detected.
         */
        if (ProximitySensorState::ACTIVATE == states[idx])
        {
            m_data[idx].pin = Zumo2040Pins::PROXIMITY_SENSOR_PINS[idx];

            gpio_init(m_data[idx].pin);
            gpio_set_dir(m_data[idx].pin, GPIO_IN);
            gpio_pull_up(m_data[idx].pin);

            m_numSensors++;
        }
    }
}

void Zumo2040ProximitySensors::startIRPuls(EmitterSide side, uint16_t brightness)
{
    uint16_t level = brightness;

    if (PWM_RANGE < level)
    {
        level = PWM_RANGE;
    }

    uint16_t rightLevel = U_INTEGER_16_ZERO;
    uint16_t leftLevel = U_INTEGER_16_ZERO;

    if (EmitterSide::LEFT == side)
    {
        leftLevel = level;
    }
    else
    {
        rightLevel = level;
    }

    /* Set the PWM compare levels so only the selected emitter side is active. */
    pwm_set_both_levels(PWM_SLICE_PROX_EMITTERS,
                        rightLevel,
                        leftLevel);
}

void Zumo2040ProximitySensors::stopIRPuls()
{
    /* Set both PWM compare levels to zero to turn off both emitter sides. */
    pwm_set_both_levels(PWM_SLICE_PROX_EMITTERS,
                        U_INTEGER_16_ZERO,
                        U_INTEGER_16_ZERO);
}

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/