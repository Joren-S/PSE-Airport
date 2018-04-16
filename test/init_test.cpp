#include "../headers/System.h"


class SystemTest: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
        Input input = Input("../input.xml");
        system.import(input);
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
    }

    // Declares the variables your tests want to use.
    System system;
};


TEST_F(SystemTest, DefaultCon) {
    ASSERT_FALSE(system.getAirplanes().empty());
}


TEST_F(SystemTest, DefaultConstructor) {
    system.getAirplanes().empty();
    // Test to check if there are 0 airplanes, airports or runways.
    ASSERT_TRUE(system.getAirplanes().empty());
    ASSERT_TRUE(system.getAirports().empty());
    ASSERT_TRUE(system.getRunways().empty());
}


TEST_F(SystemTest, Setup) {
    // Test to check if there are more than 0 airplanes, airports and runways.
    ASSERT_TRUE(!system.getAirplanes().empty());
    ASSERT_TRUE(!system.getAirports().empty());
    ASSERT_TRUE(!system.getRunways().empty());

    // Test to check if all runways are free.
    for (size_t i = 0; i < system.getRunways().size(); ++i) {
        Runway* rw = system.getRunways().at(i);
        ASSERT_TRUE(rw->isFree());
    }

    // Test to check if all airplanes have a valid status (kApproaching or kGate).
    for (size_t i = 0; i < system.getAirplanes().size(); ++i) {
        Airplane* ap = system.getAirplanes().at((int)i);
        ASSERT_TRUE(ap->getStatus() != (3  || 1)); // kLanded, kFinished
    }
}

TEST_F(SystemTest, Run) {
    system.run();

    // Test to check if all planes are finished and no longer have a gate assigned to them.
    for (size_t i = 0; i < system.getAirplanes().size(); ++i) {
        Airplane* ap = system.getAirplanes().at((int)i);
        ASSERT_TRUE(ap->getStatus() == kFinished);
        ASSERT_TRUE(ap->getGateID() == -1);
    }

    // Test to check if all runways are free.
    for (size_t i = 0; i < system.getRunways().size(); ++i) {
        Runway* rw = system.getRunways().at((int)i);
        ASSERT_TRUE(rw->isFree());
    }
}

//TEST_F(SystemTest, Log) {
//    system.setup("../input.xml");
//    system.run();
//    system.log();
//
//    // Test to check if the output file is correct.
//    ASSERT_TRUE(FileCompare);
//}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}