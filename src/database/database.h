#ifndef DATABASE_H
#define DATABASE_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

#include <string>

class DBConn {
   public:
    DBConn();
    DBConn(std::string);

    template <typename T, typename... Args>
    int exec(const std::string, const T, const Args...);
    template <typename T, typename... Args>
    int prepare(const std::string, const T, const Args...);
    int stepExec();
    auto getColumn(int);

   private:
    SQLite::Database db;
    SQLite::Statement query;
};
#endif  // !DATABASE_H
