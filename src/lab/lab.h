#pragma once
///@file

#include <chrono>
#include <memory>
#include <string_view>
#include <vector>

#include "body.h"
#include "database.h"
#include "excercise.h"
#include "history.h"
#include "workout.h"

namespace Lab {
class TheLab {
   public:
    TheLab();
    TheLab(std::string_view dbName);
    TheLab(TheLab &&) = default;
    TheLab(const TheLab &) = default;
    TheLab &operator=(TheLab &&) = default;
    TheLab &operator=(const TheLab &) = default;
    ~TheLab() = default;

    bool saveExcercises();
    void setExcercises(const std::vector<Lab::Excercise> &newExcercises);
    std::vector<Lab::Excercise> &getExcercises();
    const std::vector<Lab::Excercise> &getExcercises() const;
    void addExcercise(const Lab::Excercise &newExcercise);
    void removeExcercise(std::vector<Lab::Excercise>::iterator iter);

    bool saveWorkouts();
    void setWorkouts(const std::vector<Lab::Workout> &newWorkouts);
    std::vector<Lab::Workout> &getWorkouts();
    const std::vector<Lab::Workout> &getWorkouts() const;
    void addWorkout(const Lab::Workout &newWorkout);
    void removeWorkout(std::vector<Lab::Workout>::iterator iter);

    bool saveBody();
    void setBody(const Lab::Body &newBody);
    Lab::Body &getBody();
    const Lab::Body &getBody() const;

    bool saveHistory();
    const Lab::History &getHistoryObject() const;
    void setHistory(const Lab::History &newHistory);
    Lab::historyVector &getHistory();
    Lab::historyVector &getHistory(std::chrono::time_point<std::chrono::system_clock> startDate,
                                   std::chrono::time_point<std::chrono::system_clock> endDate);
    const Lab::historyVector &getHistory() const;
    const Lab::historyVector &getHistory(std::chrono::time_point<std::chrono::system_clock> startDate,
                                         std::chrono::time_point<std::chrono::system_clock> endDate) const;
    void addItemToHistory(Lab::historyTuple excerciseItem);
    void removeItemFromHistory(Lab::historyVector::iterator iter);
    void removeItemFromHistory(Lab::historyVector::iterator start, Lab::historyVector::iterator end);

   private:
    std::shared_ptr<Lab::DBConn> database;
    Lab::History history;
    Lab::Body body;
    std::vector<Lab::Excercise> excercises;
    std::vector<Lab::Workout> workouts;
};

}  // namespace Lab
