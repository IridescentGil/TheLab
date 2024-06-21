#include "history.h"

#include <algorithm>
#include <chrono>
#include <sstream>
#include <string>
#include <tuple>

#include "excercise.h"

Lab::History::History(std::shared_ptr<Lab::DBConn> newDB) : db(newDB) {
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, std::string, int, int>>
        tempHist;

    db->execMulti("SELECT * FROM history");
    while (db->stepExec()) {
        tempHist.push_back(std::make_tuple(
            std::chrono::system_clock::time_point{
                std::chrono::system_clock::duration{
                    db->getColumn(1).getInt64()}},
            db->getColumn(2), db->getColumn(3), db->getColumn(4),
            db->getColumn(5)));
    }

    for (auto const iter : tempHist) {
        auto const &[date, workoutName, excercise, type1, type2] = iter;
        std::vector<std::string> mWorked, exType;
        db->prepare("SELECT * FROM excercises WHERE name = ?", excercise);
        db->stepExec();

        std::stringstream streamMusclesWorked(db->getColumn(3)),
            streamExcerciseType(db->getColumn(4));
        std::string tempString;
        while (std::getline(streamMusclesWorked, tempString, ','))
            mWorked.push_back(tempString);
        while (std::getline(streamExcerciseType, tempString, ','))
            exType.push_back(tempString);

        history.push_back(
            std::make_tuple(date, workoutName,
                            Lab::Excercise(db->getColumn(0), db->getColumn(1),
                                           db->getColumn(2), mWorked, exType),
                            type1, type2));
    }
}
Lab::History::History(
    std::shared_ptr<Lab::DBConn> newDB,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        newHistory)
    : db(newDB), history(newHistory) {}

std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>> &
Lab::History::getHistory() {
    return history;
}

const std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                             std::string, Lab::Excercise, int, int>> &
Lab::History::getHistory() const {
    return history;
}

std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>
Lab::History::getSliceHistory(
    const std::chrono::time_point<std::chrono::system_clock> &start,
    const std::chrono::time_point<std::chrono::system_clock> &end) const {
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        slice;
    for (auto const &iter : history) {
        if (std::get<0>(iter) >= start && std::get<0>(iter) < end) {
            slice.push_back(iter);
        }
    }
    return slice;
}

std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                       std::string, Lab::Excercise, int, int>>
Lab::History::getItem(
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>::iterator
        start,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>::iterator
        end) const {
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        slice;
    slice.assign(start, end);
    return slice;
}

std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string,
           Lab::Excercise, int, int>
Lab::History::getItem(
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>::iterator it)
    const {
    return *it;
}

void Lab::History::setHistory(
    const std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, int, int>> &newHistory) {
    history = newHistory;
}

void Lab::History::addItem(
    const std::chrono::time_point<std::chrono::system_clock> &date,
    const std::string &workoutName, const Lab::Excercise &ex,
    const int &type1Val, const int &type2Val) {
    history.push_back(
        std::make_tuple(date, workoutName, ex, type1Val, type2Val));
}

void Lab::History::remItem(
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>::iterator
        it) {
    history.erase(it);
}

void Lab::History::remItem(
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>::iterator
        start,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>::iterator
        end) {
    history.erase(start, end);
}

bool Lab::History::save() {
    std::sort(history.begin(), history.end(),
              [](auto a, auto b) { return (std::get<0>(a) < std::get<0>(b)); });

    unsigned int size = 0;
    unsigned int index = 0;
    db->execMulti("SELECT ID FROM history");
    while (db->stepExec()) size++;

    if (history.size() < size) db->exec("DELETE FROM history");

    for (auto const &iter : history) {
        const auto &[date, workoutName, excerciseName, type1, type2] = iter;
        if (index < size) {
            if (db->prepare("UPDATE history SET date = ?, workout = ?, "
                            "excercise = ?, type1 = ?, type2 = ? WHERE ID = ?",
                            date.time_since_epoch().count(), workoutName,
                            excerciseName.getName(), type1, type2, index) == -1)
                return false;
            if (db->execQuery() == -1) return false;
        } else {
            if (db->prepare("INSERT INTO history (date, workout, excercise, "
                            "type1, type2) VALUES (?, ?, ?, ?, ?)",
                            date.time_since_epoch().count(), workoutName,
                            excerciseName.getName(), type1, type2) == -1)
                return false;
            if (db->execQuery() == -1) return false;
        }
        index++;
    }
    return true;
}
