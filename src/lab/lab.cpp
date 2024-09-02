#include "lab.h"

#include <algorithm>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

#include "excercise.h"

Lab::TheLab::TheLab(const std::string &dbName, const std::filesystem::path &path) {
    try {
        if (std::filesystem::exists(path) || std::filesystem::create_directory(path)) {
            database = std::make_unique<Lab::DBConn>(path.string() + "/" + dbName);
            history = Lab::History(database.get());
            body = Lab::Body(database.get());
            std::vector<std::string> databaseExcercises;
            std::vector<std::string> databaseWorkouts;

            database->exec_and_retrieve("SELECT name FROM excercises");
            while (database->retrieve_next_row()) {
                databaseExcercises.push_back(database->get_column(0));
            }
            for (const auto &excerciseToRetrieve : databaseExcercises) {
                Lab::Excercise tempExcercise;
                tempExcercise.load(database.get(), excerciseToRetrieve);
                excercises.push_back(tempExcercise);
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

bool Lab::TheLab::saveExcercises() {
    std::vector<std::string> excerciseNames;
    bool saveSuccess = false;

    database->exec_and_retrieve("SELECT name FROM excercises");
    while (database->retrieve_next_row()) {
        excerciseNames.push_back(database->get_column(0));
    }

    saveSuccess = std::ranges::all_of(excercises.cbegin(), excercises.cend(),
                                      [this, &excerciseNames](const Excercise &excerciseToSave) {
                                          std::erase(excerciseNames, excerciseToSave.getName());
                                          return excerciseToSave.save(database.get());
                                      });
    saveSuccess &= std::ranges::all_of(
        excerciseNames.cbegin(), excerciseNames.cend(), [this](const std::string &excerciseToDelete) {
            if (database->prepare("DELETE FROM excercises WHERE name = ?", excerciseToDelete) == 1) {
                return database->exec_prepared() == 1;
            }
            return false;
        });

    return saveSuccess;
}

void Lab::TheLab::setExcercises(const std::vector<Lab::Excercise> &newExcercises) { excercises = newExcercises; }

std::vector<Lab::Excercise> &Lab::TheLab::getExcercises() { return excercises; }

const std::vector<Lab::Excercise> &Lab::TheLab::getExcercises() const { return excercises; }

void Lab::TheLab::addExcercise(const Lab::Excercise &newExcercise) { excercises.push_back(newExcercise); }

void Lab::TheLab::removeExcercise(std::vector<Lab::Excercise>::iterator iter) {
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
        for (auto excerciseData = individualWorkout.begin(); excerciseData != individualWorkout.end();
             ++excerciseData) {
            if (excerciseData->exc == *iter) {
                excerciseData = individualWorkout.erase(excerciseData);
            }
        }
    }

    database->prepare("DELETE FROM excercises WHERE name = ?", iter->getName());
    database->exec_prepared();
    excercises.erase(iter);
}

void Lab::TheLab::EditExcercise(std::vector<Lab::Excercise>::iterator iter, const Lab::Excercise &newExcercise) {
    bool smallerType = newExcercise.getType().size() < iter->getType().size();

    if (iter->getName() != newExcercise.getName()) {
        database->prepare("UPDATE excercises SET name = ? WHERE name = ?", newExcercise.getName(), iter->getName());
        database->exec_prepared();
    }

    if (iter->getType() == newExcercise.getType()) {
        auto &historyVector = history.getHistory();
        for (auto historyIter = historyVector.begin(); historyIter != historyVector.end(); ++historyIter) {
            if (std::get<2>(*historyIter) == *iter) {
                std::get<2>(*historyIter) = newExcercise;
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
        for (auto &excerciseData : workout.getWorkout()) {
            if (excerciseData.exc == *iter) {
                excerciseData.exc = newExcercise;
                if (smallerType) {
                    excerciseData.type2 = 0;
                }
            }
        }
    }

    *iter = newExcercise;
    this->saveExcercises();
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

void Lab::TheLab::addItemToHistory(const Lab::historyTuple &excerciseItem) {
    const auto &[date, workoutName, excercise, type1Val, type2Val] = excerciseItem;
    history.addItem(date, workoutName, excercise, type1Val, type2Val);
}

void Lab::TheLab::removeItemFromHistory(Lab::historyVector::iterator iter) { history.remItem(iter); }

void Lab::TheLab::removeItemFromHistory(Lab::historyVector::iterator start, Lab::historyVector::iterator end) {
    history.remItem(start, end);
}
