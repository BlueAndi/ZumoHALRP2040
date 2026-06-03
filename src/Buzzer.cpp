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
 * @brief  Buzzer realization
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Buzzer.h"
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

/******************************************************************************
 * Public Methods
 *****************************************************************************/

void Buzzer::playFrequency(uint16_t freq, uint16_t duration, uint8_t volume)
{
    unsigned int freqTarget = freq;

    /* If the frequency has a fractional value, the most significant bit in the target frequency
     * must be set. This means that the lower 15 bit are considered in 0.1 Hz.
     */
    if (0 != (freq % 10))
    {
        freqTarget |= DIV_BY_10_BIT;
    }
    else
    /* The target frequency is considered in Hz, therefore divide by 10. */
    {
        freqTarget /= 10;
    }

    /* The Zumo2040 implementation does not have the Zumo32U4 limitation
    * frequency * duration / 1000 <= 65535 because playback timing is handled
    * differently.
    * Applications should still respect the common buzzer interface limits to
    * remain portable to the Zumo32U4 implementation.
    */
    m_buzzer.playFrequency(freqTarget, static_cast<uint32_t>(duration), static_cast<uint16_t>(volume));
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
