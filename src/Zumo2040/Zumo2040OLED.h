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
 * @brief  OLED driver
 * @author Felix Reitenauer
 *
 * @addtogroup OLED
 *
 * @{
 */

#ifndef ZUMO2040OLED_H
#define ZUMO2040OLED_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Zumo2040Pin.h"
#include <PololuOLED.h>
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** Low-level functions for writing data to the SH1106 OLED on
 *  the Pololu Zumo 2040 robot.
 *  This class is used as the template parameter for PololuSH1106Main.
 */
class Zumo2040OLEDcore
{
    public:
        /**
         * @brief Configures what pins this class will use.
         *
         * @param clk SPI SCK pin for OLED clock.
         * @param mos SPI MOSI pin for the OLED data.
         * @param res OLED reset pin (use 255 if it is not used).
         * @param dc OLED data/not command pin (use 255 if it is not used).
         * @param cs Chip-select pin (use 255 if it is not used).
         */
        void setPins(uint8_t clk, uint8_t mos, uint8_t res = 255, uint8_t dc = 255, uint8_t cs = 255);

        /**
         * @brief This function is called by PololuSH1106Main to perform any initializations
         *        that might be needed for the other functions to work properly.
         */
        void initPins();

        /**
         * @brief This function is called by PololuSH1106Main to reset the SH1106.
         */
        void reset();

        /**
         * @brief This function is called by PololuSH1106Main to start a data
         *        transfer to the SH1106.
         */
        void sh1106TransferStart();

        /**
         * @brief This function is called by PololuSH1106Main to end a data
         *        transfer to the SH1106.
         */
        void sh1106TransferEnd();

        /**
         * @brief This function is called by PololuSH1106Main to indicate that the
         *        bytes it is about to pass to sh1106Write are command bytes.
         */
        void sh1106CommandMode();

        /**
         * @brief This function is called by PololuSH1106Main to indicate that the
         *        bytes it is about to pass to sh1106Write are data bytes.
         */
        void sh1106DataMode();

        /**
         * @brief This function is called by PololuSH1106Main to write commands or
         *        data to the SH1106.
         *
         * @param data data/command byte.
         */
        void sh1106Write(uint8_t data);

    private:
        /** SPI SCK pin for OLED clock. */
        uint8_t m_clkPin;
        /** SPI MOSI pin for the OLED data. */
        uint8_t m_mosPin;
        /** OLED reset pin. */
        uint8_t m_resetPin;
        /** OLED data/not command pin. */
        uint8_t m_dcPin;
        /** Chip-select pin. */
        uint8_t m_csPin;
        /** Indicates the current SH1106 mode. */
        bool m_dataMode;
};

/** This class is based on PololuSH1106Main from the PololuOLED
 *  library, which provides almost all of its functionality. See the
 *  [PololuOLED library documentation](https://pololu.github.io/pololu-oled-arduino/)
 *  for more information about how to use this class.
 *
 *  This class also inherits from the Arduino Print class
 *  (via PololuSH1106Main), so you can call the `print()` function on it with a
 *  variety of arguments. See the [Arduino print() documentation](http://arduino.cc/en/Serial/Print) for
 *  more information.
 */
class Zumo2040OLED : public PololuSH1106Main<Zumo2040OLEDcore>
{
    public:
    /**
     * @brief Constructs a new Zumo2040OLED object and initializes the display
     *        layout to 11x4 as well as the hardware pins.
     *
     * @param clk SPI SCK pin for OLED clock.
     * @param mos SPI MOSI pin for the OLED data.
     * @param res OLED reset pin (use 255 if it is not used).
     * @param dc OLED data/not command pin (use 255 if it is not used).
     * @param cs Chip-select pin (use 255 if it is not used).
     */
    Zumo2040OLED(uint8_t clk = Zumo2040Pins::OLED_CLOCK_PIN,
                 uint8_t mos = Zumo2040Pins::RGB_OLED_DATA_PIN,
                 uint8_t res = Zumo2040Pins::OLED_RESET_PIN,
                 uint8_t dc = Zumo2040Pins::BUTTON_C_OLED_DATA_COMMAND_PIN,
                 uint8_t cs = Zumo2040Pins::UNUSED_OLED_PIN);
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040OLED_H */
/** @} */
