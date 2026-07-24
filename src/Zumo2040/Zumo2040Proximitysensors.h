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
 * @brief Proximity sensors driver
 * @author Felix Reitenauer
 *
 * @addtogroup Zumo2040ProximitySensors
 *
 * @{
 */

#ifndef ZUMO2040PROXIMITYSENSORS_H
#define ZUMO2040PROXIMITYSENSORS_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <cstdint>
#include "Zumo2040Pin.h"
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

namespace Zumo2040
{
    enum class EmitterSide
    {
        /** Indicates the left side of the IR emitters. */
        LEFT,
        /** Indicates the right side of the IR emitters. */
        RIGHT
    };

    struct ProximitySensorStatus
    {
        /** Sensor activation states in the following order: left, front, right. */
        bool status[Zumo2040::Pins::PROXIMITY_SENSOR_COUNT];
    };


    enum ProximitySensor
    {
        /** Indicates the left proximity sensor. */
        PROX_LEFT,
        /** Indicates the front proximity sensor. */
        PROX_FRONT,
        /** Indicates the right proximity sensor. */
        PROX_RIGHT
    };
}

/** Driver for the proximity sensors.
 *
 * @note The read() method disables the line sensor emitter before measuring,
 *       because the line sensor emitter can interfere with proximity readings.
 */
class Zumo2040ProximitySensors
{
    public:
        /** Initializes the member variables and the PWM. */
        Zumo2040ProximitySensors();
        /** Disables the PWM. */
        ~Zumo2040ProximitySensors();
        /** Delete copy constructor. */
        Zumo2040ProximitySensors(const Zumo2040ProximitySensors&) = delete;
        /** Delete copy assignment operator. */
        Zumo2040ProximitySensors& operator=(const Zumo2040ProximitySensors&) = delete;

        /**
         * @brief Initializes the left proximity sensor.
         */
        void initLeftSensor();

        /**
         * @brief Initializes the front proximity sensor.
         */
        void initFrontSensor();

        /**
         * @brief Initializes the right proximity sensor.
         */
        void initRightSensor();

        /**
         * @brief Initializes all proximity sensors.
         */
        void initAllSensors();

        /**
         * @brief Gets the number of initialized sensors.
         *
         * @return The number of initialized sensors.
         */
        uint8_t getNumSensors() const;

        /**
         * @brief Gets the number of brightness levels which are used in the read process.
         *
         * @return The number of brightness levels.
         */
        uint32_t getNumBrightnessLevels() const;

        /**
         * @brief Measures the proximity sensor response for all configured brightness levels.
         *
         * This function prepares all initialized sensor pins with pull-ups, resets the
         * stored counts, and then cycles through all brightness levels. For each
         * brightness level, the left and right IR emitters are activated separately.
         *
         * If a sensor output is pulled low while an emitter side is active, the
         * corresponding count is incremented. A higher count indicates a stronger
         * reflected IR signal.
         */
        void read();

        /**
         * @brief Gets the count measured with the left emitters.
         *
         * @param sensor Sensor for which the count shall be returned.
         * @return Count measured with the left emitters.
         */
        uint32_t getCountsWithLeftLeds(Zumo2040::ProximitySensor sensor) const;

        /**
         * @brief Gets the count measured with the right emitters.
         *
         * @param sensor Sensor for which the count shall be returned.
         * @return Count measured with the right emitters.
         */
        uint32_t getCountsWithRightLeds(Zumo2040::ProximitySensor sensor) const;

    private:
        /**
         * @brief Initializes the sensors selected by the given state array.
         *
         * @param states Sensor activation states in proximity sensor order.
         */
        void init(Zumo2040::ProximitySensorStatus &sensor);

        /**
         * @brief Starts the IR pulses on the corresponding side.
         *
         * @param side The emitter side which shall be activated.
         * @param brightness The brightness level which shall be used
         * for the IR pulses.
         */
        void startIRPuls(Zumo2040::EmitterSide side, uint16_t brightness);

        /**
         * @brief Stops all of the IR pulses.
         */
        void stopIRPuls();

        struct SensorData
        {
            /** Sensor input pin or invalid pin value if unused. */
            uint8_t pin;

            /** Number of brightness levels detected while the left emitters were active. */
            uint32_t withLeftLeds;

            /** Number of brightness levels detected while the right emitters were active. */
            uint32_t withRightLeds;
        };

        /** Line sensor emitter pin. */
        uint8_t m_lineSensorEmitterPin;
        /** Sensor data for all proximity sensor slots. */
        SensorData m_data[Zumo2040::Pins::PROXIMITY_SENSOR_COUNT];
        /** Number of sensors which are initialized. */
        uint32_t m_numSensors;
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040PROXIMITYSENSORS_H */
/** @} */
