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
 * @brief Line sensors driver
 * @author Felix Reitenauer
 *
 * @addtogroup Line sensor
 *
 * @{
 */

#ifndef ZUMO2040LINESENSORS_H
#define ZUMO2040LINESENSORS_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "RP2040Linesensors.h"
#include "Zumo2040ErrorCodes.h"
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** Diagnostic information for the line sensor driver. */
struct LinesensorsInfo
{
    /** Indicates whether the sensors are calibrated. */
    bool m_isCalibrated;

    /** Indicates whether the calibration bounds are up to date. */
    bool m_curBounds;

    /** Indicates whether the line sensors detect the line. */
    bool m_position;

    /** Last error code of the line sensors. */
    ErrorCode m_linesensorError;

    /** Error code for each sensor.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    ErrorCode m_sensorError[SENSOR_COUNT];

    /** Sensor values compensated with the enabled and disabled emitter readings.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    uint32_t m_sensorValuesOn[SENSOR_COUNT];

    /** Buffer for sensor values with disabled emitter.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    uint32_t m_sensorValuesOff[SENSOR_COUNT];

    /** Minimum values of the sensors.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    uint32_t m_minValues[SENSOR_COUNT];

    /** Maximum values of the sensors.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    uint32_t m_maxValues[SENSOR_COUNT];

    /** Calibrated minimum values with disabled emitter.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    uint32_t m_calibratedMinValuesOff[SENSOR_COUNT];

    /** Calibrated maximum values with disabled emitter.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    uint32_t m_calibratedMaxValuesOff[SENSOR_COUNT];

    /** Calibrated minimum values with enabled emitter.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    uint32_t m_calibratedMinValuesOn[SENSOR_COUNT];

    /** Calibrated maximum values with enabled emitter.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    uint32_t m_calibratedMaxValuesOn[SENSOR_COUNT];

    /** Lower bounds for the sensor values.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    uint32_t m_calMin[SENSOR_COUNT];

    /** Upper bounds for the sensor values.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    uint32_t m_calMax[SENSOR_COUNT];

    /** The calibrated sensor values.
     * Sensor index order when looking at the robot from the front:
     *  [0] = leftmost sensor
     *  [1] = left-center sensor
     *  [2] = center sensor
     *  [3] = right-center sensor
     *  [4] = rightmost sensor
     */
    uint32_t m_calibratedSensorValues[SENSOR_COUNT];

    /** Last seen position of the sensors with respect to the track. */
    uint32_t m_lastSeen;
};

/** Driver for the line sensors. */
class Zumo2040Linesensors
{
    public:
        /** Initializes the line sensor buffers. */
        Zumo2040Linesensors();

        /** Default destructor. */
        ~Zumo2040Linesensors() = default;

        /** Delete copy constructor. */
        Zumo2040Linesensors(const Zumo2040Linesensors&) = delete;

        /** Delete copy assignment operator. */
        Zumo2040Linesensors& operator=(const Zumo2040Linesensors&) = delete;

        /**
         * @brief Initializes the low-level line sensor core.
         */
        void init();

        /**
         * @brief Reads the sensors for calibration. Call this method several times
         * while moving the sensors over the line to determine the minimum and maximum
         * values.
         *
         * The calibration values are stored internally.
         */
        void calibrate();

        /**
         * @brief Determines the deviation and returns an estimated position of the robot
         * with respect to a line. The estimate is made using a weighted average of
         * the sensor indices multiplied by 1000, so that a return value of 0
         * indicates that the line is directly below sensor 0, a return value of
         * 1000 indicates that the line is directly below sensor 1, 2000
         * indicates that it's below sensor 2, etc.  Intermediate values
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
        uint32_t readLine();

        /**
         * @brief Reset the internal states of the line sensor.
         */
        void reset();

        /**
        * @brief Checks whether the calibration minimum and maximum values are valid.
         *
         * @return true if the calibration is valid, otherwise false.
         */
        bool isCalibrationSuccessful();

        /**
         * @brief Returns internal diagnostic information of the line sensor object.
         *
         * @return Struct containing the internal diagnostic state.
         */
        LinesensorsInfo getInfo() const;

    private:
        /**
         * @brief Reads the raw sensor values once with the emitter enabled
         * and once with the emitter disabled.
         */
        void read();

        /**
         * @brief Reads the sensors and normalizes the values using the calibration bounds.
         */
        void readCalibrated();

        /**
         * @brief Calculates the bounds for each sensor according to the calibration values.
         */
        void calcBounds();

        /** Indicates whether the sensors are calibrated. */
        bool m_isCalibrated;

        /** Indicates whether the calibration bounds are up to date. */
        bool m_curBounds;

        /** Indicates whether the line sensors detect the line. */
        bool m_position;

        /** Last error code of the line sensor. */
        ErrorCode m_linesensorError;

        /** Error code for each sensor.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        ErrorCode m_sensorError[SENSOR_COUNT];

        /** Sensor values compensated with the enabled and disabled emitter readings.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        uint32_t m_sensorValuesOn[SENSOR_COUNT];

        /** Buffer for sensor values measured with the emitter disabled.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        uint32_t m_sensorValuesOff[SENSOR_COUNT];

        /** Minimum values of the sensors.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        uint32_t m_minValues[SENSOR_COUNT];

        /** Maximum values of the sensors.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        uint32_t m_maxValues[SENSOR_COUNT];

        /** Calibrated minimum values with disabled emitter.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        uint32_t m_calibratedMinValuesOff[SENSOR_COUNT];

        /** Calibrated maximum values with disabled emitter.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        uint32_t m_calibratedMaxValuesOff[SENSOR_COUNT];

        /** Calibrated minimum values with enabled emitter.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        uint32_t m_calibratedMinValuesOn[SENSOR_COUNT];

        /** Calibrated maximum values with enabled emitter.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        uint32_t m_calibratedMaxValuesOn[SENSOR_COUNT];

        /** Lower bounds for the sensor values.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        uint32_t m_calMin[SENSOR_COUNT];

        /** Upper bounds for the sensor values.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        uint32_t m_calMax[SENSOR_COUNT];

        /** The calibrated sensor values.
         * Sensor index order when looking at the robot from the front:
         *  [0] = leftmost sensor
         *  [1] = left-center sensor
         *  [2] = center sensor
         *  [3] = right-center sensor
         *  [4] = rightmost sensor
         */
        uint32_t m_calibratedSensorValues[SENSOR_COUNT];

        /** Last seen position of the sensors with respect to the track. */
        uint32_t m_lastSeen;

        /** Low-level driver line sensor object. */
        LinesensorsCore m_core;
};
/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040LINESENSORS_H */
/** @} */
