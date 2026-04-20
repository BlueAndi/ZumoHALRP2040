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
 * @brief  Low-Level Encoder driver
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "RP2040Encoder.h"
#include "Zumo2040Pin.h"
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

/** Offset at which the PIO program is loaded. */
constexpr uint8_t OFFSET_ZERO = 0u;

/** Bit threshold for ISR autopush.
 *  Autopush is disabled in this program, but a threshold value is still required.
 */
constexpr uint8_t PUSH_THRESHOLD = 32u;

/** Number of pins used by one encoder. */
constexpr uint8_t PIN_COUNT = 2u;

/** Integer value zero used in several places. */
constexpr int8_t INTEGER_ZERO = 0;

/** Integer value one used in several places. */
constexpr int8_t INTEGER_ONE = 1;

/** Encoded PIO instruction used to reset the counter. */
constexpr uint PIO_RESET_COUNTER_COMMAND = 0xa023;

/** Configures ISR shifting to the left. */
constexpr bool SHIFT_LEFT = false;

/** Disables ISR autopush. */
constexpr bool DISABLE_AUTOPUSH = false;

/** Prevents a panic if no free state machine can be claimed. */
constexpr bool DONT_PANIC = false;

/** Configures pins as inputs for PIO usage. */
constexpr bool INPUT = false;

/** Enables the PIO state machine. */
constexpr bool ENABLE = true;

/** Disables the PIO state machine. */
constexpr bool DISABLE = false;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

EncoderCore::EncoderCore() : m_errorCode(NONE),
                             m_pio(pio0),
                             m_programEntry(rp2040encoder_offset_entry_point)
{}

EncoderCore::~EncoderCore()
{
    /* Disable and unclaim the state machine used by the left encoder. */
    pio_sm_set_enabled(m_pio, m_smLeft, DISABLE);
    pio_sm_unclaim(m_pio, m_smLeft);

    /* Disable and unclaim the state machine used by the right encoder. */
    pio_sm_set_enabled(m_pio, m_smRight, DISABLE);
    pio_sm_unclaim(m_pio, m_smRight);
}

EncoderError EncoderCore::init()
{
    /* Check whether the PIO program can be loaded at offset 0.
     * Offset 0 is required because the PIO program uses 'mov pc, isr'.
     */
    if (!pio_can_add_program_at_offset(m_pio, &rp2040encoder_program, OFFSET_ZERO))
    {
        m_errorCode = CANT_ADD_PROGRAM;
        return m_errorCode;
    }

    /* Load the program into the selected PIO instance. */
    pio_add_program_at_offset(m_pio, &rp2040encoder_program, OFFSET_ZERO);
    m_smLeft = pio_claim_unused_sm(m_pio, DONT_PANIC);

    /* If no unused state machine is available,
     * pio_claim_unused_sm() returns PICO_ERROR_GENERIC.
     */
    if (m_smLeft == PICO_ERROR_GENERIC)
    {
        m_errorCode = CANT_CLAIM_SM_LEFT;
        return m_errorCode;
    }

    /* Configure the left encoder pins for PIO input usage. */
    pio_sm_set_consecutive_pindirs(m_pio,
                                   m_smLeft,
                                   Zumo2040Pins::LEFT_ENCODER_A_PIN,
                                   PIN_COUNT,
                                   INPUT);

    pio_gpio_init(m_pio, Zumo2040Pins::LEFT_ENCODER_A_PIN);
    pio_gpio_init(m_pio, Zumo2040Pins::LEFT_ENCODER_B_PIN);

    /* Configure the state machine for the left encoder. */
    m_configLeft = rp2040encoder_program_get_default_config(OFFSET_ZERO);
    sm_config_set_in_pin_base(&m_configLeft, Zumo2040Pins::LEFT_ENCODER_A_PIN);
    sm_config_set_in_shift(&m_configLeft,
                           SHIFT_LEFT,
                           DISABLE_AUTOPUSH,
                           PUSH_THRESHOLD);

    pio_sm_init(m_pio, m_smLeft, m_programEntry, &m_configLeft);
    pio_sm_set_enabled(m_pio, m_smLeft, ENABLE);

    /* Repeat the same initialization sequence for the right encoder. */
    m_smRight = pio_claim_unused_sm(m_pio, DONT_PANIC);

    if (m_smRight == PICO_ERROR_GENERIC)
    {
        m_errorCode = CANT_CLAIM_SM_RIGHT;
        return m_errorCode;
    }

    /* Configure the right encoder pins as PIO inputs. */
    pio_sm_set_consecutive_pindirs(m_pio,
                                   m_smRight,
                                   Zumo2040Pins::RIGHT_ENCODER_A_PIN,
                                   PIN_COUNT,
                                   INPUT);

    pio_gpio_init(m_pio, Zumo2040Pins::RIGHT_ENCODER_A_PIN);
    pio_gpio_init(m_pio, Zumo2040Pins::RIGHT_ENCODER_B_PIN);

    /* Configure the state machine for the right encoder. */
    m_configRight = rp2040encoder_program_get_default_config(OFFSET_ZERO);
    sm_config_set_in_pin_base(&m_configRight, Zumo2040Pins::RIGHT_ENCODER_A_PIN);
    sm_config_set_in_shift(&m_configRight,
                           SHIFT_LEFT,
                           DISABLE_AUTOPUSH,
                           PUSH_THRESHOLD);

    pio_sm_init(m_pio, m_smRight, m_programEntry, &m_configRight);
    pio_sm_set_enabled(m_pio, m_smRight, ENABLE);

    return NONE;
}

int32_t EncoderCore::getCount(EncoderSide side)
{
    const uint sm = (LEFT == side) ? m_smLeft : m_smRight;

    int32_t raw = INTEGER_ZERO;
    /* Get the current number of entries in the RX FIFO. */
    uint count = pio_sm_get_rx_fifo_level(m_pio, sm);
    /* Drain all currently buffered FIFO entries. */
    while (count > INTEGER_ZERO)
    {
        pio_sm_get(m_pio, sm);
        count--;
    }
    /* Read a fresh encoder count value. This call blocks until a new value is available. */
    raw = pio_sm_get_blocking(m_pio, sm);

    return raw;
}

void EncoderCore::resetCount(EncoderSide side)
{
    const uint sm = (LEFT == side) ? m_smLeft : m_smRight;
    /* Execute the PIO reset instruction on the selected state machine. */
    pio_sm_exec(m_pio, sm, PIO_RESET_COUNTER_COMMAND);
}

EncoderError EncoderCore::getError()
{
    return m_errorCode;
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
