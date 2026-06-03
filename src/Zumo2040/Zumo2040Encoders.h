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
 * @brief Encoders driver
 * @author Felix Reitenauer
 *
 * @addtogroup Encoder
 *
 * @{
 */

#ifndef ZUMO2040ENCODERS_H
#define ZUMO2040ENCODERS_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "RP2040Encoder.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** Represents the encoder driver. */
class Zumo2040Encoder
{
    public:
        /** Initialize encoders. */
        Zumo2040Encoder();

        /** Default destructor. */
        ~Zumo2040Encoder() = default;

        /** Delete copy constructor. */
        Zumo2040Encoder(const Zumo2040Encoder&) = delete;

        /** Delete copy assignment operator. */
        Zumo2040Encoder& operator=(const Zumo2040Encoder&) = delete;

        /**
         * @brief Initializes the PIO state machines for both encoders.
         *
         * @return The error code of the initialization.
         */
        ErrorCode init();

        /**
         * @brief Gets the count of the left encoder.
         *
         * The encoders provide a resolution of 12 counts per revolution of the
         * motor shaft. To compute the counts per revolution of a wheel, multiply
         * the motor gear ratio by 12.
         *
         * For example, the 75:1 motors are specified more accurately with a gear
         * ratio of 75.81:1. This results in 75.81 * 12 = 909.72 counts per wheel
         * revolution.
         *
         * The exact gear ratios of the different motors can be found here:
         * https://www.pololu.com/file/0J1487/pololu-micro-metal-gearmotors-rev-6-2.pdf
         *
         * @return The left encoder count.
         */
        int32_t getCountLeft();

        /**
         * @brief Gets the count of the right encoder.
         *
         * The encoders provide a resolution of 12 counts per revolution of the
         * motor shaft. To compute the counts per revolution of a wheel, multiply
         * the motor gear ratio by 12.
         *
         * For example, the 75:1 motors are specified more accurately with a gear
         * ratio of 75.81:1. This results in 75.81 * 12 = 909.72 counts per wheel
         * revolution.
         *
         * The exact gear ratios of the different motors can be found here:
         * https://www.pololu.com/file/0J1487/pololu-micro-metal-gearmotors-rev-6-2.pdf
         *
         * @return The right encoder count.
         */
        int32_t getCountRight();

        /**
         * @brief Gets the count of the left encoder and resets it.
         *
         * The encoders provide a resolution of 12 counts per revolution of the
         * motor shaft. To compute the counts per revolution of a wheel, multiply
         * the motor gear ratio by 12.
         *
         * For example, the 75:1 motors are specified more accurately with a gear
         * ratio of 75.81:1. This results in 75.81 * 12 = 909.72 counts per wheel
         * revolution.
         *
         * The exact gear ratios of the different motors can be found here:
         * https://www.pololu.com/file/0J1487/pololu-micro-metal-gearmotors-rev-6-2.pdf
         *
         * @return The left encoder count.
         */
        int32_t getCountAndResetLeft();

        /**
         * @brief Gets the count of the right encoder and resets it.
         *
         * The encoders provide a resolution of 12 counts per revolution of the
         * motor shaft. To compute the counts per revolution of a wheel, multiply
         * the motor gear ratio by 12.
         *
         * For example, the 75:1 motors are specified more accurately with a gear
         * ratio of 75.81:1. This results in 75.81 * 12 = 909.72 counts per wheel
         * revolution.
         *
         * The exact gear ratios of the different motors can be found here:
         * https://www.pololu.com/file/0J1487/pololu-micro-metal-gearmotors-rev-6-2.pdf
         *
         * @return The right encoder count.
         */
        int32_t getCountAndResetRight();

        /**
         * @brief Get the current error code.
         *
         * @return Current error code.
         */
        ErrorCode getError();

    private:
        /** Low-level encoder driver. */
        EncoderCore m_core;
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040ENCODERS_H */
/** @} */
