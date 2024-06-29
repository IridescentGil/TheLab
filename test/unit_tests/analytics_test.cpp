#include "analytics.h"

#include <gtest/gtest.h>

#include <chrono>
#include <cstddef>
#include <cstdio>
#include <map>
#include <memory>
#include <tuple>
#include <vector>

#include "database.h"
#include "excercise.h"
#include "history.h"
#include "testHelper.h"

class AnalyticsTest : public testing::Test {
   protected:
    void SetUp() override {}
    void TearDown() override {
        remove("thelab.db");
        remove("empty.db");
    }

    std::shared_ptr<Lab::DBConn> db = std::make_shared<Lab::DBConn>();
    std::shared_ptr<Lab::DBConn> emptyDb =
        std::make_shared<Lab::DBConn>("empty.db");

    constexpr static std::chrono::time_point<std::chrono::system_clock>
        tpMarch4 = std::chrono::sys_days{std::chrono::March / 4 / 2024} +
                   std::chrono::hours(12);

    constexpr static std::chrono::time_point<std::chrono::system_clock>
        tpJan13 = std::chrono::sys_days{std::chrono::January / 13 / 2024} +
                  std::chrono::hours(12);

    constexpr static std::chrono::time_point<std::chrono::system_clock>
        tpFeb20 = std::chrono::sys_days{std::chrono::February / 20 / 2024} +
                  std::chrono::hours(12);

    constexpr static std::chrono::time_point<std::chrono::system_clock>
        tpFeb10 = std::chrono::sys_days{std::chrono::February / 10 / 2024} +
                  std::chrono::hours(12);

    constexpr static std::chrono::time_point<std::chrono::system_clock>
        tpJanWeek2 = std::chrono::time_point_cast<std::chrono::weeks>(tpJan13);

    constexpr static std::chrono::time_point<std::chrono::system_clock>
        tpFebWeek1 = std::chrono::time_point_cast<std::chrono::weeks>(tpFeb10);

    constexpr static std::chrono::time_point<std::chrono::system_clock>
        tpFebWeek3 = std::chrono::time_point_cast<std::chrono::weeks>(tpFeb20);

    constexpr static std::chrono::time_point<std::chrono::system_clock>
        tpMarchWeek1 =
            std::chrono::time_point_cast<std::chrono::weeks>(tpMarch4);

    constexpr static std::chrono::time_point<std::chrono::system_clock> tpFeb =
        std::chrono::time_point_cast<std::chrono::months>(tpFeb10);

    constexpr static std::chrono::time_point<std::chrono::system_clock>
        tpMarch = std::chrono::time_point_cast<std::chrono::months>(tpMarch4);

    constexpr static std::chrono::time_point<std::chrono::system_clock> tpJan =
        std::chrono::time_point_cast<std::chrono::months>(tpJan13);

    constexpr static std::chrono::time_point<std::chrono::system_clock> tp2024 =
        std::chrono::time_point_cast<std::chrono::years>(tpJan13);

    Lab::History emptyHist{emptyDb};

