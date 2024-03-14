#include "body.h"

#include <gtest/gtest.h>

#include <memory>

#include "database.h"

class BodyTest : public testing::Test {
   protected:
    void SetUp() override {}

    std::shared_ptr<DBConn> db = std::make_shared<DBConn>();
    Body b1{db};
};

TEST_F(BodyTest, EmptyDBTest) {
    Measurements tMes;
    Muscles tMus;

    EXPECT_EQ(b1.getAge(), -1);
    EXPECT_EQ(b1.getHeight(), -1);
    EXPECT_EQ(b1.getWeight(), -1);
    EXPECT_EQ(tMes, b1.getMeasurements());
    EXPECT_EQ(tMus, b1.getCondition());
}

TEST_F(BodyTest, SetDBTest) {
    Measurements tMes = b1.getMeasurements();
    Muscles tMus = b1.getCondition();

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

    EXPECT_TRUE(b1.save());
    Body b2{db};
    EXPECT_EQ(175, b2.getHeight());
    EXPECT_EQ(80, b2.getWeight());
    EXPECT_EQ(18, b2.getAge());
    EXPECT_EQ(tMes, b2.getMeasurements());
    EXPECT_EQ(tMus, b2.getCondition());
}
