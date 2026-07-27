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
 * @brief  Inertial Measurement Unit driver
 * @author Felix Reitenauer
 *
 * @addtogroup Zumo2040IMU
 *
 * @{
 */

#ifndef ZUMO2040IMU_H
#define ZUMO2040IMU_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Zumo2040I2C0.h"
#include "Zumo2040ErrorCodes.h"
#include <cstdint>
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

namespace Zumo2040
{
    /** Represents a 3-axis IMU sensor value vector. */
    struct IMUvector
    {
        /** Represents the x-axis. */
        int16_t x;
        /** Represents the y-axis. */
        int16_t y;
        /** Represents the z-axis. */
        int16_t z;
    };

    /** Indicates whether new IMU data is available. */
    enum DataStatus
    {
        /** Indicates that there is no new data available. */
        NO_NEW_DATA = false,
        /** Indicates that there is new data available. */
        NEW_DATA = true
    };

    /** Indicates the IMU calibration status. */
    enum IMUCalibrationStatus
    {
        /** Indicates that the IMU is not calibrated. */
        IMU_NOT_CALIBRATED = false,
        /** Indicates that the IMU is calibrated. */
        IMU_CALIBRATED = true
    };
} /* namespace Zumo2040 */

/** Driver for the IMU. */
class Zumo2040IMU
{
    public:
        /** Initializes the IMU driver members. */
        Zumo2040IMU();

        /** Default destructor. */
        ~Zumo2040IMU() = default;

        /** Delete copy constructor. */
        Zumo2040IMU(const Zumo2040IMU&) = delete;

        /** Delete copy assignment operator. */
        Zumo2040IMU& operator= (const Zumo2040IMU&) = delete;

        /**
         * @brief Checks whether the known IMU can be detected.
         *
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode checkIMU();

        /**
         * @brief Configures the IMU with the default configuration.
         *
         * Accelerometer: ODR = 52 Hz in high-performance mode,
         *                full scale = +/- 2 g.
         *
         * Gyroscope: ODR = 208 Hz in high-performance mode,
         *            full scale = +/- 250 dps.
         *
         * Accelerometer + gyroscope: Auto-increment for register addresses enabled.
         *
         * Magnetometer: OM = ultra-high-performance mode for X and Y,
         *               ODR = 10 Hz,
         *               full scale = +/- 4 gauss,
         *               MD = continuous-conversion mode,
         *               OMZ = ultra-high-performance mode for Z.
         *
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode enableDefault();

        /**
         * @brief Configures the IMU with settings optimized for turn sensing.
         *
         * Gyroscope: ODR = 833 Hz in high-performance mode,
         *            full scale = +/- 500 dps.
         *
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode configureForTurnSensing();

        /**
         * @brief Configures the IMU with settings optimized for compass heading.
         *
         * Magnetometer: OM = ultra-high-performance mode for X and Y,
         *               ODR = 80 Hz.
         *
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode configureForCompassHeading();

        /**
         * @brief Reads the accelerometer data from the LSM6DSO.
         *
         * @param data Reference to the vector where the results are stored.
         *
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode readAcc(Zumo2040::IMUvector& data);

        /**
         * @brief Reads the gyroscope data from the LSM6DSO.
         *
         * @param data Reference to the vector where the results are stored.
         *
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode readGyro(Zumo2040::IMUvector& data);

        /**
         * @brief Reads the magnetometer data from the LIS3MDL.
         *
         * @param data Reference to the vector where the results are stored.
         *
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode readMag(Zumo2040::IMUvector& data);

        /**
         * @brief Checks whether new acceleration data is available.
         *
         * @return Data status indicating whether new data is available.
         */
        Zumo2040::DataStatus accDataReady();

        /**
         * @brief Checks whether new angular velocity data is available.
         *
         * @return Data status indicating whether new data is available.
         */
        Zumo2040::DataStatus gyroDataReady();

        /**
         * @brief Checks whether new magnetic field data is available.
         *
         * @return Data status indicating whether new data is available.
         */
        Zumo2040::DataStatus magDataReady();

        /**
         * @brief Calibrates the IMU for line-following behavior.
         *
         * Only the used axes are calibrated: acceleration along X and turn rate around Z.
         *
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode calibrateLineFollower();

        /**
         * @brief Gets the last error code.
         *
         * @return The last error code.
         */
        Zumo2040::ErrorCode getLastError() const;

        /**
         * @brief Gets the accelerometer x-axis offset.
         *
         * @return Offset of the accelerometer x-axis, or zero if the IMU is not calibrated.
         */
        int16_t getAccelOffsetX();

        /**
         * @brief Gets the gyroscope z-axis offset.
         *
         * @return Offset of the gyroscope z-axis, or zero if the IMU is not calibrated.
         */
        int16_t getGyroOffsetZ();

        /**
         * @brief Resets the error status of the IMU.
         */
        void resetError();

        /**
         * @brief Reads one byte from the IMU using I2C.
         *
         * @param addr Address of the slave.
         * @param reg Register address.
         * @return The read value.
         */
        uint8_t readOneByte(uint8_t addr, uint8_t reg);

    private:

        /**
         * @brief Writes one byte to the IMU using I2C.
         *
         * @param addr Address of the slave.
         * @param reg Register address.
         * @param value Value to write to the register.
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode writeOneByte(uint8_t addr, uint8_t reg, uint8_t value);

        /**
         * @brief Reads six consecutive registers containing the x-, y-, and z-axis values
         * and combines them into three vector coordinates.
         *
         * @param addr Address of the slave.
         * @param startReg Register address containing the first byte of the vector values.
         * @param data Reference to the vector where the results are stored.
         * @return The resulting error code.
         */
        Zumo2040::ErrorCode readAxes16Bit(uint8_t addr, uint8_t startReg, Zumo2040::IMUvector& data);

        /** Last occurred error code. */
        Zumo2040::ErrorCode m_error;

        /** Offset of the accelerometer x-axis. */
        int16_t m_rawAccelerometerOffsetX;

        /** Offset of the gyroscope z-axis. */
        int16_t m_rawGyroOffsetZ;

        /** Indicates whether the IMU is calibrated. */
        Zumo2040::IMUCalibrationStatus m_calStatus;

        /** Driver for the IMU I2C communication. */
        Zumo2040IMU_I2C m_i2c;
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040IMU_H */
/** @} */
