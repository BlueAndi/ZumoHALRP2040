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
 * @brief  Receiver realization
 * @author Felix Reitenauer
 *
 * @addtogroup HALTarget
 *
 *
 * @{
 */

#ifndef RECEIVER_H
#define RECEIVER_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <IReceiver.h>
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** This class simulates a receiver, because its only relevant for the simulation. */
class Receiver : public IReceiver
{
public:
    /**
     * Set channel which to receive data from.
     *
     * @param[in] channel   The channel which to use.
     */
    void setChannel(int32_t channel) final
    {
        (void)channel;
    }

    /**
     * Receives data from the configured channel.
     *
     * @param[in] data  Data buffer.
     * @param[in] size  Data buffer size in bytes.
     *
     * @return Number of bytes read from stream.
     */
    size_t receive(void* data, size_t size) final
    {
        (void)data;
        (void)size;
        return 0U;
    }

    /**
     * Check if any data has been received.
     *
     * @return Number of available bytes.
     */
    int available() const final
    {
        return 0;
    }
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* RECEIVER_H */
/** @} */
