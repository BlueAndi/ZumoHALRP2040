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
 * @brief  Settings realization
 * @author Felix Reitenauer
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Settings.h"
#include <EEPROM.h>
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

/**
 * The magic pattern which is used to determine whether the EEPROM
 * is initialized or not.
 */
static const uint32_t MAGIC_PATTERN = 0xC0FFEEu;

/**
 * Data version is used to detect whether the data in the EEPROM is
 * compatible with the current settings version.
 *
 * Increase the version number by 1 for every change!
 */
static const uint8_t DATA_VERSION = 1u;

/**
 * Max. speed default values in steps/s.
 */
static const int16_t DEFAULT_MAX_SPEED = 0;

/**
 * Size used for the emulated EEPROM capacity.
 * Even though the emulated EEPROM uses a fixed flash sector for persistent
 * storage, we keep the configured size small to reduce RAM usage.
 */
static const size_t EEPROM_SIZE = 256u;

/* ---------- Attention! ----------
 * Keep the following order of variables in the EEPROM.
 * Add new values at the tail.
 * Increase the data version (DATA_VERSION) for every change!
 */

/**
 * Magic pattern address in the emulated EEPROM.
 */
static const size_t EEPROM_ADDRESS_MAGIC_PATTERN = 0u;

/**
 * Data version address in the emulated EEPROM.
 */
static const size_t EEPROM_ADDRESS_DATA_VERSION = EEPROM_ADDRESS_MAGIC_PATTERN + sizeof(MAGIC_PATTERN);

/**
 * Max. speed address in the emulated EEPROM.
 */
static const size_t EEPROM_ADDRESS_MAX_SPEED = EEPROM_ADDRESS_DATA_VERSION + sizeof(DATA_VERSION);

/* ---------- Tail of EEPROM data. ---------- */

/******************************************************************************
 * Public Methods
 *****************************************************************************/

void Settings::init()
{
    EEPROM.begin(EEPROM_SIZE);
    uint32_t magicPattern = getMagicPattern();
    uint8_t  dataVersion  = getDataVersion();

    if ((MAGIC_PATTERN != magicPattern) || (DATA_VERSION != dataVersion))
    {
        /* Write default values. */
        setMaxSpeed(DEFAULT_MAX_SPEED);

        /* Mark data in EEPROM as valid. */
        setMagicPattern(MAGIC_PATTERN);
        setDataVersion(DATA_VERSION);
    }
}

int16_t Settings::getMaxSpeed() const
{
    int16_t value = 0;
    EEPROM.get(EEPROM_ADDRESS_MAX_SPEED, value);
    return value;
}

void Settings::setMaxSpeed(int16_t maxSpeed)
{
    EEPROM.put(EEPROM_ADDRESS_MAX_SPEED, maxSpeed);
    EEPROM.commit();
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

uint32_t Settings::getMagicPattern() const
{
    uint32_t value = 0u;
    EEPROM.get(EEPROM_ADDRESS_MAGIC_PATTERN, value);
    return value;
}

void Settings::setMagicPattern(uint32_t value) const
{
    EEPROM.put(EEPROM_ADDRESS_MAGIC_PATTERN, value);
    EEPROM.commit();
}

uint8_t Settings::getDataVersion() const
{
    uint8_t value = 0u;
    EEPROM.get(EEPROM_ADDRESS_DATA_VERSION, value);
    return value;
}

void Settings::setDataVersion(uint8_t value) const
{
    EEPROM.put(EEPROM_ADDRESS_DATA_VERSION, value);
    EEPROM.commit();
}

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/