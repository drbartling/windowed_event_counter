
#include "unity.h"
#include "windowed_event_counter.h"

extern bool WEC_started;
extern WEC_TIME_T WEC_startTime;
extern WEC_TIME_T WEC_stopTime;
extern WEC_TIME_T WEC_windowLimit;
extern WEC_TIME_T WEC_eventBuffer[WEC_EVENT_BUFFER_SIZE];

WEC_TIME_T *WEC_PtrIncrement(WEC_TIME_T ptr[]);

void setUp(void) {
    (void) WEC_WindowStart(0U);
    (void) WEC_WindowStop(0U);
    (void) WEC_WindowLimitSet(10000U);
}

void tearDown(void) {
    (void) WEC_WindowStop(0U);
    WEC_EventsClear();
}

void test_WindowStart_should_returnOkay_when_moduleIsNotStarted(void) {
    TEST_ASSERT_EQUAL_MESSAGE(WEC_OKAY, WEC_WindowStart(0U),
            "Expected WEC_OKAY");
}

void test_WindowStart_should_returnAlreadyStarted_when_moduleIsStarted(void) {
    (void) WEC_WindowStart(0U);
    TEST_ASSERT_EQUAL_MESSAGE(WEC_ALREADY_STARTED, WEC_WindowStart(0U),
            "Expected WEC_ALREADY_STARTED");
}

void test_WindowStop_should_returnNotStarted_when_moduleIsNotStarted(void) {
    TEST_ASSERT_EQUAL_MESSAGE(WEC_NOT_STARTED, WEC_WindowStop(0U),
            "Expected WEC_NOT_STARTED");
}

void test_WindowStop_should_returnOkay_when_moduleStarted(void) {
    (void) WEC_WindowStart(0U);
    TEST_ASSERT_EQUAL_MESSAGE(WEC_OKAY, WEC_WindowStop(0U),
            "Expected WEC_OKAY");
}

void test_WindowTimeGet_should_returnTheWindowTime(void) {
    WEC_TIME_T startTime = 0U;
    WEC_TIME_T check1 = 100U;
    WEC_TIME_T check2 = 200U;

    (void) WEC_WindowStart(startTime);

    TEST_ASSERT_EQUAL(check1, WEC_WindowTimeGet(check1));
    TEST_ASSERT_EQUAL(check2, WEC_WindowTimeGet(check2));
}

void test_WindowTimeGet_should_returnCorrectWindowTimeRegardlessOfStartTime(void) {
    WEC_TIME_T startTime = 245U;
    WEC_TIME_T window1 = 125U;
    WEC_TIME_T window2 = 653U;
    WEC_TIME_T check1 = startTime + window1;
    WEC_TIME_T check2 = startTime + window2;

    (void) WEC_WindowStart(startTime);

    TEST_ASSERT_EQUAL(window1, WEC_WindowTimeGet(check1));
    TEST_ASSERT_EQUAL(window2, WEC_WindowTimeGet(check2));
}

void test_WindowTimeGet_should_return0BeforeFirstStart(void) {
    WEC_TIME_T check1 = 100U;
    WEC_TIME_T check2 = 200U;
    TEST_ASSERT_EQUAL(0U, WEC_WindowTimeGet(check1));
    TEST_ASSERT_EQUAL(0U, WEC_WindowTimeGet(check2));
}

void test_WindowTimeGet_should_returnConstantValue_when_stopped(void) {
    WEC_TIME_T timeStamps[] = {0U, 123U, 234U, 245U};
    WEC_TIME_T windows[] = {0U, 0U, 123U, 123U};

    (void) WEC_WindowStart(timeStamps[0]);
    (void) WEC_WindowStop(timeStamps[1]);

    TEST_ASSERT_EQUAL(windows[2], WEC_WindowTimeGet(timeStamps[2]));
    TEST_ASSERT_EQUAL(windows[3], WEC_WindowTimeGet(timeStamps[3]));
}

void test_WindowTimeGet_should_returnNoLargerThanSpecifiedWindowTimeLimit(void) {
    WEC_TIME_T timeStamps[] = {123U, 234U, 334U, 357U, 456U};
    WEC_TIME_T windowMax = 200U;
    WEC_TIME_T windows[] = {0U, 111U, windowMax, windowMax, windowMax};
    int i = 0;

    (void) WEC_WindowLimitSet(windowMax);
    (void) WEC_WindowStart(timeStamps[i]);
    i = 1;

    TEST_ASSERT_EQUAL(windows[i], WEC_WindowTimeGet(timeStamps[i]));
    i = 2;
    TEST_ASSERT_EQUAL(windows[i], WEC_WindowTimeGet(timeStamps[i]));
    i = 3;

    (void) WEC_WindowStop(timeStamps[i]);
    i = 4;

    TEST_ASSERT_EQUAL(windows[i], WEC_WindowTimeGet(timeStamps[i]));
}

