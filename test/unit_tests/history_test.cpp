#include "history.h"

#include <gtest/gtest.h>

#include <chrono>
#include <string>
#include <tuple>
#include <vector>

#include "database.h"
#include "excercise.h"
#include "testHelper.h"

class HistoryTest : public testing::Test {
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
                           {"weight", "reps"})};
        for (auto const& iter : testEx) {
            std::string musclesWorked;
            std::string type;
            for (const auto& bter : iter.getMusclesWorked()) {
                musclesWorked += bter;
                if (*(iter.getMusclesWorked().end() - 1) != bter) {
                    musclesWorked += ", ";
                }
            }
            for (const auto& bter : iter.getType()) {
                type += bter;
                if (*(iter.getType().end() - 1) != bter) {
                    type += ", ";
                }
            }
            db->prepare(
                "INSERT INTO excercises (name, description, muscleGroup, "
                "musclesTargeted, type) VALUES (?, ?, ?, ?, ?)",
                iter.getName(), iter.getDescription(), iter.getMuscleGroup(), musclesWorked, type);
            db->execQuery();
        }
    }
    void TearDown() override { remove("thelab.db"); }

   public:
    std::shared_ptr<Lab::DBConn> db = std::make_shared<Lab::DBConn>();

    constexpr static std::chrono::time_point<std::chrono::system_clock> tpMarch4 =
        std::chrono::sys_days{std::chrono::March / 4 / 2024} + std::chrono::hours(12);

    constexpr static std::chrono::time_point<std::chrono::system_clock> tpJan13 =
        std::chrono::sys_days{std::chrono::January / 13 / 2024} + std::chrono::hours(12);

    constexpr static std::chrono::time_point<std::chrono::system_clock> tpFeb20 =
        std::chrono::sys_days{std::chrono::February / 20 / 2024} + std::chrono::hours(12);

    constexpr static std::chrono::time_point<std::chrono::system_clock> tpFeb10 =
        std::chrono::sys_days{std::chrono::February / 10 / 2024} + std::chrono::hours(12);

    // Object to test general history class funtionality
    Lab::History h1{db,
                    {std::make_tuple(tpMarch4, "Upper-Body Workout",
                                     Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell",
                                                    "Chest", {"Pectoral", "Tricep"}, {"weight", "reps"}),
                                     60, 10),
                     std::make_tuple(tpMarch4, "Upper-Body Workout",
                                     Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell",
                                                    "Chest", {"Pectoral", "Tricep"}, {"weight", "reps"}),
                                     60, 10),
                     std::make_tuple(tpMarch4, "Upper-Body Workout",
                                     Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell",
                                                    "Chest", {"Pectoral", "Tricep"}, {"weight", "reps"}),
                                     60, 10),
                     std::make_tuple(tpMarch4, "Full-Body Workout",
                                     Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                                    {"Upper-back", "Lats"}, {"weight", "reps"}),
                                     75, 10),
                     std::make_tuple(tpMarch4, "Full-Body Workout",
                                     Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                                    {"Upper-back", "Lats"}, {"weight", "reps"}),
                                     75, 10),
                     std::make_tuple(tpMarch4, "Full-Body Workout",
                                     Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                                    {"Upper-back", "Lats"}, {"weight", "reps"}),
                                     75, 10)}};

    // Object to test functionailty with empty history
    Lab::History h2{db};

    // Object to test history date slices functionality
    Lab::History h3{db,
                    {std::make_tuple(tpFeb10, "Upper-Body Workout",
                                     Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell",
                                                    "Chest", {"Pectoral", "Tricep"}, {"weight", "reps"}),
                                     60, 10),
                     std::make_tuple(tpFeb10, "Upper-Body Workout",
                                     Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell",
                                                    "Chest", {"Pectoral", "Tricep"}, {"weight", "reps"}),
                                     60, 10),
                     std::make_tuple(tpFeb10, "Upper-Body Workout",
                                     Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell",
                                                    "Chest", {"Pectoral", "Tricep"}, {"weight", "reps"}),
                                     60, 10),
                     std::make_tuple(tpFeb20, "Full-Body Workout",
                                     Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                                    {"Upper-back", "Lats"}, {"weight", "reps"}),
                                     75, 10),
                     std::make_tuple(tpFeb20, "Full-Body Workout",
                                     Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                                    {"Upper-back", "Lats"}, {"weight", "reps"}),
                                     75, 10),
                     std::make_tuple(tpFeb20, "Full-Body Workout",
                                     Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                                    {"Upper-back", "Lats"}, {"weight", "reps"}),
                                     75, 10)}};

    // Object to test saving to database
    Lab::History h4{
        db,
        {std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                        {"Upper-back", "Lats"}, {"weight", "reps"}),
                         60, 10),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                        {"Upper-back", "Lats"}, {"weight", "reps"}),
                         60, 10),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                        {"Upper-back", "Lats"}, {"weight", "reps"}),
                         60, 10),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards", "Back",
                                        {"Upper-back"}, {"weight", "reps"}),
                         20, 12),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards", "Back",
                                        {"Upper-back"}, {"weight", "reps"}),
                         20, 12),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Dumbbell Flys", "Standing bent forward, lift dumbbell outwards", "Back",
                                        {"Upper-back"}, {"weight", "reps"}),
                         20, 12),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Wide Grip Pull Ups",
                                        "Holding the bar with a wide grip, do a "
                                        "pullup until your chin is over the bar",
                                        "Back", {"Upper-back"}, {"reps"}),
                         10, 0),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Wide Grip Pull Ups",
                                        "Holding the bar with a wide grip, do a "
                                        "pullup until your chin is over the bar",
                                        "Back", {"Upper-back"}, {"reps"}),
                         10, 0),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Wide Grip Pull Ups",
                                        "Holding the bar with a wide grip, do a "
                                        "pullup until your chin is over the bar",
                                        "Back", {"Upper-back"}, {"reps"}),
                         10, 0),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Jumping Jacks", "Jump and spread your legs and put your arms to the sky",
                                        "Cardio", {""}, {"reps"}),
                         20, 0),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Jumping Jacks", "Jump and spread your legs and put your arms to the sky",
                                        "Cardio", {""}, {"reps"}),
                         20, 0),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Jumping Jacks", "Jump and spread your legs and put your arms to the sky",
                                        "Cardio", {""}, {"reps"}),
                         20, 0),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Plank", "In pushup position, lift yourself on your elbows and toes", "Core",
                                        {"Abs"}, {"time", "reps"}),
                         60, 10),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Plank", "In pushup position, lift yourself on your elbows and toes", "Core",
                                        {"Abs"}, {"time", "reps"}),
                         60, 10),
         std::make_tuple(tpJan13, "Full-Body Workout",
                         Lab::Excercise("Plank", "In pushup position, lift yourself on your elbows and toes", "Core",
                                        {"Abs"}, {"time", "reps"}),
                         60, 10)}};
};

