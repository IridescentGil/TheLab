#include "excercise.h"

#include <gtest/gtest.h>

#include <memory>

class ExcerciseTest : public testing::Test {
   protected:
    void SetUp() override {
        e2.setName("Bulgarian Split Squat");
        e3.setDescription("");
    }

    Lab::Excercise e1{"Deadlift",
                      "Lift Barbell off the floor",
                      "Legs",
                      {"Glutes", "Hamstring", "Lower-back"},
                      {"weight", "reps"}};
    Lab::Excercise e2{
        "Barbell Squat",
        "With the bar resting high on the shoulders squat until your "
        "thigh are perpendicular with the floor",
        "Legs",
        {"Glutes", "Quads"},
        {"weight", "reps"}};
    Lab::Excercise e3{"Row",
                      "Rowg in a row machine",
                      "Cardio",
                      {"Glutes", "Quads", "Lower-back", "Upper-back"},
                      {"Time"}};
    Lab::Excercise e4{"Bench Press",
                      "Flat back bench press with a barbell",
                      "Chest",
                      {"Pectorals", "Triceps"},
                      {"Distance"}};
    std::unique_ptr<Lab::Excercise[]> e5 =
        std::make_unique<Lab::Excercise[]>(10);
    std::vector<Lab::Excercise> e6;
};

TEST(TypeCheckingGroup, CorrectMGVariableTest) {
    Lab::Excercise test;
    EXPECT_TRUE(test.isMuscleGroup("Chest"));
    EXPECT_TRUE(test.isMuscleGroup("Back"));
}

TEST(TypeCheckingGroup, WrongMGVariableTest) {
    Lab::Excercise test;
    EXPECT_FALSE(test.isMuscleGroup("Finger"));
    EXPECT_FALSE(test.isMuscleGroup("Butt"));
    EXPECT_FALSE(test.isMuscleGroup("arms"));
}

TEST(TypeCheckingGroup, CorrectMWVariableTest) {
    Lab::Excercise test;
    std::vector<std::string> v1{"Bicep", "Upper-back"};
    std::vector<std::string> v2{"Tricep"};

    EXPECT_TRUE(test.isMuscle(v1));
    EXPECT_TRUE(test.isMuscle(v2));
}

TEST(TypeCheckingGroup, WrongMWVariableTest) {
    Lab::Excercise test;
    std::vector<std::string> v1{"Latimus Bordi"};
    std::vector<std::string> v2{"Quads", "Hamstring", "Blute"};
    std::vector<std::string> v3;

    EXPECT_FALSE(test.isMuscle(v1));
    EXPECT_FALSE(test.isMuscle(v2));
    EXPECT_FALSE(test.isMuscle(v3));
}

TEST(TypeCheckingGroup, CorrectTVariableTest) {
    Lab::Excercise test;
    std::vector<std::string> v1{"weight", "reps"};
    std::vector<std::string> v2{"distance", "time"};
    std::vector<std::string> v3{"reps"};

    EXPECT_TRUE(test.isType(v1));
    EXPECT_TRUE(test.isType(v2));
    EXPECT_TRUE(test.isType(v3));
}

TEST(TypeCheckingGroup, WrongTVariableTest) {
    Lab::Excercise test;
    std::vector<std::string> v1{"weight", "rpm"};
    std::vector<std::string> v2{"weight", "reps", "drinks"};
    std::vector<std::string> v3{"wight"};
    std::vector<std::string> v4;

    EXPECT_FALSE(test.isType(v1));
    EXPECT_FALSE(test.isType(v2));
    EXPECT_FALSE(test.isType(v3));
    EXPECT_FALSE(test.isType(v4));
}

TEST_F(ExcerciseTest, GetValuesTest) {
    EXPECT_EQ(e1.getName(), "Deadlift");
    EXPECT_EQ(e1.getDescription(), "Lift Barbell off the floor");
    EXPECT_EQ(e1.getMuscleGroup(), "Legs");
    std::vector<std::string> tVec = e1.getMusclesWorked();
    auto it = tVec.cbegin();
    EXPECT_EQ(*it, "Glutes");
    it++;
    EXPECT_EQ(*it, "Hamstring");
    it++;
    EXPECT_EQ(*it, "Lower-back");
    it++;
    EXPECT_EQ(it, tVec.cend());
    std::vector<std::string> tType{"weight", "reps"};
    EXPECT_EQ(e1.getType(), tType);
}

TEST_F(ExcerciseTest, NameChangedTest) {
    EXPECT_EQ(e2.getName(), "Bulgarian Split Squat");
}
TEST_F(ExcerciseTest, DescriptionChangedTest) {
    EXPECT_EQ(e3.getDescription(), "");
}

TEST_F(ExcerciseTest, ArrayValuesTest) { ASSERT_NE(e5, nullptr); }

TEST_F(ExcerciseTest, ArrayEmptyIndexValuesTest) {
    for (int x = 0; x < 10; x++) {
        ASSERT_EQ(e5[x].getName(), "");
        ASSERT_EQ(e5[x].getDescription(), "");
        ASSERT_EQ(e5[x].getMuscleGroup(), "");
        std::vector<std::string> empty;
        ASSERT_EQ(e5[x].getMusclesWorked(), empty);
        ASSERT_EQ(e5[x].getType(), empty);
    }
}

TEST_F(ExcerciseTest, ArraySetValueTest) {
    e5[6].setName("Deadbug");
    ASSERT_EQ(e5[6].getName(), "Deadbug");
}

TEST_F(ExcerciseTest, VectorSizeTest) { EXPECT_EQ(e6.size(), 0); }

TEST_F(ExcerciseTest, VectorEmptyIteratorTest) {
    EXPECT_EQ(e6.begin(), e6.end());
}

TEST_F(ExcerciseTest, VectorPushBackTest) {
    e6.push_back({"Pushups",
                  "Push up from the ground",
                  "Chest",
                  {"Pectoral", "Tricep"},
                  {"Reps"}});
    EXPECT_NE(e6.begin(), e6.end());
    EXPECT_EQ((e6.begin())->getName(), "Pushups");
}
