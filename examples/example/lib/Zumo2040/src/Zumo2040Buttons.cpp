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
 */


/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Zumo2040Buttons.h>
#include <Arduino.h>
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

/**
 * @brief Inverts the button value.
 *
 * @param[in] value Button value to invert.
 *
 * @return PRESSED if value is RELEASED, otherwise RELEASED.
 */
static ButtonValue invert(ButtonValue value);

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/** Time in us for the pin to stabilize after switching the pin mode. */
constexpr uint8_t STABILIZATION_TIME_US = 1u;

/** Time for debouncing in ms. */
constexpr uint8_t DEBOUNCE_TIME_MS = 15u;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

bool Zumo2040Button::getSingleDebouncedPress()
{

    return getSingleDebouncedRisingEdge(isPressed(),
                                        m_pressPrevValue,
                                        m_pressPrevTime,
                                        m_pressState);
}

bool Zumo2040Button::getSingleDebouncedRelease()
{
    /* Invert signal: release event is treated as rising edge on inverted input */
    return getSingleDebouncedRisingEdge(invert(isPressed()),
                                        m_releasePrevValue,
                                        m_releasePrevTime,
                                        m_releaseState);
}

Zumo2040ButtonA::Zumo2040ButtonA()
{
    /* Pull-up needed as button pulls pin to GND when pressed */
    pinMode(Zumo2040Pins::BUTTON_A, INPUT_PULLUP);
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

bool Zumo2040Button::getSingleDebouncedRisingEdge(ButtonValue value, ButtonValue& prevValue, uint32_t& prevTime, ButtonState& state)
{
    uint32_t curTime = millis();

    switch (state)
    {
    case ButtonState::COMPARE:
        /* Compare previous and current value to detect a possible rising edge */
        if (ButtonValue::RELEASED == prevValue && ButtonValue::PRESSED == value)
        {
            /* Possible rising edge detected */
            state = ButtonState::DEBOUNCERISING;
            /* Store timestamp for debounce timing */
            prevTime = curTime;
            prevValue = value;
        }
        else
        {
            prevValue = value;
        }
        break;
    case ButtonState::DEBOUNCERISING:

        if (prevValue != value)
        {
            /* Bouncing detected */
            state = ButtonState::COMPARE;
            prevValue = value;
        }
        else if (curTime - prevTime >= DEBOUNCE_TIME_MS)
        {
            state = ButtonState::COMPARE;
            prevValue = value;
            /* Stable rising edge detected after debounce time */
            return true;
        }
        break;
    }

    return false;
}

ButtonValue Zumo2040ButtonA::isPressed()
{
    /* Buttons pin level is low when pressed, high otherwise */
    ButtonValue value = (digitalRead(Zumo2040Pins::BUTTON_A) == LOW)
                        ? ButtonValue::PRESSED
                        : ButtonValue::RELEASED;

    return value;
}


/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/

static ButtonValue invert(ButtonValue value)
{
    return (value == ButtonValue::PRESSED)
           ? ButtonValue::RELEASED
           : ButtonValue::PRESSED;
}