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
 * @brief  Example application
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "App.h"
#include "Board.h"
#include <Arduino.h>
#include <Zumo2040.h>
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
Zumo2040ButtonA button_a;
Zumo2040ButtonB button_b;
Zumo2040ButtonC button_c;
Zumo2040Motors motor;
Zumo2040Encoder encoder;
Zumo2040Linesensors linesensor;
LinesensorsInfo info;
Zumo2040ProximitySensors prox;
Zumo2040OLED oled;
Zumo2040IMU_I2C i2c;
Zumo2040IMU imu;

constexpr uint8_t SIZE_ONE_BYTE = 1u;

constexpr uint32_t LENGTH_ONE_BYTE = 1u;

constexpr uint8_t U_INTEGER_8_ZERO = 0u;

constexpr uint8_t LIS3MDL_ADDR = 0x1Eu;

constexpr uint8_t LIS3MDL_WHO_AM_I_ADDR = 0x0F;

constexpr uint8_t LIS3MDL_WHO_AM_I_ID = 0b00111101;

constexpr uint8_t LIS3MDL_REG_CTRL_REG1 = 0x20;

constexpr uint8_t LIS3MDL_REG_CTRL_REG2 = 0x21;

constexpr uint8_t LIS3MDL_REG_CTRL_REG3 = 0x22;

constexpr uint8_t LIS3MDL_REG_CTRL_REG4 = 0x23;

constexpr uint8_t LSM6DSO_ADDR = 0x6Bu;

constexpr uint8_t LSM6DSO_WHO_AM_I_ID = 0b01101100;

constexpr uint8_t LSM6DSO_WHO_AM_I_ADDR = 0x0F;

constexpr uint8_t LSM6DSO_REG_CTRL1_XL = 0x10;

constexpr uint8_t LSM6DSO_REG_CTRL2_G = 0x11;

constexpr uint8_t LSM6DSO_REG_CTRL3_C = 0x12;

ErrorCode calibError;
/******************************************************************************
 * Public Methods
 *****************************************************************************/

 void App::setup()
{
    /* Place your once executed code for the setup here.. */
    Board::getInstance().init();

    oled.setLayout21x8();
    imu.enableDefault();
    //imu.configureForTurnSensing();
    //imu.configureForCompassHeading();
    calibError = imu.calibrateLineFollower();
}

void App::loop()
{
    /* Place your periodically executed code here. */
    static uint8_t regAddr = 0x21;
    const uint32_t length = 1u;
    static uint8_t r_data[length];
    static uint8_t w_data0[length] = {0b0};
    static uint8_t w_data1[length] = {0b1};
    static uint32_t counter = 0;

    static ErrorCode test0;
    static uint8_t test1;
    static vector acc;
    static vector gyro;
    static vector magnet;
    static DataStatus status;

/*
    if (counter % 2 == 0 && button_a.getSingleDebouncedPress())
    {
        test0 = i2c.write(addr_LSM6DSO, 0x10, w_data1, length);
        counter++;

    } else if (counter % 2 != 0 && button_a.getSingleDebouncedPress())
    {
        test0 = i2c.write(addr_LSM6DSO, 0x10, w_data0, length);
        counter++;
    }

    test0 = i2c.read(addr_LSM6DSO, 0x10, r_data, length);

    oled.clear();
    oled.print(r_data[0], BIN);
    oled.print(" ");
    oled.print(test0);
*/

/*
    oled.clear();

    oled.print("Register values LIS3MDL: ");
    oled.gotoXY(0, 1);
    oled.print(imu.readOneByte(LIS3MDL_ADDR, LIS3MDL_REG_CTRL_REG1), HEX);
    oled.gotoXY(0, 2);
    oled.print(imu.readOneByte(LIS3MDL_ADDR, LIS3MDL_REG_CTRL_REG2), HEX);
    oled.gotoXY(0, 3);
    oled.print(imu.readOneByte(LIS3MDL_ADDR, LIS3MDL_REG_CTRL_REG3), HEX);
    oled.gotoXY(0, 4);
    oled.print(imu.readOneByte(LIS3MDL_ADDR, LIS3MDL_REG_CTRL_REG4), HEX);
    oled.gotoXY(0, 5);
    oled.print("Error: ");
    oled.print(imu.getLastError());
*/

/*
    oled.clear();

    oled.print("Register values LSM6DSO: ");
    oled.gotoXY(0, 1);
    oled.print(imu.readOneByte(LSM6DSO_ADDR, LSM6DSO_REG_CTRL1_XL), HEX);
    oled.gotoXY(0, 2);
    oled.print(imu.readOneByte(LSM6DSO_ADDR, LSM6DSO_REG_CTRL2_G), HEX);
    oled.gotoXY(0, 3);
    oled.print(imu.readOneByte(LSM6DSO_ADDR, LSM6DSO_REG_CTRL3_C), HEX);
    oled.gotoXY(0, 4);
    oled.print("Error: ");
    oled.print(imu.getLastError());
*/

/*
    acc = imu.readAcc();

    oled.clear();

    oled.print("Accel: ");
    oled.gotoXY(0, 1);
    oled.print("X: ");
    oled.print(acc.x);
    oled.gotoXY(0, 2);
    oled.print("Y: ");
    oled.print(acc.y);
    oled.gotoXY(0, 3);
    oled.print("Z: ");
    oled.print(acc.z);
    oled.gotoXY(0, 4);
    oled.print("Error: ");
    oled.print(imu.getLastError());
*/

/*
    gyro = imu.readGyro();

    oled.clear();

    oled.print("Gyro: ");
    oled.gotoXY(0, 1);
    oled.print("X: ");
    oled.print(gyro.x);
    oled.gotoXY(0, 2);
    oled.print("Y: ");
    oled.print(gyro.y);
    oled.gotoXY(0, 3);
    oled.print("Z: ");
    oled.print(gyro.z);
    oled.gotoXY(0, 4);
    oled.print("Error: ");
    oled.print(imu.getLastError());
*/

/*
    magnet = imu.readMag();

    oled.clear();

    oled.print("Magnet: ");
    oled.gotoXY(0, 1);
    oled.print("X: ");
    oled.print(magnet.x);
    oled.gotoXY(0, 2);
    oled.print("Y: ");
    oled.print(magnet.y);
    oled.gotoXY(0, 3);
    oled.print("Z: ");
    oled.print(magnet.z);
    oled.gotoXY(0, 4);
    oled.print("Error: ");
    oled.print(imu.getLastError());
*/

/*
    magnet = imu.readMag();
    status = imu.magDataReady();

    oled.clear();

    oled.print(status);

    delay(1000);
    status = imu.magDataReady();

    oled.print(status);
*/

/*
    acc = imu.readAcc();
    status = imu.accDataReady();

    oled.clear();

    oled.print(status);

    delay(1000);
    status = imu.accDataReady();

    oled.print(status);
*/

/*
    gyro = imu.readGyro();
    status = imu.gyroDataReady();

    oled.clear();

    oled.print(status);

    delay(1000);
    status = imu.gyroDataReady();

    oled.print(status);
*/

/*
    oled.clear();

    oled.print(imu.getAccelOffsetX());
    oled.gotoXY(0, 1);
    oled.print(imu.getGyroOffsetZ());
    oled.gotoXY(0, 2);
    oled.print("Error: ");
    oled.print(imu.getLastError());
*/

    delay(100);
}

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
