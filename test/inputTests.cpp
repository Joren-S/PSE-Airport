//============================================================================
// Name        : inputTests.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"
#include "../headers/TestUtils.h"

class inputTest: public ::testing::Test {
protected:

    Input input;

};


TEST_F(inputTest, HappyDay) {
    ASSERT_TRUE(FileExists("../test/testInput/happyDay"));
    input.read("../test/testInput/happyDay");
    vector<Flightplan*> flightplans = input.getFlightplans();
    Airport* airport = input.getAirports()[0];
    ASSERT_TRUE(airport != NULL);

    vector<Runway*> runways = airport->getRunways();
}

TEST_F(inputTest, legalInput) {
    ASSERT_TRUE(DirectoryExists("../test/testInput"));

    ofstream myfile;
    int fileCounter = 1;
    string fileName = "../test/testInput/legalInput" + ToString(fileCounter) + ".xml";

    while (FileExists (fileName)) {
        myfile.open("../test/testInput/error.txt");
        input.read(fileName, myfile);
        myfile.close();

        EXPECT_TRUE(FileIsEmpty("../test/testInput/error.txt"));

        fileCounter = fileCounter + 1;
        fileName = "../test/testInput/legalInput" + ToString(fileCounter) + ".xml";
    }
}

TEST_F(inputTest, nonexistentFile) {
    ASSERT_DEATH(input.read("nonexistent1"), "Couldn't open nonexistent1.");
    ASSERT_DEATH(input.read("nonexistent2"), "Couldn't open nonexistent2.");
    ASSERT_DEATH(input.read("nonexistent3"), "Couldn't open nonexistent3.");
}


TEST_F(inputTest, XMLSyntaxErrors) {
    ASSERT_TRUE(DirectoryExists("../test/testInput"));

    int fileCounter = 1;
    string fileName = "../test/testInput/inputSyntaxError" + ToString(fileCounter) + ".xml";

    while (FileExists (fileName)) {
        EXPECT_DEATH(input.read(fileName), "Couldn't open " + fileName + ".");

        fileCounter++;
        fileName = "../test/testInput/inputSyntaxError" + ToString(fileCounter) + ".xml";
    }
}


TEST_F(inputTest, illegalElement) {

}

// and more - see tictactoe