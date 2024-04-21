#include "history.h"

#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <tuple>
#include <vector>

#include "database.h"
#include "excercise.h"

class HistoryTest : public testing::Test {
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
    // TODO:
    History h1{db,
               {std::make_tuple(std::chrono::system_clock::now(),
                                "Full-Body Workout", Excercise(), 60, 10)}};
    History h2{db};
    // Time 3 different days
    History h3{db};
    History h4{db};
};

TEST_F(HistoryTest, GetEmptyHistoryTest) {
    auto hist = h2.getHistory();
    EXPECT_TRUE(hist.empty());
    hist = h1.getHistory();
    EXPECT_FALSE(hist.empty());
}

TEST_F(HistoryTest, GetHistoryTest) {
    // TODO:
}

TEST_F(HistoryTest, GetHistorySliceTest) {
    // TODO:
}

TEST_F(HistoryTest, GetHistoryEmptySliceTest) {
    // TODO:
}

TEST_F(HistoryTest, GetHistoryBadSliceTest) {
    // TODO:
}

TEST_F(HistoryTest, GetHistoryItemTest) {
    // TODO:
}

TEST_F(HistoryTest, GetMultipleHistoryItemsTest) {
    // TODO:
}

TEST_F(HistoryTest, GetMultipleHistoryItemsBadIteratorTest) {
    // TODO: bad start iterator, reversed start and end iterators
}

TEST_F(HistoryTest, OverrideExistingHistoryTest) {
    // TODO:
}

TEST_F(HistoryTest, AddItemToEmptyHistoryTest) {
    // TODO:
}

TEST_F(HistoryTest, AppendHistoryTest) {
    // TODO:
}

TEST_F(HistoryTest, RemoveItemTest) {
    // TODO:
}

TEST_F(HistoryTest, RemoveMultipleItemsTest) {
    // TODO:
}

TEST_F(HistoryTest, RemoveMultipleItemsBadIteratorTest) {
    // TODO: bad start iterator, reversed start and end iterators
}

TEST_F(HistoryTest, SaveHistoryTest) {
    // TODO:
}

TEST_F(HistoryTest, InvalidSaveHistoryTest) {
    // TODO:
}
