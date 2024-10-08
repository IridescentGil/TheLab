#include "database.h"

#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Exception.h>
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

   public:
    Lab::DBConn db1;
    Lab::DBConn db2;
    Lab::DBConn db3{"test.db"};
    Lab::DBConn db4{"test.db"};
    std::shared_ptr<Lab::DBConn> db5 = std::make_shared<Lab::DBConn>();
    std::shared_ptr<Lab::DBConn> db6;
};

TEST_F(DatabaseTest, NewDBTest) {
    SQLite::Database testdb1("thelab.db");
    SQLite::Database testdb2("test.db");
    EXPECT_TRUE(testdb1.tableExists("bodyStats"));
    EXPECT_TRUE(testdb1.tableExists("workouts"));
    EXPECT_TRUE(testdb1.tableExists("exercises"));
    EXPECT_TRUE(testdb1.tableExists("history"));
    EXPECT_TRUE(testdb2.tableExists("bodyStats"));
    EXPECT_TRUE(testdb2.tableExists("workouts"));
    EXPECT_TRUE(testdb2.tableExists("exercises"));
    EXPECT_TRUE(testdb2.tableExists("history"));
}

TEST_F(DatabaseTest, SetDBTest) {
    std::string quer =
        "INSERT INTO exercises (name, description, muscleGroup, "
        "musclesTargeted, type) VALUES ('Deadlift', 'Lift Bar off the floor', "
        "'Leg', 'Hamstring,Glutes,Lower-Back', 'Weight,Reps')";
    EXPECT_EQ(db1.exec(quer), 1);
    quer = "SELECT musclesTargeted FROM exercises";
    EXPECT_EQ(db1.exec_and_retrieve(quer), 1);
    EXPECT_TRUE(db1.retrieve_next_row());
    EXPECT_EQ(db1.get_column(0).getString(), static_cast<std::string>("Hamstring,Glutes,Lower-Back"));
    quer = "SELECT musclesTargeted FROM exercises WHERE name = 'Bench Press'";
    EXPECT_EQ(db1.exec_and_retrieve(quer), 1);
    EXPECT_FALSE(db1.retrieve_next_row());
    // EXPECT_EQ(db1.getColumn(0), static_cast<std::string>(""));
}

TEST_F(DatabaseTest, PrepareIndexFunctionTest) {
    EXPECT_EQ(db2.exec("INSERT INTO exercises (name) VALUES ('Bench Press'), "
                       "('Shoulder Press'), ('Pull up')"),
              1);
    EXPECT_EQ(db2.prepare(1,
                          "INSERT INTO workouts (workoutName, exOrderNum, "
                          "exercise, type1, type2) "
                          "VALUES (?, ?, ?, ?, ?), (?, ?, ?, ?, ?), (?, ?, ?, ?, ?)",
                          "Push 1", 0, "Bench Press", 20, 15, "Push 1", 1, "Shoulder Press", 2, 8, "Pull 1", 0,
                          "Pull up", 15, 0),
              1);
    EXPECT_EQ(db2.exec_prepared(), 1);
    EXPECT_EQ(db2.exec_and_retrieve("SELECT * FROM workouts"), 1);
    EXPECT_TRUE(db2.retrieve_next_row());
    EXPECT_EQ(db2.get_column(0).getString(), static_cast<std::string>("Push 1"));
    EXPECT_EQ(static_cast<int>(db2.get_column(1)), 0);
    EXPECT_EQ(db2.get_column(2).getString(), static_cast<std::string>("Bench Press"));
    EXPECT_EQ(static_cast<int>(db2.get_column(3)), 20);
    EXPECT_EQ(static_cast<int>(db2.get_column(4)), 15);
    EXPECT_TRUE(db2.retrieve_next_row());
    EXPECT_EQ(db2.get_column(0).getString(), static_cast<std::string>("Push 1"));
    EXPECT_EQ(static_cast<int>(db2.get_column(1)), 1);
    EXPECT_EQ(db2.get_column(2).getString(), static_cast<std::string>("Shoulder Press"));
    EXPECT_EQ(static_cast<int>(db2.get_column(3)), 2);
    EXPECT_EQ(static_cast<int>(db2.get_column(4)), 8);
    EXPECT_TRUE(db2.retrieve_next_row());
    EXPECT_EQ(db2.get_column(0).getString(), static_cast<std::string>("Pull 1"));
    EXPECT_EQ(static_cast<int>(db2.get_column(1)), 0);
    EXPECT_EQ(db2.get_column(2).getString(), static_cast<std::string>("Pull up"));
    EXPECT_EQ(static_cast<int>(db2.get_column(3)), 15);
    EXPECT_EQ(static_cast<int>(db2.get_column(4)), 0);
    EXPECT_FALSE(db2.retrieve_next_row());
}

