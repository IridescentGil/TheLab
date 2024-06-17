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

    const std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, int, int>> &
    getHistory() const;

    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
    getSliceHistory(
        const std::chrono::time_point<std::chrono::system_clock> &start,
        const std::chrono::time_point<std::chrono::system_clock> &end) const;

    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
    getItem(
        std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>::iterator start,
        std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>::iterator end)
        const;
    std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string,
               Lab::Excercise, int, int>
    getItem(std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>::iterator it)
        const;
    // TODO: Add index based history item retrieval and deletion

    bool save();

    void setHistory(
        const std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>> &newHistory);

    void addItem(const std::chrono::time_point<std::chrono::system_clock> &date,
                 const std::string &workoutName, const Lab::Excercise &ex,
                 const int &type1Val, const int &type2Val);

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
