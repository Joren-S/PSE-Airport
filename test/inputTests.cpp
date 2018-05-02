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
    EXPECT_DEATH(input.read("filename"), "Couldn't open filename.");
    // do stuff
}

TEST_F(inputTest, legalInput) {
    ASSERT_TRUE(DirectoryExists("../test/testInput"));

    ofstream myfile;
    int fileCounter = 1;
    string fileName = "testInput/legalInput" + ToString(fileCounter) + ".xml";

    while (FileExists (fileName)) {
        myfile.open("testInput/error.txt");
        input.read(fileName, myfile);
        myfile.close();

        EXPECT_TRUE(FileIsEmpty("testInput/error.txt"));

        fileCounter = fileCounter + 1;
        fileName = "testInput/legalInput" + ToString(fileCounter) + ".xml";
    };
}

TEST_F(inputTest, nonexistentFile) {

}


TEST_F(inputTest, XMLSyntaxErrors) {

}


TEST_F(inputTest, illegalElement) {

}

// and more - see tictactoe