TEST_F(DatabaseTest, PrepareVariadicFunctionTest) {
    EXPECT_EQ(db2.exec("INSERT INTO exercises (name) VALUES ('Bench Press'), "
                       "('Incline Bench Press')"),
              1);
    EXPECT_EQ(db2.exec("INSERT INTO workouts (workoutName, exOrderNum, exercise, type1, "
                       "type2) VALUES ('Push 1', '0', 'Bench Press', '500', '12')"),
              1);
    EXPECT_EQ(db2.prepare("UPDATE workouts SET exercise = ?, type1 = ? "
                          "WHERE exercise = ?",
                          "Incline Bench Press", 15, "Bench Press"),
              1);
    EXPECT_EQ(db2.exec_prepared(), 1);
    EXPECT_EQ(db2.exec_and_retrieve("SELECT * FROM workouts WHERE exercise ="
                                    "'Incline Bench Press'"),
              1);
    EXPECT_TRUE(db2.retrieve_next_row());
    EXPECT_EQ(db2.get_column(0).getString(), static_cast<std::string>("Push 1"));
    EXPECT_EQ(static_cast<int>(db2.get_column(1)), 0);
    EXPECT_EQ(db2.get_column(2).getString(), static_cast<std::string>("Incline Bench Press"));
    EXPECT_EQ(static_cast<int>(db2.get_column(3)), 15);
    EXPECT_EQ(static_cast<int>(db2.get_column(4)), 12);
    EXPECT_FALSE(db2.retrieve_next_row());
}

TEST_F(DatabaseTest, SameDBTest) {
    EXPECT_EQ(db3.exec("INSERT INTO exercises (name, description, muscleGroup, "
                       "musclesTargeted, type) VALUES ('Lying Leg Raise', 'Lying on "
                       "the ground raise your legs to a 90 degree angle while "
                       "keeping them straight', 'Core', 'Abs', 'reps') "),
              1);
    EXPECT_EQ(db4.exec_and_retrieve("SELECT * FROM exercises"), 1);
    EXPECT_TRUE(db4.retrieve_next_row());
    EXPECT_EQ(db4.get_column(0).getString(), static_cast<std::string>("Lying Leg Raise"));
    EXPECT_EQ(db4.get_column(1).getString(),
              static_cast<std::string>("Lying on the ground raise your legs to a 90 degree angle while "
                                       "keeping them straight"));
    EXPECT_EQ(db4.get_column(2).getString(), static_cast<std::string>("Core"));
    EXPECT_EQ(db4.get_column(3).getString(), static_cast<std::string>("Abs"));
    EXPECT_EQ(db4.get_column(4).getString(), static_cast<std::string>("reps"));
}

TEST_F(DatabaseTest, SharedPtrDBTest) {
    EXPECT_EQ(db5->exec("INSERT INTO exercises (name, description, muscleGroup, "
                        "musclesTargeted, type) VALUES ('Lying Leg Raise', 'Lying on "
                        "the ground raise your legs to a 90 degree angle while "
                        "keeping them straight', 'Core', 'Abs', 'reps') "),
              1);
    EXPECT_EQ(db5->exec_and_retrieve("SELECT * FROM exercises WHERE name = 'Lying Leg Raise'"), 1);
    db6 = db5;
    EXPECT_TRUE(db6->retrieve_next_row());
    EXPECT_EQ(db6->get_column(0).getString(), static_cast<std::string>("Lying Leg Raise"));
    EXPECT_FALSE(db6->retrieve_next_row());
}

TEST_F(DatabaseTest, SanatizeQueryTest) {
    EXPECT_EQ(db1.prepare("INSERT INTO exercises (name) VALUES (?)", "Crunch"), 1);
    EXPECT_EQ(db1.exec_prepared(), 1);
    EXPECT_EQ(db1.prepare("INSERT INTO exercises (name) VALUES (?)", "Plank); DROP TABLE history;"), 1);
    EXPECT_EQ(db1.exec_prepared(), 1);
    SQLite::Database testdb1("thelab.db");
    EXPECT_TRUE(testdb1.tableExists("history"));
}

TEST_F(DatabaseTest, ThrowTest) {
    // Test preparing query with too many variable to bind
    EXPECT_EQ(db1.prepare("INSERT INTO exercises (name) VALUES (?)", "Plank", "Crunch"), -1);
    EXPECT_EQ(db1.prepare(1, "INSERT INTO exercises (name) VALUES (?)", "Plank", "Crunch"), -1);

    // Test preparing query with too few variables to bind
    EXPECT_EQ(db1.prepare("INSERT INTO exercises (name, description) VALUES (?, ?)", "Plank"), 1);
    EXPECT_EQ(db1.prepare(1, "INSERT INTO exercises (name, description) VALUES (?, ?)", "Plank"), 1);
}

TEST_F(DatabaseTest, ErrorReturnTest) {
    // Test executing prepared query with no prepared query
    EXPECT_EQ(db1.exec_prepared(), -1);
    EXPECT_FALSE(db1.retrieve_next_row());
    EXPECT_THROW(db1.get_column(0), SQLite::Exception);

    // Test SQL syntax errors
    EXPECT_EQ(db1.exec("INSERT INTO exercisess (name) VALUES ('Step up')"), -1);
    EXPECT_EQ(db1.exec_and_retrieve("SELECT * FROOM bodyStats"), -1);
    EXPECT_FALSE(db1.retrieve_next_row());

    // Expect errors when attempting to prepare select statement
    EXPECT_EQ(db1.prepare("SELECT * FROM ?", "history"), -1);
    EXPECT_EQ(db1.prepare(1, "SELECT * FROM ?", "history"), -1);
    EXPECT_EQ(db1.prepare("SELECT * FROM history WHERE exercise = ?", "Plank"), 1);
}
