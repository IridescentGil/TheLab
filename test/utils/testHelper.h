#include <gtest/gtest.h>

#include <tuple>

#include "body.h"
#include "excercise.h"
#include "workout.h"

bool excerEqual(const Lab::Excercise& first, const Lab::Excercise& second);
bool workoutEqual(const Lab::ExcerciseData& first,
                  const Lab::ExcerciseData& second);
bool historyEqual(
    const std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, double, unsigned long>>& first,
    const std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, double, unsigned long>>&
        second);
bool historyNEqual(
    const std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, double, unsigned long>>& first,
    const std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, double, unsigned long>>& second,
    int diff);

testing::AssertionResult AssertExcerciseEqual(const char* firstExpr,
                                              const char* secondExpr,
                                              const Lab::Excercise& first,
                                              const Lab::Excercise& second);
testing::AssertionResult AssertWorkoutEqual(const char* firstExpr,
                                            const char* secondExpr,
                                            const Lab::ExcerciseData& first,
                                            const Lab::ExcerciseData& second);

testing::AssertionResult AssertHistoryEqual(
    const char* firstExpr, const char* secondExpr,
    const std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, double, unsigned long>>& first,
    const std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, double, unsigned long>>&
        second);

testing::AssertionResult AssertHistoryNEqual(
    const char* firstExpr, const char* secondExpr, const char* o_expr,
    const std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, double, unsigned long>>& first,
    const std::vector<
        std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                   std::string, Lab::Excercise, double, unsigned long>>& second,
    int third);

bool mesEqual(Lab::Measurements first, Lab::Measurements second);
bool musEqual(Lab::Muscles first, Lab::Muscles second);

testing::AssertionResult AssertConditionsEqual(const char* firstExpr,
                                               const char* secondExpr,
                                               Lab::Muscles first,
                                               Lab::Muscles second);

testing::AssertionResult AssertMeasureEqual(const char* firstExpr,
                                            const char* secondExpr,
                                            Lab::Measurements first,
                                            Lab::Measurements second);
