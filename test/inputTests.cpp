//============================================================================
// Name        : inputTests.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"

class inputTest: public ::testing::Test {
protected:

    Input input;

};


TEST_F(inputTest, HappyDay) {
    EXPECT_DEATH(input.read("filename"), "Couldn't open filename.");
    // do stuff
}

TEST_F(inputTest, nonexistentFile) {

}


TEST_F(inputTest, XMLSyntaxErrors) {

}


TEST_F(inputTest, illegalElement) {

}

// and more - see tictactoe