TEST_F(HistoryTest, GetEmptyHistoryTest) {
    auto hist = h2.getHistory();
    EXPECT_TRUE(hist.empty());
    hist = h1.getHistory();
    EXPECT_FALSE(hist.empty());
}

TEST_F(HistoryTest, GetHistoryTest) {
    auto hist = h1.getHistory();
    EXPECT_FALSE(hist.empty());
    EXPECT_EQ(hist.size(), 6);
    int index = 0;
    for (auto const& iter : hist) {
        const auto [time, woPlan, excer, type1, type2] = iter;
        if (index < 3) {
            EXPECT_EQ(time, tpMarch4);
            EXPECT_EQ(woPlan, "Upper-Body Workout");
            EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                                Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest",
                                               {"Pectoral", "Tricep"}, {"weight", "reps"}));
            EXPECT_EQ(type1, 60);
            EXPECT_EQ(type2, 10);
        } else {
            EXPECT_EQ(time, tpMarch4);
            EXPECT_EQ(woPlan, "Full-Body Workout");
            EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                                Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                               {"Upper-back", "Lats"}, {"weight", "reps"}));
            EXPECT_EQ(type1, 75);
            EXPECT_EQ(type2, 10);
        }
        ++index;
    }
    EXPECT_EQ(index, 6);
}

