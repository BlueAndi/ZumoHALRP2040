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
 * @brief  APA102 RGB LED driver for the Zumo 2040.
 * @author Felix Reitenauer
 *
 * @addtogroup RGB
 *
 * @{
 */

#ifndef ZUMO2040LED_H
#define ZUMO2040LED_H

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

#define SPEEDHZ_LED       20000000 /**< Clock frequency for SPI */
#define START_FRAME_BYTES 4        /**< Number of start frame bytes */
#define NUM_LEDS          6        /**< Number of APA102 RGB LEDs on the Zumo 2040 */
#define END_FRAME_BYTES   4        /**< Number of end frame bytes */

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/******************************************************************************
 * Functions
 *****************************************************************************/

/**
 * @brief Enables/Disables the Yellow LED.
 *
 * @param[in] on  Enable LED with true, disable it with false.
 */

void ledYellow(bool on);

/**
 * @brief Updates all RGB LEDs based on the current LED state array.
 *        Sends the corresponding SPI frames to the APA102 LEDs.
 * 
 */

void ledControl();

#endif /* ZUMO2040LED_H */
/** @} */
