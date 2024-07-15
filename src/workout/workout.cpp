#include "workout.h"

#include <utility>

#include "database.h"

Lab::Workout::Workout(std::shared_ptr<Lab::DBConn> initDB) : db(std::move(initDB)) {}

Lab::Workout::Workout(std::shared_ptr<Lab::DBConn> initDB, std::string workoutName)
    : name(std::move(workoutName)), db(std::move(initDB)) {}

Lab::Workout::Workout(std::shared_ptr<Lab::DBConn> initDB, std::string workoutName,
                      std::vector<Lab::ExcerciseData> newWorkout)
    : name(std::move(workoutName)), db(std::move(initDB)), workout(std::move(newWorkout)) {}

void Lab::Workout::setWorkout(const std::vector<Lab::ExcerciseData> &newWorkout) { workout = newWorkout; }

void Lab::Workout::addExcercise(const Lab::Excercise &newExcercise, const double &type1Val,
                                const unsigned long &type2Val) {
    workout.push_back(Lab::ExcerciseData{newExcercise, type1Val, type2Val});
}

void Lab::Workout::remExcercise(std::vector<Lab::ExcerciseData>::iterator iter) { workout.erase(iter); }

void Lab::Workout::remExcercise(std::vector<Lab::ExcerciseData>::iterator start,
                                std::vector<Lab::ExcerciseData>::iterator end) {
    workout.erase(start, end);
}

void Lab::Workout::changeExcercise(std::vector<Lab::ExcerciseData>::iterator iter, const Lab::Excercise &newExcercise,
                                   const double &type1Val, const unsigned long &type2Val) {
    *iter = Lab::ExcerciseData{newExcercise, type1Val, type2Val};
}

void Lab::Workout::changeExcercise(std::vector<Lab::ExcerciseData>::iterator start,
                                   std::vector<Lab::ExcerciseData>::iterator end, const Lab::Excercise &newExcercise,
                                   const double &type1Val, const unsigned long &type2Val) {
    while (start != end) {
        *start = Lab::ExcerciseData{newExcercise, type1Val, type2Val};
        ++start;
    }
}

void Lab::Workout::editName(const std::string &workoutName) { name = workoutName; }

bool Lab::Workout::save() {
    size_t size = 0;
    db->prepare("SELECT exOrderNum FROM workouts WHERE workoutName = ?", name);
    while (db->stepExec()) {
        ++size;
    }

    if (workout.size() < size) {
        db->exec("DELETE FROM workouts");
        size = 0;
    }

    size_t index = 0;
    for (auto iter = workout.cbegin(); iter != workout.cend(); ++iter) {
        const auto &[exc, type1, type2] = *iter;
        long place = iter - workout.cbegin();

        /*
         * NOTE: unsign long cast to long due SQLiteCPP not recongnizing
         * uint64_t
         */
        if (index >= size) {
            if (db->prepare("INSERT INTO workouts (workoutName, exOrderNum, "
                            "excercise, type1, type2) VALUES (?, ?, ?, ?, ?)",
                            name, place, exc.getName(), type1, static_cast<long>(type2)) == -1) {
                return false;
            }
        } else if (index < size) {
            if (db->prepare("UPDATE workouts SET workoutName = ?, exOrderNum = ?, "
                            "excercise = ?, type1 = ?, type2 = ? WHERE workoutName = ? "
                            "AND exOrderNum = ?",
                            name, place, exc.getName(), type1, static_cast<long>(type2), name, place) == -1) {
                return false;
            }
        }
        ++index;
        if (db->execQuery() == 0) {
            return false;
        }
    }
    return true;
}
