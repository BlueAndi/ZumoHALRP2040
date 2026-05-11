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
 * @brief  IMU I2C driver
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Zumo2040I2C0.h"
#include "Zumo2040Pin.h"
#include <hardware/i2c.h>
#include <pico/stdlib.h>
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

/** Unsigned 32-bit integer zero. */
constexpr uint32_t U_INTEGER_32_ZERO = 0u;

/** Clock speed for the I2C data transmission. */
constexpr uint32_t I2C_CLOCK_SPEED = 400000u;

/** Timeout for the I2C transfer. */
constexpr uint32_t I2C_TIMEOUT_US = 1000u;

/** Size of the register address in bytes. */
constexpr uint32_t REGISTER_ADDRESS_SIZE_BYTES = 1u;

/** Maximum number of data bytes that can be transmitted in one write call. */
constexpr uint32_t MAX_SIZE_WRITE_DATA_BYTES = 15u;

/** Size of the buffer that holds the register address and the data to write. */
constexpr uint32_t WRITE_BUFFER_SIZE = MAX_SIZE_WRITE_DATA_BYTES + REGISTER_ADDRESS_SIZE_BYTES;

/** Do not stop the I2C transfer after the current transmission. */
constexpr bool I2C_NO_STOP = true;

/** Stop the I2C transfer after the current transmission. */
constexpr bool I2C_STOP = false;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

Zumo2040IMU_I2C::Zumo2040IMU_I2C()
{
    /* Initialize the I2C bus and the pins used by the IMU. */
    i2c_init(i2c0, I2C_CLOCK_SPEED);
    gpio_set_function(Zumo2040Pins::I2C0_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(Zumo2040Pins::I2C0_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(Zumo2040Pins::I2C0_SDA_PIN);
    gpio_pull_up(Zumo2040Pins::I2C0_SCL_PIN);
}

Zumo2040IMU_I2C::~Zumo2040IMU_I2C()
{
    /* Deinitialize the pins used by the IMU. */
    gpio_set_function(Zumo2040Pins::I2C0_SDA_PIN, GPIO_FUNC_NULL);
    gpio_set_function(Zumo2040Pins::I2C0_SCL_PIN, GPIO_FUNC_NULL);
    gpio_disable_pulls(Zumo2040Pins::I2C0_SDA_PIN);
    gpio_disable_pulls(Zumo2040Pins::I2C0_SCL_PIN);
}

ErrorCode Zumo2040IMU_I2C::read(uint8_t addr, uint8_t startReg, uint8_t* data, uint32_t length)
{
    int test = PICO_ERROR_GENERIC;
    if (nullptr == data || U_INTEGER_32_ZERO == length)
    {
        return I2C_READ_FAILED;
    }

    /* Write the register address to the slave. This is needed so the slave knows
     * which register shall be read.
     */
    test = i2c_write_blocking_until(i2c0, addr, &startReg, REGISTER_ADDRESS_SIZE_BYTES, I2C_NO_STOP, make_timeout_time_us(I2C_TIMEOUT_US));

     /* Check whether the expected number of bytes was written. */
    if (static_cast<int>(REGISTER_ADDRESS_SIZE_BYTES) != test)
    {
        return I2C_READ_FAILED;
    }

    /* Read length bytes starting from the previously configured register. */
    test = i2c_read_blocking_until(i2c0, addr, data, length, I2C_STOP, make_timeout_time_us(I2C_TIMEOUT_US));

    /* Check whether the expected number of bytes was read. */
    if (static_cast<int>(length) != test)
    {
        return I2C_READ_FAILED;
    }

    return NONE;
}

ErrorCode Zumo2040IMU_I2C::write(uint8_t addr, uint8_t startReg, const uint8_t* data, uint32_t length)
{
    int test = PICO_ERROR_GENERIC;
    uint8_t buffer[WRITE_BUFFER_SIZE];

    if (nullptr == data || MAX_SIZE_WRITE_DATA_BYTES < length || U_INTEGER_32_ZERO == length)
    {
        return I2C_WRITE_FAILED;
    }

    /* The first byte to transmit is the register address. */
    buffer[U_INTEGER_32_ZERO] = startReg;

    for (uint32_t idx = U_INTEGER_32_ZERO; idx < length; idx++)
    {
        buffer[idx + REGISTER_ADDRESS_SIZE_BYTES] = data[idx];
    }

    const uint32_t writeLength = length + REGISTER_ADDRESS_SIZE_BYTES;

    /* Write the buffer content to the slave. */
    test = i2c_write_blocking_until(i2c0, addr, buffer, writeLength, I2C_STOP, make_timeout_time_us(I2C_TIMEOUT_US));

    /* Check whether the expected number of bytes was written. */
    if (static_cast<int>(writeLength) != test)
    {
        return I2C_WRITE_FAILED;
    }

    return NONE;
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
