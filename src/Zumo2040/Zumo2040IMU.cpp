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
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Zumo2040IMU.h"
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

/** Size of a one-byte buffer. */
constexpr uint8_t SIZE_ONE_BYTE = 1u;

/** Transfer length for one byte. */
constexpr uint32_t LENGTH_ONE_BYTE = 1u;

/** Unsigned 8-bit integer zero used in several places. */
constexpr uint8_t U_INTEGER_8_ZERO = 0u;

/** Unsigned 32-bit integer zero used in several places. */
constexpr uint32_t U_INTEGER_32_ZERO = 0u;

/** Signed 16-bit integer zero used in several places. */
constexpr int16_t INTEGER_16_ZERO = 0;

/** Signed 32-bit integer zero used in several places. */
constexpr int32_t INTEGER_32_ZERO = 0;

/** Unsigned 8-bit integer one used in several places. */
constexpr uint8_t U_INTEGER_8_ONE = 1u;

/** I2C address of the LIS3MDL. */
constexpr uint8_t LIS3MDL_ADDR = 0x1Eu;

/** Register address of the LIS3MDL WHO_AM_I register. */
constexpr uint8_t LIS3MDL_WHO_AM_I_ADDR = 0x0Fu;

/** Expected value of the LIS3MDL WHO_AM_I register. */
constexpr uint8_t LIS3MDL_WHO_AM_I_ID = 0b00111101u;

/** Register address of the LIS3MDL CTRL_REG1 register. */
constexpr uint8_t LIS3MDL_REG_CTRL_REG1 = 0x20u;

/** Register address of the LIS3MDL CTRL_REG2 register. */
constexpr uint8_t LIS3MDL_REG_CTRL_REG2 = 0x21u;

/** Register address of the LIS3MDL CTRL_REG3 register. */
constexpr uint8_t LIS3MDL_REG_CTRL_REG3 = 0x22u;

/** Register address of the LIS3MDL CTRL_REG4 register. */
constexpr uint8_t LIS3MDL_REG_CTRL_REG4 = 0x23u;

/** LIS3MDL magnetometer default configuration for the CTRL_REG1 register:
 *  OM = 11 (ultra-high-performance mode for X and Y),
 *  DO = 100 (10 Hz ODR).
 */
constexpr uint8_t LIS3MDL_MAGNET_DEFAULT_OM_ODR = 0x70u;

/** LIS3MDL magnetometer default configuration for the CTRL_REG2 register:
 *  FS = 00 (+/- 4 gauss full scale).
 */
constexpr uint8_t LIS3MDL_MAGNET_DEFAULT_FS = 0x00u;

/** LIS3MDL magnetometer default configuration for the CTRL_REG3 register:
 *  MD = 00 (continuous-conversion mode).
 */
constexpr uint8_t LIS3MDL_MAGNET_DEFAULT_CONV_MODE = 0x00u;

/** LIS3MDL magnetometer default configuration for the CTRL_REG4 register:
 *  OMZ = 11 (ultra-high-performance mode for Z).
 */
constexpr uint8_t LIS3MDL_MAGNET_DEFAULT_OM = 0x0Cu;

/** LIS3MDL magnetometer compass heading configuration for the CTRL_REG1 register:
 *  OM = 11 (ultra-high-performance mode for X and Y),
 *  DO = 111 (80 Hz ODR).
 */
constexpr uint8_t LIS3MDL_MAGNET_CONFIG_COMPASS_HEADING = 0x7Cu;

/** Starting register address of the LIS3MDL magnetometer output values.
 *  Auto-increment is enabled.
 */
constexpr uint8_t LIS3MDL_START_REG_MAGNET_AUTO_INC = 0xA8u;

/** Status register of the LIS3MDL. */
constexpr uint8_t LIS3MDL_STATUS_REG = 0x27u;

/** Mask used to isolate the magnetometer data-ready bit in the status register. */
constexpr uint8_t LIS3MDL_STATUS_MAGNET_MASK = 0b00001000u;

/** I2C address of the LSM6DSO. */
constexpr uint8_t LSM6DSO_ADDR = 0x6Bu;

