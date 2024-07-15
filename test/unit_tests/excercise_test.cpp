#include "excercise.h"

#include <gtest/gtest.h>

class ExcerciseTest : public testing::Test {
   protected:
    void SetUp() override {
        e2.setName("Bulgarian Split Squat");
        e3.setDescription("");
    }

   public:
    Lab::Excercise e1{
        "Deadlift", "Lift Barbell off the floor", "Legs", {"Glutes", "Hamstring", "Lower-back"}, {"weight", "reps"}};
    Lab::Excercise e2{"Barbell Squat",
                      "With the bar resting high on the shoulders squat until your "
                      "thigh are perpendicular with the floor",
                      "Legs",
                      {"Glutes", "Quads"},
                      {"weight", "reps"}};
    Lab::Excercise e3{
        "Row", "Rowg in a row machine", "Cardio", {"Glutes", "Quads", "Lower-back", "Upper-back"}, {"Time"}};
    Lab::Excercise e4{
        "Bench Press", "Flat back bench press with a barbell", "Chest", {"Pectorals", "Triceps"}, {"Distance"}};
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
    std::vector<std::string> vec1{"Bicep", "Upper-back"};
    std::vector<std::string> vec2{"Tricep"};

    EXPECT_TRUE(test.isMuscle(vec1));
    EXPECT_TRUE(test.isMuscle(vec2));
}

TEST(TypeCheckingGroup, WrongMWVariableTest) {
    Lab::Excercise test;
    std::vector<std::string> vec1{"Latimus Bordi"};
    std::vector<std::string> vec2{"Quads", "Hamstring", "Blute"};
    std::vector<std::string> vec3;

    EXPECT_FALSE(test.isMuscle(vec1));
    EXPECT_FALSE(test.isMuscle(vec2));
    EXPECT_FALSE(test.isMuscle(vec3));
}

TEST(TypeCheckingGroup, CorrectTVariableTest) {
    Lab::Excercise test;
    std::vector<std::string> vec1{"weight", "reps"};
    std::vector<std::string> vec2{"distance", "time"};
    std::vector<std::string> vec3{"reps"};

    EXPECT_TRUE(test.isType(vec1));
    EXPECT_TRUE(test.isType(vec2));
    EXPECT_TRUE(test.isType(vec3));
}

TEST(TypeCheckingGroup, WrongTVariableTest) {
    Lab::Excercise test;
    std::vector<std::string> vec1{"weight", "rpm"};
    std::vector<std::string> vec2{"weight", "reps", "drinks"};
    std::vector<std::string> vec3{"wight"};
    std::vector<std::string> vec4;

    EXPECT_FALSE(test.isType(vec1));
    EXPECT_FALSE(test.isType(vec2));
    EXPECT_FALSE(test.isType(vec3));
    EXPECT_FALSE(test.isType(vec4));
}

TEST(OperatorOverrideGroup, EqualityTest) {
    Lab::Excercise ex1{
        "Bench Press", "Flat back bench press with a barbell", "Chest", {"Pectorals", "Triceps"}, {"Distance"}};
    Lab::Excercise ex2{
        "Bench Press", "Flat back bench press with a barbell", "Chest", {"Pectorals", "Triceps"}, {"Distance"}};
    Lab::Excercise ex3{
        "Row", "Rowg in a row machine", "Cardio", {"Glutes", "Quads", "Lower-back", "Upper-back"}, {"Time"}};

    EXPECT_TRUE(ex1 == ex2);
    EXPECT_FALSE(ex1 == ex3);
}

TEST(OperatorOverrideGroup, ConstructorsTest) {
    Lab::Excercise ex1{
        "Bench Press", "Flat back bench press with a barbell", "Chest", {"Pectorals", "Triceps"}, {"Distance"}};
    Lab::Excercise ex2 = {
        "Bench Press", "Flat back bench press with a barbell", "Chest", {"Pectorals", "Triceps"}, {"Distance"}};
    Lab::Excercise ex3 = Lab::Excercise("Bench Press", "Flat back bench press with a barbell", "Chest",
                                        {"Pectorals", "Triceps"}, {"Distance"});
    Lab::Excercise ex4("Bench Press", "Flat back bench press with a barbell", "Chest", {"Pectorals", "Triceps"},
                       {"Distance"});

    EXPECT_TRUE(ex1 == ex2);
    EXPECT_TRUE(ex1 == ex3);
    EXPECT_TRUE(ex1 == ex4);
}

TEST_F(ExcerciseTest, GetValuesTest) {
    EXPECT_EQ(e1.getName(), "Deadlift");
    EXPECT_EQ(e1.getDescription(), "Lift Barbell off the floor");
    EXPECT_EQ(e1.getMuscleGroup(), "Legs");
    std::vector<std::string> tVec = e1.getMusclesWorked();
    auto iter = tVec.cbegin();
    EXPECT_EQ(*iter, "Glutes");
    ++iter;
    EXPECT_EQ(*iter, "Hamstring");
    ++iter;
    EXPECT_EQ(*iter, "Lower-back");
    ++iter;
    EXPECT_EQ(iter, tVec.cend());
    std::vector<std::string> tType{"weight", "reps"};
    EXPECT_EQ(e1.getType(), tType);
}

TEST_F(ExcerciseTest, NameChangedTest) { EXPECT_EQ(e2.getName(), "Bulgarian Split Squat"); }
TEST_F(ExcerciseTest, DescriptionChangedTest) { EXPECT_EQ(e3.getDescription(), ""); }

TEST_F(ExcerciseTest, VectorSizeTest) { EXPECT_EQ(e6.size(), 0); }

TEST_F(ExcerciseTest, VectorEmptyIteratorTest) { EXPECT_EQ(e6.begin(), e6.end()); }

TEST_F(ExcerciseTest, VectorPushBackTest) {
    e6.push_back({"Pushups", "Push up from the ground", "Chest", {"Pectoral", "Tricep"}, {"Reps"}});
    EXPECT_NE(e6.begin(), e6.end());
    EXPECT_EQ((e6.begin())->getName(), "Pushups");
}
