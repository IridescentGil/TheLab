#include "database.h"

#include <SQLiteCpp/Database.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

class DatabaseTest : public testing::Test {
   protected:
    void SetUp() override {}
    void TearDown() override {
        remove("thelab.db");
        remove("test.db");
    }

    DBConn db1;
    DBConn db2;
    DBConn db3{"test.db"};
    DBConn db4{"test.db"};
    std::shared_ptr<DBConn> db5 = std::make_shared<DBConn>();
    std::shared_ptr<DBConn> db6;
};

TEST_F(DatabaseTest, NewDBTest) {
    SQLite::Database testdb1("thelab.db");
    SQLite::Database testdb2("test.db");
    EXPECT_TRUE(testdb1.tableExists("bodyStats"));
    EXPECT_TRUE(testdb1.tableExists("workouts"));
    EXPECT_TRUE(testdb1.tableExists("excercises"));
    EXPECT_TRUE(testdb1.tableExists("history"));
    EXPECT_TRUE(testdb2.tableExists("bodyStats"));
    EXPECT_TRUE(testdb2.tableExists("workouts"));
    EXPECT_TRUE(testdb2.tableExists("excercises"));
    EXPECT_TRUE(testdb2.tableExists("history"));
}

TEST_F(DatabaseTest, SetDBTest) {
    std::string quer =
        "INSERT INTO excercises (name, description, muscleGroup, "
        "musclesTargeted, type) VALUES ('Deadlift', 'Lift Bar off the floor', "
        "'Leg', 'Hamstring,Glutes,Lower-Back', 'Weight,Reps')";
    EXPECT_EQ(db1.exec(quer), 1);
    quer = "SELECT musclesTargeted FROM excercises";
    EXPECT_EQ(db1.execMulti(quer), 1);
    EXPECT_TRUE(db1.stepExec());
    EXPECT_EQ(db1.getColumn(0).getString(),
              static_cast<std::string>("Hamstring,Glutes,Lower-Back"));
    quer = "SELECT musclesTargeted FROM excercises WHERE name = 'Bench Press'";
    EXPECT_EQ(db1.execMulti(quer), 1);
    EXPECT_FALSE(db1.stepExec());
    // EXPECT_EQ(db1.getColumn(0), static_cast<std::string>(""));
}

TEST_F(DatabaseTest, PrepareIndexFunctionTest) {
    EXPECT_EQ(
        db2.prepareI("INSERT INTO workouts (workoutName, ExOrderNum, "
                     "excercise, type1, type2) "
                     "VALUES (?, ?, ?, ?, ?), (?, ?, ?, ?, ?), (?, ?, ?, ?, ?)",
                     1, "Push 1", 0, "Bench Press", 20, 15, "Push 1", 1,
                     "Shoulder Press", 2, 8, "Pull 1", 0, "Pull up", 15, 0),
        1);
    EXPECT_EQ(db2.execQuery(), 1);
    EXPECT_EQ(db2.execMulti("SELECT * FROM workouts"), 1);
    EXPECT_TRUE(db2.stepExec());
    EXPECT_EQ(db2.getColumn(1).getString(), static_cast<std::string>("Push 1"));
    EXPECT_EQ(static_cast<int>(db2.getColumn(2)), 0);
    EXPECT_EQ(db2.getColumn(3).getString(),
              static_cast<std::string>("Bench Press"));
    EXPECT_EQ(static_cast<int>(db2.getColumn(4)), 20);
    EXPECT_EQ(static_cast<int>(db2.getColumn(5)), 15);
    EXPECT_TRUE(db2.stepExec());
    EXPECT_EQ(db2.getColumn(1).getString(), static_cast<std::string>("Push 1"));
    EXPECT_EQ(static_cast<int>(db2.getColumn(2)), 1);
    EXPECT_EQ(db2.getColumn(3).getString(),
              static_cast<std::string>("Shoulder Press"));
    EXPECT_EQ(static_cast<int>(db2.getColumn(4)), 2);
    EXPECT_EQ(static_cast<int>(db2.getColumn(5)), 8);
    EXPECT_TRUE(db2.stepExec());
    EXPECT_EQ(db2.getColumn(1).getString(), static_cast<std::string>("Pull 1"));
    EXPECT_EQ(static_cast<int>(db2.getColumn(2)), 0);
    EXPECT_EQ(db2.getColumn(3).getString(),
              static_cast<std::string>("Pull up"));
    EXPECT_EQ(static_cast<int>(db2.getColumn(4)), 15);
    EXPECT_EQ(static_cast<int>(db2.getColumn(5)), 0);
    EXPECT_FALSE(db2.stepExec());
}

