#pragma once
///@file

#include <string>
#include <utility>
#include <vector>

#include "exercise.h"

namespace Lab {
class DBConn;

struct ExerciseData {
    ExerciseData(const Lab::Exercise &newEx, double newT1, unsigned long newT2)
        : exc(newEx), type1(newT1), type2(newT2) {}
    ExerciseData(ExerciseData &&other) noexcept : exc(std::move(other.exc)), type1(other.type1), type2(other.type2) {}
    ExerciseData(const ExerciseData &other) : exc(other.exc), type1(other.type1), type2(other.type2) {}
    ExerciseData &operator=(ExerciseData &&other) noexcept {
        if (&other != this) {
            exc = std::move(other.exc);
            type1 = other.type1;
            type2 = other.type2;
        }
        return *this;
    }
    ExerciseData &operator=(const ExerciseData &other) {
        if (&other != this) {
            exc = other.exc;
            type1 = other.type1;
            type2 = other.type2;
        }
        return *this;
    }
    ~ExerciseData() = default;

    Lab::Exercise exc;
    double type1;
    unsigned long type2;
};

class Workout {
   public:
    Workout(Lab::DBConn *initDB);
    Workout(Lab::DBConn *initDB, std::string workoutName);
    Workout(Lab::DBConn *initDB, std::string workoutName, std::vector<Lab::ExerciseData> newWorkout);

    const std::vector<Lab::ExerciseData> &getWorkout() const { return workout; };
    std::vector<Lab::ExerciseData> &getWorkout() { return workout; };

    void setWorkout(const std::vector<Lab::ExerciseData> &newWorkout);

    void addExercise(const Lab::Exercise &newExercise, const double &type1Val, const unsigned long &type2Val);

    void remExercise(std::vector<Lab::ExerciseData>::iterator iter);
    void remExercise(std::vector<Lab::ExerciseData>::iterator start, std::vector<Lab::ExerciseData>::iterator end);

    void changeExercise(std::vector<Lab::ExerciseData>::iterator iter, const Lab::Exercise &newExercise,
                        const double &type1Val, const unsigned long &type2Val);
    void changeExercise(std::vector<Lab::ExerciseData>::iterator start, std::vector<Lab::ExerciseData>::iterator end,
                        const Lab::Exercise &newExercise, const double &type1Val, const unsigned long &type2Val);

    void editName(const std::string &workoutName);

    std::string getName() const { return name; };
    bool save();

   private:
    void removeExercisesNotInDB();
    std::string name;
    Lab::DBConn *db;
    std::vector<Lab::ExerciseData> workout;
};

}  // namespace Lab
