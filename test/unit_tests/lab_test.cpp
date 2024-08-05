#include "lab.h"

#include <gtest/gtest.h>

#include <filesystem>

#include "testHelper.h"

TEST(TheLabTest, InitTest) {
    Lab::TheLab initLab("init.db", ".");

    EXPECT_TRUE(std::filesystem::exists("./init.db"));
    std::filesystem::remove("./init.db");
}

TEST(TheLabTest, ReadFromDatabaseTest) {
    constexpr static std::chrono::time_point<std::chrono::system_clock> tpMarch4 =
        std::chrono::sys_days{std::chrono::March / 4 / 2024} + std::chrono::hours(12);

    constexpr static std::chrono::time_point<std::chrono::system_clock> tpJan13 =
        std::chrono::sys_days{std::chrono::January / 13 / 2024} + std::chrono::hours(12);

    constexpr static std::chrono::time_point<std::chrono::system_clock> tpFeb20 =
        std::chrono::sys_days{std::chrono::February / 20 / 2024} + std::chrono::hours(12);

    constexpr static std::chrono::time_point<std::chrono::system_clock> tpFeb10 =
        std::chrono::sys_days{std::chrono::February / 10 / 2024} + std::chrono::hours(12);

    Lab::Excercise barbellOverheadPress =
        Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest", {"Pectoral", "Tricep"},
                       {"weight", "reps"});
    Lab::Excercise barbellRow = Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                               {"Upper-back", "Lats"}, {"weight", "reps"});
    Lab::Excercise barbellOverheadPressModified =
        Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest", {"Pectoral", "Tricep"},
                       {"time", "reps"});
    Lab::Excercise dumbbellFlys = Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards",
                                                 "Back", {"Upper-back"}, {"weight", "reps"});
    Lab::Excercise wideGripPullUps = Lab::Excercise("Wide Grip Pull Ups",
                                                    "Holding the bar with a wide grip, do a "
                                                    "pullup until your chin is over the bar",
                                                    "Back", {"Upper-back"}, {"reps"});
    Lab::Excercise jumpingJacks = Lab::Excercise(
        "Jumping Jacks", "Jump and spread your legs and put your arms to the sky", "Cardio", {""}, {"reps"});
    Lab::Excercise plank = Lab::Excercise("Plank", "In pushup position, lift yourself on your elbows and toes", "Core",
                                          {"Abs"}, {"time", "reps"});
    Lab::Excercise running =
        Lab::Excercise("Running (Treadmill)", "Running on a treadmill", "Cardio", {}, {"distance", "time"});
    Lab::Excercise barbellOverheadPressModifiedTwo =
        Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest", {"Pectoral", "Tricep"},
                       {"weight", "time"});
    Lab::Excercise calfPress = Lab::Excercise("Calf Press", "Lift yourself on your tiptoes with your calf", "Legs",
                                              {"Calf"}, {"weight", "reps"});

    const int AGE = 45;
    const int LATS_CONDITION = 23;

    std::shared_ptr<Lab::DBConn> testDB = std::make_shared<Lab::DBConn>("read.db");
    Lab::Body testBody(testDB);
    std::vector<Lab::Excercise> testEx = {barbellOverheadPress, barbellRow, calfPress, dumbbellFlys,
                                          jumpingJacks,         plank,      running,   wideGripPullUps};
    Lab::Workout testWorkout{testDB,
                             "Pull Day 1",
                             {Lab::ExcerciseData(barbellRow, 50, 5), Lab::ExcerciseData(dumbbellFlys, 10, 10),
                              Lab::ExcerciseData(wideGripPullUps, 10, 0)}};

    Lab::History testHistory;

    for (auto &excercise : testEx) {
        excercise.save(testDB);
    }

    testHistory = {testDB,
                   {std::make_tuple(tpMarch4, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                    std::make_tuple(tpMarch4, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                    std::make_tuple(tpMarch4, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                    std::make_tuple(tpMarch4, "Full-Body Workout", barbellRow, 75, 10),
                    std::make_tuple(tpMarch4, "Full-Body Workout", barbellRow, 75, 10),
                    std::make_tuple(tpMarch4, "Full-Body Workout", barbellRow, 75, 10),
                    std::make_tuple(tpFeb10, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                    std::make_tuple(tpFeb10, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                    std::make_tuple(tpFeb10, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                    std::make_tuple(tpFeb20, "Full-Body Workout", barbellRow, 75, 10),
                    std::make_tuple(tpFeb20, "Full-Body Workout", barbellRow, 75, 10),
                    std::make_tuple(tpMarch4, "Full-Body Workout", barbellOverheadPressModified, 60, 7),
                    std::make_tuple(tpFeb20, "Full-Body Workout", barbellRow, 75, 10),
                    std::make_tuple(tpJan13, "Full-Body Workout", barbellRow, 60, 10),
                    std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                    std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                    std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                    std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 80, 4),
                    std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 80, 4),
                    std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 80, 4),
                    std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 40, 15),
                    std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 40, 15),
                    std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 40, 15),
                    std::make_tuple(tpJan13, "Full-Body Workout", barbellRow, 60, 10),
                    std::make_tuple(tpJan13, "Full-Body Workout", barbellRow, 60, 10),
                    std::make_tuple(tpJan13, "Full-Body Workout", dumbbellFlys, 20, 12),
                    std::make_tuple(tpJan13, "Full-Body Workout", dumbbellFlys, 20, 12),
                    std::make_tuple(tpJan13, "Full-Body Workout", dumbbellFlys, 20, 12),
                    std::make_tuple(tpJan13, "Full-Body Workout", wideGripPullUps, 10, 0),
                    std::make_tuple(tpJan13, "Full-Body Workout", wideGripPullUps, 10, 0),
                    std::make_tuple(tpJan13, "Full-Body Workout", wideGripPullUps, 10, 0),
                    std::make_tuple(tpJan13, "Full-Body Workout", jumpingJacks, 20, 0),
                    std::make_tuple(tpJan13, "Full-Body Workout", jumpingJacks, 20, 0),
                    std::make_tuple(tpJan13, "Full-Body Workout", jumpingJacks, 20, 0),
                    std::make_tuple(tpJan13, "Full-Body Workout", plank, 60, 10),
                    std::make_tuple(tpJan13, "Full-Body Workout", plank, 60, 10),
                    std::make_tuple(tpJan13, "Full-Body Workout", plank, 60, 10),
                    std::make_tuple(tpJan13, "Run", running, 6, 60),
                    std::make_tuple(tpMarch4, "Run", barbellOverheadPressModifiedTwo, 25, 30)}};

    testWorkout.save();
    testBody.setAge(AGE);
    auto testCondition = testBody.getCondition();
    testCondition.lats = LATS_CONDITION;
    testBody.setCondition(testCondition);
    testBody.save();
    testHistory.save();

    Lab::TheLab testLab("read.db", ".");

    auto readExcercise = testLab.getExcercises();
    EXPECT_EQ(testEx.size(), readExcercise.size());
    for (auto testExcerciseIter = testEx.cbegin(), readExcerciseIter = readExcercise.cbegin();
         readExcerciseIter != readExcercise.cend(); ++testExcerciseIter, ++readExcerciseIter) {
        EXPECT_PRED_FORMAT2(AssertExcerciseEqual, *testExcerciseIter, *readExcerciseIter);
    }

    auto readWorkout = testLab.getWorkouts();
    EXPECT_EQ(1, readWorkout.size());
    auto readWorkoutIter = readWorkout.cbegin();
    auto readWorkoutData = readWorkoutIter->getWorkout();
    auto testWorkoutData = testWorkout.getWorkout();

    EXPECT_EQ(testWorkoutData.size(), readWorkoutData.size());
    for (auto testWorkoutDataIter = testWorkoutData.cbegin(), readWorkoutDataIter = readWorkoutData.cbegin();
         readWorkoutDataIter != readWorkoutData.cend(); ++testWorkoutDataIter, ++readWorkoutDataIter) {
        EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *testWorkoutDataIter, *readWorkoutDataIter);
    }
    EXPECT_EQ(testWorkout.getName(), readWorkoutIter->getName());

    auto readBody = testLab.getBody();
    EXPECT_PRED_FORMAT2(AssertMeasureEqual, testBody.getMeasurements(), readBody.getMeasurements());
    EXPECT_PRED_FORMAT2(AssertConditionsEqual, testBody.getCondition(), readBody.getCondition());
    EXPECT_EQ(AGE, readBody.getAge());

    auto &testHistoryVector = testHistory.getHistory();

    auto readHistory = testLab.getHistoryObject();
    EXPECT_EQ(testHistory.getHistory().size(), readHistory.getHistory().size());
    EXPECT_PRED_FORMAT2(AssertHistoryEqual, testHistoryVector, readHistory.getHistory());

    std::remove("read.db");
}
