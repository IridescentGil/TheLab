#include "lab.h"

#include <algorithm>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

#include "exercise.h"

Lab::TheLab::TheLab(const std::string &dbName, const std::filesystem::path &path) {
    try {
        if (std::filesystem::exists(path) || std::filesystem::create_directory(path)) {
            database = std::make_unique<Lab::DBConn>(path.string() + "/" + dbName);
            history = Lab::History(database.get());
            body = Lab::Body(database.get());
            std::vector<std::string> databaseExercises;
            std::vector<std::string> databaseWorkouts;

            database->exec_and_retrieve("SELECT name FROM exercises");
            while (database->retrieve_next_row()) {
                databaseExercises.push_back(database->get_column(0));
            }
            for (const auto &exerciseToRetrieve : databaseExercises) {
                Lab::Exercise tempExercise;
                tempExercise.load(database.get(), exerciseToRetrieve);
                exercises.push_back(tempExercise);
            }

            database->exec_and_retrieve("SELECT workoutName FROM workouts");
            while (database->retrieve_next_row()) {
                if (std::find(databaseWorkouts.cbegin(), databaseWorkouts.cend(),
                              database->get_column(0).getString()) == databaseWorkouts.cend()) {
                    databaseWorkouts.push_back(database->get_column(0));
                }
            }
            for (const auto &workoutToRetrieve : databaseWorkouts) {
                workouts.push_back(Lab::Workout(database.get(), workoutToRetrieve));
            }
        }
    } catch (std::exception &e) {
        std::clog << e.what();
    }
}

bool Lab::TheLab::saveExercises() {
    std::vector<std::string> exerciseNames;
    bool saveSuccess = false;

    database->exec_and_retrieve("SELECT name FROM exercises");
    while (database->retrieve_next_row()) {
        exerciseNames.push_back(database->get_column(0));
    }

    saveSuccess = std::ranges::all_of(exercises.cbegin(), exercises.cend(),
                                      [this, &exerciseNames](const Exercise &exerciseToSave) {
                                          std::erase(exerciseNames, exerciseToSave.getName());
                                          return exerciseToSave.save(database.get());
                                      });
    saveSuccess &=
        std::ranges::all_of(exerciseNames.cbegin(), exerciseNames.cend(), [this](const std::string &exerciseToDelete) {
            if (database->prepare("DELETE FROM exercises WHERE name = ?", exerciseToDelete) == 1) {
                return database->exec_prepared() == 1;
            }
            return false;
        });

    return saveSuccess;
}

void Lab::TheLab::setExercises(const std::vector<Lab::Exercise> &newExercises) { exercises = newExercises; }

std::vector<Lab::Exercise> &Lab::TheLab::getExercises() { return exercises; }

const std::vector<Lab::Exercise> &Lab::TheLab::getExercises() const { return exercises; }

void Lab::TheLab::addExercise(const Lab::Exercise &newExercise) { exercises.push_back(newExercise); }

void Lab::TheLab::removeExercise(std::vector<Lab::Exercise>::iterator iter) {
    auto &historyVector = history.getHistory();

    for (auto historyIter = historyVector.begin(); historyIter != historyVector.end();) {
        if (std::get<2>(*historyIter) == *iter) {
            historyIter = historyVector.erase(historyIter);
        } else {
            ++historyIter;
        }
    }

    for (auto &workout : workouts) {
        auto &individualWorkout = workout.getWorkout();
        for (auto exerciseData = individualWorkout.begin(); exerciseData != individualWorkout.end(); ++exerciseData) {
            if (exerciseData->exc == *iter) {
                exerciseData = individualWorkout.erase(exerciseData);
            }
        }
    }

    database->prepare("DELETE FROM exercises WHERE name = ?", iter->getName());
    database->exec_prepared();
    exercises.erase(iter);
}

