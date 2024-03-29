#include "database.h"

DBConn::DBConn()
    : db("thelab.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
      query(db, "") {
    createTables();
}

DBConn::DBConn(std::string name)
    : db(name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), query(db, "") {
    createTables();
}

void DBConn::createTables() {
    try {
        if (!db.tableExists("excercises"))
            db.exec(
                "CREATE TABLE excercises (name TEXT NOT NULL PRIMARY KEY, "
                "description "
                "TEXT, muscleGroup TEXT, musclesTargeted TEXT, type TEXT)");
        if (!db.tableExists("workouts"))
            db.exec(
                "CREATE TABLE workouts (ID INTEGER NOT NULL PRIMARY KEY "
                "AUTOINCREMENT, "
                "workoutName TEXT NOT NULL, exOrderNum INTEGER NOT NULL, "
                "excercise "
                "TEXT "
                "NOT NULL, type1 "
                "INTEGER "
                "NOT "
                "NULL, "
                "type2 INTEGER, FOREIGN KEY(excercise) REFERENCES "
                "excercises(name))");
        if (!db.tableExists("workoutPlans"))
            db.exec(
                "CREATE TABLE workoutPlans (ID INTEGER NOT NULL PRIMARY KEY "
                "AUTOINCREMENT, "
                "planName TEXT NOT NULL, workoutID NOT NULL, FOREIGN "
                "KEY(workoutID) "
                "REFERENCES "
                "workouts(ID))");
        if (!db.tableExists("history"))
            db.exec(
                "CREATE TABLE history (ID INTEGER NOT NULL PRIMARY KEY "
                "AUTOINCREMENT, "
                "DATE "
                "INTEGER NOT NULL, excercise TEXT NOT NULL, workout TEXT, "
                "type1 TEXT NOT NULL, type2 TEXT, FOREIGN KEY(excercise) "
                "REFERENCES "
                "excercises(name))");
        if (!db.tableExists("bodyCondition"))
            db.exec(
                "CREATE TABLE bodyCondition (date INTEGER NOT NULL PRIMARY "
                "KEY, "
                "neckCondition INTEGER NOT NULL, trapeziusCondition INTEGER "
                "NOT NULL, "
                "bicepCondition INTEGER NOT NULL, tricepCondition INTEGER NOT "
                "NULL, "
                "forearmCondition INTEGER NOT NULL, pectoralCondition INTEGER "
                "NOT "
                "NULL, "
                "absCondition INTEGER NOT NULL, latsCondition INTEGER NOT "
                "NULL, "
                "upperBackCondition INTEGER NOT NULL, lowerBackCondition "
                "INTEGER NOT "
                "NULL, "
                "quadsCondition INTEGER NOT NULL, glutesCondition INTEGER NOT "
                "NULL, "
                "hamstringCondition INTEGER NOT NULL, calfCondition INTEGER "
                "NOT NULL)");
        if (!db.tableExists("bodyStats"))
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
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
    }
}

int DBConn::exec(const std::string queryStr) {
    try {
        int ret = db.exec(queryStr);
        return ret > 0 ? 1 : -1;
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
        return -1;
    }
}

int DBConn::execQuery() {
    try {
        int ret = query.exec();
        return (ret >= 0) ? 1 : -1;
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
        return -1;
    }
}

int DBConn::execMulti(const std::string queryStr) {
    try {
        query = SQLite::Statement(db, queryStr);
        return 1;
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
        return -1;
    }
}

bool DBConn::stepExec() {
    try {
        if (query.executeStep()) return true;
        return false;
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
        return false;
    }
}
