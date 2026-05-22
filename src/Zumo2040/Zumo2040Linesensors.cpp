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
 * @brief  Line sensors driver.
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "Zumo2040Linesensors.h"

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

/** Maximum sensor value. */
constexpr uint32_t MAX_VALUE = 32767u;

/** Minimum sensor value. */
constexpr uint32_t MIN_VALUE = 0u;

/** Unsigned 32-bit integer one used in several places. */
constexpr uint32_t U_INTEGER_32_ONE = 1u;

/** Signed 32-bit integer one used in several places. */
constexpr int32_t INTEGER_32_ONE = 1;

/** Unsigned 32-bit integer zero used in several places. */
constexpr uint32_t U_INTEGER_32_ZERO = 0u;

/** Signed 64-bit integer zero used in several places. */
constexpr int32_t INTEGER_32_ZERO = 0;

/** Scaling factor for the position calculation of the line sensors. */
constexpr int32_t SCALE_FACTOR = 1000;

/** Round limit for one calibration cycle. */
constexpr uint32_t ROUND_LIMIT = 10u;

/** Noise threshold which sensor values have to exceed to be used for position calculation. */
constexpr uint32_t NOISE_THRESHOLD = 50u;

/** Threshold which one sensor value has to exceed to accept that the line is detected. */
constexpr uint32_t ACCEPT_LINE_THRESHOLD = 200u;

/** Center divisor for edge case calculation in readLine(). */
constexpr int32_t CENTER_DIVISOR = 2;

/** Divisor used for checking the calibration range. */
constexpr uint32_t CALIBRATION_CONTROL_DIVISOR = 6u;

/** Indicates that the line sensors are not calibrated. */
constexpr bool NOT_CALIBRATED = false;

/** Indicates that the line sensors are calibrated. */
constexpr bool CALIBRATED = true;

/** Indicates that the calibration bounds are outdated. */
constexpr bool OUTDATED_BOUNDS = false;

/** Indicates that the calibration bounds are up to date. */
constexpr bool CURRENT_BOUNDS = true;

/** Indicates that the line sensors are off the line. */
constexpr bool OFF_LINE = false;

/** Indicates that the line sensors are on the line. */
constexpr bool ON_LINE = true;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

Zumo2040Linesensors::Zumo2040Linesensors() : m_isCalibrated(NOT_CALIBRATED),
                                             m_curBounds(OUTDATED_BOUNDS),
                                             m_position(OFF_LINE),
                                             m_lastSeen(U_INTEGER_32_ZERO),
                                             m_linesensorError(NONE)
{
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        m_sensorError[idx] = NONE;
        m_minValues[idx] = MAX_VALUE;
        m_calibratedMinValuesOff[idx] = MAX_VALUE;
        m_calibratedMinValuesOn[idx] = MAX_VALUE;
        m_maxValues[idx] = MIN_VALUE;
        m_calibratedMaxValuesOff[idx] = MIN_VALUE;
        m_calibratedMaxValuesOn[idx] = MIN_VALUE;
        m_sensorValuesOn[idx] = MIN_VALUE;
        m_calMin[idx] = MAX_VALUE;
        m_calMax[idx] = MIN_VALUE;
        m_calibratedSensorValues[idx] = MIN_VALUE;
        m_sensorValuesOff[idx] = MIN_VALUE;
    }
}


void Zumo2040Linesensors::init()
{
    if (m_core.init() == NONE)
    {
        return;
    }

    m_linesensorError = LINESENSOR_CORE_INITIALIZATION_FAILED;
}


