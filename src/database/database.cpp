#include "database.h"

Lab::DBConn::DBConn() : db("thelab.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), query(db, "") { createTables(); }

Lab::DBConn::DBConn(std::string_view name) : db(name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), query(db, "") {
    createTables();
}

void Lab::DBConn::createTables() {
    try {
        db.exec("PRAGMA foreign_keys = ON");
        if (!db.tableExists("exercises")) {
            db.exec(
                "CREATE TABLE exercises (name TEXT NOT NULL PRIMARY KEY, "
                "description "
                "TEXT, muscleGroup TEXT, musclesTargeted TEXT, type TEXT)");
        }
        if (!db.tableExists("workouts")) {
            db.exec(
                "CREATE TABLE workouts ("
                "workoutName TEXT NOT NULL, exOrderNum INTEGER NOT NULL, "
                "exercise TEXT NOT NULL, type1 REAL NOT NULL, "
                "type2 INTEGER, PRIMARY KEY (workoutName, exOrderNum), FOREIGN "
                "KEY(exercise) REFERENCES "
                "exercises(name) ON UPDATE CASCADE ON DELETE CASCADE)");
        }
        if (!db.tableExists("workoutPlans")) {
            db.exec(
                "CREATE TABLE workoutPlans (ID INTEGER NOT NULL PRIMARY KEY "
                "AUTOINCREMENT, "
                "planName TEXT NOT NULL, workoutName TEXT NOT NULL, "
                "initialExercise INTEGER NOT NULL, FOREIGN "
                "KEY(workoutName, initialExercise) "
                "REFERENCES "
                "workouts(workoutName, exOrderNum) ON UPDATE CASCADE ON DELETE "
                "CASCADE)");
        }
        if (!db.tableExists("history")) {
            db.exec(
                "CREATE TABLE history (ID INTEGER NOT NULL PRIMARY KEY, "
                "date "
                "INTEGER NOT NULL, workout TEXT, exercise TEXT NOT NULL, "
                "type1 REAL NOT NULL, type2 INTEGER, FOREIGN KEY(exercise) "
                "REFERENCES "
                "exercises(name) ON UPDATE CASCADE ON DELETE CASCADE)");
        }
        if (!db.tableExists("bodyCondition")) {
            db.exec(
                "CREATE TABLE bodyCondition (date INTEGER NOT NULL PRIMARY "
                "KEY, "
                "neckCondition INTEGER NOT NULL, trapeziusCondition INTEGER "
                "NOT NULL, "
                "bicepsCondition INTEGER NOT NULL, tricepsCondition INTEGER "
                "NOT "
                "NULL, "
                "forearmsCondition INTEGER NOT NULL, pectoralsCondition "
                "INTEGER "
                "NOT "
                "NULL, "
                "absCondition INTEGER NOT NULL, latsCondition INTEGER NOT "
                "NULL, "
                "upperBackCondition INTEGER NOT NULL, lowerBackCondition "
                "INTEGER NOT "
                "NULL, "
                "quadsCondition INTEGER NOT NULL, glutesCondition INTEGER NOT "
                "NULL, "
                "hamstringsCondition INTEGER NOT NULL, calfsCondition INTEGER "
                "NOT NULL)");
        }
        if (!db.tableExists("bodyStats")) {
            db.exec(
                "CREATE TABLE bodyStats (date INTEGER NOT NULL PRIMARY KEY, "
                "weight "
                "INTEGER, "
                "height INTEGER, age INTEGER, neckMeasurement INTEGER, "
                "shouldersMeasurement "
                "INTEGER, "
                "chestMeasurement INTEGER, waistMeasurement INTEGER, "
                "hipsMeasurement "
                "INTEGER, "
                "upperArmRightMeasurement INTEGER, upperArmLeftMeasurement "
                "INTEGER, "
                "forearmRightMeasurement INTEGER, forearmLeftMeasurement "
                "INTEGER, "
                "thighRightMeasurement INTEGER, thighLeftMeasurement INTEGER, "
                "calfRightMeasurement INTEGER, calfLeftMeasurement INTEGER)");
        }
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << '\n';
    }
}

int Lab::DBConn::exec(const std::string &queryStr) {
    try {
        int ret = db.exec(queryStr);
        return ret > 0 ? 1 : -1;
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << '\n';
        return -1;
    }
}

int Lab::DBConn::exec_prepared() {
    try {
        int ret = query.exec();
        return (ret >= 0) ? 1 : -1;
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << '\n';
        return -1;
    }
}

int Lab::DBConn::exec_and_retrieve(const std::string &queryStr) {
    try {
        query = SQLite::Statement(db, queryStr);
        return 1;
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << '\n';
        return -1;
    }
}

bool Lab::DBConn::retrieve_next_row() {
    try {
        if (query.executeStep()) {
            return true;
        }
        query.reset();
        return false;

    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << '\n';
        return false;
    }
}
