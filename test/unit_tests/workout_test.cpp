#include "workout.h"

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "database.h"
#include "excercise.h"
#include "testHelper.h"

class WorkoutTest : public testing::Test {
   protected:
    void SetUp() override {
        std::vector<Lab::Excercise> testEx = {
            Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back", {"Upper-back", "Lats"},
                           {"weight", "reps"}),
            Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards", "Back", {"Upper-back"},
                           {"weight", "reps"}),
            Lab::Excercise("Wide Grip Pull Ups",
                           "Holding the bar with a wide grip, do a "
                           "pullup until your chin is over the bar",
                           "Back", {"Upper-back"}, {"reps"}),
            Lab::Excercise("Jumping Jacks", "Jump and spread your legs and put your arms to the sky", "Cardio", {""},
                           {"reps"}),
            Lab::Excercise("Plank", "In pushup position, lift yourself on your elbows and toes", "Core", {"Abs"},
                           {"time", "reps"}),
            Lab::Excercise("Calf Press", "Lift yourself on your tiptoes with your calf", "Legs", {"Calf"},
                           {"weight", "reps"}),
            Lab::Excercise("Kettlebell Swing",
                           "With the kettlebell on the ground hinge at the hips and hike the "
                           "kettlebell between your legs, push the kettlebell with your hips "
                           "as you straighten and let it lift to chest height and fall again",
                           "Core", {"Abs", "Glutes", "Lower-Back"}, {"weight", "reps"}),

            Lab::Excercise("Chest Supported Row", "Row at a chest supported row machine", "Back",
                           {"Upper-Back", "Lats"}, {"weight", "reps"})

        };
        for (const auto& excerciseToSave : testEx) {
            excerciseToSave.save(db.get());
        }

        e3 = {db.get(),
              "Pull Day 1",
              {Lab::ExcerciseData(Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                                 {"Upper-back", "Lats"}, {"weight", "reps"}),
                                  50, 5),
               Lab::ExcerciseData(Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards",
                                                 "Back", {"Upper-back"}, {"weight", "reps"}),
                                  10, 10),
               Lab::ExcerciseData(Lab::Excercise("Wide Grip Pull Ups",
                                                 "Holding the bar with a wide grip, do a "
                                                 "pullup until your chin is over the bar",
                                                 "Back", {"Upper-back"}, {"reps"}),
                                  10, 0)}};
    }
    void TearDown() override { remove("thelab.db"); }

   public:
    std::unique_ptr<Lab::DBConn> db = std::make_unique<Lab::DBConn>();
    Lab::Workout e1{db.get()};
    Lab::Workout e2{db.get(), "Push Day 1"};
    Lab::Workout e3{db.get()};
    Lab::Workout e4 = Lab::Workout(db.get());
};

TEST_F(WorkoutTest, EmptyWorkOutPlanTest) {
    EXPECT_TRUE(e1.getWorkout().empty());
    EXPECT_TRUE(e2.getWorkout().empty());
    EXPECT_FALSE(e3.getWorkout().empty());
}

TEST_F(WorkoutTest, NameTest) {
    EXPECT_EQ(e1.getName(), "");
    EXPECT_EQ(e2.getName(), "Push Day 1");
    EXPECT_EQ(e3.getName(), "Pull Day 1");
}

TEST_F(WorkoutTest, SetNameTest) {
    EXPECT_EQ(e1.getName(), "");
    EXPECT_EQ(e2.getName(), "Push Day 1");
    EXPECT_EQ(e3.getName(), "Pull Day 1");

    e1.editName("Upper-Body Day");
    EXPECT_EQ(e1.getName(), "Upper-Body Day");
    e2.editName("Full Body Day");
    EXPECT_EQ(e2.getName(), "Full Body Day");
    e3.editName("Back Workout");
    EXPECT_EQ(e3.getName(), "Back Workout");
}

TEST_F(WorkoutTest, RetrieveWorkoutFroDBTest) {
    e3.save();

    Lab::Workout readWorkout(db.get(), "Pull Day 1");
    const auto& savedWorkoutData = e3.getWorkout();
    const auto& readWorkoudData = readWorkout.getWorkout();

    EXPECT_EQ(e3.getName(), readWorkout.getName());
    EXPECT_EQ(savedWorkoutData.size(), readWorkoudData.size());

    for (auto savedExcerciseData = savedWorkoutData.cbegin(), readExcerciseData = readWorkoudData.cbegin();
         savedExcerciseData != savedWorkoutData.cend(); ++savedExcerciseData, ++readExcerciseData) {
        EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *savedExcerciseData, *readExcerciseData);
    }
}

