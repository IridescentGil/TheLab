
#include "workout.h"

#include <gtest/gtest.h>

#include <memory>
#include <tuple>
#include <vector>

#include "database.h"
#include "excercise.h"

class BodyTest : public testing::Test {
   protected:
    void SetUp() override {
        std::vector<Excercise> testEx = {
            Excercise("Barbell Row", "Standing bent over row with Barbell",
                      "Back", {"Upper-back", "Lats"}, {"weight", "reps"}),
            Excercise("Dumbbell Flys",
                      "Standing bent forward, lift dumbbell outwards", "Back",
                      {"Upper-back"}, {"weight", "reps"}),
            Excercise("Wide Grip Pull Ups",
                      "Holding the bar with a wide grip, do a "
                      "pullup until your chin is over the bar",
                      "Back", {"Upper-back"}, {"reps"}),
            Excercise("Jumping Jacks",
                      "Jump and spread your legs and put your arms to the sky",
                      "Cardio", {""}, {"reps"}),
            Excercise(
                "Plank",
                "In pushup position, lift yourself on your elbows and toes",
                "Core", {"Abs"}, {"time", "reps"}),
            Excercise("Calf Press",
                      "Lift yourself on your tiptoes with your calf", "Legs",
                      {"Calf"}, {"weight", "reps"})};
        for (auto iter : testEx) {
            std::string musclesWorked = "";
            std::string type = "";
            for (auto bter : iter.getMusclesWorked()) {
                musclesWorked += bter;
                if (*(iter.getMusclesWorked().end() - 1) != bter) {
                    musclesWorked += ", ";
                }
            }
            for (auto bter : iter.getType()) {
                type += bter;
                if (*(iter.getType().end() - 1) != bter) {
                    type += ", ";
                }
            }
            db->prepare(
                "INSERT INTO excercises (excercises, description, muscleGroup, "
                "musclesTargeted, type) VALUES (?, ?, ?, ?, ?)",
                iter.getName(), iter.getDescription(), iter.getMuscleGroup(),
                musclesWorked, type);
        }
    }
    void TearDown() override { remove("thelab.db"); }

    std::shared_ptr<DBConn> db = std::make_shared<DBConn>();
    Workout e1{db};
    Workout e2{db, "Push Day 1"};
    Workout e3{
        db,
        "Pull Day 1",
        {std::make_tuple(
             Excercise("Barbell Row", "Standing bent over row with Barbell",
                       "Back", {"Upper-back", "Lats"}, {"weight", "reps"}),
             50, 5),
         std::make_tuple(
             Excercise("Dumbbell Flys",
                       "Standing bent forward, lift dumbbell outwards", "Back",
                       {"Upper-back"}, {"weight", "reps"}),
             10, 10),
         std::make_tuple(Excercise("Wide Grip Pull Ups",
                                   "Holding the bar with a wide grip, do a "
                                   "pullup until your chin is over the bar",
                                   "Back", {"Upper-back"}, {"reps"}),
                         10, 0)}};
    Workout e4 = Workout(db);
};

auto excerEqual = [](Excercise a, Excercise b) -> bool {
    if (a.getName() != b.getName()) return false;
    if (a.getDescription() != b.getDescription()) return false;
    if (a.getMuscleGroup() != b.getMuscleGroup()) return false;
    if (a.getMusclesWorked() != b.getMusclesWorked()) return false;
    if (a.getType() == b.getType()) return false;
    return true;
};

auto woTupleEqual = [](std::tuple<Excercise, int, int> a,
                       std::tuple<Excercise, int, int> b) -> bool {
    const auto [aE, aT1, aT2] = a;
    const auto [bE, bT1, bT2] = b;

    if (!excerEqual(aE, bE)) return false;
    if (aT1 != bT1) return false;
    if (aT2 != bT2) return false;
    return true;
};

testing::AssertionResult AssertExcerciseEqual(const char* m_expr,
                                              const char* n_expr, Excercise m,
                                              Excercise n) {
    if (excerEqual(m, n)) return testing::AssertionSuccess();

    return testing::AssertionFailure()
           << m_expr << "and" << n_expr << " Conditions not equal";
}

