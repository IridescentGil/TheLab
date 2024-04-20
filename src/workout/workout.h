#ifndef WORKOUT_H
#define WORKOUT_H

#include <memory>
#include <tuple>
#include <vector>

#include "database.h"
#include "excercise.h"

class Workout {
   public:
    Workout(std::shared_ptr<DBConn>);
    Workout(std::shared_ptr<DBConn>, std::string);
    Workout(std::shared_ptr<DBConn>, std::string,
            std::vector<std::tuple<Excercise, int, int>>);
    std::vector<std::tuple<Excercise, int, int>> getWoPlan();
    void setWoPlan(std::vector<std::tuple<Excercise, int, int>>);
    void addExcercise(Excercise, int, int);
    void remExcercise(std::vector<std::tuple<Excercise, int, int>>::iterator);
    void remExcercise(std::vector<std::tuple<Excercise, int, int>>::iterator,
                      std::vector<std::tuple<Excercise, int, int>>::iterator);
    void changeExcercise(
        std::vector<std::tuple<Excercise, int, int>>::const_iterator, Excercise,
        int, int);
    void changeExcercise(
        std::vector<std::tuple<Excercise, int, int>>::const_iterator,
        std::vector<std::tuple<Excercise, int, int>>::const_iterator, Excercise,
        int, int);
    void editName(std::string);
    std::string getName();
    bool save();

   private:
    std::string name;
    std::shared_ptr<DBConn> db;
    std::vector<std::tuple<Excercise, int, int>> woPlan;
};

#endif  // !WORKOUT_H
