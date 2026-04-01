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
 * Implementation reuses information from PololuZumo2040 example code at
 * https://github.com/pololu/zumo-2040-robot/tree/master/c/pololu_zumo_2040_robot
 */


/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Zumo2040Buttons.h>
#include <Arduino.h>
#include <cstdint>
/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and classes
 *****************************************************************************/

/** Class to configure the Buttonpin using RAII */
class ButtonPinAccess
{
    public:
        /** Set Button Pin into the right mode */
        ButtonPinAccess(uint8_t pin);

        /** Set Button Pin into the standard mode */
        ~ButtonPinAccess();

        /** Read Button Pin */
        bool readButton();

    private:

        uint8_t m_buttonPin;
};

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/** PIN for the Button A */
static const uint8_t g_PIN_BUTTON_A = 25;

/** Time in ms for the pin to stabilize after switching the pin mode */
static const uint8_t g_STABILIZATION_TIME = 1;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

ButtonPinAccess::ButtonPinAccess(uint8_t pin) : m_buttonPin(pin)
{
    /* Pull-up needed as button pulls pin to GND when pressed */
    pinMode(m_buttonPin, INPUT_PULLUP);
    /* Give PIN time to stabilize */
    delay(g_STABILIZATION_TIME);
}

ButtonPinAccess::~ButtonPinAccess()
{
    /* switch back to the normal input mode */
    pinMode(m_buttonPin, INPUT);
}

bool ButtonPinAccess::readButton()
{
    return !digitalRead(m_buttonPin);
}


/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External Functions
 *****************************************************************************/

 bool isPressedButtonA()
{
    ButtonPinAccess buttonA(g_PIN_BUTTON_A);

    return buttonA.readButton();
}

/******************************************************************************
 * Local Functions
 *****************************************************************************/