TEST_F(WorkoutTest, SetWoPlanTest) {
    std::vector<Lab::ExcerciseData> plan1{
        Lab::ExcerciseData(Lab::Excercise("Scull-Crushers",
                                          "Laying Down on a bench, hold a bar straight and bend at "
                                          "the elbow to bring the bar towards your head",
                                          "Arms", {"Tricep"}, {"weight", "reps"}),
                           40, 8),
        Lab::ExcerciseData(Lab::Excercise("Overhead Press", "Press the bar overhead while standing", "Chest",
                                          {"Pectorals", "Tricep"}, {"weight", "reps"}),
                           100, 1),
        Lab::ExcerciseData(Lab::Excercise("Jump Rope", "Skip with skipping rope", "Cardio", {""}, {"reps"}), 55, 0),
        Lab::ExcerciseData(Lab::Excercise("Helms Row", "Bent over row, with chest supported by the bench", "Back",
                                          {"Upper-back", "Lats"}, {"weight", "reps"}),
                           80, 12)};
    e1.setWorkout(plan1);
    auto plan1Check = e1.getWorkout();
    auto iter = plan1Check.cbegin();
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Scull-Crushers",
                                                          "Laying Down on a bench, hold a bar straight and bend at "
                                                          "the elbow to bring the bar towards your head",
                                                          "Arms", {"Tricep"}, {"weight", "reps"}),
                                           40, 8));
    ++iter;
    EXPECT_PRED_FORMAT2(AssertExcerciseEqual, iter->exc,
                        Lab::Excercise("Overhead Press", "Press the bar overhead while standing", "Chest",
                                       {"Pectorals", "Tricep"}, {"weight", "reps"}));
    EXPECT_EQ(iter->type1, 100);
    EXPECT_EQ(iter->type2, 1);
    ++iter;
    const auto [excer, type1, type2] = *iter;
    EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                        Lab::Excercise("Jump Rope", "Skip with skipping rope", "Cardio", {""}, {"reps"}));
    EXPECT_EQ(type1, 55);
    EXPECT_EQ(type2, 0);
    ++iter;
    EXPECT_PRED_FORMAT2(
        AssertWorkoutEqual, *iter,
        Lab::ExcerciseData(Lab::Excercise("Helms Row", "Bent over row, with chest supported by the bench", "Back",
                                          {"Upper-back", "Lats"}, {"weight", "reps"}),
                           80, 12));
    ++iter;
    EXPECT_EQ(iter, plan1Check.cend());
}

TEST_F(WorkoutTest, OverrideWoPlanTest) {
    std::vector<Lab::ExcerciseData> newWoPlan = {
        Lab::ExcerciseData(Lab::Excercise("Running (Treadmill)", "Running on a treadmill", "Cardio", {""}, {"time"}),
                           3600, 0),
        Lab::ExcerciseData(Lab::Excercise("Decline Bench-Press", "Bench Press with the bench int a declined position",
                                          "Chest", {"Pectorals", "Tricep"}, {"weight", "reps"}),
                           120, 10)};
    e3.setWorkout(newWoPlan);
    auto woPlan = e3.getWorkout();
    auto iter = woPlan.cbegin();
    EXPECT_PRED_FORMAT2(
        AssertWorkoutEqual, *iter,
        Lab::ExcerciseData(Lab::Excercise("Running (Treadmill)", "Running on a treadmill", "Cardio", {""}, {"time"}),
                           3600, 0));
    ++iter;
    EXPECT_PRED_FORMAT2(
        AssertWorkoutEqual, *iter,
        Lab::ExcerciseData(Lab::Excercise("Decline Bench-Press", "Bench Press with the bench int a declined position",
                                          "Chest", {"Pectorals", "Tricep"}, {"weight", "reps"}),
                           120, 10));
    ++iter;
    EXPECT_EQ(iter, woPlan.cend());
}

