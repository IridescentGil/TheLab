#include "database.h"

DBConn::DBConn()
    : db("lab.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
      query(db, "SELECT * FROM created") {}

DBConn::DBConn(std::string name)
    : db(name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
      query(db, "SELECT * FROM created") {}

auto DBConn::exec(const std::string queryStr) {}

template <typename T, typename... Args>
int DBConn::prepareI(const std::string queryStr, std::size_t index,
                     const T fArg, const Args... args) {}

template <typename... Args>
int DBConn::prepare(const std::string queryStr, const Args... args) {}

int DBConn::stepExec() {}
auto DBConn::getColumn(std::size_t index) {}
