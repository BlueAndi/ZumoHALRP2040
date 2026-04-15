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
 * @brief Motors driver
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <Zumo2040Motors.h>
#include <Zumo2040Pin.h>
#include <Arduino.h>
#include <hardware/pwm.h>

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

/** PWM slice which is used to control the motors. */
constexpr uint8_t PWM_SLICE_MOTORS = 7u;
/** 8 bit integer part of the clock divider. */
constexpr uint8_t PWM_CLK_DIV_INT = 4u;
/** 4 bit fractional part of the clock divider. */
constexpr uint8_t PWM_CLK_DIV_FRAC4 = 0u;
/** PWM counter wrap value. */
constexpr uint16_t PWM_RANGE = 400u;
/** 8 bit integer zero. */
constexpr uint8_t ZERO = 0u;
/** State for the PWM slice. */
constexpr bool ENABLED = true;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

Zumo2040Motors::Zumo2040Motors()
{
    /* Initialize the direction PINs */
    gpio_init(Zumo2040Pins::RIGHT_MOTOR_DIRECTION_PIN);
    gpio_init(Zumo2040Pins::LEFT_MOTOR_DIRECTION_PIN);
    gpio_set_dir(Zumo2040Pins::RIGHT_MOTOR_DIRECTION_PIN, GPIO_OUT);
    gpio_set_dir(Zumo2040Pins::LEFT_MOTOR_DIRECTION_PIN, GPIO_OUT);
    gpio_put(Zumo2040Pins::RIGHT_MOTOR_DIRECTION_PIN, LOW);
    gpio_put(Zumo2040Pins::LEFT_MOTOR_DIRECTION_PIN, LOW);
    /* Initialize the PWM slice 7 */
    gpio_set_function(Zumo2040Pins::RIGHT_MOTOR_PWM_PIN, GPIO_FUNC_PWM);
    gpio_set_function(Zumo2040Pins::LEFT_MOTOR_PWM_PIN, GPIO_FUNC_PWM);
    /* PWM frequency = 125 MHz / 4 / 400 = 78.1 kHz */
    pwm_set_clkdiv_int_frac4(PWM_SLICE_MOTORS, PWM_CLK_DIV_INT, PWM_CLK_DIV_FRAC4);
    pwm_set_wrap(PWM_SLICE_MOTORS, PWM_RANGE);
    pwm_set_enabled(PWM_SLICE_MOTORS, ENABLED);
}

void Zumo2040Motors::flipLeftMotor(bool flip)
{
    m_flipLeftMotor = flip;
}

void Zumo2040Motors::flipRightMotor(bool flip)
{
    m_flipRightMotor = flip;
}

void Zumo2040Motors::setLeftSpeed(int16_t speed)
{
    if (ZERO > speed)
    {
        speed = (speed < -PWM_RANGE) ? PWM_RANGE : -speed;
        gpio_put(Zumo2040Pins::LEFT_MOTOR_DIRECTION_PIN,
                 m_flipLeftMotor ? LOW : HIGH);
    }
    else if (ZERO <= speed)
    {
        speed = (speed > PWM_RANGE) ? PWM_RANGE : speed;
        gpio_put(Zumo2040Pins::LEFT_MOTOR_DIRECTION_PIN,
                 m_flipLeftMotor ? HIGH : LOW);
    }
    /* Set the current PWM counter compare value for the left motor channel */
    pwm_set_chan_level(PWM_SLICE_MOTORS,
                       PwmChannel::LEFT_MOTOR,
                       static_cast<uint16_t>(speed));
}

void Zumo2040Motors::setRightSpeed(int16_t speed)
{
    if (ZERO > speed)
    {
        speed = (speed < -PWM_RANGE) ? PWM_RANGE : -speed;
        gpio_put(Zumo2040Pins::RIGHT_MOTOR_DIRECTION_PIN,
                 m_flipRightMotor ? LOW : HIGH);
    }
    else if (ZERO <= speed)
    {
        speed = (speed > PWM_RANGE) ? PWM_RANGE : speed;
        gpio_put(Zumo2040Pins::RIGHT_MOTOR_DIRECTION_PIN,
                 m_flipRightMotor ? HIGH : LOW);
    }
    /* Set the current PWM counter compare value for the right motor channel */
    pwm_set_chan_level(PWM_SLICE_MOTORS,
                       PwmChannel::RIGHT_MOTOR,
                       static_cast<uint16_t>(speed));
}

void Zumo2040Motors::setSpeeds(int16_t leftSpeed, int16_t rightSpeed)
{
    setLeftSpeed(leftSpeed);
    setRightSpeed(rightSpeed);
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
