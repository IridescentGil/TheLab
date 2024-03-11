#ifndef DATABASE_H
#define DATABASE_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

#include <string>

class DBConn {
   public:
    DBConn();
    DBConn(std::string);

    int exec(std::string, auto...);
    int prepare(std::string, auto...);
    int stepExec();
    auto getColumn(int);

   private:
    SQLite::Database db;
    SQLite::Statement query;
};
#endif  // !DATABASE_H
