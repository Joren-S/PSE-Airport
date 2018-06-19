//============================================================================
// Name        : outputTests.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"
#include "../headers/TestUtils.h"



TEST(outputTest, happyDay) {
    ASSERT_TRUE(DirectoryExists("../test/testOutput"));
    int counter = 1;
    std::string filename = "../test/testOutput/happyDay" + ToString(counter) + ".xml";

    while (FileExists(filename)) {
        std::string atcname = "../test/testOutput/happyDay" + ToString(counter) + "ATC.txt";
        std::string logname = "../test/testOutput/happyDay" + ToString(counter) + "Log.txt";
        std::string infoname = "../test/testOutput/happyDay" + ToString(counter) + "Info.txt";
        std::ofstream atc(atcname.c_str());
        std::ofstream log(logname.c_str());
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

        std::string expectedatc = "../test/testOutput/happyDay" + ToString(counter) + "ExpectedATC.txt";
        std::string expectedlog = "../test/testOutput/happyDay" + ToString(counter) + "ExpectedLog.txt";
        std::string expectedinfo = "../test/testOutput/happyDay" + ToString(counter) + "ExpectedInfo.txt";

        EXPECT_TRUE(FileCompare(atcname, expectedatc));
        EXPECT_TRUE(FileCompare(logname, expectedlog));
        EXPECT_TRUE(FileCompare(infoname, expectedinfo));

        counter++;
        filename = "../test/testOutput/happyDay" + ToString(counter) + ".xml";
    }
}