/** Register address of the LSM6DSO WHO_AM_I register. */
constexpr uint8_t LSM6DSO_WHO_AM_I_ADDR = 0x0Fu;

/** Expected value of the LSM6DSO WHO_AM_I register. */
constexpr uint8_t LSM6DSO_WHO_AM_I_ID = 0b01101100u;

/** Register address of the LSM6DSO CTRL1_XL register. */
constexpr uint8_t LSM6DSO_REG_CTRL1_XL = 0x10u;

/** Register address of the LSM6DSO CTRL2_G register. */
constexpr uint8_t LSM6DSO_REG_CTRL2_G = 0x11u;

/** Register address of the LSM6DSO CTRL3_C register. */
constexpr uint8_t LSM6DSO_REG_CTRL3_C = 0x12u;

/** LSM6DSO accelerometer default configuration for the CTRL1_XL register:
 *  ODR = 0011 (52 Hz in high-performance mode),
 *  FS_XL = 00 (+/- 2 g full scale).
 */
constexpr uint8_t LSM6DSO_ACCEL_DEFAULT_CONFIG = 0x30u;

/** LSM6DSO gyroscope default configuration for the CTRL2_G register:
 *  ODR = 0101 (208 Hz in high-performance mode),
 *  FS_G = 00 (+/- 250 dps full scale).
 */
constexpr uint8_t LSM6DSO_GYRO_DEFAULT_CONFIG = 0x50u;

/** LSM6DSO automatic register increment configuration for the CTRL3_C register:
 *  IF_INC = 1, automatically increment register addresses.
 */
constexpr uint8_t LSM6DSO_AUTO_INC_CONFIG = 0x04u;

/** LSM6DSO gyroscope turn-sensing configuration for the CTRL2_G register:
 *  ODR = 0111 (833 Hz in high-performance mode),
 *  FS_G = 01 (+/- 500 dps full scale).
 */
constexpr uint8_t LSM6DSO_GYRO_CONFIG_TURN_SENS = 0x74u;

/** Starting register address of the LSM6DSO accelerometer output values. */
constexpr uint8_t LSM6DSO_START_REG_ACCEL = 0x28u;

/** Starting register address of the LSM6DSO gyroscope output values. */
constexpr uint8_t LSM6DSO_START_REG_GYRO = 0x22u;

/** Status register of the LSM6DSO. */
constexpr uint8_t LSM6DSO_STATUS_REG = 0x1Eu;

/** Mask used to isolate the accelerometer data-ready bit in the status register. */
constexpr uint8_t LSM6DSO_STATUS_ACC_MASK = 0b1u;

/** Mask used to isolate the gyroscope data-ready bit in the status register. */
constexpr uint8_t LSM6DSO_STATUS_GYRO_MASK = 0b10u;

/** Index of the low byte of the x-axis value. */
constexpr int16_t X_L = 0;

/** Index of the high byte of the x-axis value. */
constexpr int16_t X_H = 1;

/** Index of the low byte of the y-axis value. */
constexpr int16_t Y_L = 2;

/** Index of the high byte of the y-axis value. */
constexpr int16_t Y_H = 3;

/** Index of the low byte of the z-axis value. */
constexpr int16_t Z_L = 4;

/** Index of the high byte of the z-axis value. */
constexpr int16_t Z_H = 5;

/** Number of 8-bit registers used for one 3-axis vector. */
constexpr uint8_t COUNT_REGISTER_VECTOR_8BIT = 6u;

/** Number of 16-bit buffer entries used while assembling one 3-axis vector. */
constexpr uint16_t COUNT_REGISTER_VECTOR_16BIT = 6u;

/** Number of 8-bit registers to read for one 3-axis vector. */
constexpr uint32_t COUNT_REGISTER_VECTOR_32BIT = 6u;

/** Number of bits in one byte. */
constexpr uint8_t COUNT_BITS_IN_BYTE = 8u;

/** Delay in milliseconds to ensure that the IMU setup is complete. */
constexpr uint64_t IMU_SET_UP_DELAY_MS = 100u;

/** Delay in milliseconds used during calibration when no new value is available. */
constexpr uint64_t IMU_CALIBRATION_DELAY_MS = 100u;

