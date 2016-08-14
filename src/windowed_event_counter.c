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

//
// Section: Constants
//

//
// Section: Global Variable Declarations
//

static bool WEC_started;
static WEC_TIME_T WEC_startTime;
static WEC_TIME_T WEC_stopTime;

//
// Section: Macros
//

//
// Section: Static Function Prototypes
//

//
// Section: Static Function Definitions
//

//
// Section: Template Module APIs
//

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
        WEC_started = false;
        WEC_stopTime = stopTime;
    } else {
        err = WEC_NOT_STARTED;
    }
    return err;
}

WEC_TIME_T WEC_WindowTimeGet(WEC_TIME_T currentTime) {
    WEC_TIME_T windowTime;
    if (WEC_started) {
        windowTime = currentTime - WEC_startTime;
    } else {
        windowTime = WEC_stopTime - WEC_startTime;
    }
    return windowTime;
}

//
// End of File
//
