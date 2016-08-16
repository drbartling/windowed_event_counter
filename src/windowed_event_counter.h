/**
 * @file
 * windowed_event_counter.h
 *
 * @author
 * D. Ryan Bartling
 *
 * @brief
 * Counts number of events that occured within a specified window in time.
 *
 * Records time of events, keeping track of the number of events that occured
 * within a specified amount of time.
 */

/*******************************************************************************
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 D. Ryan Bartling
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
 ******************************************************************************/

/*
 * Abbreviations Used:
 * TODO: List all abbreviations used in source file
 * Especially those used in function and variable names.
 */

#ifndef WINDOWED_EVENT_COUNTER_H    // Guards against multiple inclusion
#    define WINDOWED_EVENT_COUNTER_H

//
// Section: Included Files
//
// TODO: Remove section if not used.

#    include <stdbool.h>
#    include <stdint.h>

//
// Section: Constants
//

/// Number of available elements in the event buffer.
#    define WEC_EVENT_BUFFER_SIZE (30U)

//
// Section: Data Types
//

typedef enum {
    WEC_OKAY = 0, ///< No error detected
    WEC_ERROR, ///< Unspecified error detected
    WEC_ALREADY_STARTED, ///< Windowed event detector is already started
    WEC_NOT_STARTED, ///< Windowed event detector is not started
} WEC_ERROR_T;

typedef uint32_t WEC_TIME_T;

typedef uint8_t WEC_COUNT_T;

//
// Section: Template Module APIs
//

/**
 * Updates event count with a new event and returns the total count.
 * @param eventTime time at which the event was detected
 * @returns current count of events after adding 1 event
 */
WEC_COUNT_T WEC_EventAdd(WEC_TIME_T eventTime);

/**
 * Gets the current number of events.
 * Removes expired events and returns the count of remaining events.
 * @param currentTime
 * @returns Count of events
 */
WEC_COUNT_T WEC_EventCountGet(WEC_TIME_T currentTime);

/**
 * Clears out all events.
 */
void WEC_EventsClear(void);

/**
 * Gets the value of the current window limit.
 * @returns the current window limit
 */
WEC_TIME_T WEC_WindowLimitGet(void);

/**
 * Sets the maximum length for the measurement window
 * @param windowLimit maximum length of measurement window
 * @return error
 */
WEC_ERROR_T WEC_WindowLimitSet(WEC_TIME_T windowLimit);

/**
 * Starts measurement
 * @param startTime
 * @returns error code
 */
WEC_ERROR_T WEC_WindowStart(WEC_TIME_T startTime);

/**
 * Stops measurement
 * @param stopTime
 * @returns error code
 */
WEC_ERROR_T WEC_WindowStop(WEC_TIME_T stopTime);

/**
 * Gets length (in time) of the measurement window
 * @param currentTime
 * @returns actual length of measurement window
 */
WEC_TIME_T WEC_WindowTimeGet(WEC_TIME_T currentTime);


#endif // WINDOWED_EVENT_COUNTER_H

//
// End of File
//