void test_WindowLimitSet_should_returnOkay_when_notStarted(void) {
    TEST_ASSERT_EQUAL_MESSAGE(WEC_OKAY, WEC_WindowLimitSet(2000U),
            "Expected WEC_OKAY");
}

void test_WindowLimitSet_should_returnError_when_startedg(void) {
    (void) WEC_WindowStart(0U);
    TEST_ASSERT_EQUAL_MESSAGE(WEC_ALREADY_STARTED, WEC_WindowLimitSet(2000U),
            "Expected WEC_ALREADY_STARTED");
}

void test_WindowLimitGet_should_returnTheCurrentWindowLimit(void) {
    WEC_TIME_T testVal1 = 100U;
    (void) WEC_WindowLimitSet(testVal1);
    TEST_ASSERT_EQUAL(testVal1, WEC_WindowLimitGet());

    testVal1 = 152374U;
    (void) WEC_WindowLimitSet(testVal1);
    TEST_ASSERT_EQUAL(testVal1, WEC_WindowLimitGet());

    testVal1 = 5723621U;
    (void) WEC_WindowLimitSet(testVal1);
    TEST_ASSERT_EQUAL(testVal1, WEC_WindowLimitGet());
}

void test_windowLimit_should_notChangeWhileRunning(void) {
    WEC_TIME_T testVal1 = 100U;
    WEC_TIME_T testVal2 = 200U;
    (void) WEC_WindowLimitSet(testVal1);
    (void) WEC_WindowLimitGet();
    (void) WEC_WindowStart(0U);

    (void) WEC_WindowLimitSet(testVal2);
    TEST_ASSERT_EQUAL(testVal1, WEC_WindowLimitGet());
}

void test_EventAdd_should_returnOkay_when_addingToABufferSuccessfully(void) {
    (void) WEC_WindowStart(0U);
    TEST_ASSERT_EQUAL(WEC_OKAY, WEC_EventAdd(1U));
}

void test_EventCount_should_startAt0(void) {
    TEST_ASSERT_EQUAL(0U, WEC_EventCountGet(0U));
}

void test_EventAdd_should_increaseTheEventCount(void) {
    (void) WEC_WindowStart(0U);
    (void) WEC_EventAdd(1U);
    TEST_ASSERT_EQUAL(1U, WEC_EventCountGet(1U));
    (void) WEC_EventAdd(1U);
    TEST_ASSERT_EQUAL(2U, WEC_EventCountGet(1U));
}

void test_EventAdd_should_removeExpiredCounts(void) {
    (void) WEC_WindowLimitSet(200U);
    (void) WEC_WindowStart(0U);

    (void) WEC_EventAdd(0U);
    TEST_ASSERT_EQUAL(1U, WEC_EventCountGet(0U));

    (void) WEC_EventAdd(0U);
    TEST_ASSERT_EQUAL(2U, WEC_EventCountGet(0U));

    (void) WEC_EventAdd(100U);
    TEST_ASSERT_EQUAL(3U, WEC_EventCountGet(100U));

    (void) WEC_EventAdd(200U);
    TEST_ASSERT_EQUAL(2U, WEC_EventCountGet(200U));

    (void) WEC_EventAdd(300U);
    TEST_ASSERT_EQUAL(2U, WEC_EventCountGet(300U));
}

void test_PtrIncrement_should_incrementThePointerBy1(void) {
    WEC_TIME_T *ptr = WEC_eventBuffer;
    ptr = WEC_PtrIncrement(ptr);
    TEST_ASSERT_EQUAL_PTR(WEC_eventBuffer + 1, ptr);
}

void test_PtrIncrement_should_wrapAround(void) {
    WEC_TIME_T *ptr = &WEC_eventBuffer[WEC_EVENT_BUFFER_SIZE - 1];
    ptr = WEC_PtrIncrement(ptr);
    TEST_ASSERT_EQUAL(0, ptr - WEC_eventBuffer);
    TEST_ASSERT_EQUAL_PTR(WEC_eventBuffer, ptr);
}

