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

/**
 * @brief Contains public types and constants of the Zumo 2040 HAL.
 *
 * The namespace prevents name collisions with application code and
 * other libraries.
 */
namespace Zumo2040
{
    /** States used in the debounce state machine. */
    enum class ButtonState : uint8_t
    {
        /** Compares the current button value with the previous value. */
        COMPARE,
        /** Indicates that a detected input transition is being debounced. */
        DEBOUNCERISING,
    };

    /** States of the button. */
    enum class ButtonValue : uint8_t
    {
        /** Indicates that the button is not pressed. */
        RELEASED,
        /** Indicates that the button is pressed. */
        PRESSED
    };
} /* namespace Zumo2040 */

/** Class which represents the button base class. */
class Zumo2040Button
{
    public:

        /** Default constructor. */
        Zumo2040Button() = default;

        /** Default destructor. */
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
        virtual Zumo2040::ButtonValue isPressed() = 0;

    private:

        /** Detects a debounced rising edge of the input signal. */
        static bool getSingleDebouncedRisingEdge(
            Zumo2040::ButtonValue value,
            Zumo2040::ButtonValue& prevValue,
            uint32_t& prevTime,
            Zumo2040::ButtonState& state);

        /** Current state in the debounce state machine. */
        Zumo2040::ButtonState m_pressState = Zumo2040::ButtonState::COMPARE;

        /** Previous value of the Button */
        Zumo2040::ButtonValue m_pressPrevValue = Zumo2040::ButtonValue::RELEASED;

        /** Timestamp for last rising edge occurrence. */
        uint32_t m_pressPrevTime = 0u;

        /** Current state in the debounce state machine. */
        Zumo2040::ButtonState m_releaseState = Zumo2040::ButtonState::COMPARE;

        /** Previous button value. Initialized to pressed because release detection
         *  reuses the rising edge function with an inverted input signal. */
        Zumo2040::ButtonValue m_releasePrevValue = Zumo2040::ButtonValue::PRESSED;

        /** Timestamp for last falling edge occurrence. */
        uint32_t m_releasePrevTime = 0u;
};

/** Class which represents the button A driver. */
class Zumo2040ButtonA : public Zumo2040Button
{
    public:

        /** Initializes the button input with internal pull-up. */
        Zumo2040ButtonA();

    private:

        /** Checks whether button is pressed. */
        Zumo2040::ButtonValue isPressed() override;
};

/** Class which represents the button B driver.
 *
 * Button B is connected to the BOOTSEL function of the RP2040 and is not a
 * regular GPIO input. It is tied to the flash chip select (CS) line.
 *
 * Reading its state requires temporarily suspending flash access and executing
 * code from RAM.
 *
 * As a result, accessing this button is significantly more expensive than a
 * standard GPIO read and may interfere with normal program execution.
 *
 * Therefore, it should not be polled continuously in time-critical loops,
 * such as fast control tasks, for example when used as an abort condition in
 * a line follower.
 */
class Zumo2040ButtonB : public Zumo2040Button
{
    private:

        /** Checks whether button is pressed. */
        Zumo2040::ButtonValue isPressed() override;
};

/** Class which represents the button C driver.
 *
 * This button implementation internally uses a delay, unlike the other buttons.
 * As a result, polling functions such as isPressed() may block for a short time.
 *
 * Therefore, this button is not well suited for time-critical polling scenarios,
 * such as using it as an abort condition in a line follower, where the input
 * must be checked continuously and without delay.
 */
class Zumo2040ButtonC : public Zumo2040Button
{
    private:

        /** Checks whether button is pressed. */
        Zumo2040::ButtonValue isPressed() override;
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040BUTTONS_H */
/** @} */