void Zumo2040Linesensors::calibrate()
{
    m_curBounds = OUTDATED_BOUNDS;

    /* Read the line sensor values with the emitter disabled and determine the maximum and minimum values. */
    for (uint32_t rounds = U_INTEGER_32_ZERO; rounds < ROUND_LIMIT; rounds++)
    {
        m_core.setEmitter(EMITTER_OFF);
        m_core.read(m_sensorValuesOff);
        for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
        {
            if (m_maxValues[idx] < m_sensorValuesOff[idx])
            {
                m_maxValues[idx] = m_sensorValuesOff[idx];
            }

            if (m_minValues[idx] > m_sensorValuesOff[idx])
            {
                m_minValues[idx] = m_sensorValuesOff[idx];
            }
        }
    }

    /* Determine the calibrated maximum and minimum values for the disabled emitter. */
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        if (m_calibratedMinValuesOff[idx] > m_maxValues[idx])
        {
            m_calibratedMinValuesOff[idx] = m_maxValues[idx];
        }

        if (m_calibratedMaxValuesOff[idx] < m_minValues[idx])
        {
            m_calibratedMaxValuesOff[idx] = m_minValues[idx];
        }
    }

    /* Reset the minimum and maximum buffer. */
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        m_maxValues[idx] = MIN_VALUE;
        m_minValues[idx] = MAX_VALUE;
    }

    /* Read the line sensor values with the emitter enabled and determine the maximum and minimum values. */
    for (uint32_t rounds = U_INTEGER_32_ZERO; rounds < ROUND_LIMIT; rounds++)
    {
        m_core.setEmitter(EMITTER_ON);
        m_core.read(m_sensorValuesOn);
        for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
        {
            if (m_maxValues[idx] < m_sensorValuesOn[idx])
            {
                m_maxValues[idx] = m_sensorValuesOn[idx];
            }

            if (m_minValues[idx] > m_sensorValuesOn[idx])
            {
                m_minValues[idx] = m_sensorValuesOn[idx];
            }
        }
    }

    /* Determine the calibrated maximum and minimum values for the enabled emitter. */
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        if  (m_calibratedMinValuesOn[idx] > m_maxValues[idx])
        {
            m_calibratedMinValuesOn[idx] = m_maxValues[idx];
        }

        if (m_calibratedMaxValuesOn[idx] < m_minValues[idx])
        {
            m_calibratedMaxValuesOn[idx] = m_minValues[idx];
        }
    }

    /* Reset the minimum and maximum buffer. */
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        m_maxValues[idx] = MIN_VALUE;
        m_minValues[idx] = MAX_VALUE;
    }

    m_isCalibrated = CALIBRATED;
}


uint32_t Zumo2040Linesensors::readLine()
{
    uint32_t avg = U_INTEGER_32_ZERO;
    uint32_t sum = U_INTEGER_32_ZERO;
    bool oneSensorOnLine = OFF_LINE;

    /* Update calibrated sensor values. */
    readCalibrated();

    if (NONE != m_linesensorError)
    {
        return m_lastSeen;
    }

    /* Calculate the weighted sum and the denominator for the position calculation. */
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        uint32_t sensorValue = m_calibratedSensorValues[idx];

        if (sensorValue > ACCEPT_LINE_THRESHOLD)
        {
            oneSensorOnLine = ON_LINE;
        }

        if (NOISE_THRESHOLD < sensorValue)
        {
            avg += sensorValue * (idx * SCALE_FACTOR);
            sum += sensorValue;
        }
    }

    if (ON_LINE == oneSensorOnLine)
    {
        m_position = ON_LINE;
    }
    else
    {
        m_position = OFF_LINE;
    }

    if (OFF_LINE == m_position)
    {
        if (m_lastSeen < ((SENSOR_COUNT - U_INTEGER_32_ONE) * static_cast<uint32_t>(SCALE_FACTOR / CENTER_DIVISOR)))
        {
            return U_INTEGER_32_ZERO;
        }
        else
        {
            return (SENSOR_COUNT - U_INTEGER_32_ONE) * static_cast<uint32_t>(SCALE_FACTOR);
        }
    }

    if (U_INTEGER_32_ZERO == sum)
    {
        m_linesensorError = LINESENSOR_READ_LINE_ZERO_DIVISOR;
        return U_INTEGER_32_ZERO;
    }

    /* Position of the line sensors with respect to the track. */
    m_lastSeen = avg / sum;

    return m_lastSeen;
}


