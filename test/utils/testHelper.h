#include <gtest/gtest.h>

#include <string>
#include <tuple>
#include <vector>

namespace Lab {
class Exercise;
struct ExerciseData;
struct Measurements;
struct Muscles;
}  // namespace Lab

bool excerEqual(const Lab::Exercise& first, const Lab::Exercise& second);
bool workoutEqual(const Lab::ExerciseData& first, const Lab::ExerciseData& second);
bool historyEqual(const std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string,
                                               Lab::Exercise, double, unsigned long>>& first,
                  const std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string,
                                               Lab::Exercise, double, unsigned long>>& second);
bool historyNEqual(const std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string,
                                                Lab::Exercise, double, unsigned long>>& first,
                   const std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string,
                                                Lab::Exercise, double, unsigned long>>& second,
                   int diff);

testing::AssertionResult AssertExerciseEqual(const char* firstExpr, const char* secondExpr, const Lab::Exercise& first,
                                             const Lab::Exercise& second);
testing::AssertionResult AssertWorkoutEqual(const char* firstExpr, const char* secondExpr,
                                            const Lab::ExerciseData& first, const Lab::ExerciseData& second);

testing::AssertionResult AssertHistoryEqual(
    const char* firstExpr, const char* secondExpr,
    const std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string, Lab::Exercise, double,
                                 unsigned long>>& first,
    const std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string, Lab::Exercise, double,
                                 unsigned long>>& second);

testing::AssertionResult AssertHistoryNEqual(
    const char* firstExpr, const char* secondExpr, const char* o_expr,
    const std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string, Lab::Exercise, double,
                                 unsigned long>>& first,
    const std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::string, Lab::Exercise, double,
                                 unsigned long>>& second,
    int third);

bool mesEqual(Lab::Measurements first, Lab::Measurements second);
bool musEqual(Lab::Muscles first, Lab::Muscles second);

testing::AssertionResult AssertConditionsEqual(const char* firstExpr, const char* secondExpr, Lab::Muscles first,
                                               Lab::Muscles second);

testing::AssertionResult AssertMeasureEqual(const char* firstExpr, const char* secondExpr, Lab::Measurements first,
                                            Lab::Measurements second);
