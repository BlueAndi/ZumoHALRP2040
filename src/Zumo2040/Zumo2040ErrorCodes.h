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
 * @brief Zumo2040 error codes.
 * @author Felix Reitenauer
 *
 * @addtogroup Error codes
 *
 * @{
 */

#ifndef ZUMO2040ERRORCODES_H
#define ZUMO2040ERRORCODES_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** Defines the error codes for the Zumo2040. */
enum ErrorCode
{
    /** No error. */
    NONE,

    /** The program could not be added to the PIO instance. */
    CANT_ADD_PROGRAM,

    /** No unused state machine could be claimed. */
    CANT_CLAIM_SM,

    /** No unused state machine could be claimed for the left encoder. */
    ENCODER_CANT_CLAIM_SM_LEFT,

    /** No unused state machine could be claimed for the right encoder. */
    ENCODER_CANT_CLAIM_SM_RIGHT,

    /** The calibration of the line sensors failed. */
    LINESENSOR_CALIBRATION_FAILED,

    /** The line sensors are not calibrated. */
    LINESENSOR_IS_NOT_CALIBRATED,

    /** The read line calculation has zero as divisor. */
    LINESENSOR_READ_LINE_ZERO_DIVISOR,

    /** The core initialization failed. */
    LINESENSOR_CORE_INITIALIZATION_FAILED,

    /** Line sensor bounds calculation failed. */
    LINESENSOR_BOUND_CALCULATION_FAILED,

    /** I2C read operation failed. */
    I2C_READ_FAILED,

    /** I2C read argument is invalid. */
    I2C_READ_INVALID_ARGUMENT,

    /** I2C write operation failed. */
    I2C_WRITE_FAILED,

    /** I2C write argument is invalid. */
    I2C_WRITE_INVALID_ARGUMENT,

    /** Unknown IMU detected. */
    IMU_UNKNOWN,

    /** IMU default configuration failed. */
    IMU_DEFAULT_CONFIG_FAILED,

    /** IMU turn-sensing configuration failed. */
    IMU_CONFIG_FOR_TURN_SENSING_FAILED,

    /** IMU compass-heading configuration failed. */
    IMU_CONFIG_FOR_COMPASS_HEADING_FAILED,

    /** Reading IMU axis values failed. */
    IMU_READ_AXES_FAILED,

    /** Reading accelerometer values failed. */
    IMU_READ_ACC_FAILED,

    /** Reading gyroscope values failed. */
    IMU_READ_GYRO_FAILED,

    /** Reading magnetometer values failed. */
    IMU_READ_MAGNET_FAILED,

    /** IMU calibration failed. */
    IMU_CALIBRATION_FAILED,

    /** Cannot return the offset because the IMU is not calibrated. */
    IMU_CANT_GET_OFFSET_NOT_CALIBRATED,

    /** A frequency of zero is not allowed. */
    BUZZER_INVALID_FREQ_ZERO,

    /** The frequency is too small. */
    BUZZER_INVALID_FREQ_TOO_SMALL,

    /** The alarm for the callback could not be set. */
    BUZZER_COULD_NOT_SET_ALARM,

    /** A duration of zero is not allowed. */
    BUZZER_INVALID_DURATION_ZERO,

    /** The given note sequence pointer is a nullptr. */
    BUZZER_INVALID_MELODY_NULLPTR
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040ERRORCODES_H */
/** @} */
