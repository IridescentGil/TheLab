#ifndef DATABASE_H
#define DATABASE_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

#include <cstddef>
#include <exception>
#include <iostream>
#include <string>

class DBConn {
   public:
    DBConn();
    DBConn(std::string);

    int exec(const std::string);

    // prepareI() is used when the index of a bind is required, the index
    // of the first bind should be input. prepare() is used when the binds can
    // be done at once.
    template <typename T, typename... Args>
    int prepareI(const std::string, std::size_t, const T, Args...);
    template <typename... Args>
    int prepare(const std::string, const Args...);

    bool stepExec();
    auto getColumn(std::size_t index) { return query.getColumn(index); }

   private:
    SQLite::Database db;
    SQLite::Statement query;
};

template <typename T, typename... Args>
int DBConn::prepareI(const std::string queryStr, std::size_t index,
                     const T fArg, Args... args) {
    try {
        if (index == 0) query = SQLite::Statement(db, queryStr);
        query.bind(index, fArg);
        if constexpr (sizeof...(args) > 0) {
            if (prepareI(queryStr, index + 1, args...) == 1)
                return 1;
            else {
                return -1;
            }
        }
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
        return -1;
    }
    return 1;
}

template <typename... Args>
int DBConn::prepare(const std::string queryStr, const Args... args) {
    try {
        query = SQLite::Statement(db, queryStr);
        SQLite::bind(query, args...);
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
        return -1;
    }
    return 1;
}

#endif  // !DATABASE_H
