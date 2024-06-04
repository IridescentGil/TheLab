#pragma once
///@file

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "database.h"
#include "excercise.h"

namespace Lab {

class History {
   public:
    History(std::shared_ptr<Lab::DBConn> newDB);
    History(std::shared_ptr<Lab::DBConn> newDB,
            std::vector<
                std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
                newHistory);

    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
    getHistory();

    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
    getSliceHistory(std::chrono::time_point<std::chrono::system_clock> start,
                    std::chrono::time_point<std::chrono::system_clock> end);

    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
    getItem(
        std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>::iterator start,
        std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>::iterator end);
    std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string,
               Lab::Excercise, int, int>
    getItem(std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>::iterator it);
    // TODO: Add index based history item retrieval and deletion

    bool save();

    void setHistory(
        std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>
            newHistory);

    void addItem(std::chrono::time_point<std::chrono::system_clock> date,
                 std::string workoutName, Lab::Excercise ex, int type1Val,
                 int type2Val);

    void remItem(
        std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>::iterator it);
    void remItem(
        std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>::iterator start,
        std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>::iterator end);

   private:
    std::shared_ptr<Lab::DBConn> db;
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        history;
};

}  // namespace Lab
