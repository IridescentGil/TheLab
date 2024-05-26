#pragma once
///@file

#include <memory>
#include <tuple>
#include <vector>

#include "database.h"
#include "excercise.h"

namespace Lab {

class Workout {
   public:
    Workout(std::shared_ptr<Lab::DBConn>);
    Workout(std::shared_ptr<Lab::DBConn>, std::string);
    Workout(std::shared_ptr<Lab::DBConn>, std::string,
            std::vector<std::tuple<Lab::Excercise, int, int>>);
    std::vector<std::tuple<Lab::Excercise, int, int>> getWoPlan();
    void setWoPlan(std::vector<std::tuple<Lab::Excercise, int, int>>);
    void addExcercise(Lab::Excercise, int, int);
    void remExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::iterator);
    void remExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::iterator,
        std::vector<std::tuple<Lab::Excercise, int, int>>::iterator);
    void changeExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::const_iterator,
        Lab::Excercise, int, int);
    void changeExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::const_iterator,
        std::vector<std::tuple<Lab::Excercise, int, int>>::const_iterator,
        Lab::Excercise, int, int);
    void editName(std::string);
    std::string getName();
    bool save();

   private:
    std::string name;
    std::shared_ptr<Lab::DBConn> db;
    std::vector<std::tuple<Lab::Excercise, int, int>> woPlan;
};

}  // namespace Lab
