#include "lab.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <vector>

#include "excercise.h"
#include "testHelper.h"
#include "workout.h"

class TheLabTest : public testing::Test {
   protected:
    void SetUp() override {
        for (auto &excercise : testEx) {
            excercise.save(testDB);
        }

        testWorkout = {testDB,
                       "Pull Day 1",
                       {Lab::ExcerciseData(barbellRow, 50, 5), Lab::ExcerciseData(dumbbellFlys, 10, 10),
                        Lab::ExcerciseData(wideGripPullUps, 10, 0)}};

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
    }
    void TearDown() override { std::remove("read.db"); }

   public:
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

    Lab::Body testBody{testDB};
    std::vector<Lab::Excercise> testEx = {barbellOverheadPress, barbellRow, calfPress, dumbbellFlys,
                                          jumpingJacks,         plank,      running,   wideGripPullUps};
    Lab::Workout testWorkout{testDB};
    Lab::History testHistory;
};

TEST_F(TheLabTest, InitTest) {
    Lab::TheLab initLab("init.db", ".");

    EXPECT_TRUE(std::filesystem::exists("./init.db"));
    std::filesystem::remove("./init.db");
}

TEST_F(TheLabTest, ReadFromDatabaseTest) {
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
}

TEST_F(TheLabTest, WorkoutsSaveRemovesNonexistentWorkoutsTest) {
    std::vector<Lab::Workout> newWorkouts = {
        Lab::Workout(testDB, "Bar Day", {{barbellRow, 40, 6}, {barbellOverheadPress, 70, 2}}),
        Lab::Workout(testDB, "CrossFit", std::vector<Lab::ExcerciseData>{{running, 3600, 0}, {plank, 60, 0}})};

    {
        Lab::TheLab testLab("read.db", ".");

        testLab.setWorkouts(newWorkouts);
        EXPECT_TRUE(testLab.saveWorkouts());
    }

    Lab::TheLab testLab("read.db", ".");
    auto readWorkout = testLab.getWorkouts();
    for (auto readWorkoutIter = readWorkout.cbegin(), newWorkoutIter = newWorkouts.cbegin();
         readWorkoutIter != readWorkout.cend(); ++readWorkoutIter, ++newWorkoutIter) {
        auto readExcerciseData = readWorkoutIter->getWorkout();
        auto newExcerciseData = newWorkoutIter->getWorkout();
        EXPECT_EQ(readWorkoutIter->getName(), newWorkoutIter->getName());
        for (auto readExcerciseDataIter = readExcerciseData.cbegin(), newExcerciseDataIter = newExcerciseData.cbegin();
             readExcerciseDataIter != readExcerciseData.cend(); ++readExcerciseDataIter, ++newExcerciseDataIter) {
            EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *readExcerciseDataIter, *newExcerciseDataIter);
        }
    }
}

TEST_F(TheLabTest, ExcercisesSaveRemovedNonExistentExcercisesTest) {
    std::vector<Lab::Excercise> newExcercises = {calfPress, jumpingJacks, plank, running};

    {
        Lab::TheLab testLab("read.db", ".");
        testLab.setExcercises(newExcercises);
        EXPECT_TRUE(testLab.saveExcercises());
    }

    Lab::TheLab testLab("read.db", ".");
    auto excercises = testLab.getExcercises();
    EXPECT_EQ(excercises, newExcercises);
}

TEST_F(TheLabTest, RemoveExcerciseRemovesExcerciseFromDatabaseTest) {
    std::vector<Lab::Excercise> compareExcercises = {barbellOverheadPress, barbellRow, calfPress,      dumbbellFlys,
                                                     jumpingJacks,         running,    wideGripPullUps};
    {
        Lab::TheLab testLab("read.db", ".");
        auto &excercises = testLab.getExcercises();
        auto excerciseIter = excercises.begin() + 5;
        testLab.removeExcercise(excerciseIter);
    }

    {
        Lab::TheLab testLab("read.db", ".");
        const auto &excercises = testLab.getExcercises();
        EXPECT_EQ(excercises, compareExcercises);
    }
}
TEST_F(TheLabTest, RemoveWorkoutRemovesWorkoutFromDatabaseTest) {
    {
        Lab::TheLab testLab("read.db", ".");
        auto &workouts = testLab.getWorkouts();
        testLab.removeWorkout(workouts.begin());
    }

    {
        Lab::TheLab testLab("read.db", ".");
        auto &workouts = testLab.getWorkouts();
        EXPECT_TRUE(workouts.empty());
    }
}

TEST_F(TheLabTest, ExcerciseEditPropagatesTest) {
    Lab::TheLab testLab("read.db", ".");

    auto &excercises = testLab.getExcercises();
    auto excerciseChanged = excercises.at(3);

    EXPECT_PRED_FORMAT2(AssertExcerciseEqual, dumbbellFlys, excerciseChanged);

    excerciseChanged.setName("Reverse dumbbell flys");
    excerciseChanged.setMusclesWorked({"Upper-back", "Lats"});

    testLab.EditExcercise(excercises.begin() + 3, excerciseChanged);

    const Lab::History historyTest = {testDB,
                                      {
                                          std::make_tuple(tpMarch4, "Upper-Body Workout", barbellOverheadPress, 60, 10),
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
                                          std::make_tuple(tpJan13, "Full-Body Workout", excerciseChanged, 20, 12),
                                          std::make_tuple(tpJan13, "Full-Body Workout", excerciseChanged, 20, 12),
                                          std::make_tuple(tpJan13, "Full-Body Workout", excerciseChanged, 20, 12),
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
                                      }};

    const auto &historyCompare = historyTest.getHistory();
    const auto &history = testLab.getHistory();
    EXPECT_EQ(historyCompare.size(), history.size());
    EXPECT_PRED_FORMAT2(AssertHistoryEqual, historyCompare, history);

    const auto &workouts = testLab.getWorkouts();
    const auto &workoutObject = workouts.at(0);
    const auto &workoutData = workoutObject.getWorkout();
    const auto &excerciseFromWorkout = workoutData.at(1);

    EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excerciseChanged, excerciseFromWorkout.exc);
}