TEST_F(DatabaseTest, PrepareVariadicFunctionTest) {
    EXPECT_EQ(db2.prepare("UPDATE workouts SET workoutName = ?, type1 = ? "
                          "WHERE workoutName = ?",
                          "Incline Bench Press", 15, "Bench Press"),
              1);
    EXPECT_EQ(db2.execQuery(), 1);
    EXPECT_EQ(
        db2.execMulti(
            "SELECT * FROM workouts WHERE workoutName = 'Incline Bench Press'"),
        1);
    EXPECT_TRUE(db2.stepExec());
    EXPECT_EQ(db2.getColumn(1).getString(), static_cast<std::string>("Push 1"));
    EXPECT_EQ(static_cast<int>(db2.getColumn(2)), 0);
    EXPECT_EQ(db2.getColumn(3).getString(),
              static_cast<std::string>("Incline Bench Press"));
    EXPECT_EQ(static_cast<int>(db2.getColumn(4)), 15);
    EXPECT_EQ(static_cast<int>(db2.getColumn(5)), 15);
    EXPECT_FALSE(db2.stepExec());
}

TEST_F(DatabaseTest, SameDBTest) {
    EXPECT_EQ(
        db3.exec("INSERT INTO excercises (name, description, muscleGroup, "
                 "musclesTargeted, type) VALUES ('Lying Leg Raise', 'Lying on "
                 "the ground raise your legs to a 90 degree angle while "
                 "keeping them straight', 'Core', 'Abs', 'reps') "),
        1);
    EXPECT_EQ(db4.execMulti("SELECT * FROM excercises"), 1);
    EXPECT_TRUE(db4.stepExec());
    EXPECT_EQ(db4.getColumn(0).getString(),
              static_cast<std::string>("Lying Leg Raise"));
    EXPECT_EQ(
        db4.getColumn(1).getString(),
        static_cast<std::string>(
            "Lying on the ground raise your legs to a 90 degree angle while "
            "keeping them straight"));
    EXPECT_EQ(db4.getColumn(2).getString(), static_cast<std::string>("Core"));
    EXPECT_EQ(db4.getColumn(3).getString(), static_cast<std::string>("Abs"));
    EXPECT_EQ(db4.getColumn(4).getString(), static_cast<std::string>("reps"));
}

TEST_F(DatabaseTest, SharedPtrDBTest) {
    EXPECT_EQ(
        db5->exec("INSERT INTO excercises (name, description, muscleGroup, "
                  "musclesTargeted, type) VALUES ('Lying Leg Raise', 'Lying on "
                  "the ground raise your legs to a 90 degree angle while "
                  "keeping them straight', 'Core', 'Abs', 'reps') "),
        1);
    EXPECT_EQ(db5->execMulti(
                  "SELECT * FROM excercises WHERE name = 'Lying Leg Raise'"),
              1);
    db6 = db5;
    EXPECT_TRUE(db6->stepExec());
    EXPECT_EQ(db6->getColumn(0).getString(),
              static_cast<std::string>("Lying Leg Raise"));
    EXPECT_FALSE(db6->stepExec());
}

TEST_F(DatabaseTest, SanatizeQueryTest) {
    SQLite::Database testdb1("thelab.db");
    EXPECT_EQ(db1.prepare("SELECT * FROM ?", "history"), 1);
    EXPECT_EQ(db1.execQuery(), 1);
    EXPECT_FALSE(db1.stepExec());
    EXPECT_EQ(db1.prepare("SELECT * FROM ?", "history'; DROP TABLE history;"),
              1);
    EXPECT_EQ(db1.execQuery(), 1);
    EXPECT_FALSE(db1.stepExec());
    EXPECT_TRUE(testdb1.tableExists("history"));
}