void Zumo2040Linesensors::reset()
{
    m_isCalibrated = NOT_CALIBRATED;
    m_curBounds = OUTDATED_BOUNDS;
    m_position = OFF_LINE;
    m_lastSeen = U_INTEGER_32_ZERO;
    m_linesensorError = NONE;

    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        m_sensorError[idx] = NONE;
        m_minValues[idx] = MAX_VALUE;
        m_calibratedMinValuesOff[idx] = MAX_VALUE;
        m_calibratedMinValuesOn[idx] = MAX_VALUE;
        m_maxValues[idx] = MIN_VALUE;
        m_calibratedMaxValuesOff[idx] = MIN_VALUE;
        m_calibratedMaxValuesOn[idx] = MIN_VALUE;
        m_sensorValuesOn[idx] = MIN_VALUE;
        m_calMin[idx] = MAX_VALUE;
        m_calMax[idx] = MIN_VALUE;
        m_calibratedSensorValues[idx] = MIN_VALUE;
        m_sensorValuesOff[idx] = MIN_VALUE;
    }
}

bool Zumo2040Linesensors::isCalibrationSuccessful()
{
    if (NOT_CALIBRATED == m_isCalibrated)
    {
        m_linesensorError = LINESENSOR_IS_NOT_CALIBRATED;
        return false;
    }

    uint32_t distance = U_INTEGER_32_ZERO;

    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        if (m_calibratedMaxValuesOn[idx] < m_calibratedMinValuesOn[idx])
        {
            m_sensorError[idx] = LINESENSOR_CALIBRATION_FAILED;
            m_linesensorError = LINESENSOR_CALIBRATION_FAILED;
            return false;
        }
        else if (m_calibratedMinValuesOff[idx] < m_calibratedMaxValuesOn[idx])
        {
            m_sensorError[idx] = LINESENSOR_CALIBRATION_FAILED;
            m_linesensorError = LINESENSOR_CALIBRATION_FAILED;
            return false;
        }
        else if (m_calibratedMaxValuesOff[idx] < m_calibratedMaxValuesOn[idx])
        {
            m_sensorError[idx] = LINESENSOR_CALIBRATION_FAILED;
            m_linesensorError = LINESENSOR_CALIBRATION_FAILED;
            return false;
        }

        distance = m_calibratedMaxValuesOn[idx] - m_calibratedMinValuesOn[idx];

        /* The assumption here is, that the distance (max. value - min. value) must be
         * higher than a sixth of the max. measure duration.
         */
        if ((MAX_VALUE / CALIBRATION_CONTROL_DIVISOR) > distance)
        {
            m_sensorError[idx] = LINESENSOR_CALIBRATION_FAILED;
            m_linesensorError = LINESENSOR_CALIBRATION_FAILED;
            return false;
        }
    }

    return true;
}

LinesensorsInfo Zumo2040Linesensors::getInfo() const
{
    LinesensorsInfo info;

    info.m_isCalibrated = m_isCalibrated;
    info.m_curBounds = m_curBounds;
    info.m_position = m_position;
    info.m_linesensorError = m_linesensorError;
    info.m_lastSeen = m_lastSeen;

    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        info.m_sensorError[idx] = m_sensorError[idx];
        info.m_sensorValuesOn[idx] = m_sensorValuesOn[idx];
        info.m_sensorValuesOff[idx] = m_sensorValuesOff[idx];
        info.m_minValues[idx] = m_minValues[idx];
        info.m_maxValues[idx] = m_maxValues[idx];
        info.m_calibratedMinValuesOff[idx] = m_calibratedMinValuesOff[idx];
        info.m_calibratedMaxValuesOff[idx] = m_calibratedMaxValuesOff[idx];
        info.m_calibratedMinValuesOn[idx] = m_calibratedMinValuesOn[idx];
        info.m_calibratedMaxValuesOn[idx] = m_calibratedMaxValuesOn[idx];
        info.m_calMin[idx] = m_calMin[idx];
        info.m_calMax[idx] = m_calMax[idx];
        info.m_calibratedSensorValues[idx] = m_calibratedSensorValues[idx];
    }

    return info;
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

