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

/** RGB color values for each LED [LED index][red, green, blue] */
static const uint8_t g_LED_COLORS[NUM_LEDS][3] = {
    {255, 80, 0}, 
    {0, 0, 0},    
    {0, 0, 0},   
    {0, 0, 0},    
    {0, 0, 0},    
    {0, 0, 0}     
};

/** Status for each LED : false = off, true = on */
static bool g_ledStatus[NUM_LEDS] = {false, false, false, false, false, false};

/** Index for LED 0 */
static const int8_t g_IDX_LED_0 = 0;
/** Index for LED 1 */
static const int8_t g_IDX_LED_1 = 1;
/** Index for LED 2 */
static const int8_t g_IDX_LED_2 = 2;

/** Mark SPI to prevent re-initialization */
static bool g_initializedLED = false; 

/** Chunk of the start frame */
static const uint8_t g_START_FRAME_CHUNK = 0x00;

/** Chunk of the end frame */
static const uint8_t g_END_FRAME_CHUNK = 0xFF;

/** Value which controls the brightness of the LED : first 3 bits shall always be 1, the following 5 are controlling the brightness  */
static const uint8_t g_SET_BRIGHTNESS_ON = 0b11100111;

/** Value which turns the LED off*/
static const uint8_t g_SET_BRIGHTNESS_OFF = 0b11100000;

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
    g_ledStatus[g_IDX_LED_0] = on;
    /* Checks if SPI for RGB is already initialized */
    if (!g_initializedLED) 
    {   /* initialize the SPI for the LEDs */
        initializeLED(); 
        g_initializedLED = true;
    }
    /* Controls the MOSI output according to the status of the LEDs */
    ledControl(); 
};

void ledControl()
{
    /* configures the SPI bus with the specified settings */
    SPI.beginTransaction(SPISettings(SPEEDHZ_LED, MSBFIRST, SPI_MODE0)); 
    /* Sends the needed startframe */
    for (int i = 0; i < START_FRAME_BYTES; i++) 
    {
        SPI.transfer(g_START_FRAME_CHUNK);
    }

    for (int i = 0; i < NUM_LEDS; i++)
    {
        if (g_ledStatus[i])
        {
            SPI.transfer(g_SET_BRIGHTNESS_ON); 
        }
        else
        {
            SPI.transfer(g_SET_BRIGHTNESS_OFF);
        }
        /* sends the values for blue */
        SPI.transfer(g_LED_COLORS[i][IDX_BLUE]); 
        /* sends the values for green */
        SPI.transfer(g_LED_COLORS[i][IDX_GREEN]); 
        /* sends the values for red */
        SPI.transfer(g_LED_COLORS[i][IDX_RED]); 
    }
    /* Sends the needed endframe */
    for (int i = 0; i < END_FRAME_BYTES; i++) 
    {
        SPI.transfer(g_END_FRAME_CHUNK);
    }
    /* releases exclusive control of the SPI bus */
    SPI.endTransaction(); 
}

/******************************************************************************
 * Local Functions
 *****************************************************************************/
