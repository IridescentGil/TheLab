#include "testHelper.h"

bool excerEqual(Lab::Excercise a, Lab::Excercise b) {
    if (a.getName() != b.getName()) return false;
    if (a.getDescription() != b.getDescription()) return false;
    if (a.getMuscleGroup() != b.getMuscleGroup()) return false;
    if (a.getMusclesWorked() != b.getMusclesWorked()) return false;
    if (a.getType() != b.getType()) return false;
    return true;
};

bool workoutEqual(Lab::ExcerciseData a, Lab::ExcerciseData b) {
    const auto [aE, aT1, aT2] = a;
    const auto [bE, bT1, bT2] = b;

    if (!excerEqual(aE, bE)) return false;
    if (aT1 != bT1) return false;
    if (aT2 != bT2) return false;
    return true;
};

testing::AssertionResult AssertExcerciseEqual(const char* m_expr,
                                              const char* n_expr,
                                              Lab::Excercise m,
                                              Lab::Excercise n) {
    if (excerEqual(m, n)) return testing::AssertionSuccess();

    return testing::AssertionFailure()
           << m_expr << " and " << n_expr << " Excersises not equal";
}

testing::AssertionResult AssertWorkoutEqual(const char* m_expr,
                                            const char* n_expr,
                                            Lab::ExcerciseData m,
                                            Lab::ExcerciseData n) {
    if (workoutEqual(m, n)) return testing::AssertionSuccess();

    return testing::AssertionFailure()
           << m_expr << " and " << n_expr << " workout data are not equal";
}

bool historyEqual(
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        a,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        b) {
    if (a.size() != b.size()) return false;
    for (auto iter = a.cbegin(), bter = b.cbegin(); iter != a.cend();
         iter++, bter++) {
        auto [dateA, workoutA, excerciseA, type1A, type2A] = *iter;
        auto [dateB, workoutB, excerciseB, type1B, type2B] = *bter;
        if (dateA != dateB) return false;
        if (workoutA != workoutB) return false;
        if (!excerEqual(excerciseA, excerciseB)) return false;
        if (type1A != type1B) return false;
        if (type2A != type2B) return false;
    }
    return true;
}

int historydiff(
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        a,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        b) {
    int nDiff = 0;
    if (a.size() != b.size()) {
        nDiff = (static_cast<int>(a.size()) - static_cast<int>(b.size()) > 0)
                    ? (a.size() - b.size())
                    : -1 * (static_cast<int>(a.size()) -
                            static_cast<int>(b.size()));
    }
    if (a.size() <= b.size()) {
        for (auto iter = a.cbegin(), bter = b.cbegin(); iter != a.cend();
             iter++, bter++) {
            auto [dateA, workoutA, excerciseA, type1A, type2A] = *iter;
            auto [dateB, workoutB, excerciseB, type1B, type2B] = *bter;
            if (dateA != dateB) {
                nDiff++;
                continue;
            }
            if (workoutA != workoutB) {
                nDiff++;
                continue;
            }
            if (!excerEqual(excerciseA, excerciseB)) {
                nDiff++;
                continue;
            }
            if (type1A != type1B) {
                nDiff++;
                continue;
            }
            if (type2A != type2B) {
                nDiff++;
                continue;
            }
        }
    } else {
        for (auto iter = a.cbegin(), bter = b.cbegin(); bter != b.cend();
             iter++, bter++) {
            auto [dateA, workoutA, excerciseA, type1A, type2A] = *iter;
            auto [dateB, workoutB, excerciseB, type1B, type2B] = *bter;
            if (dateA != dateB) {
                nDiff++;
                continue;
            }
            if (workoutA != workoutB) {
                nDiff++;
                continue;
            }
            if (!excerEqual(excerciseA, excerciseB)) {
                nDiff++;
                continue;
            }
            if (type1A != type1B) {
                nDiff++;
                continue;
            }
            if (type2A != type2B) {
                nDiff++;
                continue;
            }
        }
    }
    return nDiff;
}

