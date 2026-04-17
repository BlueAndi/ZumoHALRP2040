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

namespace Zumo2040Pins
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
    /** Pin used to reset the OLED.  */
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
}

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040PIN_H */
/** @} */