TEST_F(HistoryTest, GetHistorySliceTest) {
    auto slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::February / 15 / 2024},
                                    std::chrono::sys_days{std::chrono::February / 25 / 2024});

    EXPECT_FALSE(slice.empty());
    for (auto const& iter : slice) {
        const auto [time, woPlan, excer, type1, type2] = iter;
        EXPECT_EQ(time, tpFeb20);
        EXPECT_EQ(woPlan, "Full-Body Workout");
        EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                            Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                           {"Upper-back", "Lats"}, {"weight", "reps"}));
        EXPECT_EQ(type1, 75);
        EXPECT_EQ(type2, 10);
    }

    slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::February / 05 / 2024},
                               std::chrono::sys_days{std::chrono::February / 15 / 2024});

    EXPECT_FALSE(slice.empty());
    for (auto const& iter : slice) {
        const auto [time, woPlan, excer, type1, type2] = iter;
        EXPECT_EQ(time, tpFeb10);
        EXPECT_EQ(woPlan, "Upper-Body Workout");
        EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                            Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest",
                                           {"Pectoral", "Tricep"}, {"weight", "reps"}));
        EXPECT_EQ(type1, 60);
        EXPECT_EQ(type2, 10);
    }

    slice = h3.getSliceHistory(
        std::chrono::sys_days{std::chrono::February / 10 / 2024} + std::chrono::hours(12),
        std::chrono::sys_days{std::chrono::February / 10 / 2024} + std::chrono::hours(12) + std::chrono::seconds(1));

    EXPECT_FALSE(slice.empty());
    for (auto const& iter : slice) {
        const auto [time, woPlan, excer, type1, type2] = iter;
        EXPECT_EQ(time, tpFeb10);
        EXPECT_EQ(woPlan, "Upper-Body Workout");
        EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                            Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest",
                                           {"Pectoral", "Tricep"}, {"weight", "reps"}));
        EXPECT_EQ(type1, 60);
        EXPECT_EQ(type2, 10);
    }

    slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::February / 10 / 2024},
                               std::chrono::sys_days{std::chrono::February / 10 / 2024} + std::chrono::hours(23) +
                                   std::chrono::minutes(59) + std::chrono::seconds(59) + std::chrono::milliseconds(99));

    EXPECT_FALSE(slice.empty());
    for (auto const& iter : slice) {
        const auto [time, woPlan, excer, type1, type2] = iter;
        EXPECT_EQ(time, tpFeb10);
        EXPECT_EQ(woPlan, "Upper-Body Workout");
        EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                            Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest",
                                           {"Pectoral", "Tricep"}, {"weight", "reps"}));
        EXPECT_EQ(type1, 60);
        EXPECT_EQ(type2, 10);
    }

    slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::February / 05 / 2024},
                               std::chrono::sys_days{std::chrono::February / 25 / 2024});
    int index = 0;

    EXPECT_FALSE(slice.empty());
    for (auto const& iter : slice) {
        const auto [time, woPlan, excer, type1, type2] = iter;
        if (index < 3) {
            EXPECT_EQ(time, tpFeb10);
            EXPECT_EQ(woPlan, "Upper-Body Workout");
            EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                                Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest",
                                               {"Pectoral", "Tricep"}, {"weight", "reps"}));
            EXPECT_EQ(type1, 60);
            EXPECT_EQ(type2, 10);
        } else {
            EXPECT_EQ(time, tpFeb20);
            EXPECT_EQ(woPlan, "Full-Body Workout");
            EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                                Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                               {"Upper-back", "Lats"}, {"weight", "reps"}));
            EXPECT_EQ(type1, 75);
            EXPECT_EQ(type2, 10);
        }
        ++index;
    }
    EXPECT_EQ(index, 6);

    slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::July / 05 / 2022},
                               std::chrono::sys_days{std::chrono::August / 16 / 2026});
    index = 0;

    EXPECT_FALSE(slice.empty());
    for (auto const& iter : slice) {
        const auto [time, woPlan, excer, type1, type2] = iter;
        if (index < 3) {
            EXPECT_EQ(time, tpFeb10);
            EXPECT_EQ(woPlan, "Upper-Body Workout");
            EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                                Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest",
                                               {"Pectoral", "Tricep"}, {"weight", "reps"}));
            EXPECT_EQ(type1, 60);
            EXPECT_EQ(type2, 10);
        } else {
            EXPECT_EQ(time, tpFeb20);
            EXPECT_EQ(woPlan, "Full-Body Workout");
            EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                                Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                               {"Upper-back", "Lats"}, {"weight", "reps"}));
            EXPECT_EQ(type1, 75);
            EXPECT_EQ(type2, 10);
        }
        ++index;
    }
    EXPECT_EQ(index, 6);
}

