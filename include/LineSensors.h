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
 * @brief  Line sensors array realization
 * @author Felix Reitenauer
 *
 * @addtogroup HALTarget
 *
 * @{
 */

#ifndef LINESENSORS_H
#define LINESENSORS_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "ILineSensors.h"
#include "Zumo2040Linesensors.h"
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** This class provides access to the Zumo target line sensors. */
class LineSensors : public ILineSensors
{
public:
    /**
     * Constructs the line sensors adapter.
     */
    LineSensors() : ILineSensors(), m_sensorValuesU16(), m_calibErrorInfo(CALIB_ERROR_NOT_CALIBRATED)
    {
    }

    /**
     * Destroys the line sensors adapter.
     */
    ~LineSensors()
    {
    }

    /**
     * Initializes the line sensors.
     */
    void init() final
    {
        m_lineSensors.init();
    }

    /**
     * Reads the sensors for calibration. Call this method several times during
     * turning the sensors over the line to determine the minimum and maximum
     * values.
     *
     * The calibration factors are stored internally.
     */
    void calibrate() final
    {
        m_lineSensors.calibrate();
    }

    /**
     * Determines the deviation and returns an estimated position of the robot
     * with respect to a line. The estimate is made using a weighted average of
     * the sensor indices multiplied by 1000, so that a return value of 0
     * indicates that the line is directly below sensor 0, a return value of
     * 1000 indicates that the line is directly below sensor 1, 2000
     * indicates that it's below sensor 2000, etc.  Intermediate values
     * indicate that the line is between two sensors. The formula is:
     *
     *   0*value0 + 1000*value1 + 2000*value2 + ...
     *  --------------------------------------------
     *      value0  +  value1  +  value2 + ...
     *
     * This function assumes a dark line (high values) surrounded by white
     * (low values).
     *
     * @return Estimated position with respect to track.
     */
    int16_t readLine() final
    {
        return static_cast<int16_t>(m_lineSensors.readLine());
    }

    /**
     * Get last line sensor values.
     *
     * @return Line sensor values
     */
    const uint16_t* getSensorValues() final
    {
        uint8_t index = 0;

        /* This is only done to be able to provide a platform independed data type. */
        for (index = 0; index < MAX_SENSORS; ++index)
        {
            m_sensorValuesU16[index] = static_cast<uint16_t>(m_lineSensors.getInfo().m_calibratedSensorValues[index]);
        }

        return m_sensorValuesU16;
    }

    /**
     * Checks whether the calibration was successful or not.
     * It assumes that the environment brightness compensation is active.
     *
     * @return If successful, it will return true otherwise false.
     */
    bool isCalibrationSuccessful() final
    {
        return m_lineSensors.isCalibrationSuccessful();
    }

    /**
     * It will return the index of the sensor, which caused to fail the calibration.
     * If calibration was successful, it will return 0xFF.
     * If calibration was not not done yet, it will return 0xFE.
     *
     * @return Sensor index, starting with 0. Note the other cases in description.
     */
    uint8_t getCalibErrorInfo() const final
    {
        Zumo2040::LinesensorsInfo info;
        info = m_lineSensors.getInfo();

        if(info.m_isCalibrated == false)
        {
            return CALIB_ERROR_NOT_CALIBRATED;
        }

        for (uint8_t index = 0; index < MAX_SENSORS; ++index)
        {
            if(info.m_sensorError[index] == Zumo2040::LINESENSOR_CALIBRATION_FAILED)
            {
                return index;
            }
        }

        return CALIB_ERROR_OK;
    }

    /**
     * Get number of used line sensors.
     *
     * @return Number of used line sensors
     */
    uint8_t getNumLineSensors() const final
    {
        return MAX_SENSORS;
    }

    /**
     * Get max. value of a single line sensor in digits.
     * The sensor value is indirect proportional to the reflectance.
     *
     * @return Max. line sensor value
     */
    int16_t getSensorValueMax() const final
    {
        return SENSOR_MAX_VALUE;
    }

    /**
     * Resets the maximum and minimum values measured by each sensor.
     */
    void resetCalibration() final
    {
        m_lineSensors.reset();
    }

private:
    /**
     * Number of used line sensors. This depends on the Zumo hardware configuration.
     */
    static const uint8_t    MAX_SENSORS         = 5;

    /**
     * Max. value of a single line sensor in digits (calibration already considered).
     * It depends on the Zumo32U4LineSensors implementation.
     * See Zumo32U4\QTRSensors.cpp @ readCalibrated()
     */
    static const int16_t    SENSOR_MAX_VALUE    = 1000;

    Zumo2040Linesensors m_lineSensors;                  /**< Zumo line sensors driver. */
    uint16_t            m_sensorValuesU16[MAX_SENSORS]; /**< The last value of each sensor as unsigned 16-bit values. */
    uint8_t             m_calibErrorInfo;               /**< Calibration error information. */
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* LINESENSORS_H */
/** @} */
