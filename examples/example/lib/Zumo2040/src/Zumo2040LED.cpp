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
#include <SPI.h>
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

 /** Class to access the SPI for the LEDs using RAII */
 class LedSpiAccess
 {
    public:
        /** Acquire SPI hardware for LEDs */
        LedSpiAccess(uint32_t clock, BitOrder bitOrder, arduino::SPIMode dataMode);

        /** Release SPI hardware from LEDs */
        ~LedSpiAccess();

        /** Exchange data with the SPI bus */
        uint8_t transfer(byte data);
 };

 /** LED state structure */
 struct LedState
 {
    uint8_t state;              /**< brightness state */
    const struct RGBColor {
        uint8_t red;            /**< Red color value   */
        uint8_t green;          /**< Green color value */
        uint8_t blue;           /**< Blue color value  */
    } color;                    /**< RGB color values  */
 };

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/**
 * @brief Updates all RGB LEDs based on the current LED state array.
 *        Sends the corresponding SPI frames to the APA102 LEDs.
 */

static void refreshLeds();

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/** Value which controls the brightness of the LED : first 3 bits shall always be 1, the following 5 are controlling the brightness  */
static const uint8_t g_SET_BRIGHTNESS_ON = 0b11100111;

/** Value which turns the LED off */
static const uint8_t g_SET_BRIGHTNESS_OFF = 0b11100000;

/** Number of APA102 RGB LEDs on the Zumo 2040 */
static const uint8_t g_NUM_LEDS = 6;

/** Color values and state for the LEDs. */
static LedState g_LedState[g_NUM_LEDS] = {
    { g_SET_BRIGHTNESS_OFF, {255, 80, 0}},  /**< LED 0 - Yellow */
    { g_SET_BRIGHTNESS_OFF, {0, 0, 0}},     /**< LED 1 - Off */
    { g_SET_BRIGHTNESS_OFF, {0, 0, 0}},     /**< LED 2 - Off */
    { g_SET_BRIGHTNESS_OFF, {0, 0, 0}},     /**< LED 3 - Off */
    { g_SET_BRIGHTNESS_OFF, {0, 0, 0}},     /**< LED 4 - Off */
    { g_SET_BRIGHTNESS_OFF, {0, 0, 0}}      /**< LED 5 - Off */
};


/** Index for LED 0 */
static const int8_t g_IDX_LED_YELLOW = 0;

/** Chunk of the start frame */
static const uint8_t g_START_FRAME_CHUNK = 0x00;

/** Chunk of the end frame */
static const uint8_t g_END_FRAME_CHUNK = 0xFF;

/** Clock frequency for SPI */
static const uint32_t g_SPEEDHZ_LED = 20000000;

/** Number of start frame bytes */
static const uint8_t g_START_FRAME_BYTES = 4;

/** Number of end frame bytes */
static const uint8_t g_END_FRAME_BYTES = 4;

/** SPI MOSI pin for APA102 data */
static const uint8_t g_RGB_DATA_PIN = 3;

/** SPI SCK pin for APA102 clock */
static const uint8_t g_RGB_CLOCK_PIN = 6;


/******************************************************************************
 * Public Methods
 *****************************************************************************/

LedSpiAccess::LedSpiAccess(uint32_t clock, BitOrder bitOrder, arduino::SPIMode dataMode)
{
    /* Switch SPI pins to leds. */
    SPI.setSCK(g_RGB_CLOCK_PIN);
    SPI.setTX(g_RGB_DATA_PIN);

    SPI.begin();
    SPI.beginTransaction(SPISettings(clock, bitOrder, dataMode));
}

LedSpiAccess::~LedSpiAccess()
{
    SPI.endTransaction();
    SPI.end();

}

inline uint8_t LedSpiAccess::transfer(uint8_t data)
{
    return SPI.transfer(data);
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

void setLedYellow(bool onOff)
{
    g_LedState[g_IDX_LED_YELLOW].state =
        onOff ? g_SET_BRIGHTNESS_ON : g_SET_BRIGHTNESS_OFF;

    refreshLeds();
}

/******************************************************************************
 * Local Functions
 *****************************************************************************/

static void refreshLeds()
{
    LedSpiAccess ledAccess(g_SPEEDHZ_LED, MSBFIRST, SPI_MODE0);

    /* Sends the needed startframe */
    for (int chunk = 0; chunk < g_START_FRAME_BYTES; chunk++)
    {
        ledAccess.transfer(g_START_FRAME_CHUNK);
    }

    for (const auto& ledState : g_LedState) {
        /* Send the LED brightness state. */
        ledAccess.transfer(ledState.state);

        /* Send RGB color values. */
        ledAccess.transfer(ledState.color.blue);
        ledAccess.transfer(ledState.color.green);
        ledAccess.transfer(ledState.color.red);
    }

    /* Sends the needed endframe */
    for (int chunk = 0; chunk < g_END_FRAME_BYTES; chunk++)
    {
        ledAccess.transfer(g_END_FRAME_CHUNK);
    }

}