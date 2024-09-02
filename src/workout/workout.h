#pragma once
///@file

#include <string>
#include <utility>
#include <vector>

#include "excercise.h"

namespace Lab {
class DBConn;

struct ExcerciseData {
    ExcerciseData(const Lab::Excercise &newEx, double newT1, unsigned long newT2)
        : exc(newEx), type1(newT1), type2(newT2) {}
    ExcerciseData(ExcerciseData &&other) noexcept : exc(std::move(other.exc)), type1(other.type1), type2(other.type2) {}
    ExcerciseData(const ExcerciseData &other) : exc(other.exc), type1(other.type1), type2(other.type2) {}
    ExcerciseData &operator=(ExcerciseData &&other) noexcept {
        if (&other != this) {
            exc = std::move(other.exc);
            type1 = other.type1;
            type2 = other.type2;
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
    double type1;
    unsigned long type2;
};

class Workout {
   public:
    Workout(Lab::DBConn *initDB);
    Workout(Lab::DBConn *initDB, std::string workoutName);
    Workout(Lab::DBConn *initDB, std::string workoutName, std::vector<Lab::ExcerciseData> newWorkout);

    const std::vector<Lab::ExcerciseData> &getWorkout() const { return workout; };
    std::vector<Lab::ExcerciseData> &getWorkout() { return workout; };

    void setWorkout(const std::vector<Lab::ExcerciseData> &newWorkout);

    void addExcercise(const Lab::Excercise &newExcercise, const double &type1Val, const unsigned long &type2Val);

    void remExcercise(std::vector<Lab::ExcerciseData>::iterator iter);
    void remExcercise(std::vector<Lab::ExcerciseData>::iterator start, std::vector<Lab::ExcerciseData>::iterator end);

    void changeExcercise(std::vector<Lab::ExcerciseData>::iterator iter, const Lab::Excercise &newExcercise,
                         const double &type1Val, const unsigned long &type2Val);
    void changeExcercise(std::vector<Lab::ExcerciseData>::iterator start, std::vector<Lab::ExcerciseData>::iterator end,
                         const Lab::Excercise &newExcercise, const double &type1Val, const unsigned long &type2Val);

    void editName(const std::string &workoutName);

    std::string getName() const { return name; };
    bool save();

   private:
    void removeExcercisesNotInDB();
    std::string name;
    Lab::DBConn *db;
    std::vector<Lab::ExcerciseData> workout;
};

}  // namespace Lab
