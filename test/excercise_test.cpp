#include "excercise/excercise.h"

#include <gtest/gtest.h>

#include <iterator>
#include <memory>

class ExcerciseTest : public testing::Test {
   protected:
    void SetUp() override {
        e2.setName("Bulgarian Split Squat");
        e3.setDescription("");
    }

    Excercise e1{"Deadlift",
                 "Lift Barbell off the floor",
                 "Leg",
                 {"Glute", "Hamstring", "Lower-back"},
                 {"weight", "rep"}};
    Excercise e2{"Barbell Squat",
                 "With the bar resting high on the shoulders squat until your "
                 "thigh are perpendicular with the floor",
                 "Leg",
                 {"Glute", "Quads"},
                 {"weight", "rep"}};
    Excercise e3{"Row",
                 "Rowg in a row machine",
                 "Cardio",
                 {"Glute", "Quad", "Lower-back", "Upper-back"},
                 {"Time"}};
    Excercise e4{"Bench Press",
                 "Flat back bench press with a barbell",
                 "Chest",
                 {"Pectorals", "Tricepts"},
                 {"Distance"}};
    std::unique_ptr<Excercise[]> e5 = std::make_unique<Excercise[]>(10);
    std::vector<Excercise> e6;
};

TEST_F(ExcerciseTest, GetValuesTest) {
    EXPECT_EQ(e1.getName(), "Deadlift");
    EXPECT_EQ(e1.getDescription(), "Lift Barbell off the floor");
    EXPECT_EQ(e1.getMuscleGroup(), "Leg");
    std::vector<std::string> tVec = e1.getMusclesWorked();
    auto it = tVec.begin();
    EXPECT_EQ(*it, "Glute");
    it++;
    EXPECT_EQ(*it, "Hamstring");
    it++;
    EXPECT_EQ(*it, "Lower-back");
    it++;
    EXPECT_EQ(it, e1.getMusclesWorked().end());
    std::vector<std::string> tType{"weight", "reps"};
    EXPECT_EQ(e1.getMusclesWorked(), tType);
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
                  {"Pectorals", "Tricepts"},
                  {"Reps"}});
    EXPECT_NE(e6.begin(), e6.end());
    EXPECT_EQ((e6.begin())->getName(), "Pushup");
}
