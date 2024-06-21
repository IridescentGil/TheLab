#pragma once
///@file

#include <memory>
#include <vector>

#include "database.h"
#include "excercise.h"

namespace Lab {
struct ExcerciseData {
    ExcerciseData(Lab::Excercise newEx, int newT1, int newT2)
        : exc(newEx), type1(newT1), type2(newT2) {}
    ExcerciseData(ExcerciseData &&other)
        : exc(std::move(other.exc)),
          type1(std::move(other.type1)),
          type2(std::move(other.type2)) {}
    ExcerciseData(const ExcerciseData &other)
        : exc(other.exc), type1(other.type1), type2(other.type2) {}
    ExcerciseData &operator=(ExcerciseData &&other) {
        if (&other != this) {
            exc = std::move(other.exc);
            type1 = std::move(other.type1);
            type2 = std::move(other.type2);
        }
        return *this;
    }
    ExcerciseData &operator=(const ExcerciseData &other) {
        if (&other != this) {
            exc = other.exc;
            type1 = other.type1;
            type2 = other.type2;
        }
        return *this;
    }
    ~ExcerciseData() = default;

    Lab::Excercise exc;
    int type1;
    int type2;
};

class Workout {
   public:
    Workout(std::shared_ptr<Lab::DBConn> initDB);
    Workout(std::shared_ptr<Lab::DBConn> initDB, std::string workoutName);
    Workout(std::shared_ptr<Lab::DBConn> initDB, std::string workoutName,
            std::vector<Lab::ExcerciseData> newWorkout);

    const std::vector<Lab::ExcerciseData> &getWorkout() const {
        return workout;
    };
    std::vector<Lab::ExcerciseData> &getWorkout() { return workout; };

    void setWorkout(const std::vector<Lab::ExcerciseData> &newWorkout);

    void addExcercise(const Lab::Excercise &newExcercise, const int &type1Val,
                      const int &type2Val);

    void remExcercise(std::vector<Lab::ExcerciseData>::iterator it);
    void remExcercise(std::vector<Lab::ExcerciseData>::iterator start,
                      std::vector<Lab::ExcerciseData>::iterator end);

    void changeExcercise(std::vector<Lab::ExcerciseData>::iterator it,
                         const Lab::Excercise &newExcercise,
                         const int &type1Val, const int &type2Val);
    void changeExcercise(std::vector<Lab::ExcerciseData>::iterator start,
                         std::vector<Lab::ExcerciseData>::iterator end,
                         const Lab::Excercise &newExcercise,
                         const int &type1Val, const int &type2Val);

    void editName(const std::string &workoutName);

    std::string getName() const { return name; };
    bool save();

   private:
    std::string name;
    std::shared_ptr<Lab::DBConn> db;
    std::vector<Lab::ExcerciseData> workout;
};

}  // namespace Lab
