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
 * @brief  Low-Level Encoder driver
 * @author Felix Reitenauer
 *
 * @addtogroup Encoder
 *
 * @{
 */

#ifndef RP2040ENCODER_H
#define RP2040ENCODER_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <hardware/pio.h>
#include "Zumo2040ErrorCodes.h"
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

namespace Zumo2040
{
    /** Defines the encoder sides. */
    enum EncoderSide
    {
        /** Left encoder. */
        LEFT,

        /** Right encoder. */
        RIGHT
    };
}

/** Low-level encoder driver class. */
class EncoderCore
{
    public:
        /**
         * Creates an encoder core object in an uninitialized state.
         * Call init() before using the encoder.
         */
        EncoderCore();

        /** Destructor disables and unclaims both state machines. */
        ~EncoderCore();

        /** Delete copy constructor. */
        EncoderCore(const EncoderCore&) = delete;

        /** Delete copy assignment operator. */
        EncoderCore& operator=(const EncoderCore&) = delete;

        /**
         * @brief Initializes the PIO state machines for both encoders.
         *
         * @return The error code of the initialization.
         */
        Zumo2040::ErrorCode init();

        /**
         * @brief Get the count of the selected encoder.
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
         * @param side LEFT for the left encoder, RIGHT for the right encoder.
         *
         * @return Count of the encoder.
         */
        int32_t getCount(Zumo2040::EncoderSide side);

        /**
         * @brief Reset the count of the selected encoder.
         *
         * @param side LEFT for the left encoder, RIGHT for the right encoder.
         */
        void resetCount(Zumo2040::EncoderSide side);

        /**
         * @brief Get the current error code.
         *
         * @return Current error code.
         */
        Zumo2040::ErrorCode getError();

    private:
        /** Encoder error code. */
        Zumo2040::ErrorCode m_errorCode;
        /** Selected PIO instance. */
        PIO m_pio;
        /** State machine used for the left encoder. */
        int m_smLeft;
        /** State machine used for the right encoder. */
        int m_smRight;
        /** Program offset of the public entry point in the PIO program. */
        uint m_programEntry;
        /** Configuration of the left state machine. */
        pio_sm_config m_configLeft;
        /** Configuration of the right state machine. */
        pio_sm_config m_configRight;
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* RP2040ENCODER_H */
/** @} */
