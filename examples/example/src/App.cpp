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
Zumo2040Motors motor;
Zumo2040Encoder encoder;
Zumo2040Linesensors linesensor;
LinesensorsInfo info;
Zumo2040ProximitySensors prox;
Zumo2040OLED oled;
Zumo2040IMU_I2C i2c;
Zumo2040IMU imu;
Zumo2040Buzzer buzzer;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

 void App::setup()
{
    /* Place your once executed code for the setup here.. */
    oled.setLayout21x8();
    linesensor.init();

    int i = 0;
    while (i < 300)
    {
        linesensor.calibrate();
        i++;
    }

}

void App::loop()
{
    /* Place your periodically executed code here. */

static uint32_t pos = 0;

pos = linesensor.readLine();
info = linesensor.getInfo();

oled.print("Sensor 0: ");
oled.print(info.m_sensorValuesOn[0]);
oled.gotoXY(0, 1);
oled.print("Sensor 1: ");
oled.print(info.m_sensorValuesOn[1]);
oled.gotoXY(0, 2);
oled.print("Sensor 2: ");
oled.print(info.m_sensorValuesOn[2]);
oled.gotoXY(0, 3);
oled.print("Sensor 3: ");
oled.print(info.m_sensorValuesOn[3]);
oled.gotoXY(0, 4);
oled.print("Sensor 4: ");
oled.print(info.m_sensorValuesOn[4]);
oled.gotoXY(0,5);
oled.print("Position: ");
oled.print(pos);
oled.gotoXY(0,6);
oled.print("Calib good?: ");
oled.print(linesensor.isCalibrationSuccessful());

delay(30);

oled.clear();


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