TEST_F(WorkoutTest, AddExcerciseTest) {
    EXPECT_TRUE(e2.getWorkout().empty());
    e2.addExcercise(Lab::Excercise("Kettlebell Swing",
                                   "With the kettlebell on the ground hinge at the hips and hike the "
                                   "kettlebell between your legs, push the kettlebell with your hips "
                                   "as you straighten and let it lift to chest height and fall again",
                                   "Core", {"Abs", "Glutes", "Lower-Back"}, {"weight", "reps"}),
                    20, 10);
    EXPECT_EQ(e2.getWorkout().size(), 1);
    e2.addExcercise(Lab::Excercise("Kettlebell Swing",
                                   "With the kettlebell on the ground hinge at the hips and hike the "
                                   "kettlebell between your legs, push the kettlebell with your hips "
                                   "as you straighten and let it lift to chest height and fall again",
                                   "Core", {"Abs", "Glutes", "Lower-Back"}, {"weight", "reps"}),
                    20, 10);
    EXPECT_EQ(e2.getWorkout().size(), 2);
    e2.addExcercise(Lab::Excercise("Kettlebell Swing",
                                   "With the kettlebell on the ground hinge at the hips and hike the "
                                   "kettlebell between your legs, push the kettlebell with your hips "
                                   "as you straighten and let it lift to chest height and fall again",
                                   "Core", {"Abs", "Glutes", "Lower-Back"}, {"weight", "reps"}),
                    20, 10);
    EXPECT_EQ(e2.getWorkout().size(), 3);
    e2.addExcercise(Lab::Excercise("Kettlebell Swing",
                                   "With the kettlebell on the ground hinge at the hips and hike the "
                                   "kettlebell between your legs, push the kettlebell with your hips "
                                   "as you straighten and let it lift to chest height and fall again",
                                   "Core", {"Abs", "Glutes", "Lower-Back"}, {"weight", "reps"}),
                    20, 10);
    EXPECT_EQ(e2.getWorkout().size(), 4);
    e2.addExcercise(Lab::Excercise("Chest Supported Row", "Row at a chest supported row machine", "Back",
                                   {"Upper-Back", "Lats"}, {"weight", "reps"}),
                    60, 12);
    EXPECT_EQ(e2.getWorkout().size(), 5);
    e2.addExcercise(Lab::Excercise("Chest Supported Row", "Row at a chest supported row machine", "Back",
                                   {"Upper-Back", "Lats"}, {"weight", "reps"}),
                    60, 12);
    EXPECT_EQ(e2.getWorkout().size(), 6);
    e2.addExcercise(Lab::Excercise("Chest Supported Row", "Row at a chest supported row machine", "Back",
                                   {"Upper-Back", "Lats"}, {"weight", "reps"}),
                    60, 12);
    EXPECT_EQ(e2.getWorkout().size(), 7);

    int excerciseIndex = 0;
    for (const auto& woPlan : e2.getWorkout()) {
        const auto [excer, type1, type2] = woPlan;
        if (excerciseIndex < 4) {
            EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                                Lab::Excercise("Kettlebell Swing",
                                               "With the kettlebell on the ground hinge at the hips "
                                               "and hike the kettlebell between your legs, push the "
                                               "kettlebell with your hips as you straighten and let "
                                               "it lift to chest height and fall again",
                                               "Core", {"Abs", "Glutes", "Lower-Back"}, {"weight", "reps"}));
            EXPECT_EQ(type1, 20);
            EXPECT_EQ(type2, 10);
        } else {
            EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                                Lab::Excercise("Chest Supported Row", "Row at a chest supported row machine", "Back",
                                               {"Upper-Back", "Lats"}, {"weight", "reps"}));
            EXPECT_EQ(type1, 60);
            EXPECT_EQ(type2, 12);
        }
        ++excerciseIndex;
    }
}

TEST_F(WorkoutTest, AppendExcerciseTest) {
    EXPECT_EQ(e3.getWorkout().size(), 3);
    e3.addExcercise(Lab::Excercise("Chest Supported Row", "Row at a chest supported row machine", "Back",
                                   {"Upper-Back", "Lats"}, {"weight", "reps"}),
                    50, 14);
    EXPECT_EQ(e3.getWorkout().size(), 4);
    auto woPlan = e3.getWorkout();
    auto iter = woPlan.cbegin();
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                                          {"Upper-back", "Lats"}, {"weight", "reps"}),
                                           50, 5));
    ++iter;
    EXPECT_PRED_FORMAT2(
        AssertWorkoutEqual, *iter,
        Lab::ExcerciseData(Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards", "Back",
                                          {"Upper-back"}, {"weight", "reps"}),
                           10, 10));
    ++iter;
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Wide Grip Pull Ups",
                                                          "Holding the bar with a wide grip, do a "
                                                          "pullup until your chin is over the bar",
                                                          "Back", {"Upper-back"}, {"reps"}),
                                           10, 0));
    ++iter;
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Chest Supported Row", "Row at a chest supported row machine",
                                                          "Back", {"Upper-Back", "Lats"}, {"weight", "reps"}),
                                           50, 14));
    ++iter;
    EXPECT_EQ(iter, woPlan.cend());
}

