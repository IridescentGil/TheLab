#pragma once
///@file

#include <chrono>
#include <string>
#include <vector>

#include "database.h"
#include "excercise.h"

namespace Lab {

using historyTuple =
    std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string, Lab::Excercise, double, unsigned long>;
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
                 const Lab::Excercise &excercise, const double &type1Val, const unsigned long &type2Val);

    void remItem(historyVector::iterator iter);
    void remItem(historyVector::iterator start, historyVector::iterator end);

   private:
    Lab::DBConn *db{};
    historyVector history;

    void sort();
    void removeExcercisesNotInDB();
};

}  // namespace Lab
