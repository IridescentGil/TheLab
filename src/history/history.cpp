#include "history.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

#include "excercise.h"

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

Lab::History::History(std::shared_ptr<Lab::DBConn> newDB)
    : db(std::move(newDB)) {
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, std::string, double, unsigned long>>
        tempHist;

    db->execMulti("SELECT * FROM history");
    while (db->stepExec()) {
        tempHist.push_back(std::make_tuple(
            std::chrono::system_clock::time_point{
                std::chrono::system_clock::duration{
                    db->getColumn(DATE_DB_INDEX).getInt64()}},
            db->getColumn(WORKOUT_NAME_DB_INDEX),
            db->getColumn(HISTORY_EXCERCISE_NAME_DB_INDEX),
            db->getColumn(TYPE_1_DB_INDEX).getDouble(),
            db->getColumn(TYPE_2_DB_INDEX).getInt64()));
    }

    for (auto const &iter : tempHist) {
        auto const &[date, workoutName, excercise, type1, type2] = iter;
        std::vector<std::string> mWorked;
        std::vector<std::string> exType;
        db->prepare("SELECT * FROM excercises WHERE name = ?", excercise);
        db->stepExec();

        std::stringstream streamMusclesWorked(
            db->getColumn(MUSCLES_WORKED_DB_INDEX));
        std::stringstream streamExcerciseType(
            db->getColumn(EXCERCISE_TYPE_DB_INDEX));
        std::string tempString;
        while (std::getline(streamMusclesWorked, tempString, ',')) {
            mWorked.push_back(tempString);
        }
        while (std::getline(streamExcerciseType, tempString, ',')) {
            exType.push_back(tempString);
        }

        history.push_back(std::make_tuple(
            date, workoutName,
            Lab::Excercise(db->getColumn(EXCERCISE_NAME_DB_INDEX),
                           db->getColumn(EXCERCISE_DESCRIPTION_DB_INDEX),
                           db->getColumn(MUSCLE_GROUP_DB_INDEX), mWorked,
                           exType),
            type1, type2));
    }
}
Lab::History::History(std::shared_ptr<Lab::DBConn> newDB,
                      historyVector newHistory)
    : db(std::move(newDB)), history(std::move(newHistory)) {}

Lab::historyVector &Lab::History::getHistory() { return history; }

const Lab::historyVector &Lab::History::getHistory() const { return history; }

Lab::historyVector Lab::History::getSliceHistory(
    const std::chrono::time_point<std::chrono::system_clock> &start,
    const std::chrono::time_point<std::chrono::system_clock> &end) const {
    historyVector slice;
    for (auto const &iter : history) {
        if (std::get<0>(iter) >= start && std::get<0>(iter) < end) {
            slice.push_back(iter);
        }
    }
    return slice;
}

Lab::historyVector Lab::History::getItem(historyVector::iterator start,
                                         historyVector::iterator end) const {
    historyVector slice;
    slice.assign(start, end);
    return slice;
}

Lab::historyTuple Lab::History::getItem(historyVector::iterator iter) const {
    return *iter;
}

void Lab::History::setHistory(const historyVector &newHistory) {
    history = newHistory;
}

void Lab::History::addItem(
    const std::chrono::time_point<std::chrono::system_clock> &date,
    const std::string &workoutName, const Lab::Excercise &excercise,
    const double &type1Val, const unsigned long &type2Val) {
    history.push_back(
        std::make_tuple(date, workoutName, excercise, type1Val, type2Val));
}

void Lab::History::remItem(historyVector::iterator iter) {
    history.erase(iter);
}

void Lab::History::remItem(historyVector::iterator start,
                           historyVector::iterator end) {
    history.erase(start, end);
}

bool Lab::History::save() {
    std::sort(history.begin(), history.end(), [](auto first, auto second) {
        return (std::get<0>(first) < std::get<0>(second));
    });

    std::uint64_t size = 0;
    std::uint64_t index = 0;
    db->execMulti("SELECT ID FROM history");
    while (db->stepExec()) {
        ++size;
    }

    if (history.size() < size) {
        db->exec("DELETE FROM history");
        size = 0;
    }

    /*
     * NOTE: unsign long cast to long due SQLiteCPP not recongnizing uint64_t
     */
    for (auto const &iter : history) {
        const auto &[date, workoutName, excerciseName, type1, type2] = iter;
        if (index < size) {
            if (db->prepare("UPDATE history SET date = ?, workout = ?, "
                            "excercise = ?, type1 = ?, type2 = ? WHERE ID = ?",
                            date.time_since_epoch().count(), workoutName,
                            excerciseName.getName(), type1,
                            static_cast<long>(type2),
                            static_cast<long>(index)) == -1) {
                return false;
            }
            if (db->execQuery() == -1) {
                return false;
            }
        } else {
            if (db->prepare("INSERT INTO history (date, workout, excercise, "
                            "type1, type2) VALUES (?, ?, ?, ?, ?)",
                            date.time_since_epoch().count(), workoutName,
                            excerciseName.getName(), type1,
                            static_cast<long>(type2)) == -1) {
                return false;
            }
            if (db->execQuery() == -1) {
                return false;
            }
        }
        ++index;
    }
    return true;
}
