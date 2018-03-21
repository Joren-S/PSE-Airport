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
    ASSERT_TRUE(sys.getAirplanes().empty());
    ASSERT_TRUE(sys.getAirports().empty());
    ASSERT_TRUE(sys.getRunways().empty());
}


TEST_F(SystemTest, Setup) {
    sys.setup("../input.xml");
    ASSERT_TRUE(!sys.getAirplanes().empty());
    ASSERT_TRUE(!sys.getAirports().empty());
    ASSERT_TRUE(!sys.getRunways().empty());

    for (size_t i = 0; i < sys.getRunways().size(); ++i) {
        runway* rw = sys.getRunways().at((int)i);
        ASSERT_TRUE(rw->isFree());
    }

    for (size_t i = 0; i < sys.getAirplanes().size(); ++i) {
        airplane* ap = sys.getAirplanes().at((int)i);
        ASSERT_TRUE(ap->getFStatus() != (3  || 1)); // kLanded, kFinished
    }
}

TEST_F(SystemTest, Run) {
    sys.setup("../input.xml");
    sys.run();

    for (size_t i = 0; i < sys.getAirplanes().size(); ++i) {
        airplane* ap = sys.getAirplanes().at((int)i);
        ASSERT_TRUE(ap->getFStatus() == kFinished);
        ASSERT_TRUE(ap->getFGateID() == -1);
    }

    for (size_t i = 0; i < sys.getRunways().size(); ++i) {
        runway* rw = sys.getRunways().at((int)i);
        ASSERT_TRUE(rw->isFree());
    }

    for (size_t i = 0; i < sys.getAirplanes().size(); ++i) {
        airplane* ap = sys.getAirplanes().at((int)i);
        ASSERT_TRUE(ap->getFStatus() == (kFinished)); // kLanded, kFinished
    }
}

//TEST_F(SystemTest, Log) {
//    sys.setup("../input.xml");
//    sys.run();
//    sys.log();
//    ASSERT_TRUE(FileCompare);
//}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}