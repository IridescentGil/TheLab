#pragma once
///@file

#include <tuple>
#include <vector>

#include "database.h"
#include "excercise.h"

namespace Lab {

class Workout {
   public:
    Workout(Lab::DBConn &initDB);
    Workout(Lab::DBConn &initDB, std::string workoutName);
    Workout(Lab::DBConn &initDB, std::string workoutName,
            std::vector<std::tuple<Lab::Excercise, int, int>> newWorkout);

    std::vector<std::tuple<Lab::Excercise, int, int>> getWorkout();

    void setWorkout(
        std::vector<std::tuple<Lab::Excercise, int, int>> newWorkout);

    void addExcercise(Lab::Excercise newExcercise, int type1Val, int type2Val);

    void remExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::iterator);
    void remExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::iterator start,
        std::vector<std::tuple<Lab::Excercise, int, int>>::iterator end);

    void changeExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::const_iterator it,
        Lab::Excercise newExcercise, int type1Val, int type2Val);
    void changeExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::const_iterator start,
        std::vector<std::tuple<Lab::Excercise, int, int>>::const_iterator end,
        Lab::Excercise newExcercise, int type1Val, int type2Val);

    void editName(std::string workoutName);

    std::string getName();
    bool save();

   private:
    std::string name;
    Lab::DBConn &db;
    std::vector<std::tuple<Lab::Excercise, int, int>> workout;
};

}  // namespace Lab