/** Number of measurements used for calibration. */
constexpr int32_t NUMBER_OF_MEASUREMENTS = 50u;

/** Maximum number of calibration retries. */
constexpr uint32_t MAX_CALIBRATION_RETRY_COUNT = 100u;

/** Number of registers used for the default configuration of the LSM6DSO. */
constexpr uint8_t COUNT_LSM6DSO_DEFAULT_CONFIG_REG = 3u;

/** Register addresses used for the LSM6DSO default configuration. */
constexpr uint8_t LSM6DSO_DEFAULT_CONFIG_REG[COUNT_LSM6DSO_DEFAULT_CONFIG_REG] = {LSM6DSO_REG_CTRL1_XL,
                                                                                  LSM6DSO_REG_CTRL2_G,
                                                                                  LSM6DSO_REG_CTRL3_C};

/** Number of configuration values used for the LSM6DSO default configuration. */
constexpr uint8_t COUNT_LSM6DSO_DEFAULT_CONFIGS = 3u;

/** Configuration values for the LSM6DSO default configuration. */
constexpr uint8_t LSM6DSO_DEFAULT_CONFIG[COUNT_LSM6DSO_DEFAULT_CONFIGS] = {LSM6DSO_ACCEL_DEFAULT_CONFIG,
                                                                           LSM6DSO_GYRO_DEFAULT_CONFIG,
                                                                           LSM6DSO_AUTO_INC_CONFIG};

/** Number of registers used for the default configuration of the LIS3MDL. */
constexpr uint8_t COUNT_LIS3MDL_DEFAULT_CONFIG_REG = 4u;

/** Register addresses used for the LIS3MDL default configuration. */
constexpr uint8_t LIS3MDL_DEFAULT_CONFIG_REG[COUNT_LIS3MDL_DEFAULT_CONFIG_REG] = {LIS3MDL_REG_CTRL_REG1,
                                                                                  LIS3MDL_REG_CTRL_REG2,
                                                                                  LIS3MDL_REG_CTRL_REG3,
                                                                                  LIS3MDL_REG_CTRL_REG4};

/** Number of configuration values used for the LIS3MDL default configuration. */
constexpr uint8_t COUNT_LIS3MDL_DEFAULT_CONFIGS = 4u;

/** Configuration values for the LIS3MDL default configuration. */
constexpr uint8_t LIS3MDL_DEFAULT_CONFIG[COUNT_LIS3MDL_DEFAULT_CONFIGS] = {LIS3MDL_MAGNET_DEFAULT_OM_ODR,
                                                                           LIS3MDL_MAGNET_DEFAULT_FS,
                                                                           LIS3MDL_MAGNET_DEFAULT_CONV_MODE,
                                                                           LIS3MDL_MAGNET_DEFAULT_OM};

/** Number of IMU slave devices. */
constexpr uint8_t COUNT_IMU_SLAVES = 2u;

/** I2C addresses of the IMU slave devices. */
constexpr uint8_t IMU_SLAVE_ADDRESSES[COUNT_IMU_SLAVES] = {LIS3MDL_ADDR,
                                                           LSM6DSO_ADDR};

/** Register addresses of the WHO_AM_I registers. */
constexpr uint8_t IMU_WHO_AM_I_ADDRESSES[COUNT_IMU_SLAVES] = {LIS3MDL_WHO_AM_I_ADDR,
                                                              LSM6DSO_WHO_AM_I_ADDR};

/******************************************************************************
 * Public Methods
 *****************************************************************************/

Zumo2040IMU::Zumo2040IMU() : m_error(NONE),
                             m_rawAccelerometerOffsetX(INTEGER_16_ZERO),
                             m_rawGyroOffsetZ(INTEGER_16_ZERO),
                             m_calStatus(IMU_NOT_CALIBRATED)
{
}

