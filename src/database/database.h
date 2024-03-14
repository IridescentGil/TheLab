#ifndef DATABASE_H
#define DATABASE_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

#include <cstddef>
#include <string>

class DBConn {
   public:
    DBConn();
    DBConn(std::string);

    auto exec(const std::string);

    // prepareI() is used when the index of a bind is required, the index
    // of the first bind should be input. prepare() is used when the binds can
    // be done at once.
    template <typename T, typename... Args>
    int prepareI(const std::string, std::size_t, const T, const Args...);
    template <typename... Args>
    int prepare(const std::string, const Args...);

    int stepExec();
    auto getColumn(std::size_t);

   private:
    SQLite::Database db;
    SQLite::Statement query;
};
#endif  // !DATABASE_H