testing::AssertionResult AssertWoTupleEqual(const char* m_expr,
                                            const char* n_expr,
                                            std::tuple<Excercise, int, int> m,
                                            std::tuple<Excercise, int, int> n) {
    if (woTupleEqual(m, n)) return testing::AssertionSuccess();

    return testing::AssertionFailure()
           << m_expr << "and" << n_expr << " Conditions not equal";
}

TEST_F(BodyTest, EmptyWorkOutPlanTest) {
    EXPECT_TRUE(e1.getWoPlan().empty());
    EXPECT_TRUE(e2.getWoPlan().empty());
    EXPECT_FALSE(e3.getWoPlan().empty());
}

TEST_F(BodyTest, NameTest) {
    EXPECT_EQ(e1.getName(), "");
    EXPECT_EQ(e2.getName(), "Push Day 1");
    EXPECT_EQ(e3.getName(), "Pull Day 1");
}

TEST_F(BodyTest, SetNameTest) {
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

TEST_F(BodyTest, SetWoPlanTest) {
    std::vector<std::tuple<Excercise, int, int>> plan1{
        std::make_tuple(
            Excercise("Scull-Crushers",
                      "Laying Down on a bench, hold a bar straight and bend at "
                      "the elbow to bring the bar towards your head",
                      "Arms", {"Tricep"}, {"weight", "reps"}),
            40, 8),
        std::make_tuple(
            Excercise("Overhead Press", "Press the bar overhead while standing",
                      "Chest", {"Pectorals", "Tricep"}, {"weight", "reps"}),
            100, 1),
        std::make_tuple(Excercise("Jump Rope", "Skip with skipping rope",
                                  "Cardio", {""}, {"reps"}),
                        55, 0),
        std::make_tuple(
            Excercise("Helms Row",
                      "Bent over row, with chest supported by the bench",
                      "Back", {"Upper-back", "Lats"}, {"weight", "reps"}),
            80, 12)};
    e1.setWoPlan(plan1);
    auto plan1Check = e1.getWoPlan();
    auto iter = plan1Check.cbegin();
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Scull-Crushers",
                      "Laying Down on a bench, hold a bar straight and bend at "
                      "the elbow to bring the bar towards your head",
                      "Arms", {"Tricep"}, {"weight", "reps"}),
            40, 8));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertExcerciseEqual, std::get<0>(*iter),
        Excercise("Overhead Press", "Press the bar overhead while standing",
                  "Chest", {"Pectorals", "Tricep"}, {"weight", "reps"}));
    EXPECT_EQ(std::get<1>(*iter), 100);
    EXPECT_EQ(std::get<2>(*iter), 1);
    iter++;
    const auto [excer, type1, type2] = *iter;
    EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                        Excercise("Jump Rope", "Skip with skipping rope",
                                  "Cardio", {""}, {"reps"}));
    EXPECT_EQ(type1, 55);
    EXPECT_EQ(type2, 0);
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Helms Row",
                      "Bent over row, with chest supported by the bench",
                      "Back", {"Upper-back", "Lats"}, {"weight", "reps"}),
            80, 12));
    iter++;
    EXPECT_EQ(iter, plan1Check.cend());
}

TEST_F(BodyTest, OverrideWoPlanTest) {
    std::vector<std::tuple<Excercise, int, int>> newWoPlan = {
        std::make_tuple(
            Excercise("Running (Treadmill)", "Running on a treadmill", "Cardio",
                      {""}, {"time"}),
            3600, 0),
        std::make_tuple(
            Excercise("Decline Bench-Press",
                      "Bench Press with the bench int a declined position",
                      "Chest", {"Pectorals", "Tricep"}, {"weight", "reps"}),
            120, 10)};
    e3.setWoPlan(newWoPlan);
    auto woPlan = e3.getWoPlan();
    auto iter = woPlan.cbegin();
    EXPECT_PRED_FORMAT2(AssertWoTupleEqual, *iter,
                        std::make_tuple(Excercise("Running (Treadmill)",
                                                  "Running on a treadmill",
                                                  "Cardio", {""}, {"time"}),
                                        3600, 0));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Decline Bench-Press",
                      "Bench Press with the bench int a declined position",
                      "Chest", {"Pectorals", "Tricep"}, {"weight", "reps"}),
            120, 10));
    iter++;
    EXPECT_EQ(iter, woPlan.cend());
}

