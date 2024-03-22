#include "database.h"

DBConn::DBConn()
    : db("lab.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
      query(db, "SELECT * FROM excercises") {}

DBConn::DBConn(std::string name)
    : db(name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
      query(db, "SELECT * FROM excercises") {}

int DBConn::exec(const std::string queryStr) {
    try {
        int ret = db.exec(queryStr);
        return ret;
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
        return -1;
    }
}

bool DBConn::stepExec() {
    if (query.executeStep()) return true;
    return false;
}