void test_OperationAroundOverflow(void) {
    WEC_TIME_T time = 0 - 342;
    (void) WEC_WindowLimitSet(200U);
    TEST_ASSERT_TRUE(1000U < time); // Checking that I set the test up correctly
    (void) WEC_WindowStart(time);


    (void) WEC_EventAdd(time);
    TEST_ASSERT_EQUAL(1U, WEC_EventCountGet(time));

    (void) WEC_EventAdd(time);
    TEST_ASSERT_EQUAL(2U, WEC_EventCountGet(time));

    time += 100U;
    (void) WEC_EventAdd(time);
    TEST_ASSERT_EQUAL(3U, WEC_EventCountGet(time));

    time += 100U;
    (void) WEC_EventAdd(time);
    TEST_ASSERT_EQUAL(2U, WEC_EventCountGet(time));

    time += 100U;
    (void) WEC_EventAdd(time);
    TEST_ASSERT_EQUAL(2U, WEC_EventCountGet(time));

    time += 100U;
    (void) WEC_EventAdd(time);
    TEST_ASSERT_EQUAL(2U, WEC_EventCountGet(time));

    TEST_ASSERT_TRUE(1000U > time); // Checking that I set the test up correctly
}

void test_EventAdd_should_removeExpiredEventsBeforeAddingNewEvents(void) {
    WEC_TIME_T time = 0;
    (void) WEC_WindowLimitSet(WEC_EVENT_BUFFER_SIZE);
    (void) WEC_WindowStart(time);
    for (time = 0; time < WEC_EVENT_BUFFER_SIZE; time++) {
        (void) WEC_EventAdd(time);
        TEST_ASSERT_EQUAL(time + 1, WEC_EventCountGet(time));
    }
    TEST_ASSERT_EQUAL(WEC_OKAY, WEC_EventAdd(time));
    TEST_ASSERT_EQUAL(WEC_EVENT_BUFFER_SIZE, WEC_EventCountGet(time));
}

void test_EventAdd_should_removeOldestEvent_when_addingToFullBuffer(void) {
    WEC_TIME_T time = 0;
    (void) WEC_WindowLimitSet(WEC_EVENT_BUFFER_SIZE + 1);
    (void) WEC_WindowStart(time);
    for (time = 0; time < WEC_EVENT_BUFFER_SIZE; time++) {
        (void) WEC_EventAdd(time);
        TEST_ASSERT_EQUAL(time + 1, WEC_EventCountGet(time));
    }
    (void) WEC_EventAdd(time);
    TEST_ASSERT_EQUAL(WEC_EVENT_BUFFER_SIZE, WEC_EventCountGet(time));
}

void test_EventAdd_should_returnError_when_addingToFullBuffer(void) {
    WEC_TIME_T time = 0;
    (void) WEC_WindowLimitSet(WEC_EVENT_BUFFER_SIZE + 1);
    (void) WEC_WindowStart(time);
    for (time = 0; time < WEC_EVENT_BUFFER_SIZE; time++) {
        (void) WEC_EventAdd(time);
    }
    TEST_ASSERT_EQUAL(WEC_BUFFER_OVERFLOW, WEC_EventAdd(time));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_WindowStart_should_returnOkay_when_moduleIsNotStarted);
    RUN_TEST(test_WindowStart_should_returnAlreadyStarted_when_moduleIsStarted);
    RUN_TEST(test_WindowStop_should_returnNotStarted_when_moduleIsNotStarted);
    RUN_TEST(test_WindowStop_should_returnOkay_when_moduleStarted);
    RUN_TEST(test_WindowTimeGet_should_returnTheWindowTime);
    RUN_TEST(test_WindowTimeGet_should_returnCorrectWindowTimeRegardlessOfStartTime);
    RUN_TEST(test_WindowTimeGet_should_return0BeforeFirstStart);
    RUN_TEST(test_WindowTimeGet_should_returnConstantValue_when_stopped);
    RUN_TEST(test_WindowTimeGet_should_returnNoLargerThanSpecifiedWindowTimeLimit);
    RUN_TEST(test_WindowLimitSet_should_returnOkay_when_notStarted);
    RUN_TEST(test_WindowLimitSet_should_returnError_when_startedg);
    RUN_TEST(test_WindowLimitGet_should_returnTheCurrentWindowLimit);
    RUN_TEST(test_windowLimit_should_notChangeWhileRunning);
    RUN_TEST(test_EventAdd_should_returnOkay_when_addingToABufferSuccessfully);
    RUN_TEST(test_EventAdd_should_increaseTheEventCount);
    RUN_TEST(test_EventCount_should_startAt0);
    RUN_TEST(test_EventAdd_should_removeExpiredCounts);
    RUN_TEST(test_PtrIncrement_should_incrementThePointerBy1);
    RUN_TEST(test_PtrIncrement_should_wrapAround);
    RUN_TEST(test_OperationAroundOverflow);
    RUN_TEST(test_EventAdd_should_removeExpiredEventsBeforeAddingNewEvents);
    RUN_TEST(test_EventAdd_should_removeOldestEvent_when_addingToFullBuffer);
    RUN_TEST(test_EventAdd_should_returnError_when_addingToFullBuffer);
    return UNITY_END();
}
