//============================================================================
// Name        : outputTests.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"
#include "../headers/TestUtils.h"



TEST(outputTest, happyDay) {
    int counter = 1;
    string filename = "../test/testOutput/happyDay" + ToString(counter) + ".xml";

    while (FileExists(filename)) {
        string atcname = "../test/testOutput/happyDay" + ToString(counter) + "ATC.txt";
        string logname = "../test/testOutput/happyDay" + ToString(counter) + "Log.txt";
        string infoname = "../test/testOutput/happyDay" + ToString(counter) + "Info.txt";
        ofstream atc(atcname.c_str());
        ofstream log(logname.c_str());
        Input input;
        input.read(filename);
        System system;
        system.initializeATC(atc, true);
        system.import(input);
        system.setEndTime(14);
        system.run(log);
        system.info(infoname);
        atc.close();
        log.close();

        string expectedatc = "../test/testOutput/happyDay" + ToString(counter) + "ExpectedATC.txt";
        string expectedlog = "../test/testOutput/happyDay" + ToString(counter) + "ExpectedLog.txt";
        string expectedinfo = "../test/testOutput/happyDay" + ToString(counter) + "ExpectedInfo.txt";

        EXPECT_TRUE(FileCompare(atcname, expectedatc));
        EXPECT_TRUE(FileCompare(logname, expectedlog));
        EXPECT_TRUE(FileCompare(infoname, expectedinfo));

        counter++;
        filename = "../test/testOutput/happyDay" + ToString(counter) + ".xml";
    }
}