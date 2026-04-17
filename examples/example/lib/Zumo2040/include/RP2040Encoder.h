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

#ifndef ENCODER_H
#define ENCODER_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <cstdint>
#include <hardware/pio.h>
#include "RP2040Encoder.pio.h"
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** Defines the error codes for the encoder. */
enum EncoderError
{
    /** No error. */
    NONE,

    /** The program could not be added to the PIO instance. */
    CANT_ADD_PROGRAM,

    /** No unused state machine could be claimed for the left encoder. */
    CANT_CLAIM_SM_LEFT,

    /** No unused state machine could be claimed for the right encoder. */
    CANT_CLAIM_SM_RIGHT
};

/** Defines the encoder sides. */
enum EncoderSide
{
    /** Left encoder. */
    LEFT,

    /** Right encoder. */
    RIGHT
};

/** Low-level encoder driver class. */
class EncoderCore
{
    public:
        /** Initialize the PIO state machines for both encoders. */
        EncoderCore();

        /** Destructor disables and unclaims both state machines. */
        ~EncoderCore();

        /**
         * @brief Get the count of the selected encoder.
         *
         * @param side LEFT for the left encoder, RIGHT for the right encoder.
         *
         * @return Count of the encoder.
         */
        int32_t getCount(EncoderSide side);

        /**
         * @brief Reset the count of the selected encoder.
         *
         * @param side LEFT for the left encoder, RIGHT for the right encoder.
         */
        void resetCount(EncoderSide side);

        /**
         * @brief Get the current error code.
         *
         * @return Current error code.
         */
        EncoderError getError();

    private:
        /** Encoder error code. */
        EncoderError m_errorCode = NONE;
        /** Selected PIO instance. */
        PIO m_pio = pio0;
        /** State machine used for the left encoder. */
        uint m_smLeft;
        /** State machine used for the right encoder. */
        uint m_smRight;
        /** Program offset of the public entry point in the PIO program. */
        uint m_programEntry = rp2040encoder_offset_entry_point;
        /** Configuration of the left state machine. */
        pio_sm_config m_configLeft;
        /** Configuration of the right state machine. */
        pio_sm_config m_configRight;
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ENCODER_H */
/** @} */