TEST_F(TheLabTest, ExcerciseRemovalPropagatesTest) {
    const int HISTORY_SIZE = 37;
    const int NUMBER_OF_BARBELLROW_IN_HISTORY = 9;
    const int HISTORY_WITHOUT_BARBELLROW_SIZE = HISTORY_SIZE - NUMBER_OF_BARBELLROW_IN_HISTORY;
    const int WORKOUTS_SIZE = 3;
    const int WORKOUTS_WITHOUT_BARBELLROW_SIZE = 2;

    Lab::TheLab testLab("read.db", ".");

    const Lab::History historyTest = {testDB,
                                      {
                                          std::make_tuple(tpMarch4, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                                          std::make_tuple(tpMarch4, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                                          std::make_tuple(tpMarch4, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                                          std::make_tuple(tpFeb10, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                                          std::make_tuple(tpFeb10, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                                          std::make_tuple(tpFeb10, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                                          std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                                          std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                                          std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 60, 10),
                                          std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 80, 4),
                                          std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 80, 4),
                                          std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 80, 4),
                                          std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 40, 15),
                                          std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 40, 15),
                                          std::make_tuple(tpFeb20, "Upper-Body Workout", barbellOverheadPress, 40, 15),
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
                                      }};

    const Lab::Workout workoutTest{
        testDB, "Pull Day 1", {Lab::ExcerciseData(dumbbellFlys, 10, 10), Lab::ExcerciseData(wideGripPullUps, 10, 0)}};

    auto &history = testLab.getHistory();
    auto &workouts = testLab.getWorkouts();
    auto &excercises = testLab.getExcercises();
    auto excerciseIter = excercises.begin();
    for (; excerciseIter != excercises.end(); ++excerciseIter) {
        if (excerciseIter->getName() == "Barbell Row") {
            break;
        }
    }

    const auto &workoutComparison = workoutTest.getWorkout();

    EXPECT_EQ(HISTORY_SIZE, history.size());
    EXPECT_EQ(WORKOUTS_SIZE, workouts.at(0).getWorkout().size());

    testLab.removeExcercise(excerciseIter);
    const auto &workoutObject = workouts.at(0);
    const auto &workoutVector = workoutObject.getWorkout();

    EXPECT_EQ(HISTORY_WITHOUT_BARBELLROW_SIZE, history.size());
    EXPECT_EQ(WORKOUTS_WITHOUT_BARBELLROW_SIZE, workouts.at(0).getWorkout().size());

    EXPECT_PRED_FORMAT2(AssertHistoryEqual, historyTest.getHistory(), history);

    EXPECT_EQ(workoutTest.getName(), workoutObject.getName());
    for (auto workoutComparisonIter = workoutComparison.cbegin(), workoutIter = workoutVector.cbegin();
         workoutComparisonIter != workoutComparison.cend(); ++workoutComparisonIter, ++workoutIter) {
        EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *workoutComparisonIter, *workoutIter);
    }
}

TEST_F(TheLabTest, ExcerciseTypeChangeTest) {
    Lab::TheLab testLab("read.db", ".");
    const int HISTORY_SIZE = 37;
    const int NUMBER_OF_OVERHEADPRESS_IN_HISTORY = 15;
    const int HISTORY_WITHOUT_OVERHEADPRESS_SIZE = HISTORY_SIZE - NUMBER_OF_OVERHEADPRESS_IN_HISTORY;
    const Lab::History historyTest = {testDB,
                                      {std::make_tuple(tpMarch4, "Full-Body Workout", barbellRow, 75, 10),
                                       std::make_tuple(tpMarch4, "Full-Body Workout", barbellRow, 75, 10),
                                       std::make_tuple(tpMarch4, "Full-Body Workout", barbellRow, 75, 10),
                                       std::make_tuple(tpFeb20, "Full-Body Workout", barbellRow, 75, 10),
                                       std::make_tuple(tpFeb20, "Full-Body Workout", barbellRow, 75, 10),
                                       std::make_tuple(tpFeb20, "Full-Body Workout", barbellRow, 75, 10),
                                       std::make_tuple(tpJan13, "Full-Body Workout", barbellRow, 60, 10),
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
                                       std::make_tuple(tpJan13, "Run", running, 6, 60)}};

    auto &excercises = testLab.getExcercises();
    auto &history = testLab.getHistory();
    size_t historySize = history.size();
    /* excercise at 0 expected to be barbell Overhead Press */
    auto excercisetoChange = excercises.at(0);
    auto excercisesIter = excercises.begin();

    EXPECT_EQ(HISTORY_SIZE, historySize);
    excercisetoChange.setType({"reps"});
    testLab.EditExcercise(excercisesIter, excercisetoChange);
    EXPECT_EQ(HISTORY_WITHOUT_OVERHEADPRESS_SIZE, history.size());
    EXPECT_PRED_FORMAT2(AssertHistoryEqual, historyTest.getHistory(), history);
}