TEST_F(HistoryTest, GetHistoryEmptySliceTest) {
    auto slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::February / 10 / 2024},
                                    std::chrono::sys_days{std::chrono::February / 10 / 2024});
    EXPECT_TRUE(slice.empty());

    slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::January / 7 / 2024},
                               std::chrono::sys_days{std::chrono::February / 4 / 2024});
    EXPECT_TRUE(slice.empty());

    slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::August / 16 / 2024},
                               std::chrono::sys_days{std::chrono::September / 8 / 2024});
    EXPECT_TRUE(slice.empty());

    slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::February / 14 / 2024},
                               std::chrono::sys_days{std::chrono::February / 19 / 2024});
    EXPECT_TRUE(slice.empty());
}

TEST_F(HistoryTest, GetHistoryEmptySliceBoundries) {
    auto slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::February / 21 / 2024},
                                    std::chrono::sys_days{std::chrono::March / 9 / 2024});
    EXPECT_TRUE(slice.empty());

    slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::February / 8 / 2024},
                               std::chrono::sys_days{std::chrono::February / 9 / 2024});
    EXPECT_TRUE(slice.empty());

    slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::January / 7 / 2024},
                               std::chrono::sys_days{std::chrono::February / 9 / 2024} + std::chrono::hours(23) +
                                   std::chrono::minutes(59) + std::chrono::seconds(59) + std::chrono::milliseconds(99));
    EXPECT_TRUE(slice.empty());

    slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::February / 20 / 2024} + std::chrono::hours(24) +
                                   std::chrono::milliseconds(1),
                               std::chrono::sys_days{std::chrono::March / 9 / 2024});
    EXPECT_TRUE(slice.empty());
}

TEST_F(HistoryTest, GetHistoryBadSliceTest) {
    auto slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::August / 20 / 2024},
                                    std::chrono::sys_days{std::chrono::May / 5 / 2023});
    EXPECT_TRUE(slice.empty());
    slice = h3.getSliceHistory(std::chrono::sys_days{std::chrono::February / 20 / 2024} + std::chrono::milliseconds(1),
                               std::chrono::sys_days{std::chrono::February / 20 / 2024} - std::chrono::milliseconds(1));
    EXPECT_TRUE(slice.empty());
}

TEST_F(HistoryTest, GetHistoryItemTest) {
    auto hist = h1.getHistory();
    auto iter = hist.begin();
    ++iter;
    auto histItem = h1.getItem(iter);

    EXPECT_EQ(hist.size(), 6);
    auto [date, woPlan, excer, type1, type2] = histItem;
    EXPECT_EQ(date, tpMarch4);
    EXPECT_EQ(woPlan, "Upper-Body Workout");
    EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                        Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest",
                                       {"Pectoral", "Tricep"}, {"weight", "reps"}));
    EXPECT_EQ(type1, 60);
    EXPECT_EQ(type2, 10);
}

TEST_F(HistoryTest, GetMultipleHistoryItemsTest) {
    auto hist = h1.getHistory();
    auto histItem = h1.getItem(hist.begin() + 2, hist.begin() + 4);

    EXPECT_EQ(hist.size(), 6);
    EXPECT_EQ(histItem.size(), 2);
    auto iter = histItem.cbegin();
    {
        const auto [date, woPlan, excer, type1, type2] = *iter;
        EXPECT_EQ(date, tpMarch4);
        EXPECT_EQ(woPlan, "Upper-Body Workout");
        EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                            Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest",
                                           {"Pectoral", "Tricep"}, {"weight", "reps"}));
        EXPECT_EQ(type1, 60);
        EXPECT_EQ(type2, 10);
    }
    ++iter;
    {
        const auto [date, woPlan, excer, type1, type2] = *iter;
        EXPECT_EQ(date, tpMarch4);
        EXPECT_EQ(woPlan, "Full-Body Workout");
        EXPECT_PRED_FORMAT2(AssertExcerciseEqual, excer,
                            Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                           {"Upper-back", "Lats"}, {"weight", "reps"}));
        EXPECT_EQ(type1, 75);
        EXPECT_EQ(type2, 10);
    }
}

