#include "body.h"

#include <gtest/gtest.h>

#include "database.h"
#include "testHelper.h"

class BodyTest : public testing::Test {
   protected:
    void SetUp() override {}
    void TearDown() override { remove("thelab.db"); }

    std::shared_ptr<Lab::DBConn> db = std::make_shared<Lab::DBConn>();
    Lab::Body b1{db};
};

TEST_F(BodyTest, EmptyDBTest) {
    Lab::Measurements tMes;
    Lab::Muscles tMus;

    EXPECT_EQ(b1.getAge(), 0);
    EXPECT_EQ(b1.getHeight(), 0);
    EXPECT_EQ(b1.getWeight(), 0);
    EXPECT_PRED_FORMAT2(AssertConditionsEqual, tMus, b1.getCondition());
    EXPECT_PRED_FORMAT2(AssertMeasureEqual, tMes, b1.getMeasurements());
}

TEST_F(BodyTest, SetDBTest) {
    Lab::Measurements tMes = b1.getMeasurements();
    Lab::Muscles tMus = b1.getCondition();

    tMus.calf = 80;
    tMus.lowerBack = 20;
    tMus.forearm = 95;

    tMes.chest = 50;
    tMes.thighLeft = 30;
    tMes.thighRight = 30;

    b1.setHeight(175);
    b1.setWeight(80);
    b1.setAge(18);
    b1.setCondition(tMus);
    b1.setMeasurement(tMes);

    EXPECT_EQ(b1.save(), 1);
    Lab::Body b2{db};
    EXPECT_EQ(175, b2.getHeight());
    EXPECT_EQ(80, b2.getWeight());
    EXPECT_EQ(18, b2.getAge());
    EXPECT_PRED_FORMAT2(AssertConditionsEqual, tMus, b2.getCondition());
    EXPECT_PRED_FORMAT2(AssertMeasureEqual, tMes, b2.getMeasurements());
}
