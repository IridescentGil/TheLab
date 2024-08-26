#include "workout.h"

#include <utility>

#include "database.h"

Lab::Workout::Workout(Lab::DBConn *initDB) : db(initDB) {}

Lab::Workout::Workout(Lab::DBConn *initDB, std::string workoutName) : name(std::move(workoutName)), db(initDB) {
    std::vector<std::string> excerciseNameList;
    std::vector<Lab::Excercise> excerciseObjects;
    Lab::Excercise excerciseLoader;

    db->prepare("SELECT excercise FROM workouts WHERE workoutName = ?", name);
    while (db->retrieve_next_row()) {
        excerciseNameList.push_back(db->get_column(0));
    }
    for (const auto &excerciseName : excerciseNameList) {
        excerciseLoader.load(db, excerciseName);
        excerciseObjects.push_back(excerciseLoader);
    }

    db->prepare("SELECT * FROM workouts WHERE workoutName = ?", name);
    auto excercise = excerciseObjects.cbegin();
    while (db->retrieve_next_row()) {
        workout.push_back(Lab::ExcerciseData(*excercise, db->get_column(3), db->get_column(4).getInt64()));
        ++excercise;
    }
}

Lab::Workout::Workout(Lab::DBConn *initDB, std::string workoutName, std::vector<Lab::ExcerciseData> newWorkout)
    : name(std::move(workoutName)), db(initDB), workout(std::move(newWorkout)) {
    this->removeExcercisesNotInDB();
}

void Lab::Workout::setWorkout(const std::vector<Lab::ExcerciseData> &newWorkout) { workout = newWorkout; }

void Lab::Workout::addExcercise(const Lab::Excercise &newExcercise, const double &type1Val,
                                const unsigned long &type2Val) {
    Lab::Excercise excerciseComp;
    excerciseComp.load(db, newExcercise.getName());
    if (newExcercise == excerciseComp) {
        workout.push_back(Lab::ExcerciseData{newExcercise, type1Val, type2Val});
    }
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
    while (db->retrieve_next_row()) {
        ++size;
    }

    if (workout.size() < size) {
        db->prepare("DELETE FROM workouts WHERE workoutName = ?", name);
        db->exec_prepared();
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
        if (db->exec_prepared() == 0) {
            return false;
        }
    }
    return true;
}

void Lab::Workout::removeExcercisesNotInDB() {
    for (auto workoutIter = workout.begin(); workoutIter != workout.end(); ++workoutIter) {
        Lab::Excercise excercise;
        excercise.load(db, workoutIter->exc.getName());
        if (workoutIter->exc != excercise) {
            workoutIter = workout.erase(workoutIter);
            --workoutIter;
        }
    }
}
