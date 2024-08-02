#include "testHelper.h"

#include <string>

#include "history.h"

bool excerEqual(const Lab::Excercise &first, const Lab::Excercise &second) { return first == second; };

bool workoutEqual(const Lab::ExcerciseData &first, const Lab::ExcerciseData &second) {
    const auto [aE, aT1, aT2] = first;
    const auto [bE, bT1, bT2] = second;

    if (aE != bE) {
        return false;
    }
    if (aT1 != bT1) {
        return false;
    }
    if (aT2 != bT2) {
        return false;
    }
    return true;
};

testing::AssertionResult AssertExcerciseEqual(const char *firstExpr, const char *secondExpr,
                                              const Lab::Excercise &first, const Lab::Excercise &second) {
    if (excerEqual(first, second)) {
        return testing::AssertionSuccess();
    }
    auto vectostr = [](const std::vector<std::string> &stringArray) {
        std::string flatString;
        for (const auto &index : stringArray) {
            flatString += index;
            if (*(stringArray.end() - 1) != index) {
                flatString += ", ";
            }
        }
        return flatString;
    };

    return testing::AssertionFailure() << "Excercises:\n"
                                       << firstExpr << ":\nName: " << first.getName()
                                       << "\nDescription: " << first.getDescription()
                                       << "\nMuscle Group: " << first.getMuscleGroup()
                                       << "\nMuscles Worked: " << vectostr(first.getMusclesWorked())
                                       << "\nTypes: " << vectostr(first.getType()) << "\n"
                                       << " and " << secondExpr << ":\nName: " << second.getName()
                                       << "\nDescription: " << second.getDescription()
                                       << "\nMuscle Group: " << second.getMuscleGroup()
                                       << "\nMuscles Worked: " << vectostr(second.getMusclesWorked())
                                       << "\nTypes: " << vectostr(second.getType()) << "\n"
                                       << " not equal";
}

testing::AssertionResult AssertWorkoutEqual(const char *firstExpr, const char *secondExpr,
                                            const Lab::ExcerciseData &first, const Lab::ExcerciseData &second) {
    if (workoutEqual(first, second)) {
        return testing::AssertionSuccess();
    }

    return testing::AssertionFailure() << "Expected equality of ExcerciseData:\n"
                                       << firstExpr << "\n which is:\n ExcerciseData(Excercise(" << first.exc.getName()
                                       << ", " << first.exc.getDescription() << ", " << first.exc.getMuscleGroup()
                                       << ", " << first.exc.getMusclesWorked().size() << ", "
                                       << first.exc.getType().size() << "), " << first.type1 << ", " << first.type2
                                       << ")"
                                       << "\nand\n"
                                       << secondExpr << " which is:\n ExcerciseData(Excercise(" << second.exc.getName()
                                       << ", " << second.exc.getDescription() << ", " << second.exc.getMuscleGroup()
                                       << ", " << second.exc.getMusclesWorked().size() << ", "
                                       << second.exc.getType().size() << "), " << second.type1 << ", " << second.type2
                                       << ")";
}

bool historyEqual(const Lab::historyVector &first, const Lab::historyVector &second) {
    if (first.size() != second.size()) {
        return false;
    }
    for (auto iter = first.cbegin(), bter = second.cbegin(); iter != first.cend(); ++iter, ++bter) {
        auto [dateA, workoutA, excerciseA, type1A, type2A] = *iter;
        auto [dateB, workoutB, excerciseB, type1B, type2B] = *bter;
        if (dateA != dateB) {
            return false;
        }
        if (workoutA != workoutB) {
            return false;
        }
        if (!excerEqual(excerciseA, excerciseB)) {
            return false;
        }
        if (type1A != type1B) {
            return false;
        }
        if (type2A != type2B) {
            return false;
        }
    }
    return true;
}

int historyDiff(const Lab::historyVector &first, const Lab::historyVector &second) {
    int difference = 0;
    Lab::historyVector::const_iterator smallVector;
    Lab::historyVector::const_iterator largeVector;
    Lab::historyVector::const_iterator smallVectorEnd;

    if (first.size() != second.size()) {
        if (static_cast<int>(first.size()) - static_cast<int>(second.size()) > 0) {
            difference = static_cast<int>(first.size() - second.size());
            smallVector = second.cbegin();
            largeVector = first.cbegin();
            smallVectorEnd = second.cend();
        } else {
            difference = -1 * (static_cast<int>(first.size()) - static_cast<int>(second.size()));
            smallVector = first.cbegin();
            largeVector = second.cbegin();
            smallVectorEnd = first.cend();
        }
    } else {
        smallVector = first.cbegin();
        smallVectorEnd = first.cend();
        largeVector = second.cbegin();
    }
    for (; smallVector != smallVectorEnd; ++smallVector, ++largeVector) {
        auto [dateA, workoutA, excerciseA, type1A, type2A] = *smallVector;
        auto [dateB, workoutB, excerciseB, type1B, type2B] = *largeVector;
        if (dateA != dateB) {
            ++difference;
            continue;
        }
        if (workoutA != workoutB) {
            ++difference;
            continue;
        }
        if (!excerEqual(excerciseA, excerciseB)) {
            ++difference;
            continue;
        }
        if (type1A != type1B) {
            ++difference;
            continue;
        }
        if (type2A != type2B) {
            ++difference;
            continue;
        }
    }
    return difference;
}