    Lab::History hist{
        db,
        {std::make_tuple(
             tpMarch4, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpMarch4, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpMarch4, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpMarch4, "Full-Body Workout",
             Lab::Excercise("Barbell Row",
                            "Standing bent over row with Barbell", "Back",
                            {"Upper-back", "Lats"}, {"weight", "reps"}),
             75, 10),
         std::make_tuple(
             tpMarch4, "Full-Body Workout",
             Lab::Excercise("Barbell Row",
                            "Standing bent over row with Barbell", "Back",
                            {"Upper-back", "Lats"}, {"weight", "reps"}),
             75, 10),
         std::make_tuple(
             tpMarch4, "Full-Body Workout",
             Lab::Excercise("Barbell Row",
                            "Standing bent over row with Barbell", "Back",
                            {"Upper-back", "Lats"}, {"weight", "reps"}),
             75, 10),
         std::make_tuple(
             tpFeb10, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpFeb10, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpFeb10, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpFeb20, "Full-Body Workout",
             Lab::Excercise("Barbell Row",
                            "Standing bent over row with Barbell", "Back",
                            {"Upper-back", "Lats"}, {"weight", "reps"}),
             75, 10),
         std::make_tuple(
             tpFeb20, "Full-Body Workout",
             Lab::Excercise("Barbell Row",
                            "Standing bent over row with Barbell", "Back",
                            {"Upper-back", "Lats"}, {"weight", "reps"}),
             75, 10),
         std::make_tuple(
             tpMarch4, "Full-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"time", "reps"}),
             60, 7),
         std::make_tuple(
             tpFeb20, "Full-Body Workout",
             Lab::Excercise("Barbell Row",
                            "Standing bent over row with Barbell", "Back",
                            {"Upper-back", "Lats"}, {"weight", "reps"}),
             75, 10),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise("Barbell Row",
                            "Standing bent over row with Barbell", "Back",
                            {"Upper-back", "Lats"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpFeb20, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpFeb20, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpFeb20, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpFeb20, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             80, 4),
         std::make_tuple(
             tpFeb20, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             80, 4),
         std::make_tuple(
             tpFeb20, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             80, 4),
         std::make_tuple(
             tpFeb20, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             40, 15),
         std::make_tuple(
             tpFeb20, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             40, 15),
         std::make_tuple(
             tpFeb20, "Upper-Body Workout",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "reps"}),
             40, 15),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise("Barbell Row",
                            "Standing bent over row with Barbell", "Back",
                            {"Upper-back", "Lats"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise("Barbell Row",
                            "Standing bent over row with Barbell", "Back",
                            {"Upper-back", "Lats"}, {"weight", "reps"}),
             60, 10),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise("Dumbbell Flys",
                            "Standing bent forward, lift dumbbell outwards",
                            "Back", {"Upper-back"}, {"weight", "reps"}),
             20, 12),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise("Dumbbell Flys",
                            "Standing bent forward, lift dumbbell outwards",
                            "Back", {"Upper-back"}, {"weight", "reps"}),
             20, 12),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise("Dumbbell Flys",
                            "Standing bent forward, lift dumbbell outwards",
                            "Back", {"Upper-back"}, {"weight", "reps"}),
             20, 12),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise("Wide Grip Pull Ups",
                            "Holding the bar with a wide grip, do a "
                            "pullup until your chin is over the bar",
                            "Back", {"Upper-back"}, {"reps"}),
             10, 0),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise("Wide Grip Pull Ups",
                            "Holding the bar with a wide grip, do a "
                            "pullup until your chin is over the bar",
                            "Back", {"Upper-back"}, {"reps"}),
             10, 0),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise("Wide Grip Pull Ups",
                            "Holding the bar with a wide grip, do a "
                            "pullup until your chin is over the bar",
                            "Back", {"Upper-back"}, {"reps"}),
             10, 0),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise(
                 "Jumping Jacks",
                 "Jump and spread your legs and put your arms to the sky",
                 "Cardio", {""}, {"reps"}),
             20, 0),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise(
                 "Jumping Jacks",
                 "Jump and spread your legs and put your arms to the sky",
                 "Cardio", {""}, {"reps"}),
             20, 0),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise(
                 "Jumping Jacks",
                 "Jump and spread your legs and put your arms to the sky",
                 "Cardio", {""}, {"reps"}),
             20, 0),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise(
                 "Plank",
                 "In pushup position, lift yourself on your elbows and toes",
                 "Core", {"Abs"}, {"time", "reps"}),
             60, 10),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise(
                 "Plank",
                 "In pushup position, lift yourself on your elbows and toes",
                 "Core", {"Abs"}, {"time", "reps"}),
             60, 10),
         std::make_tuple(
             tpJan13, "Full-Body Workout",
             Lab::Excercise(
                 "Plank",
                 "In pushup position, lift yourself on your elbows and toes",
                 "Core", {"Abs"}, {"time", "reps"}),
             60, 10),
         std::make_tuple(
             tpJan13, "Run",
             Lab::Excercise("Running (Treadmill)", "Running on a treadmill",
                            "Cardio", {}, {"distance", "time"}),
             6, 60),
         std::make_tuple(
             tpMarch4, "Run",
             Lab::Excercise("Barbell Overhead Press",
                            "Standing bent over row with Barbell", "Chest",
                            {"Pectoral", "Tricep"}, {"weight", "time"}),
             25, 30)}};
};