TEST_F(WorkoutTest, ChangeExcerciseTest) {
    auto woPlan = e3.getWorkout();
    auto iter = woPlan.begin();
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                                          {"Upper-back", "Lats"}, {"weight", "reps"}),
                                           50, 5));
    e3.changeExcercise(iter,
                       Lab::Excercise("Chest Supported Row", "Row at a chest supported row machine", "Back",
                                      {"Upper-Back", "Lats"}, {"weight", "reps"}),
                       50, 14);
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Chest Supported Row", "Row at a chest supported row machine",
                                                          "Back", {"Upper-Back", "Lats"}, {"weight", "reps"}),
                                           50, 14));
    ++iter;
    EXPECT_PRED_FORMAT2(
        AssertWorkoutEqual, *iter,
        Lab::ExcerciseData(Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards", "Back",
                                          {"Upper-back"}, {"weight", "reps"}),
                           10, 10));
    ++iter;
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Wide Grip Pull Ups",
                                                          "Holding the bar with a wide grip, do a "
                                                          "pullup until your chin is over the bar",
                                                          "Back", {"Upper-back"}, {"reps"}),
                                           10, 0));
    ++iter;
    EXPECT_EQ(iter, woPlan.cend());
}

TEST_F(WorkoutTest, ChangedMulipleExcerciseTest) {
    auto& woPlan = e3.getWorkout();
    auto iter = woPlan.cbegin();
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                                          {"Upper-back", "Lats"}, {"weight", "reps"}),
                                           50, 5));
    ++iter;
    EXPECT_PRED_FORMAT2(
        AssertWorkoutEqual, *iter,
        Lab::ExcerciseData(Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards", "Back",
                                          {"Upper-back"}, {"weight", "reps"}),
                           10, 10));
    ++iter;
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Wide Grip Pull Ups",
                                                          "Holding the bar with a wide grip, do a "
                                                          "pullup until your chin is over the bar",
                                                          "Back", {"Upper-back"}, {"reps"}),
                                           10, 0));
    ++iter;
    EXPECT_EQ(iter, woPlan.cend());
    e3.changeExcercise(woPlan.begin(), woPlan.begin() + 3,
                       Lab::Excercise("Chest Supported Row", "Row at a chest supported row machine", "Back",
                                      {"Upper-Back", "Lats"}, {"weight", "reps"}),
                       50, 14);
    for (const auto& plan : woPlan) {
        EXPECT_PRED_FORMAT2(
            AssertWorkoutEqual, plan,
            Lab::ExcerciseData(Lab::Excercise("Chest Supported Row", "Row at a chest supported row machine", "Back",
                                              {"Upper-Back", "Lats"}, {"weight", "reps"}),
                               50, 14));
    }
}

