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

/** Class to access the Button pin using RAII. */
class ButtonAccess
{
    public:

        /** Configure button pin as input with pull-up. */
        ButtonAccess(uint8_t pin);

        /** Set Button Pin into the standard mode. */
        ~ButtonAccess();

        /** Read current button state. */
        bool readButton();

    private:

        uint8_t m_buttonPin;
};

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/**
 * @brief Detects a debounced rising edge of the input signal.
 *
 * @param value      Current input value of the button.
 * @param prevValue  Previous input value.
 * @param prevTime   Timestamp of the last edge.
 * @param state      Current state of the debounce state machine.
 *
 * @return True if debounced rising edge is detected, otherwise false.
 */
static bool getSingleDebouncedRisingEdge(
    bool value,
    bool& prevValue,
    uint32_t& prevTime,
    ButtonState& state);

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/** PIN for the Button A. */
static const uint8_t g_PIN_BUTTON_A = 25;

/** Flag which is used to detect the BOOTSEL button */
static const uint8_t g_BUTTON_B_IDENTIFIER = 99;

/** Time in ms for the pin to stabilize after switching the pin mode. */
static const uint8_t g_STABILIZATION_TIME = 1;

/** Time for debouncing in ms. */
static const uint8_t g_DEBOUNCE_TIME_MS = 15;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

ButtonAccess::ButtonAccess(uint8_t pin) : m_buttonPin(pin)
{
    if (m_buttonPin != g_BUTTON_B_IDENTIFIER)
    {
        /* Pull-up needed as button pulls pin to GND when pressed */
        pinMode(m_buttonPin, INPUT_PULLUP);
        /* Give PIN time to stabilize */
        delay(g_STABILIZATION_TIME);
    }
}

ButtonAccess::~ButtonAccess()
{
    if (m_buttonPin != g_BUTTON_B_IDENTIFIER)
    {
        /* switch back to the normal input mode */
        pinMode(m_buttonPin, INPUT);
    }
}

bool ButtonAccess::readButton()
{

    if (m_buttonPin != g_BUTTON_B_IDENTIFIER)
    {
        return !digitalRead(m_buttonPin);
    }
    else
    {
        /* BOOTSEL button is not connected to a standard GPIO,
        so it is read via the Bootsel object provided by the framework */
        return BOOTSEL;
    }

}

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
    return getSingleDebouncedRisingEdge(!isPressed(),
                                        m_releasePrevValue,
                                        m_releasePrevTime,
                                        m_releaseState);
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

bool Zumo2040ButtonA::isPressed()
{
    ButtonAccess buttonA(g_PIN_BUTTON_A);

    return buttonA.readButton();
}

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/

static bool getSingleDebouncedRisingEdge(bool value, bool& prevValue, uint32_t& prevTime, ButtonState& state)
{
    uint32_t curTime = millis();

    switch (state)
    {
    case ButtonState::compare:
        /* Compare previous and current value to detect a possible rising edge */
        if (false == prevValue && true == value)
        {
            /* Possible rising edge detected */
            state = ButtonState::debounceRising;
            /* Store timestamp for debounce timing */
            prevTime = curTime;
            prevValue = value;
        }
        else
        {
            prevValue = value;
        }
        break;
    case ButtonState::debounceRising:

        if (prevValue != value)
        {
            /* Bouncing detected */
            state = ButtonState::compare;
            prevValue = value;
        }
        else if (curTime - prevTime >= g_DEBOUNCE_TIME_MS)
        {
            state = ButtonState::compare;
            prevValue = value;
            /* Stable rising edge detected after debounce time */
            return true;
        }
        break;
    }

    return false;
}