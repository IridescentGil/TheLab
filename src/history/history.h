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
                           std::string, Lab::Excercise, double, unsigned long>>
                newHistory);

    std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, double, unsigned long>> &
    getHistory();
    const std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, double, unsigned long>> &
    getHistory() const;

    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, double, unsigned long>>
    getSliceHistory(
        const std::chrono::time_point<std::chrono::system_clock> &start,
        const std::chrono::time_point<std::chrono::system_clock> &end) const;

    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, double, unsigned long>>
    getItem(std::vector<std::tuple<
                std::chrono::time_point<std::chrono::system_clock>, std::string,
                Lab::Excercise, double, unsigned long>>::iterator start,
            std::vector<std::tuple<
                std::chrono::time_point<std::chrono::system_clock>, std::string,
                Lab::Excercise, double, unsigned long>>::iterator end) const;
    std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string,
               Lab::Excercise, double, unsigned long>
    getItem(std::vector<std::tuple<
                std::chrono::time_point<std::chrono::system_clock>, std::string,
                Lab::Excercise, double, unsigned long>>::iterator iter) const;

    bool save();

    void setHistory(
        const std::vector<
            std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, double, unsigned long>>
            &newHistory);

    void addItem(const std::chrono::time_point<std::chrono::system_clock> &date,
                 const std::string &workoutName,
                 const Lab::Excercise &excercise, const double &type1Val,
                 const unsigned long &type2Val);

    void remItem(
        std::vector<std::tuple<
            std::chrono::time_point<std::chrono::system_clock>, std::string,
            Lab::Excercise, double, unsigned long>>::iterator iter);
    void remItem(
        std::vector<std::tuple<
            std::chrono::time_point<std::chrono::system_clock>, std::string,
            Lab::Excercise, double, unsigned long>>::iterator start,
        std::vector<std::tuple<
            std::chrono::time_point<std::chrono::system_clock>, std::string,
            Lab::Excercise, double, unsigned long>>::iterator end);

   private:
    std::shared_ptr<Lab::DBConn> db;
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, double, unsigned long>>
        history;
};

}  // namespace Lab