void Zumo2040Linesensors::read()
{
    m_core.setEmitter(EMITTER_OFF);
    m_core.read(m_sensorValuesOff);

    m_core.setEmitter(EMITTER_ON);
    m_core.read(m_sensorValuesOn);

    /* Compensate the raw sensor values using the enabled and disabled emitter readings. */
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        m_sensorValuesOn[idx] += MAX_VALUE - m_sensorValuesOff[idx];
    }
}


void Zumo2040Linesensors::readCalibrated()
{
    if (OUTDATED_BOUNDS == m_curBounds)
    {
        /* Update the upper and lower bounds. */
        calcBounds();
        if (NONE != m_linesensorError)
        {
            return;
        }
    }

    /* Update the sensor values. */
    read();

    int32_t denominator = INTEGER_32_ONE;

    /* Calculate the calibrated sensor values. */
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        denominator = m_calMax[idx] - m_calMin[idx];

        int32_t normValue = INTEGER_32_ZERO;

        if (denominator != U_INTEGER_32_ZERO)
        {
            normValue = ((static_cast<int32_t>(m_sensorValuesOn[idx]) - static_cast<int32_t>(m_calMin[idx])) * SCALE_FACTOR) / denominator;
        }

        if (INTEGER_32_ZERO > normValue)
        {
            normValue = INTEGER_32_ZERO;
        }
        else if (normValue > SCALE_FACTOR)
        {
            normValue = SCALE_FACTOR;
        }

        m_calibratedSensorValues[idx] = static_cast<uint32_t>(normValue);
    }
}


void Zumo2040Linesensors::calcBounds()
{
    if (NOT_CALIBRATED == m_isCalibrated)
    {
        m_linesensorError = LINESENSOR_IS_NOT_CALIBRATED;
        return;
    }

    /* Reset the upper and lower bound buffers. */
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        m_calMin[idx] = MAX_VALUE;
        m_calMax[idx] = MIN_VALUE;
    }

    /* Calculate upper and lower bounds for each sensor. */
    for (uint32_t idx = U_INTEGER_32_ZERO; idx < SENSOR_COUNT; idx++)
    {
        if (m_calibratedMinValuesOff[idx] > m_calibratedMinValuesOn[idx])
        {
            /* Compensate the bound using the disabled emitter reading. */
            m_calMin[idx] = m_calibratedMinValuesOn[idx] + MAX_VALUE - m_calibratedMinValuesOff[idx];
        }
        else
        {
            m_sensorError[idx] = LINESENSOR_BOUND_CALCULATION_FAILED;
            m_linesensorError = LINESENSOR_BOUND_CALCULATION_FAILED;
            return;
        }

        if (m_calibratedMaxValuesOff[idx] > m_calibratedMaxValuesOn[idx])
        {
            /* Compensate the bound using the disabled emitter reading. */
            m_calMax[idx] = m_calibratedMaxValuesOn[idx] + MAX_VALUE - m_calibratedMaxValuesOff[idx];
        }
        else
        {
            m_sensorError[idx] = LINESENSOR_BOUND_CALCULATION_FAILED;
            m_linesensorError = LINESENSOR_BOUND_CALCULATION_FAILED;
            return;
        }

        if (m_calMax[idx] <= m_calMin[idx])
        {
            m_sensorError[idx] = LINESENSOR_BOUND_CALCULATION_FAILED;
            m_linesensorError =  LINESENSOR_BOUND_CALCULATION_FAILED;
            return;
        }
    }

    m_curBounds = CURRENT_BOUNDS;
}

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/
