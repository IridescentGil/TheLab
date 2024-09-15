#include "lab.h"

#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <tuple>
#include <vector>

#include "exercise.h"
#include "history.h"
#include "testHelper.h"
#include "workout.h"

class TheLabTest : public testing::Test {
   protected:
    void SetUp() override {
        for (auto &exercise : testEx) {
            exercise.save(testDB.get());
        }

        testWorkout = {testDB.get(),
                       "Pull Day 1",
                       {Lab::ExerciseData(barbellRow, 50, 5), Lab::ExerciseData(dumbbellFlys, 10, 10),
                        Lab::ExerciseData(wideGripPullUps, 10, 0)}};

        testHistory = {testDB.get(),
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

    Lab::Exercise barbellOverheadPress =
        Lab::Exercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest", {"Pectoral", "Tricep"},
                       {"weight", "reps"});
    Lab::Exercise barbellRow = Lab::Exercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                               {"Upper-back", "Lats"}, {"weight", "reps"});
    Lab::Exercise barbellOverheadPressModified =
        Lab::Exercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest", {"Pectoral", "Tricep"},
                       {"time", "reps"});
    Lab::Exercise dumbbellFlys = Lab::Exercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards",
                                                 "Back", {"Upper-back"}, {"weight", "reps"});
    Lab::Exercise wideGripPullUps = Lab::Exercise("Wide Grip Pull Ups",
                                                    "Holding the bar with a wide grip, do a "
                                                    "pullup until your chin is over the bar",
                                                    "Back", {"Upper-back"}, {"reps"});
    Lab::Exercise jumpingJacks = Lab::Exercise(
        "Jumping Jacks", "Jump and spread your legs and put your arms to the sky", "Cardio", {""}, {"reps"});
    Lab::Exercise plank = Lab::Exercise("Plank", "In pushup position, lift yourself on your elbows and toes", "Core",
                                          {"Abs"}, {"time", "reps"});
    Lab::Exercise running =
        Lab::Exercise("Running (Treadmill)", "Running on a treadmill", "Cardio", {}, {"distance", "time"});
    Lab::Exercise barbellOverheadPressModifiedTwo =
        Lab::Exercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest", {"Pectoral", "Tricep"},
                       {"weight", "time"});
    Lab::Exercise calfPress = Lab::Exercise("Calf Press", "Lift yourself on your tiptoes with your calf", "Legs",
                                              {"Calf"}, {"weight", "reps"});

    const unsigned short AGE = 45;
    const unsigned short LATS_CONDITION = 23;

    std::unique_ptr<Lab::DBConn> testDB = std::make_unique<Lab::DBConn>("read.db");

    Lab::Body testBody{testDB.get()};
    std::vector<Lab::Exercise> testEx = {barbellOverheadPress, barbellRow, calfPress, dumbbellFlys,
                                          jumpingJacks,         plank,      running,   wideGripPullUps};
    Lab::Workout testWorkout{testDB.get()};
    Lab::History testHistory;
};

TEST_F(TheLabTest, InitTest) {
    Lab::TheLab initLab("init.db", ".");

    EXPECT_TRUE(std::filesystem::exists("./init.db"));
    std::filesystem::remove("./init.db");
}

TEST_F(TheLabTest, ReadFromDatabaseTest) {
    Lab::TheLab testLab("read.db", ".");

    auto readExercise = testLab.getExercises();
    EXPECT_EQ(testEx.size(), readExercise.size());
    for (auto testExerciseIter = testEx.cbegin(), readExerciseIter = readExercise.cbegin();
         readExerciseIter != readExercise.cend(); ++testExerciseIter, ++readExerciseIter) {
        EXPECT_PRED_FORMAT2(AssertExerciseEqual, *testExerciseIter, *readExerciseIter);
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
        Lab::Workout(testDB.get(), "Bar Day", {{barbellRow, 40, 6}, {barbellOverheadPress, 70, 2}}),
        Lab::Workout(testDB.get(), "CrossFit", std::vector<Lab::ExerciseData>{{running, 3600, 0}, {plank, 60, 0}})};

    {
        Lab::TheLab testLab("read.db", ".");

        testLab.setWorkouts(newWorkouts);
        EXPECT_TRUE(testLab.saveWorkouts());
    }

    Lab::TheLab testLab("read.db", ".");
    auto readWorkout = testLab.getWorkouts();
    for (auto readWorkoutIter = readWorkout.cbegin(), newWorkoutIter = newWorkouts.cbegin();
         readWorkoutIter != readWorkout.cend(); ++readWorkoutIter, ++newWorkoutIter) {
        auto readExerciseData = readWorkoutIter->getWorkout();
        auto newExerciseData = newWorkoutIter->getWorkout();
        EXPECT_EQ(readWorkoutIter->getName(), newWorkoutIter->getName());
        for (auto readExerciseDataIter = readExerciseData.cbegin(), newExerciseDataIter = newExerciseData.cbegin();
             readExerciseDataIter != readExerciseData.cend(); ++readExerciseDataIter, ++newExerciseDataIter) {
            EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *readExerciseDataIter, *newExerciseDataIter);
        }
    }
}

