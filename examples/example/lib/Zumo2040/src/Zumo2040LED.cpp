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
 * @brief  APA102 RGB LED driver for the Zumo 2040.
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <Zumo2040LED.h>
#include <Zumo2040SPI.h>
#include <SPI.h>

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
static const uint8_t leds[NUM_LEDS][3] = {
    {255, 80, 0}, /* LED 0 Yellow */
    {0, 0, 0},    /* LED 1 */
    {0, 0, 0},    /* LED 2 */
    {0, 0, 0},    /* LED 3 */
    {0, 0, 0},    /* LED 4 */
    {0, 0, 0}     /* LED 5 */
};

static bool ledstatus[NUM_LEDS] = {false, false, false, false, false, false};

static const int8_t idx_led0 = 0;
static const int8_t idx_led1 = 1;
static const int8_t idx_led2 = 2;

static bool initializedLED = false; /**< Mark SPI to prevent re-initialization */

/******************************************************************************
 * Public Methods
 *****************************************************************************/

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External Functions
 *****************************************************************************/

void ledYellow(bool on)
{
    ledstatus[idx_led0] = on;

    if (!initializedLED) /* Checks if SPI for RGB is already initialized */
    {
        initializeLED(); /* initialize the SPI for the LEDs */
        initializedLED = true;
    }

    ledControl(); /* Controls the MOSI output according to the status of the LEDs */
};

void ledControl()
{

    SPI.beginTransaction(
        SPISettings(SPEEDHZ_LED, MSBFIRST, SPI_MODE0)); /* configures the SPI bus with the specified settings */

    for (int i = 0; i < START_FRAME_BYTES; i++) /* Sends the needed startframe */
    {
        SPI.transfer(0x00);
    }

    for (int i = 0; i < NUM_LEDS; i++)
    {
        if (ledstatus[i])
        {
            SPI.transfer(0b11100111); /* first 3 bits shall always be 1, the following 5 are controlling the brightness */
        }
        else
        {
            SPI.transfer(0b11100000);
        }
        SPI.transfer(leds[i][2]); /* sends the values for blue */
        SPI.transfer(leds[i][1]); /* sends the values for green */
        SPI.transfer(leds[i][0]); /* sends the values for red */
    }

    for (int i = 0; i < END_FRAME_BYTES; i++) /* Sends the needed endframe */
    {
        SPI.transfer(0xFF);
    }

    SPI.endTransaction(); /* releases exclusive control of the SPI bus */
}

/******************************************************************************
 * Local Functions
 *****************************************************************************/