TEST_F(BodyTest, AddExcerciseTest) {
    EXPECT_TRUE(e2.getWoPlan().empty());
    e2.addExcercise(
        Excercise(
            "Kettlebell Swing",
            "With the kettlebell on the ground hinge at the hips and hike the "
            "kettlebell between your legs, push the kettlebell with your hips "
            "as you straighten and let it lift to chest height and fall again",
            "Core", {"Abs", "Glutes", "Lower-Back"}, {"weight", "reps"}),
        20, 10);
    EXPECT_EQ(e2.getWoPlan().size(), 1);
    e2.addExcercise(
        Excercise(
            "Kettlebell Swing",
            "With the kettlebell on the ground hinge at the hips and hike the "
            "kettlebell between your legs, push the kettlebell with your hips "
            "as you straighten and let it lift to chest height and fall again",
            "Core", {"Abs", "Glutes", "Lower-Back"}, {"weight", "reps"}),
        20, 10);
    EXPECT_EQ(e2.getWoPlan().size(), 2);
    e2.addExcercise(
        Excercise(
            "Kettlebell Swing",
            "With the kettlebell on the ground hinge at the hips and hike the "
            "kettlebell between your legs, push the kettlebell with your hips "
            "as you straighten and let it lift to chest height and fall again",
            "Core", {"Abs", "Glutes", "Lower-Back"}, {"weight", "reps"}),
        20, 10);
    EXPECT_EQ(e2.getWoPlan().size(), 3);
    e2.addExcercise(
        Excercise(
            "Kettlebell Swing",
            "With the kettlebell on the ground hinge at the hips and hike the "
            "kettlebell between your legs, push the kettlebell with your hips "
            "as you straighten and let it lift to chest height and fall again",
            "Core", {"Abs", "Glutes", "Lower-Back"}, {"weight", "reps"}),
        20, 10);
    EXPECT_EQ(e2.getWoPlan().size(), 4);
    e2.addExcercise(
        Excercise("Chest Supported Row", "Row at a chest supported row machine",
                  "Back", {"Upper-Back", "Lats"}, {"weight", "reps"}),
        60, 12);
    EXPECT_EQ(e2.getWoPlan().size(), 5);
    e2.addExcercise(
        Excercise("Chest Supported Row", "Row at a chest supported row machine",
                  "Back", {"Upper-Back", "Lats"}, {"weight", "reps"}),
        60, 12);
    EXPECT_EQ(e2.getWoPlan().size(), 6);
    e2.addExcercise(
        Excercise("Chest Supported Row", "Row at a chest supported row machine",
                  "Back", {"Upper-Back", "Lats"}, {"weight", "reps"}),
        60, 12);
    EXPECT_EQ(e2.getWoPlan().size(), 7);

    int i = 0;
    for (auto woPlan : e2.getWoPlan()) {
        const auto [excer, type1, type2] = woPlan;
        if (i < 4) {
            EXPECT_PRED_FORMAT2(
                AssertExcerciseEqual, excer,
                Excercise("Kettlebell Swing",
                          "With the kettlebell on the ground hinge at the hips "
                          "and hike the kettlebell between your legs, push the "
                          "kettlebell with your hips as you straighten and let "
                          "it lift to chest height and fall again",
                          "Core", {"Abs", "Glutes", "Lower-Back"},
                          {"weight", "reps"}));
            EXPECT_EQ(type1, 20);
            EXPECT_EQ(type2, 10);
        } else {
            EXPECT_PRED_FORMAT2(
                AssertExcerciseEqual, excer,
                Excercise("Chest Supported Row",
                          "Row at a chest supported row machine", "Back",
                          {"Upper-Back", "Lats"}, {"weight", "reps"}));
            EXPECT_EQ(type1, 60);
            EXPECT_EQ(type2, 12);
        }
    }
}

