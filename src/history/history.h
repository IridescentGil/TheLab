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
    History(std::shared_ptr<Lab::DBConn>);
    History(std::shared_ptr<Lab::DBConn>,
            std::vector<
                std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>);

    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
    getHistory();
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        getSliceHistory(std::chrono::time_point<std::chrono::system_clock>,
                        std::chrono::time_point<std::chrono::system_clock>);
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        getItem(
            std::vector<
                std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>::iterator,
            std::vector<
                std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>::iterator);
    std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string,
               Lab::Excercise, int, int>
        getItem(std::vector<
                std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>::iterator);
    // TODO: Add index based history item retrieval and deletion
    bool save();
    void setHistory(
        std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>);
    void addItem(std::chrono::time_point<std::chrono::system_clock>,
                 std::string, Lab::Excercise, int, int);
    void remItem(std::vector<
                 std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                            std::string, Lab::Excercise, int, int>>::iterator);
    void remItem(
        std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>::iterator,
        std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>::iterator);

   private:
    std::shared_ptr<Lab::DBConn> db;
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        history;
};

}  // namespace Lab