ErrorCode Zumo2040IMU::checkIMU()
{
    if (NONE != m_error)
    {
        return m_error;
    }

    uint8_t slaveIdentification[COUNT_IMU_SLAVES];

    /* Read the WHO_AM_I ID for each IMU component. */
    for (uint8_t idx = 0; idx < COUNT_IMU_SLAVES; idx++)
    {
        slaveIdentification[idx] = readOneByte(IMU_SLAVE_ADDRESSES[idx], IMU_WHO_AM_I_ADDRESSES[idx]);
        if (NONE != m_error)
        {
            return m_error;
        }
    }

    if ((LIS3MDL_WHO_AM_I_ID != slaveIdentification[U_INTEGER_8_ZERO]) || (LSM6DSO_WHO_AM_I_ID != slaveIdentification[U_INTEGER_8_ONE]))
    {
        m_error = IMU_UNKNOWN;
        return m_error;
    }

    return NONE;
}

ErrorCode Zumo2040IMU::enableDefault()
{
    if (NONE != m_error)
    {
        return m_error;
    }

    m_calStatus = IMU_NOT_CALIBRATED;

    for (uint8_t idx = U_INTEGER_8_ZERO; idx < COUNT_LSM6DSO_DEFAULT_CONFIG_REG; idx++)
    {
        writeOneByte(LSM6DSO_ADDR, LSM6DSO_DEFAULT_CONFIG_REG[idx], LSM6DSO_DEFAULT_CONFIG[idx]);
        if (NONE != m_error)
        {
            m_error = IMU_DEFAULT_CONFIG_FAILED;
            return m_error;
        }
    }

    for (uint8_t idx = U_INTEGER_8_ZERO; idx < COUNT_LIS3MDL_DEFAULT_CONFIG_REG; idx++)
    {
        writeOneByte(LIS3MDL_ADDR, LIS3MDL_DEFAULT_CONFIG_REG[idx], LIS3MDL_DEFAULT_CONFIG[idx]);
        if (NONE != m_error)
        {
            m_error = IMU_DEFAULT_CONFIG_FAILED;
            return m_error;
        }
    }

    return NONE;
}

ErrorCode Zumo2040IMU::configureForTurnSensing()
{
    if (NONE != m_error)
    {
        return m_error;
    }

    m_calStatus = IMU_NOT_CALIBRATED;

    /* In contrast to the LSM6DS33, the LSM6DSO cannot disable individual axes. */
    writeOneByte(LSM6DSO_ADDR, LSM6DSO_REG_CTRL2_G, LSM6DSO_GYRO_CONFIG_TURN_SENS);
    if (NONE != m_error)
    {
        m_error = IMU_CONFIG_FOR_TURN_SENSING_FAILED;
        return m_error;
    }

    return NONE;
}

ErrorCode Zumo2040IMU::configureForCompassHeading()
{
    if (NONE != m_error)
    {
        return m_error;
    }

    m_calStatus = IMU_NOT_CALIBRATED;

    m_error = writeOneByte(LIS3MDL_ADDR, LIS3MDL_REG_CTRL_REG1, LIS3MDL_MAGNET_CONFIG_COMPASS_HEADING);

    if (NONE != m_error)
    {
        m_error = IMU_CONFIG_FOR_COMPASS_HEADING_FAILED;
        return m_error;
    }

    return NONE;
}

ErrorCode Zumo2040IMU::readAcc(IMUvector& data)
{
    data = {INTEGER_16_ZERO};

    if (NONE != m_error)
    {
        return m_error;
    }

    /* Read the accelerometer values into the m_accel vector. */
    m_error = readAxes16Bit(LSM6DSO_ADDR, LSM6DSO_START_REG_ACCEL, data);
    if (NONE != m_error)
    {
        m_error = IMU_READ_ACC_FAILED;

        data = {INTEGER_16_ZERO};

        return m_error;
    }

    return m_error;
}

ErrorCode Zumo2040IMU::readGyro(IMUvector& data)
{
    data = {INTEGER_16_ZERO};

    if (NONE != m_error)
    {
        return m_error;
    }

    /* Read the gyroscope values into the m_gyro vector. */
    m_error = readAxes16Bit(LSM6DSO_ADDR, LSM6DSO_START_REG_GYRO, data);
    if (NONE != m_error)
    {
        m_error = IMU_READ_GYRO_FAILED;

        data = {INTEGER_16_ZERO};

        return m_error;
    }

    return m_error;
}

