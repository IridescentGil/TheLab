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
    Workout(std::shared_ptr<Lab::DBConn> initDB);
    Workout(std::shared_ptr<Lab::DBConn> initDB, std::string workoutName);
    Workout(std::shared_ptr<Lab::DBConn> initDB, std::string workoutName,
            std::vector<std::tuple<Lab::Excercise, int, int>> newWorkout);

    std::vector<std::tuple<Lab::Excercise, int, int>> getWorkout() const;

    void setWorkout(
        const std::vector<std::tuple<Lab::Excercise, int, int>> &newWorkout);

    void addExcercise(const Lab::Excercise &newExcercise, const int &type1Val,
                      const int &type2Val);

    void remExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::iterator);
    void remExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::iterator start,
        std::vector<std::tuple<Lab::Excercise, int, int>>::iterator end);

    void changeExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::const_iterator it,
        const Lab::Excercise &newExcercise, const int &type1Val,
        const int &type2Val);
    void changeExcercise(
        std::vector<std::tuple<Lab::Excercise, int, int>>::const_iterator start,
        std::vector<std::tuple<Lab::Excercise, int, int>>::const_iterator end,
        const Lab::Excercise &newExcercise, const int &type1Val,
        const int &type2Val);

    void editName(const std::string &workoutName);

    std::string getName() const;
    bool save();

   private:
    std::string name;
    std::shared_ptr<Lab::DBConn> db;
    std::vector<std::tuple<Lab::Excercise, int, int>> workout;
};

}  // namespace Lab