TEST_F(BodyTest, AppendExcerciseTest) {
    EXPECT_EQ(e3.getWoPlan().size(), 3);
    e3.addExcercise(
        Excercise("Chest Supported Row", "Row at a chest supported row machine",
                  "Back", {"Upper-Back", "Lats"}, {"weight", "reps"}),
        50, 14);
    EXPECT_EQ(e3.getWoPlan().size(), 4);
    auto woPlan = e3.getWoPlan();
    auto iter = woPlan.cbegin();
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Barbell Row", "Standing bent over row with Barbell",
                      "Back", {"Upper-back", "Lats"}, {"weight", "reps"}),
            50, 5));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Dumbbell Flys",
                      "Standing bent forward, lift dumbbell outwards", "Back",
                      {"Upper-back"}, {"weight", "reps"}),
            10, 10));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(Excercise("Wide Grip Pull Ups",
                                  "Holding the bar with a wide grip, do a "
                                  "pullup until your chin is over the bar",
                                  "Back", {"Upper-back"}, {"reps"}),
                        10, 0));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Chest Supported Row",
                      "Row at a chest supported row machine", "Back",
                      {"Upper-Back", "Lats"}, {"weight", "reps"}),
            50, 14));
    iter++;
    EXPECT_EQ(iter, woPlan.cend());
}

TEST_F(BodyTest, ChangeExcerciseTest) {
    auto woPlan = e3.getWoPlan();
    auto iter = woPlan.cbegin();
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Barbell Row", "Standing bent over row with Barbell",
                      "Back", {"Upper-back", "Lats"}, {"weight", "reps"}),
            50, 5));
    e3.changeExcercise(
        iter,
        Excercise("Chest Supported Row", "Row at a chest supported row machine",
                  "Back", {"Upper-Back", "Lats"}, {"weight", "reps"}),
        50, 14);
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Chest Supported Row",
                      "Row at a chest supported row machine", "Back",
                      {"Upper-Back", "Lats"}, {"weight", "reps"}),
            50, 14));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Dumbbell Flys",
                      "Standing bent forward, lift dumbbell outwards", "Back",
                      {"Upper-back"}, {"weight", "reps"}),
            10, 10));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(Excercise("Wide Grip Pull Ups",
                                  "Holding the bar with a wide grip, do a "
                                  "pullup until your chin is over the bar",
                                  "Back", {"Upper-back"}, {"reps"}),
                        10, 0));
    iter++;
    EXPECT_EQ(iter, woPlan.cend());
}

TEST_F(BodyTest, ChangedMulipleExcerciseTest) {
    auto woPlan = e3.getWoPlan();
    auto iter = woPlan.cbegin();
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Barbell Row", "Standing bent over row with Barbell",
                      "Back", {"Upper-back", "Lats"}, {"weight", "reps"}),
            50, 5));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Dumbbell Flys",
                      "Standing bent forward, lift dumbbell outwards", "Back",
                      {"Upper-back"}, {"weight", "reps"}),
            10, 10));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(Excercise("Wide Grip Pull Ups",
                                  "Holding the bar with a wide grip, do a "
                                  "pullup until your chin is over the bar",
                                  "Back", {"Upper-back"}, {"reps"}),
                        10, 0));
    iter++;
    EXPECT_EQ(iter, woPlan.cend());
    e3.changeExcercise(
        woPlan.cbegin(), woPlan.cbegin() + 2,
        Excercise("Chest Supported Row", "Row at a chest supported row machine",
                  "Back", {"Upper-Back", "Lats"}, {"weight", "reps"}),
        50, 14);
    for (auto plan : woPlan) {
        EXPECT_PRED_FORMAT2(
            AssertWoTupleEqual, *iter,
            std::make_tuple(
                Excercise("Chest Supported Row",
                          "Row at a chest supported row machine", "Back",
                          {"Upper-Back", "Lats"}, {"weight", "reps"}),
                50, 14));
    }
}

TEST_F(BodyTest, ChangeExcerciseBadIteratorTest) {
    // TODO: bad start iterator, reversed start and end iterators
}

