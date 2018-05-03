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
    ASSERT_TRUE(FileExists("../test/testInput/happyDay.xml"));
    input.read("../test/testInput/happyDay.xml");

    vector<Flightplan*> flightplans = input.getFlightplans();
    EXPECT_EQ(flightplans.size(), size_t(1));

    vector<Airport*> airports = input.getAirports();
    EXPECT_EQ(airports.size(), size_t(1));

    Flightplan* flightplan = flightplans[0];
    EXPECT_TRUE(flightplan != NULL);

    Airport* airport = input.getAirports()[0];
    EXPECT_TRUE(airport != NULL);

    vector<Runway*> runways = airport->getRunways();
    EXPECT_EQ(runways.size(), size_t(2));

    // Airport
    EXPECT_EQ(airport->getName(), "Antwerp International Airport");
    EXPECT_EQ(airport->getIata(), "ANR");
    EXPECT_EQ(airport->getCallsign(), "Antwerp Tower");
    EXPECT_EQ(airport->getGates(), 10);

    // Runways
    Runway* runway1 = runways[0];
    EXPECT_EQ(runway1->getName(), "11R");
    EXPECT_EQ(runway1->getAirport(), airport);
    EXPECT_EQ(runway1->getType(), kAsphalt);
    EXPECT_EQ(runway1->getLength(), 1510);
    EXPECT_EQ(runway1->getTaxiPoint(), "Alpha");

    Runway* runway2 = runways[1];
    EXPECT_EQ(runway2->getName(), "11L");
    EXPECT_EQ(runway2->getAirport(), airport);
    EXPECT_EQ(runway2->getType(), kGrass);
    EXPECT_EQ(runway2->getLength(), 1010);
    EXPECT_EQ(runway2->getTaxiPoint(), "Bravo");

    // Flightplan
    EXPECT_EQ(flightplan->getDestination(), "LCY");
    EXPECT_EQ(flightplan->getDeparture(), 30);
    EXPECT_EQ(flightplan->getArrival(), 15);
    EXPECT_EQ(flightplan->getInterval(), 3);

    Airplane* airplane = flightplan->getAirplane();
    EXPECT_TRUE(airplane != NULL);

    EXPECT_EQ(airplane->getNumber(), "N11842");
    EXPECT_EQ(airplane->getCallsign(), "Cessna 842");
    EXPECT_EQ(airplane->getModel(), "Cessna 340");
    EXPECT_EQ(airplane->getType(), kPrivate);
    EXPECT_EQ(airplane->getEngine(), kJet);
    EXPECT_EQ(airplane->getSize(), kSmall);
    EXPECT_EQ(airplane->getFuel(), 10);
    EXPECT_EQ(airplane->getPassengers(), 8);
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

    ofstream myfile;
    int fileCounter = 1;
    string fileName = "../test/testInput/inputSyntaxError" + ToString(fileCounter) + ".xml";

    while (FileExists (fileName)) {

        EXPECT_DEATH(input.read(fileName, myfile), "Couldn't open " + fileName + ".");
        myfile.close();

        fileCounter++;
        fileName = "../test/testInput/inputSyntaxError" + ToString(fileCounter) + ".xml";
    }
}

TEST_F(inputTest, illegalAirport) {
    ASSERT_TRUE(DirectoryExists("../test/testInput"));

    ofstream myfile;
    int fileCounter = 1;
    string fileName = "../test/testInput/inputAirportError" + ToString(fileCounter) + ".xml";

    while (FileExists (fileName)) {
        string errorFileName = "../test/testInput/airportError" + ToString(fileCounter) + ".txt";

        myfile.open("../test/testInput/error.txt");

        Input in;
        EXPECT_DEATH(input.read(fileName, myfile), "No airport in simulation");

        myfile.close();

        fileCounter++;
        fileName = "../test/testInput/inputAirportError" + ToString(fileCounter) + ".xml";
    }
}

TEST_F(inputTest, illegalElement) {
    ASSERT_TRUE(DirectoryExists("../test/testInput"));

    ofstream myfile;
    int fileCounter = 1;
    string fileName = "../test/testInput/inputPartialError" + ToString(fileCounter) + ".xml";

    while (FileExists (fileName)) {
        string errorFileName = "../test/testInput/illegalError" + ToString(fileCounter) + ".txt";

        myfile.open("../test/testInput/error.txt");

        Input in;
        in.read(fileName, myfile);
        myfile.close();

        EXPECT_TRUE(FileCompare("../test/testInput/error.txt", errorFileName));

        fileCounter++;
        fileName = "../test/testInput/inputPartialError" + ToString(fileCounter) + ".xml";
    }
}
