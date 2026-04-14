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
Zumo2040OLED oled;
Zumo2040Motors motor;
/******************************************************************************
 * Public Methods
 *****************************************************************************/

 void App::setup()
{
    Board::getInstance().init();
    oled.setLayout21x8();
    /* Place your once executed code for the setup here.. */
}

void App::loop()
{
    /* Place your periodically executed code here. */
    int32_t speed;

    oled.print("Left forward\n");
    delay(1000);
    speed = 0;
    while (speed <= 400)
    {
      motor.setLeftSpeed(speed);
      oled.clear();
      oled.print(speed);
      delay(50);
      speed += 10;
    }
    while (speed >= 0)
    {
      motor.setLeftSpeed(speed);
      oled.clear();
      oled.print(speed);
      delay(50);
      speed -= 10;
    }
    motor.setSpeeds(0, 0);
    oled.clear();
    oled.print("Left reverse\n");
    delay(1000);
    speed = 0;
    while (speed >= -400)
    {
      motor.setLeftSpeed(speed);
      oled.clear();
      oled.print(speed);
      delay(50);
      speed -= 10;
    }
    while (speed <= 0)
    {
      motor.setLeftSpeed(speed);
      oled.clear();
      oled.print(speed);
      delay(50);
      speed += 10;
    }
    motor.setSpeeds(0, 0);
    oled.clear();
    printf("Right forward\n");
    delay(1000);
    speed = 0;
    while (speed <= 400)
    {
      motor.setRightSpeed(speed);
      oled.clear();
      oled.print(speed);
      delay(50);
      speed += 10;
    }
    while (speed >= 0)
    {
      motor.setRightSpeed(speed);
      oled.clear();
      oled.print(speed);
      delay(50);
      speed -= 10;
    }
    motor.setSpeeds(0, 0);
    oled.clear();
    oled.print("Right reverse\n");
    delay(1000);
    speed = 0;
    while (speed >= -400)
    {
      motor.setRightSpeed(speed);
      oled.clear();
      oled.print(speed);
      delay(50);
      speed -= 10;
    }
    while (speed <= 0)
    {
      motor.setRightSpeed(speed);
      oled.clear();
      oled.print(speed);
      delay(50);
      speed += 10;
    }
    oled.clear();
    motor.setSpeeds(0, 0);
    delay(100);
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
