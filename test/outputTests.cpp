//============================================================================
// Name        : outputTests.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"
#include "../headers/TestUtils.h"

class outputTest: public ::testing::Test {
protected:

    System system;

};



TEST_F(outputTest, happyDay) {
    int counter = 1;
    string filename = "../test/testOutput/happyDay" + ToString(counter) + ".xml";

    while (FileExists(filename)) {
        string atcname = "../test/testOutput/happyDayATC" + ToString(counter) + ".txt";
        string logname = "../test/testOutput/happyDayLog" + ToString(counter) + ".txt";
        string infoname = "../test/testOutput/happyDayInfo" + ToString(counter) + ".txt";
        ofstream atc(atcname.c_str());
        ofstream log(logname.c_str());
        Input input;
        input.read(filename);
        system.initializeATC(atc, true);
        system.import(input);
        system.setEndTime(18);
        system.run(log);
        system.info(infoname);
        atc.close();
        log.close();

        string expectedatc = "../test/testOutput/happyDayExpectedATC" + ToString(counter) + ".txt";
        string expectedlog = "../test/testOutput/happyDayExpectedLog" + ToString(counter) + ".txt";
        string expectedinfo = "../test/testOutput/happyDayExpectedInfo" + ToString(counter) + ".txt";

        EXPECT_TRUE(FileCompare(atcname, expectedatc));
        EXPECT_TRUE(FileCompare(logname, expectedlog));
        EXPECT_TRUE(FileCompare(infoname, expectedinfo));

        counter++;
        filename = "../test/testOutput/happyDay" + ToString(counter) + ".xml";
    }
}