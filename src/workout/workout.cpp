#include "workout.h"

#include "database.h"

Lab::Workout::Workout(std::shared_ptr<Lab::DBConn> initDB) : db(initDB) {}

Lab::Workout::Workout(std::shared_ptr<Lab::DBConn> initDB,
                      std::string workoutName)
    : name(workoutName), db(initDB) {}

Lab::Workout::Workout(std::shared_ptr<Lab::DBConn> initDB,
                      std::string workoutName,
                      std::vector<Lab::ExcerciseData> newWorkout)
    : name(workoutName), db(initDB), workout(newWorkout) {}

void Lab::Workout::setWorkout(
    const std::vector<Lab::ExcerciseData> &newWorkout) {
    // workout.assign(newWorkout.cbegin(), newWorkout.cend());
    workout = newWorkout;
}

void Lab::Workout::addExcercise(const Lab::Excercise &newExcercise,
                                const int &type1Val, const int &type2Val) {
    workout.push_back(Lab::ExcerciseData{newExcercise, type1Val, type2Val});
}

void Lab::Workout::remExcercise(std::vector<Lab::ExcerciseData>::iterator it) {
    workout.erase(it);
}

void Lab::Workout::remExcercise(std::vector<Lab::ExcerciseData>::iterator start,
                                std::vector<Lab::ExcerciseData>::iterator end) {
    workout.erase(start, end);
}

void Lab::Workout::changeExcercise(std::vector<Lab::ExcerciseData>::iterator it,
                                   const Lab::Excercise &newExcercise,
                                   const int &type1Val, const int &type2Val) {
    *it = Lab::ExcerciseData{newExcercise, type1Val, type2Val};
}

void Lab::Workout::changeExcercise(
    std::vector<Lab::ExcerciseData>::iterator start,
    std::vector<Lab::ExcerciseData>::iterator end,
    const Lab::Excercise &newExcercise, const int &type1Val,
    const int &type2Val) {
    while (start != end) {
        *start = Lab::ExcerciseData{newExcercise, type1Val, type2Val};
        start++;
    }
}

void Lab::Workout::editName(const std::string &workoutName) {
    name = workoutName;
}

bool Lab::Workout::save() {
    size_t size = 0;
    db->prepare("SELECT exOrderNum FROM workouts WHERE workoutName = ?", name);
    while (db->stepExec()) size++;

    size_t index = 0;
    for (auto iter = workout.cbegin(); iter != workout.cend(); iter++) {
        const auto &[exc, type1, type2] = *iter;
        int place = iter - workout.cbegin();

        if (index >= size) {
            if (db->prepare("INSERT INTO workouts (workoutName, exOrderNum, "
                            "excercise, type1, type2) VALUES (?, ?, ?, ?, ?)",
                            name, place, exc.getName(), type1, type2) == -1)
                return false;
        } else if (index < size) {
            if (db->prepare(
                    "UPDATE workouts SET workoutName = ?, exOrderNum = ?, "
                    "excercise = ?, type1 = ?, type2 = ? WHERE workoutName = ? "
                    "AND exOrderNum = ?",
                    name, place, exc.getName(), type1, type2, name,
                    place) == -1)
                return false;
        }
        index++;
        if (!db->execQuery()) return false;
    }
    return true;
}