TEST_F(HistoryTest, OverrideExistingHistoryTest) {
    auto hist1 = h1.getHistory();
    Lab::historyVector newHist = {
        std::make_tuple(
            tpMarch4, "Arm Workout",
            Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}), 50,
            15),
        std::make_tuple(
            tpMarch4, "Arm Workout",
            Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}), 50,
            15),
        std::make_tuple(
            tpMarch4, "Arm Workout",
            Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}), 50,
            15),
        std::make_tuple(tpMarch4, "Arm Workout",
                        Lab::Excercise("Tricep Extensions", "Tricep extensions on machine pullies", "Arms", {"Triceps"},
                                       {"weight", "reps"}),
                        50, 15),
        std::make_tuple(tpMarch4, "Arm Workout",
                        Lab::Excercise("Tricep Extensions", "Tricep extensions on machine pullies", "Arms", {"Triceps"},
                                       {"weight", "reps"}),
                        50, 15),
        std::make_tuple(tpMarch4, "Arm Workout",
                        Lab::Excercise("Tricep Extensions", "Tricep extensions on machine pullies", "Arms", {"Triceps"},
                                       {"weight", "reps"}),
                        50, 15),
        std::make_tuple(tpJan13, "Marathon Practice",
                        Lab::Excercise("Running", "Long distance timed run", "Cardio", {}, {"distance", "time"}), 10,
                        100)};
    h1.setHistory(newHist);
    auto hist2 = h1.getHistory();
    EXPECT_EQ(hist1.size(), 6);
    EXPECT_PRED_FORMAT3(AssertHistoryNEqual, hist1, hist2, 7);
    EXPECT_PRED_FORMAT2(AssertHistoryEqual, newHist, hist2);
    EXPECT_EQ(hist2.size(), 7);
}

TEST_F(HistoryTest, OverrideEmptyHistoryTest) {
    auto hist1 = h2.getHistory();
    Lab::historyVector newHist = {
        std::make_tuple(tpJan13, "Marathon Practice",
                        Lab::Excercise("Running", "Long distance timed run", "Cardio", {}, {"distance", "time"}), 10,
                        100),
        std::make_tuple(tpFeb10, "Marathon Practice",
                        Lab::Excercise("Running", "Long distance timed run", "Cardio", {}, {"distance", "time"}), 10,
                        100),
        std::make_tuple(tpFeb20, "Marathon Practice",
                        Lab::Excercise("Running", "Long distance timed run", "Cardio", {}, {"distance", "time"}), 10,
                        100),
        std::make_tuple(tpMarch4, "Marathon Practice",
                        Lab::Excercise("Running", "Long distance timed run", "Cardio", {}, {"distance", "time"}), 10,
                        100)};
    h2.setHistory(newHist);
    auto hist2 = h2.getHistory();
    EXPECT_TRUE(hist1.empty());
    EXPECT_PRED_FORMAT3(AssertHistoryNEqual, hist1, hist2, 4);
    EXPECT_PRED_FORMAT2(AssertHistoryEqual, newHist, hist2);
    EXPECT_EQ(hist2.size(), 4);
}

TEST_F(HistoryTest, AddItemToEmptyHistoryTest) {
    auto hist1 = h2.getHistory();
    h2.addItem(tpMarch4, "Arm Workout",
               Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}),
               50, 15);
    h2.addItem(tpMarch4, "Arm Workout",
               Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}),
               50, 15);
    h2.addItem(tpMarch4, "Arm Workout",
               Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}),
               50, 15);
    Lab::historyVector newHist = {
        std::make_tuple(
            tpMarch4, "Arm Workout",
            Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}), 50,
            15),
        std::make_tuple(
            tpMarch4, "Arm Workout",
            Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}), 50,
            15),
        std::make_tuple(
            tpMarch4, "Arm Workout",
            Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}), 50,
            15)};
    auto hist2 = h2.getHistory();
    EXPECT_TRUE(hist1.empty());
    EXPECT_EQ(hist2.size(), 3);
    EXPECT_PRED_FORMAT2(AssertHistoryEqual, newHist, hist2);
}

