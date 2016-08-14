
#include "unity.h"
#include "windowed_event_counter.h"

void setUp(void) {
    (void) WEC_WindowStart(0U);
    (void) WEC_WindowStop(0U);
}

void tearDown(void) {
    (void) WEC_WindowStop(0U);
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
    return UNITY_END();
}
