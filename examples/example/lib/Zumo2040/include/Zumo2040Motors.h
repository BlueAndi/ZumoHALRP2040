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
 * @brief Motors driver
 * @author Felix Reitenauer
 *
 * @addtogroup Motors
 *
 * @{
 */

#ifndef ZUMO2040MOTORS_H
#define ZUMO2040MOTORS_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <cstdint>

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

enum PwmChannel
{
    /** Index for the right motor channel on the PWM slice 7 */
    RIGHT_MOTOR = 0,
    /** Index for the left motor channel on the PWM slice 7 */
    LEFT_MOTOR = 1
};

class Zumo2040Motors
{
    public:
        /** Initialize the PWM and direction PINs. */
        Zumo2040Motors();
        /**
         * @brief Flips the direction of the left motor.
         *
         * @param flip If true, then positive motor speeds will correspond to the
         * direction pin being high. If false, then positive motor speeds will
         * correspond to the direction pin being low.
         */
        void flipLeftMotor(bool flip);

        /**
         * @brief Flips the direction of the right motor.
         *
         * @param flip If true, then positive motor speeds will correspond to the
         * direction pin being high. If false, then positive motor speeds will
         * correspond to the direction pin being low.
         */
        void flipRightMotor(bool flip);

        /**
         * @brief Sets the speed for the left motor.
         *
         * @param speed A number from -400 to 400 representing the speed and
         * direction of the left motor. Values of -400 or less result in full speed
         * reverse, and values of 400 or more result in full speed forward.
         */
        void setLeftSpeed(int16_t speed);

        /**
         * @brief Sets the speed for the right motor.
         *
         * @param speed A number from -400 to 400 representing the speed and
         * direction of the right motor. Values of -400 or less result in full speed
         * reverse, and values of 400 or more result in full speed forward.
         */
        void setRightSpeed(int16_t speed);

        /**
         * @brief Sets the speeds for both motors.
         *
         * @param leftSpeed A number from -400 to 400 representing the speed and
         * direction of the left motor. Values of -400 or less result in full speed
         * reverse, and values of 400 or more result in full speed forward.
         *
         * @param rightSpeed A number from -400 to 400 representing the speed and
         * direction of the right motor. Values of -400 or less result in full speed
         * reverse, and values of 400 or more result in full speed forward.
         */
        void setSpeeds(int16_t leftSpeed, int16_t rightSpeed);

    private:
        /**
         * Indicates whether the left motor direction pin logic is inverted.
         * Default: LOW = forward, HIGH = reverse.
         */
        bool m_flipLeftMotor = false;

        /**
         * Indicates whether the right motor direction pin logic is inverted.
         * Default: LOW = forward, HIGH = reverse.
         */
        bool m_flipRightMotor = false;
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* ZUMO2040MOTORS_H */
/** @} */