TEST_F(BodyTest, RemoveExcerciseTest) {
    auto woPlan = e3.getWoPlan();
    auto iter = woPlan.begin();
    EXPECT_EQ(woPlan.size(), 3);
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Barbell Row", "Standing bent over row with Barbell",
                      "Back", {"Upper-back", "Lats"}, {"weight", "reps"}),
            50, 5));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Dumbbell Flys",
                      "Standing bent forward, lift dumbbell outwards", "Back",
                      {"Upper-back"}, {"weight", "reps"}),
            10, 10));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(Excercise("Wide Grip Pull Ups",
                                  "Holding the bar with a wide grip, do a "
                                  "pullup until your chin is over the bar",
                                  "Back", {"Upper-back"}, {"reps"}),
                        10, 0));
    iter++;
    EXPECT_EQ(iter, woPlan.end());
    iter = woPlan.begin();
    e3.remExcercise(iter);
    EXPECT_EQ(woPlan.size(), 2);
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Dumbbell Flys",
                      "Standing bent forward, lift dumbbell outwards", "Back",
                      {"Upper-back"}, {"weight", "reps"}),
            10, 10));
    iter++;
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(Excercise("Wide Grip Pull Ups",
                                  "Holding the bar with a wide grip, do a "
                                  "pullup until your chin is over the bar",
                                  "Back", {"Upper-back"}, {"reps"}),
                        10, 0));
    iter++;
    EXPECT_EQ(iter, woPlan.end());
    e3.remExcercise(woPlan.end() - 1);
    iter = woPlan.begin();
    EXPECT_EQ(woPlan.size(), 1);
    EXPECT_PRED_FORMAT2(
        AssertWoTupleEqual, *iter,
        std::make_tuple(
            Excercise("Dumbbell Flys",
                      "Standing bent forward, lift dumbbell outwards", "Back",
                      {"Upper-back"}, {"weight", "reps"}),
            10, 10));
    iter++;
    EXPECT_EQ(iter, woPlan.end());
    e3.remExcercise(woPlan.begin());
    EXPECT_TRUE(woPlan.empty());
}

TEST_F(BodyTest, RemoveMultiExcerciseTest) {
    auto woPlan = e3.getWoPlan();
    EXPECT_EQ(woPlan.size(), 3);
    e3.remExcercise(woPlan.begin(), woPlan.end());
    EXPECT_TRUE(woPlan.empty());
}

TEST_F(BodyTest, RemoveExcerciseBadIteratorTest) {
    // TODO: bad start iterator, reversed start and end iterators
}

