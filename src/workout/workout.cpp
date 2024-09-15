#include "workout.h"

#include <utility>

#include "database.h"

Lab::Workout::Workout(Lab::DBConn *initDB) : db(initDB) {}

Lab::Workout::Workout(Lab::DBConn *initDB, std::string workoutName) : name(std::move(workoutName)), db(initDB) {
    std::vector<std::string> exerciseNameList;
    std::vector<Lab::Exercise> exerciseObjects;
    Lab::Exercise exerciseLoader;

    db->prepare("SELECT exercise FROM workouts WHERE workoutName = ?", name);
    while (db->retrieve_next_row()) {
        exerciseNameList.push_back(db->get_column(0));
    }
    for (const auto &exerciseName : exerciseNameList) {
        exerciseLoader.load(db, exerciseName);
        exerciseObjects.push_back(exerciseLoader);
    }

    db->prepare("SELECT * FROM workouts WHERE workoutName = ?", name);
    auto exercise = exerciseObjects.cbegin();
    while (db->retrieve_next_row()) {
        workout.push_back(
            Lab::ExerciseData(*exercise, db->get_column(3), static_cast<unsigned long>(db->get_column(4).getInt64())));
        ++exercise;
    }
}

Lab::Workout::Workout(Lab::DBConn *initDB, std::string workoutName, std::vector<Lab::ExerciseData> newWorkout)
    : name(std::move(workoutName)), db(initDB), workout(std::move(newWorkout)) {
    this->removeExercisesNotInDB();
}

void Lab::Workout::setWorkout(const std::vector<Lab::ExerciseData> &newWorkout) { workout = newWorkout; }

void Lab::Workout::addExercise(const Lab::Exercise &newExercise, const double &type1Val,
                               const unsigned long &type2Val) {
    Lab::Exercise exerciseComp;
    exerciseComp.load(db, newExercise.getName());
    if (newExercise == exerciseComp) {
        workout.push_back(Lab::ExerciseData{newExercise, type1Val, type2Val});
    }
}

void Lab::Workout::remExercise(std::vector<Lab::ExerciseData>::iterator iter) { workout.erase(iter); }

void Lab::Workout::remExercise(std::vector<Lab::ExerciseData>::iterator start,
                               std::vector<Lab::ExerciseData>::iterator end) {
    workout.erase(start, end);
}

void Lab::Workout::changeExercise(std::vector<Lab::ExerciseData>::iterator iter, const Lab::Exercise &newExercise,
                                  const double &type1Val, const unsigned long &type2Val) {
    *iter = Lab::ExerciseData{newExercise, type1Val, type2Val};
}

void Lab::Workout::changeExercise(std::vector<Lab::ExerciseData>::iterator start,
                                  std::vector<Lab::ExerciseData>::iterator end, const Lab::Exercise &newExercise,
                                  const double &type1Val, const unsigned long &type2Val) {
    while (start != end) {
        *start = Lab::ExerciseData{newExercise, type1Val, type2Val};
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
        db->prepare("DELETE FROM workouts WHERE workoutName = ? AND exOrderNum >= ?", name,
                    static_cast<long>(workout.size()));
        db->exec_prepared();
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
                            "exercise, type1, type2) VALUES (?, ?, ?, ?, ?)",
                            name, place, exc.getName(), type1, static_cast<long>(type2)) == -1) {
                return false;
            }
        } else if (index < size) {
            if (db->prepare("UPDATE workouts SET workoutName = ?, exOrderNum = ?, "
                            "exercise = ?, type1 = ?, type2 = ? WHERE workoutName = ? "
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

void Lab::Workout::removeExercisesNotInDB() {
    for (auto workoutIter = workout.begin(); workoutIter != workout.end(); ++workoutIter) {
        Lab::Exercise exercise;
        exercise.load(db, workoutIter->exc.getName());
        if (workoutIter->exc != exercise) {
            workoutIter = workout.erase(workoutIter);
            --workoutIter;
        }
    }
}
