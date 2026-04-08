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
 * @brief  Buttons driver
 * @author Felix Reitenauer
 *
 * @addtogroup Buttons
 *
 * @{
 */

#ifndef ZUMO2040BUTTONS_H
#define ZUMO2040BUTTONS_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Zumo2040Pin.h"
#include <cstdint>
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** States used in the debounce state machine. */
enum class ButtonState : uint8_t
{
    COMPARE,
    DEBOUNCERISING,
};
/** States of the button. */
enum class ButtonValue : uint8_t
{
    RELEASED,
    PRESSED
};

/** Class which represents the button base class. */
class Zumo2040Button
{
    public:

        Zumo2040Button() = default;

        virtual ~Zumo2040Button() = default;

        /**
         * @brief Detects the button moving from released to pressed state.
         *        This function also debounces.
         *
         * @return True if a debounced press was detected, otherwise false.
         */
        bool getSingleDebouncedPress();

        /**
         * @brief Detects the button moving from pressed to released state.
         *        This function also debounces.
         *
         * @return True if a debounced release was detected, otherwise false.
         */
        bool getSingleDebouncedRelease();

    protected:

        /** Checks whether button is pressed */
        virtual ButtonValue isPressed() = 0;

    private:

        /** Detects a debounced rising edge of the input signal. */
        static bool getSingleDebouncedRisingEdge(
            ButtonValue value,
            ButtonValue& prevValue,
            uint32_t& prevTime,
            ButtonState& state);

        /** Current state in the debounce state machine. */
        ButtonState m_pressState = ButtonState::COMPARE;

        /** Previous value of the Button */
        ButtonValue m_pressPrevValue = ButtonValue::RELEASED;

        /** Timestamp for last rising edge occurrence. */
        uint32_t m_pressPrevTime = 0;

        /** Current state in the debounce state machine. */
        ButtonState m_releaseState = ButtonState::COMPARE;

        /** Previous button value. Initialized to pressed because release detection
         *  reuses the rising edge function with an inverted input signal. */
        ButtonValue m_releasePrevValue = ButtonValue::PRESSED;

        /** Timestamp for last falling edge occurrence. */
        uint32_t m_releasePrevTime = 0;

};

/** Class which represents the button A driver. */
class Zumo2040ButtonA : public Zumo2040Button
{
    public:
        /** Initializes the button input with internal pull-up. */
        Zumo2040ButtonA();

    private:

        /** Checks whether button is pressed. */
        ButtonValue isPressed() override;

};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040BUTTONS_H */
/** @} */
