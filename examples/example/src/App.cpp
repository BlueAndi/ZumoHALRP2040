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
Zumo2040Encoder encoder;
/******************************************************************************
 * Public Methods
 *****************************************************************************/

 void App::setup()
{
    Board::getInstance().init();
    encoder.init();
    /* Place your once executed code for the setup here.. */
    oled.setLayout21x8();
}

void App::loop()
{
  /* Place your periodically executed code here. */
  static int32_t speed = 0;
  static int32_t countL = 0;
  static int32_t countR = 0;
  static EncoderError code = NONE;

  countL = encoder.getCountLeft();
  countR = encoder.getCountRight();
  code = encoder.getError();

  if (button_a.getSingleDebouncedPress())
  {
    speed += 50;
    motor.setLeftSpeed(speed);
    motor.setRightSpeed(speed);
  }
  if (button_b.getSingleDebouncedPress())
  {
    speed -= 50;
    motor.setLeftSpeed(speed);
    motor.setRightSpeed(speed);
  }
  if(button_c.getSingleDebouncedPress())
  {
    encoder.getCountAndResetLeft();
    encoder.getCountAndResetRight();
  }

  oled.clear();
  oled.print("countL: ");
  oled.print(countL);
  oled.gotoXY(0, 1);
  oled.print("countR: ");
  oled.print(countR);
  oled.gotoXY(0, 2);
  oled.print("Error?: ");
  oled.print(code);
  delay(10);
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
