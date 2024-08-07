#include "lab.h"

#include <algorithm>
#include <filesystem>
#include <string>

#include "excercise.h"

Lab::TheLab::TheLab(const std::string &dbName, const std::filesystem::path &path) {
    try {
        if (std::filesystem::exists(path) || std::filesystem::create_directory(path)) {
            database = std::make_shared<Lab::DBConn>(path.string() + "/" + dbName);
            history = Lab::History(database);
            body = Lab::Body(database);
            std::vector<std::string> databaseExcercises;
            std::vector<std::string> databaseWorkouts;

            database->exec_and_retrieve("SELECT name FROM excercises");
            while (database->retrieve_next_row()) {
                databaseExcercises.push_back(database->get_column(0));
            }
            for (const auto &excerciseToRetrieve : databaseExcercises) {
                Lab::Excercise tempExcercise;
                tempExcercise.load(database, excerciseToRetrieve);
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
                workouts.push_back(Lab::Workout(database, workoutToRetrieve));
            }
        }
    } catch (std::exception e) {
        std::clog << e.what();
    }
}

bool Lab::TheLab::saveExcercises() {
    return std::ranges::all_of(excercises.cbegin(), excercises.cend(),
                               [this](const Excercise &excerciseToSave) { return excerciseToSave.save(database); });
}

void Lab::TheLab::setExcercises(const std::vector<Lab::Excercise> &newExcercises) { excercises = newExcercises; }

std::vector<Lab::Excercise> &Lab::TheLab::getExcercises() { return excercises; }

const std::vector<Lab::Excercise> &Lab::TheLab::getExcercises() const { return excercises; }

void Lab::TheLab::addExcercise(const Lab::Excercise &newExcercise) { excercises.push_back(newExcercise); }

void Lab::TheLab::removeExcercise(std::vector<Lab::Excercise>::iterator iter) { excercises.erase(iter); }

bool Lab::TheLab::saveWorkouts() {
    for (auto &workoutsToSave : workouts) {
        if (!workoutsToSave.save()) {
            return false;
        }
    }
    return true;
}

void Lab::TheLab::setWorkouts(const std::vector<Lab::Workout> &newWorkouts) { workouts = newWorkouts; }

std::vector<Lab::Workout> &Lab::TheLab::getWorkouts() { return workouts; }

const std::vector<Lab::Workout> &Lab::TheLab::getWorkouts() const { return workouts; }

void Lab::TheLab::addWorkout(const Lab::Workout &newWorkout) { workouts.push_back(newWorkout); }

void Lab::TheLab::removeWorkout(std::vector<Lab::Workout>::iterator iter) { workouts.erase(iter); }

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
