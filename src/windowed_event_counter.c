/**
 * @file
 * TODO: windowed_event_counter.c
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

//
// Section: Included Files
//

#include "windowed_event_counter.h"
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>

//
// Section: Macros
//
#ifdef TEST
#    define STATIC
#else
#    define STATIC static
#endif

//
// Section: Constants
//

//
// Section: Global Variable Declarations
//

/// current count of events
STATIC WEC_COUNT_T WEC_count;

/// Indicates when window is started and running
STATIC bool WEC_started;

/// Timestamp marking the start of the measurement window
STATIC WEC_TIME_T WEC_startTime;

/// Timestamp marking the end of the measurement window
STATIC WEC_TIME_T WEC_stopTime;

/// Limit to the length of the time window
STATIC WEC_TIME_T WEC_windowLimit;

/// Stores the time of each event
STATIC WEC_TIME_T WEC_eventBuffer[WEC_EVENT_BUFFER_SIZE];

/// points to position to add the next event
STATIC WEC_TIME_T *WEC_eventBufferHead = WEC_eventBuffer;

/// points to oldest event
STATIC WEC_TIME_T *WEC_eventBufferTail = WEC_eventBuffer;

//
// Section: Macros
//

//
// Section: Static Function Prototypes
//

/// Remove oldest event in the queue
STATIC void WEC_EventOldestRemove(void);

/// Removes events equal to or older than the window limit
STATIC void WEC_EventExpire(WEC_TIME_T currentTime);

/// Increments pointers around the circular buffer
STATIC WEC_TIME_T *WEC_PtrIncrement(WEC_TIME_T ptr[]);

/// Updates the start time based on the window limit and current time
STATIC WEC_TIME_T WEC_StartTimeUpdate(WEC_TIME_T currentTime);

//
// Section: Static Function Definitions
//

STATIC void WEC_EventOldestRemove(void) {
    WEC_count--;
    WEC_eventBufferTail = WEC_PtrIncrement(WEC_eventBufferTail);
}

STATIC void WEC_EventExpire(WEC_TIME_T currentTime) {
    while (WEC_count) {
        WEC_TIME_T oldestEvent = *WEC_eventBufferTail;
        if (currentTime - oldestEvent < WEC_windowLimit) {
            break;
        } else {
            WEC_EventOldestRemove();
        }
    }
}

STATIC WEC_TIME_T * WEC_PtrIncrement(WEC_TIME_T ptr[]) {
    WEC_TIME_T * const limit = &WEC_eventBuffer[WEC_EVENT_BUFFER_SIZE - 1];
    if (limit > ptr) {
        ptr = &ptr[1];
    } else {
        ptr = WEC_eventBuffer;
    }
    return ptr;
}

STATIC WEC_TIME_T WEC_StartTimeUpdate(WEC_TIME_T currentTime) {
    WEC_TIME_T newStart;
    if ((currentTime - WEC_startTime) >= WEC_windowLimit) {
        newStart = currentTime - WEC_windowLimit;
    } else {
        newStart = WEC_startTime;
    }
    return newStart;
}

//
// Section: Template Module APIs
//

WEC_ERROR_T WEC_EventAdd(WEC_TIME_T eventTime) {
    WEC_ERROR_T err = WEC_ERROR;
    if (false == WEC_started) {
        return WEC_NOT_STARTED;
    }
    WEC_EventExpire(eventTime);
    if (WEC_EVENT_BUFFER_SIZE > WEC_count) {
        err = WEC_OKAY;
    } else {
        err = WEC_BUFFER_OVERFLOW;
        WEC_EventOldestRemove(); // Buffer overflow
    }
    WEC_count++;
    WEC_startTime = WEC_StartTimeUpdate(eventTime);
    *WEC_eventBufferHead = eventTime;
    WEC_eventBufferHead = WEC_PtrIncrement(WEC_eventBufferHead);
    return err;
}

WEC_COUNT_T WEC_EventCountGet(WEC_TIME_T currentTime) {
    return WEC_count;
}

void WEC_EventsClear(void) {
    WEC_count = 0;
    WEC_eventBufferHead = WEC_eventBuffer;
    WEC_eventBufferTail = WEC_eventBuffer;
}

WEC_TIME_T WEC_WindowLimitGet(void) {
    return WEC_windowLimit;
}

WEC_ERROR_T WEC_WindowLimitSet(WEC_TIME_T windowLimit) {
    WEC_ERROR_T err = WEC_ERROR;
    if (false == WEC_started) {
        err = WEC_OKAY;
        WEC_windowLimit = windowLimit;
    } else {
        err = WEC_ALREADY_STARTED;
    }
    return err;
}

WEC_ERROR_T WEC_WindowStart(WEC_TIME_T startTime) {
    WEC_ERROR_T err = WEC_ERROR;
    if (false == WEC_started) {
        err = WEC_OKAY;
        WEC_started = true;
        WEC_startTime = startTime;
    } else {
        err = WEC_ALREADY_STARTED;
    }
    return err;
}

WEC_ERROR_T WEC_WindowStop(WEC_TIME_T stopTime) {
    WEC_ERROR_T err = WEC_ERROR;
    if (true == WEC_started) {
        err = WEC_OKAY;
        WEC_startTime = WEC_StartTimeUpdate(stopTime);
        WEC_started = false;
        WEC_stopTime = stopTime;
    } else {

        err = WEC_NOT_STARTED;
    }
    return err;
}

WEC_TIME_T WEC_WindowTimeGet(WEC_TIME_T currentTime) {
    WEC_ERROR_T err = WEC_ERROR;
    WEC_TIME_T windowTime;
    if (WEC_started) {
        err = WEC_OKAY;
        WEC_startTime = WEC_StartTimeUpdate(currentTime);
        windowTime = currentTime - WEC_startTime;
    } else {
        err = WEC_NOT_STARTED;
        windowTime = WEC_stopTime - WEC_startTime;
    }
    assert(WEC_ERROR != err);
    return windowTime;
}

//
// End of File
//
