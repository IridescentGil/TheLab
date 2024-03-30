#ifndef WORKOUTPLAN_H
#define WORKOUTPLAN_H

#include <memory>
#include <tuple>
#include <vector>

#include "database.h"
#include "excercise.h"

class WorkoutPlan {
   public:
    WorkoutPlan(std::shared_ptr<DBConn>);
    WorkoutPlan(std::shared_ptr<DBConn>, std::string);
    WorkoutPlan(std::shared_ptr<DBConn>, std::string,
                std::vector<std::tuple<Excercise, int, int>>);
    std::vector<std::tuple<Excercise, int, int>> getWoPlan();
    void setWoPlan(std::vector<std::tuple<Excercise, int, int>>);
    void addExcercise(Excercise, int, int);
    void remExcercise(std::vector<std::tuple<Excercise, int, int>>::iterator);
    void remExcercise(std::vector<std::tuple<Excercise, int, int>>::iterator,
                      std::vector<std::tuple<Excercise, int, int>>::iterator);
    void changeExcercise(std::vector<std::tuple<Excercise, int, int>>::iterator,
                         Excercise, int, int);
    void changeExcercise(std::vector<std::tuple<Excercise, int, int>>::iterator,
                         std::vector<std::tuple<Excercise, int, int>>::iterator,
                         Excercise, int, int);
    void editName(std::string);
    bool save();

   private:
    std::string name;
    std::shared_ptr<DBConn> db;
    std::vector<std::tuple<Excercise, int, int>> woPlan;
};

#endif  // !WORKOUTPLAN_H