TEST_F(BodyTest, SaveWorkOutPlanTest) {
    EXPECT_TRUE(e3.save());
    EXPECT_EQ(0, db->execMulti("SELECT * FROM workouts"));
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Pull Day 1");
    EXPECT_EQ(db->getColumn(2).getInt(), 0);
    EXPECT_EQ(db->getColumn(3), "Barbell Row");
    EXPECT_EQ(db->getColumn(4).getInt(), 50);
    EXPECT_EQ(db->getColumn(5).getInt(), 5);
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Pull Day 1");
    EXPECT_EQ(db->getColumn(2).getInt(), 1);
    EXPECT_EQ(db->getColumn(3), "Dumbbell Flys");
    EXPECT_EQ(db->getColumn(4).getInt(), 10);
    EXPECT_EQ(db->getColumn(5).getInt(), 10);
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Pull Day 1");
    EXPECT_EQ(db->getColumn(2).getInt(), 2);
    EXPECT_EQ(db->getColumn(3), "Wide Grip Pull Ups");
    EXPECT_EQ(db->getColumn(4).getInt(), 10);
    EXPECT_EQ(db->getColumn(5).getInt(), 0);
    EXPECT_FALSE(db->stepExec());

    // Empty workout save
    EXPECT_TRUE(e1.save());
    EXPECT_EQ(0, db->execMulti("SELECT * FROM workouts"));
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Pull Day 1");
    EXPECT_EQ(db->getColumn(2).getInt(), 0);
    EXPECT_EQ(db->getColumn(3), "Barbell Row");
    EXPECT_EQ(db->getColumn(4).getInt(), 50);
    EXPECT_EQ(db->getColumn(5).getInt(), 5);
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Pull Day 1");
    EXPECT_EQ(db->getColumn(2).getInt(), 1);
    EXPECT_EQ(db->getColumn(3), "Dumbbell Flys");
    EXPECT_EQ(db->getColumn(4).getInt(), 10);
    EXPECT_EQ(db->getColumn(5).getInt(), 10);
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Pull Day 1");
    EXPECT_EQ(db->getColumn(2).getInt(), 2);
    EXPECT_EQ(db->getColumn(3), "Wide Grip Pull Ups");
    EXPECT_EQ(db->getColumn(4).getInt(), 10);
    EXPECT_EQ(db->getColumn(5).getInt(), 0);
    EXPECT_FALSE(db->stepExec());

    // Added more items workout save
    std::vector<std::tuple<Excercise, int, int>> plan{
        std::make_tuple(
            Excercise("Jumping Jacks",
                      "Jump and spread your legs and put your arms to the sky",
                      "Cardio", {""}, {"reps"}),
            10, 0),
        std::make_tuple(
            Excercise(
                "Plank",
                "In pushup position, lift yourself on your elbows and toes",
                "Core", {"Abs"}, {"time", "reps"}),
            60, 4),
        std::make_tuple(
            Excercise("Calf Press",
                      "Lift yourself on your tiptoes with your calf", "Legs",
                      {"Calf"}, {"weight", "reps"}),
            250, 4)};
    e2.setWoPlan(plan);
    e2.editName("Full Body Plan");
    EXPECT_TRUE(e2.save());
    EXPECT_EQ(0, db->execMulti("SELECT * FROM workouts"));
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Pull Day 1");
    EXPECT_EQ(db->getColumn(2).getInt(), 0);
    EXPECT_EQ(db->getColumn(3), "Barbell Row");
    EXPECT_EQ(db->getColumn(4).getInt(), 50);
    EXPECT_EQ(db->getColumn(5).getInt(), 5);
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Pull Day 1");
    EXPECT_EQ(db->getColumn(2).getInt(), 1);
    EXPECT_EQ(db->getColumn(3), "Dumbbell Flys");
    EXPECT_EQ(db->getColumn(4).getInt(), 10);
    EXPECT_EQ(db->getColumn(5).getInt(), 10);
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Pull Day 1");
    EXPECT_EQ(db->getColumn(2).getInt(), 2);
    EXPECT_EQ(db->getColumn(3), "Wide Grip Pull Ups");
    EXPECT_EQ(db->getColumn(4).getInt(), 10);
    EXPECT_EQ(db->getColumn(5).getInt(), 0);
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Full Body Plan");
    EXPECT_EQ(db->getColumn(2).getInt(), 0);
    EXPECT_EQ(db->getColumn(3), "Jumping Jacks");
    EXPECT_EQ(db->getColumn(4).getInt(), 10);
    EXPECT_EQ(db->getColumn(5).getInt(), 0);
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Full Body Plan");
    EXPECT_EQ(db->getColumn(2).getInt(), 1);
    EXPECT_EQ(db->getColumn(3), "Plank");
    EXPECT_EQ(db->getColumn(4).getInt(), 60);
    EXPECT_EQ(db->getColumn(5).getInt(), 4);
    EXPECT_TRUE(db->stepExec());
    EXPECT_EQ(db->getColumn(1), "Full Body Plan");
    EXPECT_EQ(db->getColumn(2).getInt(), 2);
    EXPECT_EQ(db->getColumn(3), "Calf Press");
    EXPECT_EQ(db->getColumn(4).getInt(), 250);
    EXPECT_EQ(db->getColumn(5).getInt(), 4);
    EXPECT_FALSE(db->stepExec());
}

TEST_F(BodyTest, WoPlanSetSaveTest) {
    e4.editName("Calf Crusher");
    for (int i = 0; i < 4; i++)
        e4.addExcercise(
            Excercise("Calf Press",
                      "Lift yourself on your tiptoes with your calf", "Legs",
                      {"Calf"}, {"weight", "reps"}),
            30, 10);
    e4.save();
    db->execMulti("SELECT * FROM workouts");
    int i = 0;
    while (db->stepExec()) {
        EXPECT_EQ(db->getColumn(1), "Calf Crusher");
        EXPECT_EQ(db->getColumn(2).getInt(), i);
        EXPECT_EQ(db->getColumn(3), "Calf Press");
        EXPECT_EQ(db->getColumn(4).getInt(), 30);
        EXPECT_EQ(db->getColumn(5).getInt(), 10);
    }
}

TEST_F(BodyTest, InvalidExcerciseSaveTest) {
    e1.addExcercise(
        Excercise("Front flips", "Do a front flip", "Cardio", {""}, {"reps"}),
        100, 0);
    EXPECT_FALSE(e1.save());
}
