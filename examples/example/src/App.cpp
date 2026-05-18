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

const char fugue[] =
  {"! O5 L16 agafaea dac+adaea fa<aa<bac#a dac#adaea f"
  "O6 dcd<b-d<ad<g d<f+d<gd<ad<b- d<dd<ed<f+d<g d<f+d<gd<ad"
  "L8 MS <b-d<b-d MLe-<ge-<g MSc<ac<a ML d<fd<f O5 MS b-gb-g"
  "ML >c#e>c#e MS afaf ML gc#gc# MS fdfd ML e<b-e<b-"
  "O6 L16ragafaea dac#adaea fa<aa<bac#a dac#adaea faeadaca"
  "<b-acadg<b-g egdgcg<b-g <ag<b-gcf<af dfcf<b-f<af"
  "<gf<af<b-e<ge c#e<b-e<ae<ge <fe<ge<ad<fd"
  "O5 e>ee>ef>df>d b->c#b->c#a>df>d e>ee>ef>df>d"
  "e>d>c#>db>d>c#b >c#agaegfe f O6 dc#dfdc#<b c#4"};

/******************************************************************************
 * Public Methods
 *****************************************************************************/

 void App::setup()
{
    /* Place your once executed code for the setup here.. */
    Board::getInstance().init();
    oled.setLayout21x8();
    buzzer.playMode(BuzzerPlaymode::DONT_PLAY_AUTOMATIC);
}

void App::loop()
{
    /* Place your periodically executed code here. */

    static uint8_t vol = 0;
    static uint16_t freq = 0;
    static uint32_t counter = 1;

/*
    buzzer.play(fugue);
    while (buzzer.isPlaying())
    {
    }
    delay(1000);
*/

//*
    buzzer.play(fugue);
    while (buzzer.isPlaying())
    {
        if (button_a.getSingleDebouncedPress())
        {
            counter++;
        }

        if (counter % 2 == 0)
        {
            buzzer.playCheck();
        }

        if (button_b.getSingleDebouncedPress())
        {
            buzzer.stopPlaying();
        }
    }

    delay(3000);
//*/
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
