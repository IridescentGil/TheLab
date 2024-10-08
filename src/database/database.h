#pragma once
///@file

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

#include <exception>
#include <iostream>
#include <string>

namespace Lab {
/**
 * @brief Creates a database and provides functions to execute queries
 *
 * Created database is names TheLab if string is not provided upon creation
 */
class DBConn {
   public:
    DBConn();
    DBConn(std::string_view);

    /**
     * @brief Execute the query string; will not retrieve results
     */
    int exec(const std::string &queryStr);
    /**
     * @brief Execute prepared query that doesnt retrieve results
     */
    int exec_prepared();
    /**
     * @brief prepare query that doesn't need replacements
     *
     * @param queryStr
     * @return excexution status
     */
    int exec_and_retrieve(const std::string &queryStr);

    /**
     * @name prepare
     * @param queryStr String containing the query to prepare
     * @return  1 Statement prepared successfully
     *          -1 Error in preparing statement
     */
    ///@{
    /**
     * @brief Prepare a SQL statement
     * @param args[in]
     */
    template <typename... Args>
    int prepare(const std::string &queryStr, const Args &...args);
    /**
     * @brief Prepare a SQL statement giving the index of the first variable to
     * replace
     * @param index Index of the place to replace starts at 1
     * @param fArg[in]
     * @param args[in]
     */
    template <typename T, typename... Args>
    int prepare(int index, const std::string &queryStr, const T &fArg, const Args &...args);
    ///@}

    /**
     * @brief Retrieve the results of a prepared query
     * @return True if any data is retrieved
     */
    bool retrieve_next_row();

    /**
     * @brief Retrieve the column that corresponds to the index
     * starts at 0
     *
     * @return Column
     */
    auto get_column(const int &index) { return query.getColumn(index); }

   private:
    SQLite::Database db;
    SQLite::Statement query;
    void createTables();
};

template <typename T, typename... Args>
int DBConn::prepare(int index, const std::string &queryStr, const T &fArg, const Args &...args) {
    try {
        if (index == 1) {
            query = SQLite::Statement(db, queryStr);
        }
        query.bind(index, fArg);
        if constexpr (sizeof...(args) > 0) {
            if (this->prepare(index + 1, queryStr, args...) == 1) {
                return 1;
            }
            return -1;
        }
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << '\n';
        return -1;
    }
    return 1;
}

template <typename... Args>
int DBConn::prepare(const std::string &queryStr, const Args &...args) {
    try {
        query = SQLite::Statement(db, queryStr);
        SQLite::bind(query, args...);
    } catch (std::exception &e) {
        std::cerr << "SQLite error: " << e.what() << '\n';
        return -1;
    }
    return 1;
}

}  // namespace Lab
