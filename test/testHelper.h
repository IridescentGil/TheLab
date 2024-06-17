#include <gtest/gtest.h>

#include <tuple>

#include "body.h"
#include "excercise.h"
#include "workout.h"

bool excerEqual(Lab::Excercise a, Lab::Excercise b);
bool workoutEqual(Lab::ExcerciseData a, Lab::ExcerciseData b);
bool historyEqual(
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        a,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        b);
bool historyNEqual(
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        a,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        b,
    int diff);

testing::AssertionResult AssertExcerciseEqual(const char* m_expr,
                                              const char* n_expr,
                                              Lab::Excercise m,
                                              Lab::Excercise n);
testing::AssertionResult AssertWorkoutEqual(const char* m_expr,
                                            const char* n_expr,
                                            Lab::ExcerciseData m,
                                            Lab::ExcerciseData n);

testing::AssertionResult AssertHistoryEqual(
    const char* m_expr, const char* n_expr,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        m,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        n);

testing::AssertionResult AssertHistoryNEqual(
    const char* m_expr, const char* n_expr, const char* o_expr,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        m,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        n,
    int o);

bool mesEqual(Lab::Measurements a, Lab::Measurements b);
bool musEqual(Lab::Muscles a, Lab::Muscles b);

testing::AssertionResult AssertConditionsEqual(const char* m_expr,
                                               const char* n_expr,
                                               Lab::Muscles m, Lab::Muscles n);

testing::AssertionResult AssertMeasureEqual(const char* m_expr,
                                            const char* n_expr,
                                            Lab::Measurements m,
                                            Lab::Measurements n);
