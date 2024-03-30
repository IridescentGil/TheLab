#ifndef HISTORY_H
#define HISTORY_H

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "database.h"
#include "excercise.h"
class History {
   public:
    History(std::shared_ptr<DBConn>);
    History(std::shared_ptr<DBConn>,
            std::vector<
                std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>);

    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
    getHistory();
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
        getSliceHistory(std::chrono::time_point<std::chrono::system_clock>,
                        std::chrono::time_point<std::chrono::system_clock>);
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
        getItemMulti(
            std::vector<
                std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>::iterator,
            std::vector<
                std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>::iterator);
    std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string,
               Excercise, int, int>
        getItem(std::vector<
                std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>::iterator);
    void save();

   private:
    std::shared_ptr<DBConn> db;
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
        history;
};

#endif  // !HISTORY_H