ErrorCode Zumo2040IMU::readMag(IMUvector& data)
{
    data = {INTEGER_16_ZERO};

    if (NONE != m_error)
    {
        return m_error;
    }

    /* Read the magnetometer values into the m_magnet vector. */
    m_error = readAxes16Bit(LIS3MDL_ADDR, LIS3MDL_START_REG_MAGNET_AUTO_INC, data);
    if (NONE != m_error)
    {
        m_error = IMU_READ_MAGNET_FAILED;

        data = {INTEGER_16_ZERO};

        return m_error;
    }

    return m_error;
}

DataStatus Zumo2040IMU::accDataReady()
{
    if (NONE != m_error)
    {
        return NO_NEW_DATA;
    }

    uint8_t status = readOneByte(LSM6DSO_ADDR, LSM6DSO_STATUS_REG);

    /* Mask the status register bit that indicates whether new accelerometer values are available. */
    status = status & LSM6DSO_STATUS_ACC_MASK;

    if (LSM6DSO_STATUS_ACC_MASK != status)
    {
        return NO_NEW_DATA;
    }

    return NEW_DATA;
}

DataStatus Zumo2040IMU::gyroDataReady()
{
    if (NONE != m_error)
    {
        return NO_NEW_DATA;
    }

    uint8_t status = readOneByte(LSM6DSO_ADDR, LSM6DSO_STATUS_REG);

    /* Mask the status register bit that indicates whether new gyroscope values are available. */
    status = status & LSM6DSO_STATUS_GYRO_MASK;

    if (LSM6DSO_STATUS_GYRO_MASK != status)
    {
        return NO_NEW_DATA;
    }

    return NEW_DATA;
}

DataStatus Zumo2040IMU::magDataReady()
{
    if (NONE != m_error)
    {
        return NO_NEW_DATA;
    }

    uint8_t status = readOneByte(LIS3MDL_ADDR, LIS3MDL_STATUS_REG);

    /* Mask the status register bit that indicates whether new magnetometer values are available. */
    status = status & LIS3MDL_STATUS_MAGNET_MASK;

    if (LIS3MDL_STATUS_MAGNET_MASK != status)
    {
        return NO_NEW_DATA;
    }

    return NEW_DATA;
}

ErrorCode Zumo2040IMU::calibrateLineFollower()
{
    if (NONE != m_error)
    {
        m_calStatus = IMU_NOT_CALIBRATED;
        return m_error;
    }

    /* Wait to ensure that the IMU setup is complete. */
    sleep_ms(IMU_SET_UP_DELAY_MS);

    int32_t sumOfRawAccelValuesX = INTEGER_32_ZERO;
    int32_t sumOfRawGyroValuesZ = INTEGER_32_ZERO;
    int32_t measurementIndex = INTEGER_32_ZERO;
    uint32_t retryCount = U_INTEGER_32_ZERO;
    IMUvector accel;
    IMUvector gyro;

    while (measurementIndex < NUMBER_OF_MEASUREMENTS)
    {
        if (NONE != m_error)
        {
            m_calStatus = IMU_NOT_CALIBRATED;
            return m_error;
        }
        else if (retryCount >= MAX_CALIBRATION_RETRY_COUNT)
        {
            m_calStatus = IMU_NOT_CALIBRATED;
            m_error = IMU_CALIBRATION_FAILED;
            return m_error;
        }
        else if((NEW_DATA == accDataReady()) && (NEW_DATA == gyroDataReady()))
        {
            readAcc(accel);
            if (NONE != m_error)
            {
                m_calStatus = IMU_NOT_CALIBRATED;
                m_error = IMU_CALIBRATION_FAILED;
                return m_error;
            }

            readGyro(gyro);
            if (NONE != m_error)
            {
                m_calStatus = IMU_NOT_CALIBRATED;
                m_error = IMU_CALIBRATION_FAILED;
                return m_error;
            }

            sumOfRawAccelValuesX += accel.x;
            sumOfRawGyroValuesZ += gyro.z;
            measurementIndex++;
        }
        else
        {
            /* If no new values are ready, wait before checking again. */
            sleep_ms(IMU_CALIBRATION_DELAY_MS);
            retryCount++;
        }
    }

    /* Calculate the mean values of the sensor readings. */
    m_rawAccelerometerOffsetX = static_cast<int16_t>((sumOfRawAccelValuesX / NUMBER_OF_MEASUREMENTS));
    m_rawGyroOffsetZ = static_cast<int16_t>((sumOfRawGyroValuesZ / NUMBER_OF_MEASUREMENTS));

    m_calStatus = IMU_CALIBRATED;

    return NONE;
}

