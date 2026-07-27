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
 * @brief  Defines all hardware pins used by the peripherals.
 * @author Felix Reitenauer
 *
 * @addtogroup Pin
 *
 * @{
 */

#ifndef ZUMO2040PIN_H
#define ZUMO2040PIN_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <cstdint>
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 * @brief Contains public types and constants of the Zumo 2040 HAL.
 *
 * The namespace prevents name collisions with application code and
 * other libraries.
 */
namespace Zumo2040
{
    /** Defines whether an alarm is enabled or disabled. */
    enum AlarmStatus
    {
        /** Indicates that the alarm is disabled. */
        ALARM_DISABLED = false,
        /** Indicates that the alarm is enabled. */
        ALARM_ENABLED = true
    };

    /**
     * Contains the hardware pin assignments and related constants
     * of the Zumo 2040.
     */
    namespace Pins
    {
        /** Pin for Button A. */
        constexpr uint8_t BUTTON_A_PIN = 25u;
        /** SPI MOSI pin for APA102 and OLED data. */
        constexpr uint8_t RGB_OLED_DATA_PIN = 3u;
        /** SPI SCK pin for APA102 clock. */
        constexpr uint8_t RGB_CLOCK_PIN = 6u;
        /** SPI SCK pin for OLED clock. */
        constexpr uint8_t OLED_CLOCK_PIN = 2u;
        /** OLED D/C pin, reused to read Button C. */
        constexpr uint8_t BUTTON_C_OLED_DATA_COMMAND_PIN = 0u;
        /** Pin used to reset the OLED. */
        constexpr uint8_t OLED_RESET_PIN = 1u;
        /** Value used to indicate unused pin. */
        constexpr uint8_t UNUSED_OLED_PIN = 255u;
        /** Left motor direction pin. */
        constexpr uint8_t LEFT_MOTOR_DIRECTION_PIN = 11u;
        /** Right motor direction pin. */
        constexpr uint8_t RIGHT_MOTOR_DIRECTION_PIN = 10u;
        /** Left motor PWM pin. */
        constexpr uint8_t LEFT_MOTOR_PWM_PIN = 15u;
        /** Right motor PWM pin. */
        constexpr uint8_t RIGHT_MOTOR_PWM_PIN = 14u;
        /** Left encoder A pin. */
        constexpr uint8_t LEFT_ENCODER_A_PIN = 12u;
        /** Left encoder B pin. */
        constexpr uint8_t LEFT_ENCODER_B_PIN = 13u;
        /** Right encoder A pin. */
        constexpr uint8_t RIGHT_ENCODER_A_PIN = 8u;
        /** Right encoder B pin. */
        constexpr uint8_t RIGHT_ENCODER_B_PIN = 9u;
        /** Line sensor 1 pin. */
        constexpr uint8_t LINE_SENSOR_1_PIN = 22u;
        /** Line sensor 2 pin. */
        constexpr uint8_t LINE_SENSOR_2_PIN = 21u;
        /** Line sensor 3 pin. */
        constexpr uint8_t LINE_SENSOR_3_PIN = 20u;
        /** Line sensor 4 pin. */
        constexpr uint8_t LINE_SENSOR_4_PIN = 19u;
        /** Line sensor 5 pin. */
        constexpr uint8_t LINE_SENSOR_5_PIN = 18u;
        /** Line sensor emitter control pin. */
        constexpr uint8_t LINE_SENSOR_EMITTER_PIN = 26u;
        /** Number of proximity sensors. */
        constexpr uint8_t PROXIMITY_SENSOR_COUNT = 3u;
        /** Proximity sensor pins in the following order: left, front, right. */
        constexpr uint8_t PROXIMITY_SENSOR_PINS[PROXIMITY_SENSOR_COUNT] = {23u, 27u, 24u};
        /** Control pin for the left proximity emitters. */
        constexpr uint8_t LEFT_PROXIMITY_EMITTERS_PIN = 17u;
        /** Control pin for the right proximity emitters. */
        constexpr uint8_t RIGHT_PROXIMITY_EMITTERS_PIN = 16u;
        /** I2C0 SCL pin. */
        constexpr uint8_t I2C0_SCL_PIN = 5u;
        /** I2C0 SDA pin. */
        constexpr uint8_t I2C0_SDA_PIN = 4u;
        /** Buzzer pin. */
        constexpr uint8_t BUZZER_PIN = 7u;
    } /* namespace Pins */

    /** Contains the logical levels used for digital pin access. */
    namespace PinLevel
    {
        /** Pin level low. */
        constexpr bool LOW = false;
        /** Pin level high. */
        constexpr bool HIGH = true;
    } /* namespace PinLevel */

    /** Contains the direction values used when configuring GPIO pins. */
    namespace PinDirection
    {
        /** Pin direction: Input. */
        constexpr bool IN = false;
        /** Pin direction: Output. */
        constexpr bool OUT = true;
    } /* namespace PinDirection */

    /** Contains generic enabled and disabled status values. */
    namespace Status
    {
        /** Indicates the enabled state. */
        constexpr bool ENABLED = true;
        /** Indicates the disabled state. */
        constexpr bool DISABLED = false;
    } /* namespace Status */
} /* namespace Zumo2040 */

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040PIN_H */
/** @} */