TEST_F(HistoryTest, AppendHistoryTest) {
    auto hist1 = h1.getHistory();
    h1.addItem(tpMarch4, "Arm Workout",
               Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}),
               50, 15);
    h1.addItem(tpMarch4, "Arm Workout",
               Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}),
               50, 15);
    h1.addItem(tpMarch4, "Arm Workout",
               Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}),
               50, 15);
    auto hist2 = h1.getHistory();
    EXPECT_EQ(hist1.size(), 6);

    hist1.push_back(std::make_tuple(
        tpMarch4, "Arm Workout",
        Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}), 50, 15));
    hist1.push_back(std::make_tuple(
        tpMarch4, "Arm Workout",
        Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}), 50, 15));
    hist1.push_back(std::make_tuple(
        tpMarch4, "Arm Workout",
        Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}), 50, 15));
    EXPECT_EQ(hist1.size(), 9);
    EXPECT_EQ(hist2.size(), 9);
    EXPECT_PRED_FORMAT2(AssertHistoryEqual, hist1, hist2);
}

TEST_F(HistoryTest, RemoveItemTest) {
    auto& hist1 = h3.getHistory();
    EXPECT_EQ(hist1.size(), 6);
    h3.remItem(hist1.begin());
    EXPECT_EQ(hist1.size(), 5);
    h3.remItem(hist1.begin() + 3);
    EXPECT_EQ(hist1.size(), 4);
    h3.remItem(hist1.begin());
    EXPECT_EQ(hist1.size(), 3);
    Lab::historyVector newHist = {
        std::make_tuple(tpFeb10, "Upper-Body Workout",
                        Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest",
                                       {"Pectoral", "Tricep"}, {"weight", "reps"}),
                        60, 10),
        std::make_tuple(tpFeb20, "Full-Body Workout",
                        Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                       {"Upper-back", "Lats"}, {"weight", "reps"}),
                        75, 10),
        std::make_tuple(tpFeb20, "Full-Body Workout",
                        Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                       {"Upper-back", "Lats"}, {"weight", "reps"}),
                        75, 10)};
    EXPECT_PRED_FORMAT2(AssertHistoryEqual, hist1, newHist);
}

TEST_F(HistoryTest, RemoveMultipleItemsTest) {
    auto& hist1 = h3.getHistory();
    h3.remItem(hist1.begin(), hist1.begin() + 2);
    h3.remItem(hist1.begin() + 2, hist1.end());
    Lab::historyVector newHist = {
        std::make_tuple(tpFeb10, "Upper-Body Workout",
                        Lab::Excercise("Barbell Overhead Press", "Standing bent over row with Barbell", "Chest",
                                       {"Pectoral", "Tricep"}, {"weight", "reps"}),
                        60, 10),
        std::make_tuple(tpFeb20, "Full-Body Workout",
                        Lab::Excercise("Barbell Row", "Standing bent over row with Barbell", "Back",
                                       {"Upper-back", "Lats"}, {"weight", "reps"}),
                        75, 10)};
    EXPECT_EQ(hist1.size(), 2);
    EXPECT_PRED_FORMAT2(AssertHistoryEqual, hist1, newHist);
}