TEST_F(AnalyticsTest, RepMaxTest) {
    std::map<size_t, float> repMax = Lab::Analytics::mapRepEstimates(12, 30);
    std::map<size_t, float> repTest{{1, 43.2},   {2, 41.04},   {3, 38.88},
                                    {4, 38.016}, {5, 37.152},  {6, 35.856},
                                    {7, 34.56},  {8, 33.696},  {9, 32.832},
                                    {10, 32.4},  {11, 31.104}, {12, 30}};
    for (auto iter = repMax.cbegin(), bter = repTest.cbegin();
         iter != repMax.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}

TEST_F(AnalyticsTest, MapHighestValuesTest) {
    Lab::Excercise exc{"Barbell Overhead Press",
                       "Standing bent over row with Barbell",
                       "Chest",
                       {"Pectoral", "Tricep"},
                       {"weight", "reps"}};
    auto maxRep = Lab::Analytics::mapHighestValues("reps", exc, hist);
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        testRep{
            {std::chrono::time_point_cast<std::chrono::days>(tpFeb10), 10},
            {std::chrono::time_point_cast<std::chrono::days>(tpFeb20), 15},
            {std::chrono::time_point_cast<std::chrono::days>(tpMarch4), 10}};
    EXPECT_EQ(maxRep.size(), testRep.size());
    for (auto iter = maxRep.cbegin(), bter = testRep.cbegin();
         iter != maxRep.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}

TEST_F(AnalyticsTest, MapHighestValuesNoValuesInHistoryTest) {
    Lab::Excercise exc{
        "Chest Supported Row",
        "Laying on inclined bench facing the bench row with dumbbells",
        "Back",
        {"Upper-back", "Lats"},
        {"weight", "reps"}};
    auto maxRep = Lab::Analytics::mapHighestValues("reps", exc, hist);
    EXPECT_TRUE(maxRep.empty());
}

TEST_F(AnalyticsTest, MapHighestValuesNoHistoryTest) {
    Lab::Excercise exc{"Barbell Overhead Press",
                       "Standing bent over row with Barbell",
                       "Chest",
                       {"Pectoral", "Tricep"},
                       {"weight", "reps"}};
    auto maxRep = Lab::Analytics::mapHighestValues("reps", exc, emptyHist);
    EXPECT_TRUE(maxRep.empty());
}

TEST_F(AnalyticsTest, MapHighestValuesExcerciseHasWrongTypesTest) {
    Lab::Excercise exc{"Running (Treadmill)",
                       "Running on a treadmill",
                       "Cardio",
                       {},
                       {"distance", "time"}};
    auto maxRep = Lab::Analytics::mapHighestValues("reps", exc, hist);
    EXPECT_TRUE(maxRep.empty());
}

TEST_F(AnalyticsTest, MapHighestValuesExcerciseTypeChangedTest) {
    Lab::Excercise exc{"Barbell Overhead Press",
                       "Standing bent over row with Barbell",
                       "Chest",
                       {"Pectoral", "Tricep"},
                       {"time", "reps"}};
    auto maxRep = Lab::Analytics::mapHighestValues("reps", exc, hist);
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        testRep{{std::chrono::time_point_cast<std::chrono::days>(tpMarch4), 7}};
    EXPECT_EQ(maxRep.size(), testRep.size());
    for (auto iter = maxRep.cbegin(), bter = testRep.cbegin();
         iter != maxRep.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}

TEST_F(AnalyticsTest, MapHighestValuesVolumeTest) {
    Lab::Excercise exc{"Barbell Overhead Press",
                       "Standing bent over row with Barbell",
                       "Chest",
                       {"Pectoral", "Tricep"},
                       {"weight", "reps"}};
    auto maxVolume = Lab::Analytics::mapHighestValues("volume", exc, hist);
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        testVolume{
            {std::chrono::time_point_cast<std::chrono::days>(tpFeb10), 600},
            {std::chrono::time_point_cast<std::chrono::days>(tpFeb20), 600},
            {std::chrono::time_point_cast<std::chrono::days>(tpMarch4), 600}};
    EXPECT_EQ(maxVolume.size(), testVolume.size());
    for (auto iter = maxVolume.cbegin(), bter = testVolume.cbegin();
         iter != maxVolume.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}

TEST_F(AnalyticsTest, MapHighestValuesPaceTest) {
    Lab::Excercise exc{"Running (Treadmill)",
                       "Running on a treadmill",
                       "Cardio",
                       {},
                       {"distance", "time"}};
    auto maxPace = Lab::Analytics::mapHighestValues("pace", exc, hist);
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        testPace{
            {std::chrono::time_point_cast<std::chrono::days>(tpJan13), 10}};
    EXPECT_EQ(maxPace.size(), testPace.size());
    for (auto iter = maxPace.cbegin(), bter = testPace.cbegin();
         iter != maxPace.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}

TEST_F(AnalyticsTest, MapWeightForRepTest) {
    Lab::Excercise exc{"Barbell Overhead Press",
                       "Standing bent over row with Barbell",
                       "Chest",
                       {"Pectoral", "Tricep"},
                       {"weight", "reps"}};
    auto maxWeightForRep = Lab::Analytics::mapWeightForRep(exc, hist);
    std::map<
        size_t,
        std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>>
        testWFR{
            {4,
             {{std::chrono::time_point_cast<std::chrono::days>(tpFeb20), 80}}},
            {10,
             {{std::chrono::time_point_cast<std::chrono::days>(tpFeb10), 60},
              {std::chrono::time_point_cast<std::chrono::days>(tpFeb20), 60},
              {std::chrono::time_point_cast<std::chrono::days>(tpMarch4), 60}}},
            {15,
             {{std::chrono::time_point_cast<std::chrono::days>(tpFeb20), 40}}}};
    EXPECT_EQ(maxWeightForRep.size(), testWFR.size());
    for (auto iter = maxWeightForRep.cbegin(), bter = testWFR.cbegin();
         iter != maxWeightForRep.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second.size(), bter->second.size());
        for (auto xter = iter->second.cbegin(), yter = bter->second.cbegin();
             xter != iter->second.cend(); ++xter, ++yter) {
            EXPECT_EQ(xter->first, yter->first);
            EXPECT_EQ(xter->second, yter->second);
        }
    }
}

TEST_F(AnalyticsTest, MapWeightForRepNoHistoryTest) {
    Lab::Excercise exc{"Barbell Overhead Press",
                       "Standing bent over row with Barbell",
                       "Chest",
                       {"Pectoral", "Tricep"},
                       {"weight", "reps"}};
    auto maxWeightForRep = Lab::Analytics::mapWeightForRep(exc, emptyHist);
    EXPECT_TRUE(maxWeightForRep.empty());
}

TEST_F(AnalyticsTest, MapWeightForRepNoValuesInHistoryTest) {
    Lab::Excercise exc{
        "Chest Supported Row",
        "Laying on inclined bench facing the bench row with dumbbells",
        "Back",
        {"Upper-back", "Lats"},
        {"weight", "reps"}};
    auto maxWeightForRep = Lab::Analytics::mapWeightForRep(exc, hist);
    EXPECT_TRUE(maxWeightForRep.empty());
}

TEST_F(AnalyticsTest, MapWeightForRepNoWrongTypeTest) {
    Lab::Excercise exc{"Running (Treadmill)",
                       "Running on a treadmill",
                       "Cardio",
                       {},
                       {"distance", "time"}};
    auto maxWeightForRep = Lab::Analytics::mapWeightForRep(exc, hist);
    EXPECT_TRUE(maxWeightForRep.empty());
}

TEST_F(AnalyticsTest, MapTotalValuesTest) {
    Lab::Excercise exc{"Barbell Overhead Press",
                       "Standing bent over row with Barbell",
                       "Chest",
                       {"Pectoral", "Tricep"},
                       {"weight", "reps"}};
    auto workoutVolume = Lab::Analytics::mapTotalValues("volume", exc, hist);
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        testVolume{
            {std::chrono::time_point_cast<std::chrono::days>(tpFeb10), 1800},
            {std::chrono::time_point_cast<std::chrono::days>(tpFeb20), 4560},
            {std::chrono::time_point_cast<std::chrono::days>(tpMarch4), 1800}};
    EXPECT_EQ(workoutVolume.size(), testVolume.size());
    for (auto iter = workoutVolume.cbegin(), bter = testVolume.cbegin();
         iter != workoutVolume.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}

TEST_F(AnalyticsTest, MapTotalValuesNoHistoryTest) {
    Lab::Excercise exc{"Barbell Overhead Press",
                       "Standing bent over row with Barbell",
                       "Chest",
                       {"Pectoral", "Tricep"},
                       {"weight", "reps"}};
    auto workoutVolume =
        Lab::Analytics::mapTotalValues("volume", exc, emptyHist);
    EXPECT_TRUE(workoutVolume.empty());
}

TEST_F(AnalyticsTest, MapTotalValuesExcerciseHasNoValuesInHistoryTest) {
    Lab::Excercise exc{
        "Chest Supported Row",
        "Laying on inclined bench facing the bench row with dumbbells",
        "Back",
        {"Upper-back", "Lats"},
        {"weight", "reps"}};
    auto workoutVolume = Lab::Analytics::mapTotalValues("volume", exc, hist);
    EXPECT_TRUE(workoutVolume.empty());
}

TEST_F(AnalyticsTest, MapValuesPerPeriodDayTest) {
    auto workoutSets =
        Lab::Analytics::mapValuesPerPeriod("sets", std::chrono::days(), hist);
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        testSets{
            {std::chrono::time_point_cast<std::chrono::days>(tpJan13), 16},
            {std::chrono::time_point_cast<std::chrono::days>(tpFeb10), 3},
            {std::chrono::time_point_cast<std::chrono::days>(tpFeb20), 12},
            {std::chrono::time_point_cast<std::chrono::days>(tpMarch4), 8}};
    EXPECT_EQ(workoutSets.size(), testSets.size());
    for (auto iter = workoutSets.cbegin(), bter = testSets.cbegin();
         iter != workoutSets.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}

TEST_F(AnalyticsTest, MapValuesPerPeriodWeekTest) {
    auto workoutVolume = Lab::Analytics::mapValuesPerPeriod(
        "volume", std::chrono::weeks(), hist);
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        testVolume{{tpJanWeek2, 2520},
                   {tpFebWeek1, 1800},
                   {tpFebWeek3, 6810},
                   {tpMarchWeek1, 4050}};
    EXPECT_EQ(workoutVolume.size(), testVolume.size());
    for (auto iter = workoutVolume.cbegin(), bter = testVolume.cbegin();
         iter != workoutVolume.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}

TEST_F(AnalyticsTest, MapValuesPerPeriodMonthTest) {
    auto workoutWorkouts = Lab::Analytics::mapValuesPerPeriod(
        "workouts", std::chrono::months(), hist);
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        testWorkouts{{tpJan, 1}, {tpFeb, 2}, {tpMarch, 1}};
    EXPECT_EQ(workoutWorkouts.size(), testWorkouts.size());
    for (auto iter = workoutWorkouts.cbegin(), bter = testWorkouts.cbegin();
         iter != workoutWorkouts.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}

TEST_F(AnalyticsTest, MapValuesPerPeriodYearTest) {
    auto workoutReps =
        Lab::Analytics::mapValuesPerPeriod("reps", std::chrono::years(), hist);
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        testReps{{tp2024, 400}};
    EXPECT_EQ(workoutReps.size(), testReps.size());
    for (auto iter = workoutReps.cbegin(), bter = testReps.cbegin();
         iter != workoutReps.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}

TEST_F(AnalyticsTest, MapValuesPerPeriodNoHistoryTest) {
    auto data = Lab::Analytics::mapValuesPerPeriod(
        "sets", std::chrono::months(), emptyHist);
    EXPECT_TRUE(data.empty());
}

TEST_F(AnalyticsTest, dateConstraintTest) {
    Lab::Excercise exc{"Barbell Overhead Press",
                       "Standing bent over row with Barbell",
                       "Chest",
                       {"Pectoral", "Tricep"},
                       {"weight", "reps"}};
    auto maxRep = Lab::Analytics::constrainDate(
        Lab::Analytics::mapHighestValues("reps", exc, hist),
        std::chrono::sys_days{std::chrono::February / 19 / 2024}, tpMarch4);
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        testRep{
            {std::chrono::time_point_cast<std::chrono::days>(tpFeb20), 15},
            {std::chrono::time_point_cast<std::chrono::days>(tpMarch4), 10}};
    EXPECT_EQ(maxRep.size(), testRep.size());
    for (auto iter = maxRep.cbegin(), bter = testRep.cbegin();
         iter != maxRep.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}

TEST_F(AnalyticsTest, dateConstraintWeightForRepTest) {
    Lab::Excercise exc{"Barbell Overhead Press",
                       "Standing bent over row with Barbell",
                       "Chest",
                       {"Pectoral", "Tricep"},
                       {"weight", "reps"}};
    auto weightForRep = Lab::Analytics::constrainDate(
        Lab::Analytics::mapWeightForRep(exc, hist),
        std::chrono::sys_days{std::chrono::February / 19 / 2024}, tpMarch4);
    std::map<
        size_t,
        std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>>
        testWFR{
            {4,
             {{std::chrono::time_point_cast<std::chrono::days>(tpFeb20), 80}}},
            {10,
             {{std::chrono::time_point_cast<std::chrono::days>(tpFeb20), 60},
              {std::chrono::time_point_cast<std::chrono::days>(tpMarch4), 60}}},
            {15,
             {{std::chrono::time_point_cast<std::chrono::days>(tpFeb20), 40}}}};
    EXPECT_EQ(weightForRep.size(), testWFR.size());
    for (auto iter = weightForRep.cbegin(), bter = testWFR.cbegin();
         iter != weightForRep.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second.size(), bter->second.size());
        for (auto xter = iter->second.cbegin(), yter = bter->second.cbegin();
             xter != iter->second.cend(); ++xter, ++yter) {
            EXPECT_EQ(xter->first, yter->first);
            EXPECT_EQ(xter->second, yter->second);
        }
    }
}

TEST_F(AnalyticsTest, dateConstraintNoDataBetweenDatesTest) {
    auto data = Lab::Analytics::constrainDate(
        Lab::Analytics::mapValuesPerPeriod("sets", std::chrono::months(), hist),
        std::chrono::sys_days{std::chrono::March / 10 / 2024},
        std::chrono::sys_days{std::chrono::May / 25 / 2024});
    EXPECT_TRUE(data.empty());
}

TEST_F(AnalyticsTest, dateConstraintEndDateBeforeStartDateTest) {
    auto data = Lab::Analytics::constrainDate(
        Lab::Analytics::mapValuesPerPeriod("sets", std::chrono::months(), hist),
        std::chrono::sys_days{std::chrono::March / 10 / 2024},
        std::chrono::sys_days{std::chrono::February / 5 / 2024});
    EXPECT_TRUE(data.empty());
}

TEST_F(AnalyticsTest, dateConstraintSameStartEndDateTest) {
    auto workoutWorkouts = Lab::Analytics::constrainDate(
        Lab::Analytics::mapValuesPerPeriod("workouts", std::chrono::months(),
                                           hist),
        std::chrono::sys_days{std::chrono::March / 1 / 2024},
        std::chrono::sys_days{std::chrono::March / 1 / 2024} +
            std::chrono::hours(24));
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        testWorkouts{{tpMarch, 1}};
    EXPECT_EQ(workoutWorkouts.size(), testWorkouts.size());
    for (auto iter = workoutWorkouts.cbegin(), bter = testWorkouts.cbegin();
         iter != workoutWorkouts.cend(); ++iter, ++bter) {
        EXPECT_EQ(iter->first, bter->first);
        EXPECT_EQ(iter->second, bter->second);
    }
}
