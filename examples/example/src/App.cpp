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
#include "RP2040Linesensors.h"
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
Zumo2040Motors motor;
Zumo2040Encoder encoder;
Zumo2040Linesensors linesensor;
LinesensorsInfo info;
Zumo2040ProximitySensors prox;
Zumo2040OLED oled;
/******************************************************************************
 * Public Methods
 *****************************************************************************/

 void App::setup()
{
    /* Place your once executed code for the setup here.. */
    Board::getInstance().init();
    prox.initAllSensors();
    oled.setLayout21x8();
}

void App::loop()
{
    /* Place your periodically executed code here. */
    static uint32_t left_l = 0;
    static uint32_t front_l = 0;
    static uint32_t right_l = 0;

    static uint32_t left_r = 0;
    static uint32_t front_r = 0;
    static uint32_t right_r = 0;

    static uint32_t counter = 0;

    prox.read();

    left_l = prox.getCountsWithLeftLeds(ProximitySensor::LEFT);
    front_l = prox.getCountsWithLeftLeds(ProximitySensor::FRONT);
    right_l = prox.getCountsWithLeftLeds(ProximitySensor::RIGHT);

    left_r = prox.getCountsWithRightLeds(ProximitySensor::LEFT);
    front_r = prox.getCountsWithRightLeds(ProximitySensor::FRONT);
    right_r = prox.getCountsWithRightLeds(ProximitySensor::RIGHT);


    oled.clear();

    oled.print(left_l);
    oled.print("  ");
    oled.print(front_l);
    oled.print("  ");
    oled.print(right_l);

    oled.gotoXY(0, 1);

    oled.print(left_r);
    oled.print("  ");
    oled.print(front_r);
    oled.print("  ");
    oled.print(right_r);

    oled.gotoXY(0,2);

    if (button_a.getSingleDebouncedPress())
    {
        counter++;
    }

    if ((counter % 2) == 0)
    {
        oled.print("Num_Bright: ");
        oled.print(prox.getNumBrightnessLevels());
    }
    else
    {
        oled.print("Num_Sens: ");
        oled.print(prox.getNumSensors());
    }

    delay(5);
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