ErrorCode Zumo2040IMU::getLastError() const
{
    return m_error;
}

int16_t Zumo2040IMU::getAccelOffsetX()
{
    if (NONE != m_error)
    {
        return INTEGER_16_ZERO;
    }
    else if (IMU_CALIBRATED != m_calStatus)
    {
        m_error = IMU_CANT_GET_OFFSET_NOT_CALIBRATED;
        return INTEGER_16_ZERO;
    }

    return m_rawAccelerometerOffsetX;
}

int16_t Zumo2040IMU::getGyroOffsetZ()
{
    if (NONE != m_error)
    {
        return INTEGER_16_ZERO;
    }
    else if (IMU_CALIBRATED != m_calStatus)
    {
        m_error = IMU_CANT_GET_OFFSET_NOT_CALIBRATED;
        return INTEGER_16_ZERO;
    }

    return m_rawGyroOffsetZ;
}

void Zumo2040IMU::resetError()
{
    m_error = NONE;
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

uint8_t Zumo2040IMU::readOneByte(uint8_t addr, uint8_t reg)
{
    if (NONE != m_error)
    {
        return U_INTEGER_8_ZERO;
    }

    uint8_t buffer[SIZE_ONE_BYTE] = {U_INTEGER_8_ZERO};

    /* Read one register value into the buffer. */
    m_error = m_i2c.read(addr, reg, buffer, LENGTH_ONE_BYTE);

    if (NONE != m_error)
    {
        return U_INTEGER_8_ZERO;
    }

    return buffer[U_INTEGER_8_ZERO];
}

ErrorCode Zumo2040IMU::writeOneByte(uint8_t addr, uint8_t reg, uint8_t value)
{
    if (NONE != m_error)
    {
        return m_error;
    }

    const uint8_t buffer[SIZE_ONE_BYTE] = {value};

    /* Write the buffer content to the register. */
    m_error = m_i2c.write(addr, reg, buffer, LENGTH_ONE_BYTE);

    return m_error;
}

ErrorCode Zumo2040IMU::readAxes16Bit(uint8_t addr, uint8_t startReg, IMUvector& data)
{
    if (NONE != m_error)
    {
        return m_error;
    }

    uint8_t buffer8bit[COUNT_REGISTER_VECTOR_8BIT] = {U_INTEGER_8_ZERO};

    uint16_t buffer16bit[COUNT_REGISTER_VECTOR_16BIT];

    /* Read register values into the 8-bit buffer. */
    m_error = m_i2c.read(addr, startReg, buffer8bit, COUNT_REGISTER_VECTOR_32BIT);
    if (NONE != m_error)
    {
        m_error = IMU_READ_AXES_FAILED;
        return m_error;
    }

    /* Cast the 8-bit register values to 16-bit values before combining the bytes. */
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < COUNT_REGISTER_VECTOR_32BIT; idx++)
    {
        buffer16bit[idx] = static_cast<uint16_t>(buffer8bit[idx]);
    }

    /* Combine the low and high bytes of the corresponding axis values. */
    data.x = static_cast<int16_t>((buffer16bit[X_H] << COUNT_BITS_IN_BYTE) | buffer16bit[X_L]);
    data.y = static_cast<int16_t>((buffer16bit[Y_H] << COUNT_BITS_IN_BYTE) | buffer16bit[Y_L]);
    data.z = static_cast<int16_t>((buffer16bit[Z_H] << COUNT_BITS_IN_BYTE) | buffer16bit[Z_L]);

    return NONE;
}

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/
