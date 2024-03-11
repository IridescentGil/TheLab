#include "database.h"

DBConn::DBConn()
    : db("lab.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
      query(db, "SELECT * FROM created") {}

DBConn::DBConn(std::string name)
    : db(name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
      query(db, "SELECT * FROM created") {}
