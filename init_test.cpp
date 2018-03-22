#include "headers/system.h"


class SystemTest: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
    }

    // Declares the variables your tests want to use.
    System sys;
};



TEST_F(SystemTest, DefaultConstructor) {
    // Test to check if there are 0 airplanes, airports or runways.
    ASSERT_TRUE(sys.getAirplanes().empty());
    ASSERT_TRUE(sys.getAirports().empty());
    ASSERT_TRUE(sys.getRunways().empty());
}


TEST_F(SystemTest, Setup) {
    sys.setup("../input.xml");

    // Test to check if there are more than 0 airplanes, airports and runways.
    ASSERT_TRUE(!sys.getAirplanes().empty());
    ASSERT_TRUE(!sys.getAirports().empty());
    ASSERT_TRUE(!sys.getRunways().empty());

    // Test to check if all runways are free.
    for (size_t i = 0; i < sys.getRunways().size(); ++i) {
        runway* rw = sys.getRunways().at((int)i);
        ASSERT_TRUE(rw->isFree());
    }

    // Test to check if all airplanes have a valid status (kApproaching or kGate).
    for (size_t i = 0; i < sys.getAirplanes().size(); ++i) {
        airplane* ap = sys.getAirplanes().at((int)i);
        ASSERT_TRUE(ap->getFStatus() != (3  || 1)); // kLanded, kFinished
    }
}

TEST_F(SystemTest, Run) {
    sys.setup("../input.xml");
    sys.run();

    // Test to check if all planes are finished and no longer have a gate assigned to them.
    for (size_t i = 0; i < sys.getAirplanes().size(); ++i) {
        airplane* ap = sys.getAirplanes().at((int)i);
        ASSERT_TRUE(ap->getFStatus() == kFinished);
        ASSERT_TRUE(ap->getFGateID() == -1);
    }

    // Test to check if all runways are free.
    for (size_t i = 0; i < sys.getRunways().size(); ++i) {
        runway* rw = sys.getRunways().at((int)i);
        ASSERT_TRUE(rw->isFree());
    }
}

//TEST_F(SystemTest, Log) {
//    sys.setup("../input.xml");
//    sys.run();
//    sys.log();
//
//    // Test to check if the output file is correct.
//    ASSERT_TRUE(FileCompare);
//}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}