bool historyNEqual(
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        a,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        b,
    int diff) {
    int nDiff = 0;
    if (a.size() != b.size())
        nDiff = (static_cast<int>(a.size()) - static_cast<int>(b.size()) > 0)
                    ? (a.size() - b.size())
                    : -1 * (static_cast<int>(a.size()) -
                            static_cast<int>(b.size()));
    if (a.size() <= b.size()) {
        for (auto iter = a.cbegin(), bter = b.cbegin(); iter != a.cend();
             iter++, bter++) {
            auto [dateA, workoutA, excerciseA, type1A, type2A] = *iter;
            auto [dateB, workoutB, excerciseB, type1B, type2B] = *bter;
            if (dateA != dateB) {
                nDiff++;
                continue;
            }
            if (workoutA != workoutB) {
                nDiff++;
                continue;
            }
            if (!excerEqual(excerciseA, excerciseB)) {
                nDiff++;
                continue;
            }
            if (type1A != type1B) {
                nDiff++;
                continue;
            }
            if (type2A != type2B) {
                nDiff++;
                continue;
            }
        }
    } else {
        for (auto iter = a.cbegin(), bter = b.cbegin(); bter != b.cend();
             iter++, bter++) {
            auto [dateA, workoutA, excerciseA, type1A, type2A] = *iter;
            auto [dateB, workoutB, excerciseB, type1B, type2B] = *bter;
            if (dateA != dateB) {
                nDiff++;
                continue;
            }
            if (workoutA != workoutB) {
                nDiff++;
                continue;
            }
            if (!excerEqual(excerciseA, excerciseB)) {
                nDiff++;
                continue;
            }
            if (type1A != type1B) {
                nDiff++;
                continue;
            }
            if (type2A != type2B) {
                nDiff++;
                continue;
            }
        }
    }
    if (nDiff != diff) return false;
    return true;
}

testing::AssertionResult AssertHistoryEqual(
    const char* m_expr, const char* n_expr,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        m,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        n) {
    if (historyEqual(m, n)) return testing::AssertionSuccess();

    return testing::AssertionFailure()
           << m_expr << " and " << n_expr << " Histories not equal";
}

testing::AssertionResult AssertHistoryNEqual(
    const char* m_expr, const char* n_expr, const char* o_expr,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        m,
    std::vector<std::tuple<std::chrono::time_point<std::chrono::system_clock>,
                           std::string, Lab::Excercise, int, int>>
        n,
    int o) {
    if (historyNEqual(m, n, o)) return testing::AssertionSuccess();

    return testing::AssertionFailure()
           << m_expr << " and " << n_expr
           << " Histories difference not the expected: " << o_expr
           << "\n Instead difference is: " << historydiff(m, n);
}

bool mesEqual(Lab::Measurements a, Lab::Measurements b) {
    if (a.hips != b.hips)
        return false;
    else if (a.neck != b.neck)
        return false;
    else if (a.upperArmRight != b.upperArmRight)
        return false;
    else if (a.forearmRight != b.forearmRight)
        return false;
    else if (a.upperArmLeft != b.upperArmLeft)
        return false;
    else if (a.forearmLeft != b.forearmLeft)
        return false;
    else if (a.thighRight != b.thighRight)
        return false;
    else if (a.shoulders != b.shoulders)
        return false;
    else if (a.thighLeft != b.thighLeft)
        return false;
    else if (a.calfRight != b.calfRight)
        return false;
    else if (a.calfLeft != b.calfLeft)
        return false;
    else if (a.waist != b.waist)
        return false;
    else if (a.chest != b.chest)
        return false;
    return true;
}

bool musEqual(Lab::Muscles a, Lab::Muscles b) {
    if (a.upperBack != b.upperBack)
        return false;
    else if (a.trapezius != b.trapezius)
        return false;
    else if (a.lowerBack != b.lowerBack)
        return false;
    else if (a.hamstring != b.hamstring)
        return false;
    else if (a.pectoral != b.pectoral)
        return false;
    else if (a.forearm != b.forearm)
        return false;
    else if (a.tricep != b.tricep)
        return false;
    else if (a.glutes != b.glutes)
        return false;
    else if (a.quads != b.quads)
        return false;
    else if (a.bicep != b.bicep)
        return false;
    else if (a.lats != b.lats)
        return false;
    else if (a.calf != b.calf)
        return false;
    else if (a.abs != b.abs)
        return false;
    else if (a.neck != b.neck)
        return false;
    return true;
}

testing::AssertionResult AssertConditionsEqual(const char* m_expr,
                                               const char* n_expr,
                                               Lab::Muscles m, Lab::Muscles n) {
    if (musEqual(m, n)) return testing::AssertionSuccess();

    return testing::AssertionFailure()
           << m_expr << " and " << n_expr << " Conditions not equal";
}

testing::AssertionResult AssertMeasureEqual(const char* m_expr,
                                            const char* n_expr,
                                            Lab::Measurements m,
                                            Lab::Measurements n) {
    if (mesEqual(m, n)) return testing::AssertionSuccess();

    return testing::AssertionFailure()
           << m_expr << " and " << n_expr << " Lab::Measurements not equal";
}
