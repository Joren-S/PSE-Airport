//============================================================================
// Name        : domainTestsTime.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"


class domainTestTime: public ::testing::Test {
protected:

    Time time;

};


TEST_F(domainTestTime, constructor) {
    EXPECT_EQ(time.getHour(), 12);
    EXPECT_EQ(time.getMinute(), 0);

    Time time1(14);
    EXPECT_EQ(time1.getMinute(), 0);
    EXPECT_EQ(time1.getHour(), 14);

    Time time2(15, 48);
    EXPECT_EQ(time2.getMinute(), 48);
    EXPECT_EQ(time2.getHour(), 15);
}

TEST_F(domainTestTime, happyDay) {
    time.advance(30);
    EXPECT_EQ(time.getHour(), 12);
    EXPECT_EQ(time.getMinute(), 30);

    time.advance(35);
    EXPECT_EQ(time.getHour(), 13);
    EXPECT_EQ(time.getMinute(), 5);
}

TEST_F(domainTestTime, operators) {
    EXPECT_TRUE(Time(15, 15) == Time(15, 15));
    EXPECT_FALSE(Time(15, 15) == Time(14, 15));
    EXPECT_TRUE(Time(13, 17) < Time(14, 18));
    EXPECT_FALSE(Time(13, 17) < Time(13, 16));
}

TEST_F(domainTestTime, contractViolations) {
    EXPECT_DEATH(time.setMinute(-1), "Minute has to be between 0 and 60");
    EXPECT_DEATH(time.setMinute(78), "Minute has to be between 0 and 60");
    EXPECT_DEATH(time.setHour(25), "Hour has to be between 0 and 24");
    EXPECT_DEATH(time.setHour(-1), "Hour has to be between 0 and 24");
    EXPECT_DEATH(time.advance(-1), "Advancing by a negative amount of minutes is not possible");
}