TEST_F(WorkoutTest, RemoveExcerciseTest) {
    auto& woPlan = e3.getWorkout();
    auto iter = woPlan.begin();
    EXPECT_EQ(woPlan.size(), 3);
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                                          {"Upper-back", "Lats"}, {"weight", "reps"}),
                                           50, 5));
    ++iter;
    EXPECT_PRED_FORMAT2(
        AssertWorkoutEqual, *iter,
        Lab::ExcerciseData(Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards", "Back",
                                          {"Upper-back"}, {"weight", "reps"}),
                           10, 10));
    ++iter;
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Wide Grip Pull Ups",
                                                          "Holding the bar with a wide grip, do a "
                                                          "pullup until your chin is over the bar",
                                                          "Back", {"Upper-back"}, {"reps"}),
                                           10, 0));
    ++iter;
    EXPECT_EQ(iter, woPlan.end());
    iter = woPlan.begin();
    e3.remExcercise(iter);
    EXPECT_EQ(woPlan.size(), 2);
    EXPECT_PRED_FORMAT2(
        AssertWorkoutEqual, *iter,
        Lab::ExcerciseData(Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards", "Back",
                                          {"Upper-back"}, {"weight", "reps"}),
                           10, 10));
    ++iter;
    EXPECT_PRED_FORMAT2(AssertWorkoutEqual, *iter,
                        Lab::ExcerciseData(Lab::Excercise("Wide Grip Pull Ups",
                                                          "Holding the bar with a wide grip, do a "
                                                          "pullup until your chin is over the bar",
                                                          "Back", {"Upper-back"}, {"reps"}),
                                           10, 0));
    ++iter;
    EXPECT_EQ(iter, woPlan.end());
    e3.remExcercise(woPlan.end() - 1);
    iter = woPlan.begin();
    EXPECT_EQ(woPlan.size(), 1);
    EXPECT_PRED_FORMAT2(
        AssertWorkoutEqual, *iter,
        Lab::ExcerciseData(Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards", "Back",
                                          {"Upper-back"}, {"weight", "reps"}),
                           10, 10));
    ++iter;
    EXPECT_EQ(iter, woPlan.end());
    e3.remExcercise(woPlan.begin());
    EXPECT_TRUE(woPlan.empty());
}

TEST_F(WorkoutTest, RemoveMultiExcerciseTest) {
    auto& woPlan = e3.getWorkout();
    EXPECT_EQ(woPlan.size(), 3);
    e3.remExcercise(woPlan.begin(), woPlan.end());
    EXPECT_TRUE(woPlan.empty());
}