void Lab::TheLab::EditExercise(std::vector<Lab::Exercise>::iterator iter, const Lab::Exercise &newExercise) {
    bool smallerType = newExercise.getType().size() < iter->getType().size();

    if (iter->getName() != newExercise.getName()) {
        database->prepare("UPDATE exercises SET name = ? WHERE name = ?", newExercise.getName(), iter->getName());
        database->exec_prepared();
    }

    if (iter->getType() == newExercise.getType()) {
        auto &historyVector = history.getHistory();
        for (auto historyIter = historyVector.begin(); historyIter != historyVector.end(); ++historyIter) {
            if (std::get<2>(*historyIter) == *iter) {
                std::get<2>(*historyIter) = newExercise;
            }
        }

    } else {
        auto &historyVector = history.getHistory();
        for (auto historyIter = historyVector.begin(); historyIter != historyVector.end();) {
            if (std::get<2>(*historyIter) == *iter) {
                historyIter = historyVector.erase(historyIter);
            } else {
                ++historyIter;
            }
        }
        this->saveHistory();
    }
    for (auto &workout : workouts) {
        for (auto &exerciseData : workout.getWorkout()) {
            if (exerciseData.exc == *iter) {
                exerciseData.exc = newExercise;
                if (smallerType) {
                    exerciseData.type2 = 0;
                }
            }
        }
    }

    *iter = newExercise;
    this->saveExercises();
    this->saveWorkouts();
}

bool Lab::TheLab::saveWorkouts() {
    std::vector<std::string> workoutNames;
    bool saveSuccess = false;

    database->exec_and_retrieve("SELECT DISTINCT workoutName FROM workouts");
    while (database->retrieve_next_row()) {
        workoutNames.push_back(database->get_column(0));
    }

    saveSuccess = std::ranges::all_of(workouts.begin(), workouts.end(), [&workoutNames](Lab::Workout &workoutsToSave) {
        std::erase(workoutNames, workoutsToSave.getName());
        return workoutsToSave.save();
    });

    saveSuccess &= std::ranges::all_of(workoutNames.cbegin(), workoutNames.cend(), [this](const auto &workoutToDelete) {
        if (database->prepare("DELETE FROM workouts WHERE workoutName = ?", workoutToDelete) == 1) {
            if (database->exec_prepared() == -1) {
                return false;
            }
        }
        return true;
    });
    return saveSuccess;
}

void Lab::TheLab::setWorkouts(const std::vector<Lab::Workout> &newWorkouts) { workouts = newWorkouts; }

std::vector<Lab::Workout> &Lab::TheLab::getWorkouts() { return workouts; }

const std::vector<Lab::Workout> &Lab::TheLab::getWorkouts() const { return workouts; }

void Lab::TheLab::addWorkout(const Lab::Workout &newWorkout) { workouts.push_back(newWorkout); }

void Lab::TheLab::removeWorkout(std::vector<Lab::Workout>::iterator iter) {
    database->prepare("DELETE FROM workouts WHERE workoutName = ?", iter->getName());
    database->exec_prepared();
    workouts.erase(iter);
}

bool Lab::TheLab::saveBody() { return body.save() == 1; }

void Lab::TheLab::setBody(const Lab::Body &newBody) { body = newBody; }

Lab::Body &Lab::TheLab::getBody() { return body; }

const Lab::Body &Lab::TheLab::getBody() const { return body; }

bool Lab::TheLab::saveHistory() { return history.save(); }

const Lab::History &Lab::TheLab::getHistoryObject() const { return history; }

void Lab::TheLab::setHistory(const Lab::History &newHistory) { history = newHistory; }

Lab::historyVector &Lab::TheLab::getHistory() { return history.getHistory(); }

Lab::historyVector Lab::TheLab::getHistory(const std::chrono::time_point<std::chrono::system_clock> &startDate,
                                           const std::chrono::time_point<std::chrono::system_clock> &endDate) {
    auto sliceHistory = history.getSliceHistory(startDate, endDate);
    return sliceHistory;
}

const Lab::historyVector &Lab::TheLab::getHistory() const { return history.getHistory(); }

Lab::historyVector Lab::TheLab::getHistory(const std::chrono::time_point<std::chrono::system_clock> &startDate,
                                           const std::chrono::time_point<std::chrono::system_clock> &endDate) const {
    const auto sliceHistory = history.getSliceHistory(startDate, endDate);

    return sliceHistory;
}

void Lab::TheLab::addItemToHistory(const Lab::historyTuple &exerciseItem) {
    const auto &[date, workoutName, exercise, type1Val, type2Val] = exerciseItem;
    history.addItem(date, workoutName, exercise, type1Val, type2Val);
}

void Lab::TheLab::removeItemFromHistory(Lab::historyVector::iterator iter) { history.remItem(iter); }

void Lab::TheLab::removeItemFromHistory(Lab::historyVector::iterator start, Lab::historyVector::iterator end) {
    history.remItem(start, end);
}