TEST_F(TheLabTest, ExercisesSaveRemovedNonExistentExercisesTest) {
    std::vector<Lab::Exercise> newExercises = {calfPress, jumpingJacks, plank, running};

    {
        Lab::TheLab testLab("read.db", ".");
        testLab.setExercises(newExercises);
        EXPECT_TRUE(testLab.saveExercises());
    }

    Lab::TheLab testLab("read.db", ".");
    auto exercises = testLab.getExercises();
    EXPECT_EQ(exercises, newExercises);
}

TEST_F(TheLabTest, RemoveExerciseRemovesExerciseFromDatabaseTest) {
    std::vector<Lab::Exercise> compareExercises = {barbellOverheadPress, barbellRow, calfPress,      dumbbellFlys,
                                                     jumpingJacks,         running,    wideGripPullUps};
    {
        Lab::TheLab testLab("read.db", ".");
        auto &exercises = testLab.getExercises();
        auto exerciseIter = exercises.begin() + 5;
        testLab.removeExercise(exerciseIter);
    }

    {
        Lab::TheLab testLab("read.db", ".");
        const auto &exercises = testLab.getExercises();
        EXPECT_EQ(exercises, compareExercises);
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

TEST_F(TheLabTest, ExerciseEditPropagatesTest) {
    Lab::Exercise exerciseChanged;

    {
        Lab::TheLab testLab("read.db", ".");
        auto exercises = testLab.getExercises();
        exerciseChanged = exercises.at(3);

        EXPECT_PRED_FORMAT2(AssertExerciseEqual, dumbbellFlys, exerciseChanged);

        exerciseChanged.setName("Reverse dumbbell flys");
        exerciseChanged.setMusclesWorked({"Upper-back", "Lats"});
    }

    {
        Lab::TheLab testLab("read.db", ".");

        auto &exercises = testLab.getExercises();
        testLab.EditExercise(exercises.begin() + 3, exerciseChanged);

        const Lab::History historyTest = {
            testDB.get(),
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
                std::make_tuple(tpJan13, "Full-Body Workout", exerciseChanged, 20, 12),
                std::make_tuple(tpJan13, "Full-Body Workout", exerciseChanged, 20, 12),
                std::make_tuple(tpJan13, "Full-Body Workout", exerciseChanged, 20, 12),
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
        const auto &exerciseFromWorkout = workoutData.at(1);

        EXPECT_PRED_FORMAT2(AssertExerciseEqual, exerciseChanged, exerciseFromWorkout.exc);
    }

    {
        Lab::TheLab testLab("read.db", ".");

        const Lab::History historyTest = {
            testDB.get(),
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
                std::make_tuple(tpJan13, "Full-Body Workout", exerciseChanged, 20, 12),
                std::make_tuple(tpJan13, "Full-Body Workout", exerciseChanged, 20, 12),
                std::make_tuple(tpJan13, "Full-Body Workout", exerciseChanged, 20, 12),
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
        const auto &exerciseFromWorkout = workoutData.at(1);

        EXPECT_PRED_FORMAT2(AssertExerciseEqual, exerciseChanged, exerciseFromWorkout.exc);
    }
}

TEST_F(TheLabTest, ExerciseRemovalPropagatesTest) {
    const int HISTORY_SIZE = 37;
    const int NUMBER_OF_BARBELLROW_IN_HISTORY = 9;
    const int HISTORY_WITHOUT_BARBELLROW_SIZE = HISTORY_SIZE - NUMBER_OF_BARBELLROW_IN_HISTORY;
    const int WORKOUTS_SIZE = 3;
    const int WORKOUTS_WITHOUT_BARBELLROW_SIZE = 2;
    Lab::historyVector historyCompare;
    std::vector<Lab::ExerciseData> workoutComparison;
    std::string workoutComparisonName;

    {
        auto compareDB = std::make_unique<Lab::DBConn>("read.db");
        const Lab::History historyTest = {
            compareDB.get(),
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
            compareDB.get(),
            "Pull Day 1",
            {Lab::ExerciseData(dumbbellFlys, 10, 10), Lab::ExerciseData(wideGripPullUps, 10, 0)}};

        historyCompare = historyTest.getHistory();
        workoutComparison = workoutTest.getWorkout();
        workoutComparisonName = workoutTest.getName();
    }
    {
        Lab::TheLab testLab("read.db", ".");
        auto &history = testLab.getHistory();
        auto &workouts = testLab.getWorkouts();
        auto &exercises = testLab.getExercises();
        auto exerciseIter = exercises.begin();
        for (; exerciseIter != exercises.end(); ++exerciseIter) {
            if (exerciseIter->getName() == "Barbell Row") {
                break;
            }
        }

        EXPECT_EQ(HISTORY_SIZE, history.size());
        EXPECT_EQ(WORKOUTS_SIZE, workouts.at(0).getWorkout().size());

        testLab.removeExercise(exerciseIter);
        const auto &workoutObject = workouts.at(0);
        const auto &workoutVector = workoutObject.getWorkout();

        EXPECT_EQ(HISTORY_WITHOUT_BARBELLROW_SIZE, history.size());
        EXPECT_EQ(WORKOUTS_WITHOUT_BARBELLROW_SIZE, workouts.at(0).getWorkout().size());

        EXPECT_PRED_FORMAT2(AssertHistoryEqual, historyCompare, history);

        EXPECT_EQ(workoutComparisonName, workoutObject.getName());
        for (auto workoutComparisonIter = workoutComparison.cbegin(), workoutIter = workoutVector.cbegin();
             workoutComparisonIter != workoutComparison.cend(); ++workoutComparisonIter, ++workoutIter) {
            EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *workoutComparisonIter, *workoutIter);
        }
    }

    {
        Lab::TheLab testLab("read.db", ".");

        auto &workouts = testLab.getWorkouts();
        auto &history = testLab.getHistory();

        const auto &workoutObject = workouts.at(0);
        const auto &workoutVector = workoutObject.getWorkout();

        EXPECT_EQ(HISTORY_WITHOUT_BARBELLROW_SIZE, history.size());
        EXPECT_EQ(WORKOUTS_WITHOUT_BARBELLROW_SIZE, workouts.at(0).getWorkout().size());

        EXPECT_PRED_FORMAT2(AssertHistoryEqual, historyCompare, history);

        EXPECT_EQ(workoutComparisonName, workoutObject.getName());
        EXPECT_EQ(workoutComparison.size(), workoutObject.getWorkout().size());
        for (auto workoutComparisonIter = workoutComparison.cbegin(), workoutIter = workoutVector.cbegin();
             workoutComparisonIter != workoutComparison.cend(); ++workoutComparisonIter, ++workoutIter) {
            EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *workoutComparisonIter, *workoutIter);
        }
    }
}

TEST_F(TheLabTest, ExerciseTypeChangeTest) {
    const int HISTORY_SIZE = 37;
    const int NUMBER_OF_OVERHEADPRESS_IN_HISTORY = 15;
    const int HISTORY_WITHOUT_OVERHEADPRESS_SIZE = HISTORY_SIZE - NUMBER_OF_OVERHEADPRESS_IN_HISTORY;
    Lab::historyVector historyCompare;

    {
        auto compareDB = std::make_unique<Lab::DBConn>("read.db");
        const Lab::History historyTest = {compareDB.get(),
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

        historyCompare = historyTest.getHistory();
    }

    {
        Lab::TheLab testLab("read.db", ".");
        auto &exercises = testLab.getExercises();
        auto &history = testLab.getHistory();
        size_t historySize = history.size();
        /* exercise at 0 expected to be barbell Overhead Press */
        auto exercisetoChange = exercises.at(0);
        auto exercisesIter = exercises.begin();

        EXPECT_EQ(HISTORY_SIZE, historySize);
        exercisetoChange.setType({"reps"});
        testLab.EditExercise(exercisesIter, exercisetoChange);
        EXPECT_EQ(HISTORY_WITHOUT_OVERHEADPRESS_SIZE, history.size());
        EXPECT_PRED_FORMAT2(AssertHistoryEqual, historyCompare, history);
    }

    {
        Lab::TheLab testLab("read.db", ".");
        auto &history = testLab.getHistory();

        EXPECT_EQ(HISTORY_WITHOUT_OVERHEADPRESS_SIZE, history.size());
        EXPECT_PRED_FORMAT2(AssertHistoryEqual, historyCompare, history);
    }
}