TEST_F(HistoryTest, SaveHistoryTest) {
    int iter = 0;
    EXPECT_TRUE(h4.save());
    db->execMulti("SELECT * FROM history");
    while (db->stepExec()) {
        if (iter < 3) {
            EXPECT_EQ(db->getColumn(1).getInt64(), tpJan13.time_since_epoch().count());
            EXPECT_EQ(db->getColumn(2).getString(), static_cast<std::string>("Full-Body Workout"));
            EXPECT_EQ(db->getColumn(3).getString(), static_cast<std::string>("Barbell Row"));
            EXPECT_EQ(db->getColumn(4).getInt(), 60);
            EXPECT_EQ(db->getColumn(5).getInt(), 10);
        } else if (iter < 6) {
            EXPECT_EQ(db->getColumn(1).getInt64(), tpJan13.time_since_epoch().count());
            EXPECT_EQ(db->getColumn(2).getString(), static_cast<std::string>("Full-Body Workout"));
            EXPECT_EQ(db->getColumn(3).getString(), static_cast<std::string>("Dumbbell Flys"));
            EXPECT_EQ(db->getColumn(4).getInt(), 20);
            EXPECT_EQ(db->getColumn(5).getInt(), 12);
        } else if (iter < 9) {
            EXPECT_EQ(db->getColumn(1).getInt64(), tpJan13.time_since_epoch().count());
            EXPECT_EQ(db->getColumn(2).getString(), static_cast<std::string>("Full-Body Workout"));
            EXPECT_EQ(db->getColumn(3).getString(), static_cast<std::string>("Wide Grip Pull Ups"));
            EXPECT_EQ(db->getColumn(4).getInt(), 10);
            EXPECT_EQ(db->getColumn(5).getInt(), 0);
        } else if (iter < 12) {
            EXPECT_EQ(db->getColumn(1).getInt64(), tpJan13.time_since_epoch().count());
            EXPECT_EQ(db->getColumn(2).getString(), static_cast<std::string>("Full-Body Workout"));
            EXPECT_EQ(db->getColumn(3).getString(), static_cast<std::string>("Jumping Jacks"));
            EXPECT_EQ(db->getColumn(4).getInt(), 20);
            EXPECT_EQ(db->getColumn(5).getInt(), 0);
        } else if (iter < 15) {
            EXPECT_EQ(db->getColumn(1).getInt64(), tpJan13.time_since_epoch().count());
            EXPECT_EQ(db->getColumn(2).getString(), static_cast<std::string>("Full-Body Workout"));
            EXPECT_EQ(db->getColumn(3).getString(), static_cast<std::string>("Plank"));
            EXPECT_EQ(db->getColumn(4).getInt(), 60);
            EXPECT_EQ(db->getColumn(5).getInt(), 10);
        }
        ++iter;
    }
}

TEST_F(HistoryTest, SaveHistoryOverwriteTest) {
    EXPECT_TRUE(h4.save());
    auto& hist = h4.getHistory();
    h4.remItem(hist.begin() + 3, hist.begin() + 12);
    EXPECT_TRUE(h4.save());
    db->execMulti("SELECT * FROM history");
    int iter = 0;
    while (db->stepExec()) {
        if (iter < 3) {
            EXPECT_EQ(db->getColumn(1).getInt64(), tpJan13.time_since_epoch().count());
            EXPECT_EQ(db->getColumn(2).getString(), static_cast<std::string>("Full-Body Workout"));
            EXPECT_EQ(db->getColumn(3).getString(), static_cast<std::string>("Barbell Row"));
            EXPECT_EQ(db->getColumn(4).getInt(), 60);
            EXPECT_EQ(db->getColumn(5).getInt(), 10);
        } else if (iter < 6) {
            EXPECT_EQ(db->getColumn(1).getInt64(), tpJan13.time_since_epoch().count());
            EXPECT_EQ(db->getColumn(2).getString(), static_cast<std::string>("Full-Body Workout"));
            EXPECT_EQ(db->getColumn(3).getString(), static_cast<std::string>("Plank"));
            EXPECT_EQ(db->getColumn(4).getInt(), 60);
            EXPECT_EQ(db->getColumn(5).getInt(), 10);
        }
        ++iter;
    }
    EXPECT_EQ(iter, 6);
}

TEST_F(HistoryTest, OverwriteEmptyHistorySaveTest) {
    EXPECT_TRUE(h4.save());
    EXPECT_TRUE(h2.save());
    db->execMulti("SELECT * FROM history");
    EXPECT_FALSE(db->stepExec());
}

TEST_F(HistoryTest, InvalidSaveHistoryTest) {
    h4.addItem(tpMarch4, "Arm Workout",
               Lab::Excercise("Dumbell Curls", "Alternate curling dumbells", "Arms", {"Biceps"}, {"weight", "reps"}),
               50, 15);
    EXPECT_FALSE(h4.save());
}
