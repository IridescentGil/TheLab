#include "body.h"

#include <gtest/gtest.h>

#include <memory>

#include "database.h"
#include "gtest/gtest.h"

class BodyTest : public testing::Test {
   protected:
    void SetUp() override {}
    void TearDown() override { remove("thelab.db"); }

    std::shared_ptr<DBConn> db = std::make_shared<DBConn>();
    Body b1{db};
};

auto mesEqual = [](Measurements a, Measurements b) -> bool {
    if (a.hips != b.hips)
        return false;
    else if (a.neck != b.neck)
        return false;
    else if (a.upperArmRight != b.upperArmRight)
        return false;
    else if (a.forearmRight != b.forearmRight)
        return false;
    else if (a.upperArmLeft != b.upperArmLeft)
        return false;
    else if (a.forearmLeft != b.forearmLeft)
        return false;
    else if (a.thighRight != b.thighRight)
        return false;
    else if (a.shoulders != b.shoulders)
        return false;
    else if (a.thighLeft != b.thighLeft)
        return false;
    else if (a.calfRight != b.calfRight)
        return false;
    else if (a.calfLeft != b.calfLeft)
        return false;
    else if (a.waist != b.waist)
        return false;
    else if (a.chest != b.chest)
        return false;
    return true;
};
auto musEqual = [](Muscles a, Muscles b) -> bool {
    if (a.upperBack != b.upperBack)
        return false;
    else if (a.trapezius != b.trapezius)
        return false;
    else if (a.lowerBack != b.lowerBack)
        return false;
    else if (a.hamstring != b.hamstring)
        return false;
    else if (a.pectoral != b.pectoral)
        return false;
    else if (a.forearm != b.forearm)
        return false;
    else if (a.tricep != b.tricep)
        return false;
    else if (a.glutes != b.glutes)
        return false;
    else if (a.quads != b.quads)
        return false;
    else if (a.bicep != b.bicep)
        return false;
    else if (a.lats != b.lats)
        return false;
    else if (a.calf != b.calf)
        return false;
    else if (a.abs != b.abs)
        return false;
    else if (a.neck != b.neck)
        return false;
    return true;
};

testing::AssertionResult AssertConditionsEqual(const char* m_expr,
                                               const char* n_expr, Muscles m,
                                               Muscles n) {
    if (musEqual(m, n)) return testing::AssertionSuccess();

    return testing::AssertionFailure()
           << m_expr << "and" << n_expr << " Conditions not equal";
};

testing::AssertionResult AssertMeasureEqual(const char* m_expr,
                                            const char* n_expr, Measurements m,
                                            Measurements n) {
    if (mesEqual(m, n)) return testing::AssertionSuccess();

    return testing::AssertionFailure()
           << m_expr << "and" << n_expr << " Measurements not equal";
};

TEST_F(BodyTest, EmptyDBTest) {
    Measurements tMes;
    Muscles tMus;

    EXPECT_EQ(b1.getAge(), 0);
    EXPECT_EQ(b1.getHeight(), 0);
    EXPECT_EQ(b1.getWeight(), 0);
    EXPECT_PRED_FORMAT2(AssertConditionsEqual, tMus, b1.getCondition());
    EXPECT_PRED_FORMAT2(AssertMeasureEqual, tMes, b1.getMeasurements());
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
    EXPECT_PRED_FORMAT2(AssertConditionsEqual, tMus, b2.getCondition());
    EXPECT_PRED_FORMAT2(AssertMeasureEqual, tMes, b2.getMeasurements());
}
