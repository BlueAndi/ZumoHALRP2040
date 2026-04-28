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
 * @brief Low-level linesensors driver
 * @author Felix Reitenauer
 *
 * @addtogroup Line sensor
 *
 * @{
 */

#ifndef RP2040LINESENSORS_H
#define RP2040LINESENSORS_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <hardware/pio.h>
#include "Zumo2040ErrorCodes.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** Size of the debug information array. */
static constexpr uint8_t DEBUG_INFO_COUNT = 2u;

/** Number of line sensors. */
static constexpr uint8_t SENSOR_COUNT = 5u;

/** Indicates the current state of the line sensor emitter. */
enum EmitterStates
{
    EMITTER_OFF,
    EMITTER_ON
};

/** Low-level line sensors driver class. */
class LinesensorsCore
{
    public:
        /**
         * Creates a line sensor core object in an uninitialized state.
         * Call init() before using the line sensors.
         */
        LinesensorsCore();

        /**
         * Destructor unclaims the state machine and
         * deactivates the line sensor emitter.
         */
        ~LinesensorsCore();

        /** Delete copy constructor. */
        LinesensorsCore(const LinesensorsCore&) = delete;

        /** Delete copy assignment operator. */
        LinesensorsCore& operator=(const LinesensorsCore&) = delete;

        /**
         * @brief Initializes the PIO state machine for the line sensors.
         * This method also activates the line sensor emitter.
         *
         * @return Error code of the initialization.
         */
        ErrorCode init();

        /**
         * @brief Reads the line sensors using the PIO state machine.
         *
         * The state machine uses the line sensor pins to charge the capacitors.
         * After that, the capacitors are discharged through the phototransistors,
         * whose conductivity depends on the infrared light reflected from the
         * surface.
         *
         * Lower values = brighter surface
         * Higher values = darker surface
         */
        void read(uint32_t sensorValues[SENSOR_COUNT]);

        /**
         * @brief Returns internal debug information from the last measurement.
         *
         * [0] = newPinValues, which represent the pin levels after the measurement
         * [1] = newValueCount, which represents how many sensor values were written
         *       to the result array during the measurement
         *
         * This function is useful for checking whether all sensor pins were charged
         * and discharged as expected and whether the number of written sensor values
         * matches the measured pin transitions.
         *
         * Tip: Display newPinValues in binary format.
         *
         * @return Internal debug information array.
         */
        const uint32_t* getDebugInformation() const;

        /**
         * @brief Controls the line sensors emitter.
         *
         * @param state EMITTER_OFF to deactivate the emitter,
         * EMITTER_ON to activate it.
         */
        void emitterControl(EmitterStates state);

    private:
        /** Line sensors error code. */
        ErrorCode m_errorCode;
        /** Selected PIO instance. */
        PIO m_pio;
        /** State machine used for the measurement. */
        int m_sm;
        /** Configuration of the state machine. */
        pio_sm_config m_config;
        /** The instruction memory offset the PIO program is loaded at. */
        int m_programEntry;
        /** Debug information from the last read() call.
         *  [0] = newPinValues
         *  [1] = newValueCount
         */
        uint32_t m_debugInformation[DEBUG_INFO_COUNT];
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* RP2040LINESENSORS_H */
/** @} */
