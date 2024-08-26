#include "body.h"

#include <gtest/gtest.h>

#include "database.h"
#include "testHelper.h"

class BodyTest : public testing::Test {
   protected:
    void SetUp() override {}
    void TearDown() override { remove("thelab.db"); }

   public:
    std::unique_ptr<Lab::DBConn> db = std::make_unique<Lab::DBConn>();
    Lab::Body body1{db.get()};
};

TEST_F(BodyTest, EmptyDBTest) {
    Lab::Measurements tMes;
    Lab::Muscles tMus;

    EXPECT_EQ(body1.getAge(), 0);
    EXPECT_EQ(body1.getHeight(), 0);
    EXPECT_EQ(body1.getWeight(), 0);
    EXPECT_PRED_FORMAT2(AssertConditionsEqual, tMus, body1.getCondition());
    EXPECT_PRED_FORMAT2(AssertMeasureEqual, tMes, body1.getMeasurements());
}

TEST_F(BodyTest, SetDBTest) {
    Lab::Measurements tMes = body1.getMeasurements();
    Lab::Muscles tMus = body1.getCondition();

    tMus.calf = 80;
    tMus.lowerBack = 20;
    tMus.forearm = 95;

    tMes.chest = 50;
    tMes.thighLeft = 30;
    tMes.thighRight = 30;

    body1.setHeight(175);
    body1.setWeight(80);
    body1.setAge(18);
    body1.setCondition(tMus);
    body1.setMeasurement(tMes);

    EXPECT_EQ(body1.save(), 1);
    Lab::Body body2{db.get()};
    EXPECT_EQ(175, body2.getHeight());
    EXPECT_EQ(80, body2.getWeight());
    EXPECT_EQ(18, body2.getAge());
    EXPECT_PRED_FORMAT2(AssertConditionsEqual, tMus, body2.getCondition());
    EXPECT_PRED_FORMAT2(AssertMeasureEqual, tMes, body2.getMeasurements());
}
