//============================================================================
// Name        : domainTestsATC.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"


// for each test function, set up the atc
// constructor with argument necessary


TEST(domainTestATC, DefaultConstructor) {
    ofstream out("../test/testOutput/abc.txt";)
    ATC atc(out);

    // do stuff
}