#pragma once
///@file

#include <chrono>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "body.h"
#include "database.h"
#include "excercise.h"
#include "history.h"
#include "workout.h"

namespace Lab {
class TheLab {
   public:
    TheLab(const std::string &dbName, const std::filesystem::path &path);
    TheLab(TheLab &&) noexcept;
    TheLab(const TheLab &);
    TheLab &operator=(TheLab &&) noexcept;
    TheLab &operator=(const TheLab &);
    ~TheLab() = default;

    bool saveExcercises();
    void setExcercises(const std::vector<Lab::Excercise> &newExcercises);
    std::vector<Lab::Excercise> &getExcercises();
    const std::vector<Lab::Excercise> &getExcercises() const;
    void addExcercise(const Lab::Excercise &newExcercise);
    void removeExcercise(std::vector<Lab::Excercise>::iterator iter);

    void EditExcercise(std::vector<Lab::Excercise>::iterator iter, const Lab::Excercise &newExcercise);

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
    Lab::historyVector getHistory(const std::chrono::time_point<std::chrono::system_clock> &startDate,
                                  const std::chrono::time_point<std::chrono::system_clock> &endDate);
    const Lab::historyVector &getHistory() const;
    Lab::historyVector getHistory(const std::chrono::time_point<std::chrono::system_clock> &startDate,
                                  const std::chrono::time_point<std::chrono::system_clock> &endDate) const;
    void addItemToHistory(const Lab::historyTuple &excerciseItem);
    void removeItemFromHistory(Lab::historyVector::iterator iter);
    void removeItemFromHistory(Lab::historyVector::iterator start, Lab::historyVector::iterator end);

   private:
    std::unique_ptr<Lab::DBConn> database;
    Lab::History history;
    Lab::Body body;
    std::vector<Lab::Excercise> excercises;
    std::vector<Lab::Workout> workouts;
};

}  // namespace Lab