TEST_F(WorkoutTest, SaveWorkOutPlanTest) {
    EXPECT_TRUE(e3.save());
    EXPECT_EQ(1, db->exec_and_retrieve("SELECT * FROM workouts"));
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Pull Day 1");
    EXPECT_EQ(db->get_column(1).getInt(), 0);
    EXPECT_EQ(db->get_column(2).getString(), "Barbell Row");
    EXPECT_EQ(db->get_column(3).getInt(), 50);
    EXPECT_EQ(db->get_column(4).getInt(), 5);
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Pull Day 1");
    EXPECT_EQ(db->get_column(1).getInt(), 1);
    EXPECT_EQ(db->get_column(2).getString(), "Dumbbell Flys");
    EXPECT_EQ(db->get_column(3).getInt(), 10);
    EXPECT_EQ(db->get_column(4).getInt(), 10);
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Pull Day 1");
    EXPECT_EQ(db->get_column(1).getInt(), 2);
    EXPECT_EQ(db->get_column(2).getString(), "Wide Grip Pull Ups");
    EXPECT_EQ(db->get_column(3).getInt(), 10);
    EXPECT_EQ(db->get_column(4).getInt(), 0);
    EXPECT_FALSE(db->retrieve_next_row());

    // Empty workout save
    EXPECT_TRUE(e1.save());
    EXPECT_EQ(1, db->exec_and_retrieve("SELECT * FROM workouts"));
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Pull Day 1");
    EXPECT_EQ(db->get_column(1).getInt(), 0);
    EXPECT_EQ(db->get_column(2).getString(), "Barbell Row");
    EXPECT_EQ(db->get_column(3).getInt(), 50);
    EXPECT_EQ(db->get_column(4).getInt(), 5);
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Pull Day 1");
    EXPECT_EQ(db->get_column(1).getInt(), 1);
    EXPECT_EQ(db->get_column(2).getString(), "Dumbbell Flys");
    EXPECT_EQ(db->get_column(3).getInt(), 10);
    EXPECT_EQ(db->get_column(4).getInt(), 10);
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Pull Day 1");
    EXPECT_EQ(db->get_column(1).getInt(), 2);
    EXPECT_EQ(db->get_column(2).getString(), "Wide Grip Pull Ups");
    EXPECT_EQ(db->get_column(3).getInt(), 10);
    EXPECT_EQ(db->get_column(4).getInt(), 0);
    EXPECT_FALSE(db->retrieve_next_row());

    // Added more items workout save
    std::vector<Lab::ExcerciseData> plan{
        Lab::ExcerciseData(Lab::Excercise("Jumping Jacks", "Jump and spread your legs and put your arms to the sky",
                                          "Cardio", {""}, {"reps"}),
                           10, 0),
        Lab::ExcerciseData(Lab::Excercise("Plank", "In pushup position, lift yourself on your elbows and toes", "Core",
                                          {"Abs"}, {"time", "reps"}),
                           60, 4),
        Lab::ExcerciseData(Lab::Excercise("Calf Press", "Lift yourself on your tiptoes with your calf", "Legs",
                                          {"Calf"}, {"weight", "reps"}),
                           250, 4)};
    e2.setWorkout(plan);
    e2.editName("Full Body Plan");
    EXPECT_TRUE(e2.save());
    EXPECT_EQ(1, db->exec_and_retrieve("SELECT * FROM workouts"));
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Pull Day 1");
    EXPECT_EQ(db->get_column(1).getInt(), 0);
    EXPECT_EQ(db->get_column(2).getString(), "Barbell Row");
    EXPECT_EQ(db->get_column(3).getInt(), 50);
    EXPECT_EQ(db->get_column(4).getInt(), 5);
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Pull Day 1");
    EXPECT_EQ(db->get_column(1).getInt(), 1);
    EXPECT_EQ(db->get_column(2).getString(), "Dumbbell Flys");
    EXPECT_EQ(db->get_column(3).getInt(), 10);
    EXPECT_EQ(db->get_column(4).getInt(), 10);
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Pull Day 1");
    EXPECT_EQ(db->get_column(1).getInt(), 2);
    EXPECT_EQ(db->get_column(2).getString(), "Wide Grip Pull Ups");
    EXPECT_EQ(db->get_column(3).getInt(), 10);
    EXPECT_EQ(db->get_column(4).getInt(), 0);
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Full Body Plan");
    EXPECT_EQ(db->get_column(1).getInt(), 0);
    EXPECT_EQ(db->get_column(2).getString(), "Jumping Jacks");
    EXPECT_EQ(db->get_column(3).getInt(), 10);
    EXPECT_EQ(db->get_column(4).getInt(), 0);
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Full Body Plan");
    EXPECT_EQ(db->get_column(1).getInt(), 1);
    EXPECT_EQ(db->get_column(2).getString(), "Plank");
    EXPECT_EQ(db->get_column(3).getInt(), 60);
    EXPECT_EQ(db->get_column(4).getInt(), 4);
    EXPECT_TRUE(db->retrieve_next_row());
    EXPECT_EQ(db->get_column(0).getString(), "Full Body Plan");
    EXPECT_EQ(db->get_column(1).getInt(), 2);
    EXPECT_EQ(db->get_column(2).getString(), "Calf Press");
    EXPECT_EQ(db->get_column(3).getInt(), 250);
    EXPECT_EQ(db->get_column(4).getInt(), 4);
    EXPECT_FALSE(db->retrieve_next_row());
}

TEST_F(WorkoutTest, WoPlanSetSaveTest) {
    e4.editName("Calf Crusher");
    for (int i = 0; i < 4; ++i) {
        e4.addExcercise(Lab::Excercise("Calf Press", "Lift yourself on your tiptoes with your calf", "Legs", {"Calf"},
                                       {"weight", "reps"}),
                        30, 10);
    }
    e4.save();
    db->exec_and_retrieve("SELECT * FROM workouts");
    int workoutIndex = 0;
    while (db->retrieve_next_row()) {
        EXPECT_EQ(db->get_column(0).getString(), "Calf Crusher");
        EXPECT_EQ(db->get_column(1).getInt(), workoutIndex);
        EXPECT_EQ(db->get_column(2).getString(), "Calf Press");
        EXPECT_EQ(db->get_column(3).getInt(), 30);
        EXPECT_EQ(db->get_column(4).getInt(), 10);
        ++workoutIndex;
    }
}

TEST_F(WorkoutTest, InvalidExcerciseSaveTest) {
    e1.addExcercise(Lab::Excercise("Front flips", "Do a front flip", "Cardio", {""}, {"reps"}), 100, 0);
    EXPECT_TRUE(e1.save());
    bool frontFlipsFound = false;
    db->exec_and_retrieve("SELECT * FROM workouts");
    while (db->retrieve_next_row()) {
        if (db->get_column(2).getString() == "Front flips") {
            frontFlipsFound = true;
        }
    }
    EXPECT_FALSE(frontFlipsFound);
}
