#include <gtest/gtest.h>

#include <tuple>

#include "body.h"
#include "excercise.h"

bool excerEqual(Excercise a, Excercise b);
bool woTupleEqual(std::tuple<Excercise, int, int> a,
                  std::tuple<Excercise, int, int> b);
bool historyEqual(
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
        a,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
        b);
bool historyNEqual(
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
        a,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
        b,
    int diff);

testing::AssertionResult AssertExcerciseEqual(const char* m_expr,
                                              const char* n_expr, Excercise m,
                                              Excercise n);
testing::AssertionResult AssertWoTupleEqual(const char* m_expr,
                                            const char* n_expr,
                                            std::tuple<Excercise, int, int> m,
                                            std::tuple<Excercise, int, int> n);

testing::AssertionResult AssertHistoryEqual(
    const char* m_expr, const char* n_expr,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
        m,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
        n);

testing::AssertionResult AssertHistoryNEqual(
    const char* m_expr, const char* n_expr, const char* o_expr,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
        m,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Excercise, int, int>>
        n,
    int o);

bool mesEqual(Measurements a, Measurements b);
bool musEqual(Muscles a, Muscles b);

testing::AssertionResult AssertConditionsEqual(const char* m_expr,
                                               const char* n_expr, Muscles m,
                                               Muscles n);

testing::AssertionResult AssertMeasureEqual(const char* m_expr,
                                            const char* n_expr, Measurements m,
                                            Measurements n);