bool historyNEqual(const Lab::historyVector &first, const Lab::historyVector &second, int diff) {
    return historyDiff(first, second) == diff;
}

std::string historyShowDifferences(const Lab::historyVector &first, const Lab::historyVector &second) {
    if (first.size() != second.size()) {
        return "History's are not the same size";
    }
    std::string errorMessage;
    int index = 0;
    for (auto iter = first.cbegin(), bter = second.cbegin(); iter != first.cend(); ++iter, ++bter) {
        auto [dateA, workoutA, excerciseA, type1A, type2A] = *iter;
        auto [dateB, workoutB, excerciseB, type1B, type2B] = *bter;
        errorMessage += "[" + std::to_string(index) + "]:{\n";
        index++;
        if (dateA != dateB) {
            errorMessage += "Dates are not equal " + std::to_string(dateA.time_since_epoch().count()) + " and " +
                            std::to_string(dateB.time_since_epoch().count()) + "\n";
        }
        if (workoutA != workoutB) {
            errorMessage += "Workouts are not equal " + workoutA + " and " + workoutB + "\n";
        }
        if (!excerEqual(excerciseA, excerciseB)) {
            errorMessage += "Excercises are not equal: " + excerciseA.getName() + " and " + excerciseB.getName() + "\n";
        }
        if (type1A != type1B) {
            errorMessage += "Type 1 are not equal: " + std::to_string(type1A) + " and " + std::to_string(type1B) + "\n";
        }
        if (type2A != type2B) {
            errorMessage += "Type 2 are not equal: " + std::to_string(type2A) + " and " + std::to_string(type2B) + "\n";
        }
        errorMessage += "}\n";
    }
    return errorMessage;
}

testing::AssertionResult AssertHistoryEqual(const char *firstExpr, const char *secondExpr,
                                            const Lab::historyVector &first, const Lab::historyVector &second) {
    if (historyEqual(first, second)) {
        return testing::AssertionSuccess();
    }

    return testing::AssertionFailure() << firstExpr << " and " << secondExpr << " Histories not equal\n"
                                       << historyDiff(first, second) << " Differences.\n"
                                       << historyShowDifferences(first, second);
}

testing::AssertionResult AssertHistoryNEqual(const char *firstExpr, const char *secondExpr, const char *o_expr,
                                             const Lab::historyVector &first, const Lab::historyVector &second,
                                             int third) {
    if (historyNEqual(first, second, third)) {
        return testing::AssertionSuccess();
    }

    return testing::AssertionFailure() << firstExpr << " and " << secondExpr
                                       << " Histories difference not the expected: " << o_expr
                                       << "\n Instead difference is: " << historyDiff(first, second);
}

bool mesEqual(Lab::Measurements first, Lab::Measurements second) {
    if (first.hips != second.hips) {
        return false;
    }
    if (first.neck != second.neck) {
        return false;
    }
    if (first.upperArmRight != second.upperArmRight) {
        return false;
    }
    if (first.forearmRight != second.forearmRight) {
        return false;
    }
    if (first.upperArmLeft != second.upperArmLeft) {
        return false;
    }
    if (first.forearmLeft != second.forearmLeft) {
        return false;
    }
    if (first.thighRight != second.thighRight) {
        return false;
    }
    if (first.shoulders != second.shoulders) {
        return false;
    }
    if (first.thighLeft != second.thighLeft) {
        return false;
    }
    if (first.calfRight != second.calfRight) {
        return false;
    }
    if (first.calfLeft != second.calfLeft) {
        return false;
    }
    if (first.waist != second.waist) {
        return false;
    }
    if (first.chest != second.chest) {
        return false;
    }
    return true;
}

bool musEqual(Lab::Muscles first, Lab::Muscles second) {
    if (first.upperBack != second.upperBack) {
        return false;
    }
    if (first.trapezius != second.trapezius) {
        return false;
    }
    if (first.lowerBack != second.lowerBack) {
        return false;
    }
    if (first.hamstring != second.hamstring) {
        return false;
    }
    if (first.pectoral != second.pectoral) {
        return false;
    }
    if (first.forearm != second.forearm) {
        return false;
    }
    if (first.tricep != second.tricep) {
        return false;
    }
    if (first.glutes != second.glutes) {
        return false;
    }
    if (first.quads != second.quads) {
        return false;
    }
    if (first.bicep != second.bicep) {
        return false;
    }
    if (first.lats != second.lats) {
        return false;
    }
    if (first.calf != second.calf) {
        return false;
    }
    if (first.abs != second.abs) {
        return false;
    }
    if (first.neck != second.neck) {
        return false;
    }
    return true;
}

testing::AssertionResult AssertConditionsEqual(const char *firstExpr, const char *secondExpr, Lab::Muscles first,
                                               Lab::Muscles second) {
    if (musEqual(first, second)) {
        return testing::AssertionSuccess();
    }

    return testing::AssertionFailure() << firstExpr << " and " << secondExpr << " Conditions not equal";
}

testing::AssertionResult AssertMeasureEqual(const char *firstExpr, const char *secondExpr, Lab::Measurements first,
                                            Lab::Measurements second) {
    if (mesEqual(first, second)) {
        return testing::AssertionSuccess();
    }

    return testing::AssertionFailure() << firstExpr << " and " << secondExpr << " Lab::Measurements not equal";
}
