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
        /**
         * @brief Gets the count of the left encoder.
         *
         * @return The left encoder count.
         */
        int32_t getCountsLeft();

        /**
         * @brief Gets the count of the right encoder.
         *
         * @return The right encoder count.
         */
        int32_t getCountsRight();

        /**
         * @brief Gets the count of the left encoder and resets it.
         *
         * @return The left encoder count.
         */
        int32_t getCountsAndResetLeft();

        /**
         * @brief Gets the count of the right encoder and resets it.
         *
         * @return The right encoder count.
         */
        int32_t getCountsAndResetRight();

    private:
        /** Low-level encoder driver. */
        EncoderCore m_core;
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040ENCODERS_H */
/** @} */
