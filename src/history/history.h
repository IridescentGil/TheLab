#pragma once
///@file

#include <chrono>
#include <string>
#include <vector>

#include "exercise.h"

namespace Lab {
class DBConn;

using historyTuple =
    std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string, Lab::Exercise, double, unsigned long>;
using historyVector = std::vector<historyTuple>;

/* NOTE:  add function to move item in history.
 */
class History {
   public:
    History() {};
    History(Lab::DBConn *newDB);
    History(Lab::DBConn *newDB, historyVector newHistory);

    historyVector &getHistory();
    const historyVector &getHistory() const;

    historyVector getSliceHistory(const std::chrono::time_point<std::chrono::system_clock> &start,
                                  const std::chrono::time_point<std::chrono::system_clock> &end) const;

    historyVector getItem(historyVector::iterator start, historyVector::iterator end) const;
    historyTuple getItem(historyVector::iterator iter) const;

    bool save();

    void setHistory(const historyVector &newHistory);

    void addItem(const std::chrono::time_point<std::chrono::system_clock> &date, const std::string &workoutName,
                 const Lab::Exercise &exercise, const double &type1Val, const unsigned long &type2Val);

    void remItem(historyVector::iterator iter);
    void remItem(historyVector::iterator start, historyVector::iterator end);

   private:
    Lab::DBConn *db{};
    historyVector history;

    void sort();
    void removeExercisesNotInDB();
};

}  // namespace Lab
