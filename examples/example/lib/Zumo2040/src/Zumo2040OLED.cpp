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
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Zumo2040OLED.h"
#include "SPI.h"
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
namespace Sh1106Modes
{
    /** Indicates the command mode. */
    constexpr bool COMMAND = 0;
    /** Indicates the data mode. */
    constexpr bool DATA = 1;
}

/** Stabilization time in us for the OLED reset. */
constexpr uint8_t STABILIZATION_TIME_US = 10u;
/** Clock frequency for the SPI. */
constexpr uint32_t OLED_CLOCK_FREQUENCY = 20000000u;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

void Zumo2040OLEDcore::setPins(uint8_t clk, uint8_t mos, uint8_t res, uint8_t dc, uint8_t cs)
{
    m_clkPin = clk;
    m_mosPin = mos;
    m_resPin = res;
    m_dcPin = dc;
    m_csPin = cs;
}

void Zumo2040OLEDcore::initPins()
{
    if (Zumo2040Pins::UNUSED_OLED_PIN != m_resPin)
    {
        pinMode(m_resPin, OUTPUT);
    }

    pinMode(m_clkPin, OUTPUT);
    digitalWrite(m_clkPin, LOW);
    pinMode(m_mosPin, OUTPUT);

    if (Zumo2040Pins::UNUSED_OLED_PIN != m_dcPin)
    {
        pinMode(m_dcPin, OUTPUT);
    }
    if (Zumo2040Pins::UNUSED_OLED_PIN != m_csPin)
    {
        pinMode(m_csPin, OUTPUT);
    }
}

void Zumo2040OLEDcore::reset()
{
    if (Zumo2040Pins::UNUSED_OLED_PIN == m_resPin)
    {
        return;
    }

    digitalWrite(m_resPin, LOW);
    delayMicroseconds(STABILIZATION_TIME_US);
    digitalWrite(m_resPin, HIGH);
    delayMicroseconds(STABILIZATION_TIME_US);
}

void Zumo2040OLEDcore::sh1106TransferStart()
{
    if (Zumo2040Pins::UNUSED_OLED_PIN != m_csPin)
    {
        /* Select the SH1106 controller */
        digitalWrite(m_csPin, LOW);
    }

    /* Switch SPI pins to the OLED. */
    SPI.setSCK(m_clkPin);
    SPI.setTX(m_mosPin);

    SPI.begin();
    SPI.beginTransaction(SPISettings(OLED_CLOCK_FREQUENCY, MSBFIRST, SPI_MODE0));
}

void Zumo2040OLEDcore::sh1106TransferEnd()
{
    if (Zumo2040Pins::UNUSED_OLED_PIN != m_csPin)
    {
        /* Deselect the SH1106 controller */
        digitalWrite(m_csPin, HIGH);
    }

    SPI.endTransaction();
    SPI.end();
}

void Zumo2040OLEDcore::sh1106CommandMode()
{
    m_dataMode = Sh1106Modes::COMMAND;

    if (Zumo2040Pins::UNUSED_OLED_PIN != m_dcPin)
    {
        /* Set command mode */
        digitalWrite(m_dcPin, LOW);
    }
}

void Zumo2040OLEDcore::sh1106DataMode()
{
    m_dataMode = Sh1106Modes::DATA;

    if (Zumo2040Pins::UNUSED_OLED_PIN != m_dcPin)
    {
        /* Set data mode */
        digitalWrite(m_dcPin, HIGH);
    }
}

void Zumo2040OLEDcore::sh1106Write(uint8_t data)
{
    SPI.transfer(data);
}

Zumo2040OLED::Zumo2040OLED(uint8_t clk, uint8_t mos, uint8_t res, uint8_t dc, uint8_t cs)
{
    core.setPins(clk, mos, res, dc, cs);
    /* Configure layout to support displaying 32-bit values. */
    setLayout11x4();
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
