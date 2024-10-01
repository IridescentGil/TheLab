#include "history.h"

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

#include "database.h"
#include "exercise.h"

namespace {
enum HISTORY_DATABASE_INDEXES {
    DATE_DB_INDEX = 1,
    WORKOUT_NAME_DB_INDEX,
    HISTORY_EXCERCISE_NAME_DB_INDEX,
    TYPE_1_DB_INDEX,
    TYPE_2_DB_INDEX
};

enum EXCERCISE_DATABASE_INDEXES {
    EXCERCISE_NAME_DB_INDEX = 0,
    EXCERCISE_DESCRIPTION_DB_INDEX,
    MUSCLE_GROUP_DB_INDEX,
    MUSCLES_WORKED_DB_INDEX,
    EXCERCISE_TYPE_DB_INDEX
};
}  // namespace

Lab::History::History(Lab::DBConn *newDB) : db(newDB) {
    std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string, std::string, double, unsigned long>>
        tempHist;

    db->exec_and_retrieve("SELECT * FROM history");
    while (db->retrieve_next_row()) {
        tempHist.push_back(std::make_tuple(
            std::chrono::system_clock::time_point{
                std::chrono::system_clock::duration{db->get_column(DATE_DB_INDEX).getInt64()}},
            db->get_column(WORKOUT_NAME_DB_INDEX), db->get_column(HISTORY_EXCERCISE_NAME_DB_INDEX),
            db->get_column(TYPE_1_DB_INDEX).getDouble(), db->get_column(TYPE_2_DB_INDEX).getInt64()));
    }

    for (auto const &iter : tempHist) {
        auto const &[date, workoutName, exercise, type1, type2] = iter;
        std::vector<std::string> mWorked;
        std::vector<std::string> exType;
        db->prepare("SELECT * FROM exercises WHERE name = ?", exercise);
        db->retrieve_next_row();

        std::stringstream streamMusclesWorked(db->get_column(MUSCLES_WORKED_DB_INDEX));
        std::stringstream streamExerciseType(db->get_column(EXCERCISE_TYPE_DB_INDEX));
        std::string tempString;
        while (std::getline(streamMusclesWorked, tempString, ',')) {
            mWorked.push_back(tempString);
        }
        while (std::getline(streamExerciseType, tempString, ',')) {
            exType.push_back(tempString);
        }

        history.push_back(std::make_tuple(
            date, workoutName,
            Lab::Exercise(db->get_column(EXCERCISE_NAME_DB_INDEX), db->get_column(EXCERCISE_DESCRIPTION_DB_INDEX),
                          db->get_column(MUSCLE_GROUP_DB_INDEX), mWorked, exType),
            type1, type2));
    }
    this->sort();
}
Lab::History::History(Lab::DBConn *newDB, historyVector newHistory) : db(newDB), history(std::move(newHistory)) {
    this->removeExercisesNotInDB();
    this->sort();
}

Lab::historyVector &Lab::History::getHistory() { return history; }

const Lab::historyVector &Lab::History::getHistory() const { return history; }

Lab::historyVector Lab::History::getSliceHistory(const std::chrono::time_point<std::chrono::system_clock> &start,
                                                 const std::chrono::time_point<std::chrono::system_clock> &end) const {
    historyVector slice;
    for (auto const &iter : history) {
        if (std::get<0>(iter) >= start && std::get<0>(iter) < end) {
            slice.push_back(iter);
        }
    }
    return slice;
}

Lab::historyVector Lab::History::getItem(historyVector::iterator start, historyVector::iterator end) const {
    historyVector slice;
    slice.assign(start, end);
    return slice;
}

Lab::historyTuple Lab::History::getItem(historyVector::iterator iter) const { return *iter; }

void Lab::History::setHistory(const historyVector &newHistory) { history = newHistory; }

void Lab::History::addItem(const std::chrono::time_point<std::chrono::system_clock> &date,
                           const std::string &workoutName, const Lab::Exercise &exercise, const double &type1Val,
                           const unsigned long &type2Val) {
    Lab::Exercise exerciseComp;
    exerciseComp.load(db, exercise.getName());
    if (exercise == exerciseComp) {
        history.push_back(std::make_tuple(date, workoutName, exercise, type1Val, type2Val));
    }
    this->sort();
}

void Lab::History::remItem(historyVector::iterator iter) { history.erase(iter); }

void Lab::History::remItem(historyVector::iterator start, historyVector::iterator end) { history.erase(start, end); }

void Lab::History::sort() {
    if (history.empty()) {
        return;
    }

    auto getTimeDigit = [](const std::chrono::time_point<std::chrono::system_clock> &date, long digit) {
        auto time = date.time_since_epoch().count();
        return (time / digit) % 10;
    };

    // Maximum number of digits to consider, i.e., maximum milliseconds count from epoch
    long maxDigit = 1;
    for (const auto &tuple : history) {
        auto time = std::get<0>(tuple).time_since_epoch().count();
        maxDigit = std::max(maxDigit, time);
    }

    // Radix sort for the date
    for (long digit = 1; digit <= maxDigit; digit *= 10) {
        std::stable_sort(history.begin(), history.end(), [&](const historyTuple &first, const historyTuple &second) {
            return getTimeDigit(std::get<0>(first), digit) < getTimeDigit(std::get<0>(second), digit);
        });
    }
}

void Lab::History::removeExercisesNotInDB() {
    for (auto historyIter = history.begin(); historyIter != history.end(); ++historyIter) {
        Lab::Exercise exercise;
        exercise.load(db, std::get<2>(*historyIter).getName());
        if (std::get<2>(*historyIter) != exercise) {
            historyIter = history.erase(historyIter);
            --historyIter;
        }
    }
}

bool Lab::History::save() {
    std::uint64_t size = 0;
    std::uint64_t index = 0;
    db->exec_and_retrieve("SELECT ID FROM history");
    while (db->retrieve_next_row()) {
        ++size;
    }

    if (history.size() < size) {
        db->prepare("DELETE FROM history WHERE ID >= ?", static_cast<long>(history.size()));
        db->exec_prepared();
    }

    /*
     * NOTE: unsign long cast to long due SQLiteCPP not recongnizing uint64_t
     */
    for (auto const &iter : history) {
        const auto &[date, workoutName, exerciseName, type1, type2] = iter;
        if (index < size) {
            if (db->prepare("UPDATE history SET date = ?, workout = ?, "
                            "exercise = ?, type1 = ?, type2 = ? WHERE ID = ?",
                            date.time_since_epoch().count(), workoutName, exerciseName.getName(), type1,
                            static_cast<long>(type2), static_cast<long>(index)) == -1) {
                return false;
            }
            if (db->exec_prepared() == -1) {
                return false;
            }
        } else {
            if (db->prepare("INSERT INTO history (ID, date, workout, exercise, "
                            "type1, type2) VALUES (?, ?, ?, ?, ?, ?)",
                            static_cast<long>(index), date.time_since_epoch().count(), workoutName,
                            exerciseName.getName(), type1, static_cast<long>(type2)) == -1) {
                return false;
            }
            if (db->exec_prepared() == -1) {
                return false;
            }
        }
        ++index;
    }
    return true;
}
