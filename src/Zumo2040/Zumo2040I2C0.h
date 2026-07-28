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
 * @brief IMU I2C driver
 * @author Felix Reitenauer
 *
 * @addtogroup Zumo2040IMU
 *
 * @{
 */

#ifndef ZUMO2040I2C0_H
#define ZUMO2040I2C0_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <cstdint>
#include "Zumo2040ErrorCodes.h"
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** Driver for the Zumo 2040 I2C bus used by the IMU. */
class Zumo2040IMU_I2C
{
    public:
        /** Initializes the I2C bus and pins used by the IMU. */
        Zumo2040IMU_I2C();

        /** Deinitializes the pins used by the IMU. */
        ~Zumo2040IMU_I2C();

        /** Delete copy constructor. */
        Zumo2040IMU_I2C(const Zumo2040IMU_I2C&) = delete;

        /** Delete copy assignment operator. */
        Zumo2040IMU_I2C& operator=(const Zumo2040IMU_I2C&) = delete;

        /**
         * @brief Reads data from a slave, starting at the specified register.
         *
         * @param addr Address of the slave.
         * @param startReg Address of the first register to read.
         * @param data Array that stores the read values.
         * @param length Number of bytes to read.
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode read(uint8_t addr, uint8_t startReg, uint8_t* data, uint32_t length);

        /**
         * @brief Writes data to a slave, starting at the specified register.
         *
         * @param addr Address of the slave.
         * @param startReg Address of the first register to write.
         * @param data Array that stores the values to write.
         * @param length Number of bytes to write. The maximum value is 15.
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode write(uint8_t addr, uint8_t startReg, const uint8_t* data, uint32_t length);
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040I2C0_H */
/** @} */
