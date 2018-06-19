//============================================================================
// Name        : domainTestsGraphics.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/Graphics.h"
#include "../headers/TestUtils.h"

using namespace std;

class domainTestGraphics: public ::testing::Test {
protected:

    void SetUp() {
        airport = new Airport;
        airport->setGates(6);

        graphics = new Graphics(airport);
    }

    Airport* airport;
    Graphics* graphics;

};


TEST_F(domainTestGraphics, happyDay) {
    // 1st runway
    Runway* rw2 = new Runway();
    rw2->setName("RW1");
    rw2->setTaxiPoint("Alpha");
    rw2->setType(kAsphalt);
    rw2->setLength(1000);

    // 2nd runway
    Runway *rw = new Runway();
    rw->setName("RW2");
    rw->setTaxiPoint("Bravo");
    rw->setType(kGrass);
    rw->setLength(700);

    string tempname = "../test/testOutput/temp.txt";


    graphics->addElement(rw);
    graphics->addElement(rw2);

    Airplane* airplane1 = new Airplane;
    airplane1->setStatus(kDeboarding);
    airplane1->setGateID(1);
    graphics->addElement(airplane1);

    std::ofstream out(tempname.c_str());
    out << graphics->generateINI(300, 80, 140);
    out.close();

    Airplane* airplane2 = new Airplane;
    airplane2->setStatus(kTaxiArrival);
    airplane2->setPosition("Alpha");

    graphics->addElement(airplane2);

    EXPECT_TRUE(FileCompare("../test/testOutput/graphics1.ini", tempname.c_str()));

    out.open(tempname.c_str());
    out << graphics->generateINI(300, 80, 140);
    out.close();

    EXPECT_TRUE(FileCompare("../test/testOutput/graphics2.ini", tempname.c_str()));

    Airplane* airplane3 = new Airplane;
    airplane3->setStatus(kCrossingDeparture);
    airplane3->setPosition("Alpha");

    graphics->addElement(airplane2);

    out.open(tempname.c_str());
    out << graphics->generateINI(300, 80, 140);
    out.close();

    EXPECT_TRUE(FileCompare("../test/testOutput/graphics3.ini", tempname.c_str()));

    delete rw;
    delete rw2;
    delete airplane1;
    delete airplane2;
    delete airplane3;
}
