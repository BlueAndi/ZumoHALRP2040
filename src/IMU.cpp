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
 * @brief  IMU (Inertial Measurement Unit) implementation
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "IMU.h"
/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and classes
 *****************************************************************************/

bool IMU::init()
{
    if (m_imuDrv.checkIMU() == NONE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void IMU::enableDefault()
{
    m_imuDrv.enableDefault();
}

void IMU::configureForTurnSensing()
{
    /* Individual axes cannot be disabled on the LSM6DSO. */
    m_imuDrv.configureForTurnSensing();
}

void IMU::readAccelerometer()
{
    IMUvector accel;
    m_imuDrv.readAcc(accel);
    m_accelerometerValues.valueX = accel.x - m_rawAccelerometerOffsetX;
    m_accelerometerValues.valueY = 0;
    m_accelerometerValues.valueZ = 0;
}

void IMU::readGyro()
{
    IMUvector gyro;
    m_imuDrv.readGyro(gyro);
    m_gyroValues.valueX = 0;
    m_gyroValues.valueY = 0;
    m_gyroValues.valueZ = gyro.z - m_rawGyroOffsetZ;
}

void IMU::readMagnetometer()
{
    IMUvector mag;
    m_imuDrv.readMag(mag);
    m_magnetometerValues.valueX = mag.x;
    m_magnetometerValues.valueY = mag.y;
    m_magnetometerValues.valueZ = mag.z;
}

bool IMU::accelerometerDataReady()
{
    if (m_imuDrv.accDataReady() == DataStatus::NO_NEW_DATA)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool IMU::gyroDataReady()
{
    if (m_imuDrv.gyroDataReady() == DataStatus::NO_NEW_DATA)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool IMU::magnetometerDataReady()
{
    if (m_imuDrv.magDataReady() == DataStatus::NO_NEW_DATA)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void IMU::getAccelerationValues(IMUData* accelerationValues) const
{
    if (nullptr != accelerationValues)
    {
        accelerationValues->valueX = m_accelerometerValues.valueX;
        accelerationValues->valueY = m_accelerometerValues.valueY;
        accelerationValues->valueZ = m_accelerometerValues.valueZ;
    }
}

void IMU::getTurnRates(IMUData* turnRates) const
{
    if (nullptr != turnRates)
    {
        turnRates->valueX = m_gyroValues.valueX;
        turnRates->valueY = m_gyroValues.valueY;
        turnRates->valueZ = m_gyroValues.valueZ;
    }
}

void IMU::getMagnetometerValues(IMUData* magnetometerValues) const
{
    if (nullptr != magnetometerValues)
    {
        magnetometerValues->valueX = m_magnetometerValues.valueX;
        magnetometerValues->valueY = m_magnetometerValues.valueY;
        magnetometerValues->valueZ = m_magnetometerValues.valueZ;
    }
}

void IMU::calibrate()
{
    m_imuDrv.calibrateLineFollower();
}

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/******************************************************************************
 * Public Methods
 *****************************************************************************/

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
