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
 * @brief  Example application
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "App.h"
#include "Board.h"
#include <Arduino.h>
#include <Zumo2040.h>
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

/******************************************************************************
 * Local Variables
 *****************************************************************************/
Zumo2040ButtonA button_a;
Zumo2040ButtonB button_b;
Zumo2040ButtonC button_c;
/******************************************************************************
 * Public Methods
 *****************************************************************************/

 void App::setup()
{
    Board::getInstance().init();
    /* Place your once executed code for the setup here.. */
}

void App::loop()
{
    /* Place your periodically executed code here. */
    static uint32_t counter1 = 0u;
    static uint32_t counter2 = 0u;
    static uint32_t counter3 = 0u;

    if (button_a.getSingleDebouncedPress())
    {
        counter1++;

        if (counter1 % 2 == 0)
        {
            setLedYellow(true);
        }
        else
        {
            setLedYellow(false);
        }
    }

    if (button_b.getSingleDebouncedPress())
    {
        counter2++;

        if (counter2 % 2 == 0)
        {
            setLedGreen(true);
        }
        else
        {
            setLedGreen(false);
        }
    }

    if (button_c.getSingleDebouncedPress())
    {
        counter3++;

        if (counter3 % 2 == 0)
        {
            setLedRed(true);
        }
        else
        {
            setLedRed(false);
        }
    }


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

/******************************************************************************
 * Local Functions
 *****************************************************************************/
