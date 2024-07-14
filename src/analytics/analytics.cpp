#include "analytics.h"

#include <algorithm>
#include <chrono>
#include <cstddef>

#include "history.h"

namespace {
using dateTimePoint = std::chrono::time_point<std::chrono::system_clock>;

double formula(const double &weight, const double &reps) {
    const double UPPER = 36.0;
    const double LOWER = 37.0;

    return weight * (UPPER / (LOWER - reps));
}

namespace formulaRepPercentages {
const double TWO_REP_PERCENTAGE = 0.95;
const double THREE_REP_PERCENTAGE = 0.90;
const double FOUR_REP_PERCENTAGE = 0.88;
const double FIVE_REP_PERCENTAGE = 0.86;
const double SIX_REP_PERCENTAGE = 0.83;
const double SEVEN_REP_PERCENTAGE = 0.80;
const double EIGHT_REP_PERCENTAGE = 0.78;
const double NINE_REP_PERCENTAGE = 0.76;
const double TEN_REP_PERCENTAGE = 0.75;
const double ELEVEN_REP_PERCENTAGE = 0.72;
const double TWELVE_REP_PERCENTAGE = 0.70;
};  // namespace formulaRepPercentages

enum REP_RANGE {
    ONE_REP = 1,
    TWO_REPS,
    THREE_REPS,
    FOUR_REPS,
    FIVE_REPS,
    SIX_REPS,
    SEVEN_REPS,
    EIGHT_REPS,
    NINE_REPS,
    TEN_REPS,
    ELEVEN_REPS,
    TWELVE_REPS
};

template <typename Rep, typename Period>
void addHistoryValueToDateTotal(const double &valueToAdd,
                                std::chrono::duration<Rep, Period> /* unused */,
                                const Lab::historyTuple &historyItem,
                                Lab::Analytics::analyticsMap &map) {
    std::chrono::time_point<std::chrono::system_clock,
                            std::chrono::duration<Rep, Period>>
        date = std::chrono::time_point_cast<std::chrono::duration<Rep, Period>>(
            std::get<dateTimePoint>(historyItem));
    auto index = map.find(date);

    if (index == map.end()) {
        map[date] = valueToAdd;
    } else {
        index->second += valueToAdd;
    }
}

double getCorrectValueFromTupleIndex(
    const Lab::historyTuple &historyItem,
    const std::vector<std::string> &excerciseType, std::string_view &type) {
    size_t typeIndexInTuple =
        std::find(excerciseType.cbegin(), excerciseType.cend(), type) -
        excerciseType.cbegin();

    if (typeIndexInTuple == 0) {
        return std::get<3>(historyItem);
    }
    if (typeIndexInTuple == 1) {
        return static_cast<double>(std::get<4>(historyItem));
    }

    return 0;
}

void populateMapWithHighestTypeValues(
    const Lab::historyVector &history, Lab::Analytics::analyticsMap &map,
    const Lab::Excercise &excercise,
    const std::vector<std::string> &excerciseType, std::string_view &type) {
    double typeValue = 0;

    for (auto const &historyIter : history) {
        if (std::get<Lab::Excercise>(historyIter) == excercise) {
            typeValue =
                getCorrectValueFromTupleIndex(historyIter, excerciseType, type);
            auto date = std::chrono::time_point_cast<std::chrono::days>(
                std::get<dateTimePoint>(historyIter));
            auto index = map.find(date);

            if (index == map.end()) {
                map[date] = typeValue;
            } else if (index->second < typeValue) {
                index->second = typeValue;
            };
        }
    }
}

void populateMapWithHighestVolumeValues(const Lab::historyVector &history,
                                        Lab::Analytics::analyticsMap &map,
                                        const Lab::Excercise &excercise) {
    double workoutVolume = 0;

    for (auto const &historyIter : history) {
        if (std::get<Lab::Excercise>(historyIter) == excercise) {
            auto date = std::chrono::time_point_cast<std::chrono::days>(
                std::get<dateTimePoint>(historyIter));
            auto index = map.find(date);
            workoutVolume = std::get<3>(historyIter) *
                            static_cast<double>(std::get<4>(historyIter));

            if (index == map.end()) {
                map[date] = workoutVolume;
            } else if (index->second < workoutVolume) {
                index->second = workoutVolume;
            }
        }
    }
}

void populateMapWithHighestPaceValues(const Lab::historyVector &history,
                                      Lab::Analytics::analyticsMap &map,
                                      const Lab::Excercise &excercise) {
    double workoutPace = 0;

    for (auto const &historyIter : history) {
        if (std::get<Lab::Excercise>(historyIter) == excercise) {
            auto date = std::chrono::time_point_cast<std::chrono::days>(
                std::get<dateTimePoint>(historyIter));
            auto index = map.find(date);
            workoutPace = static_cast<double>(std::get<4>(historyIter)) /
                          std::get<3>(historyIter);

            if (index == map.end()) {
                map[date] = workoutPace;
            } else if (index->second < workoutPace) {
                index->second = workoutPace;
            }
        }
    }
}

void populateMapWithHighestSpeedValues(const Lab::historyVector &history,
                                       Lab::Analytics::analyticsMap &map,
                                       const Lab::Excercise &excercise) {
    const double ONE_HOUR_IN_SECONDS = 3600;
    double workoutSpeed = 0;

    for (auto const &historyIter : history) {
        if (std::get<Lab::Excercise>(historyIter) == excercise) {
            auto date = std::chrono::time_point_cast<std::chrono::days>(
                std::get<dateTimePoint>(historyIter));
            auto index = map.find(date);
            workoutSpeed = std::get<3>(historyIter) /
                           static_cast<double>(std::get<4>(historyIter)) *
                           ONE_HOUR_IN_SECONDS;

            if (index == map.end()) {
                map[date] = workoutSpeed;
            } else if (index->second < workoutSpeed) {
                index->second = workoutSpeed;
            }
        }
    }
}

void populateMapWithTotalTypeValues(
    const Lab::historyVector &history, Lab::Analytics::analyticsMap &map,
    const Lab::Excercise &excercise,
    const std::vector<std::string> &excerciseType, std::string_view &type) {
    for (auto const &historyIter : history) {
        if (std::get<Lab::Excercise>(historyIter) == excercise) {
            addHistoryValueToDateTotal(
                getCorrectValueFromTupleIndex(historyIter, excerciseType, type),
                std::chrono::days(), historyIter, map);
        }
    }
}

void populateMapWithTotalVolumeValues(const Lab::historyVector &history,
                                      Lab::Analytics::analyticsMap &map,
                                      const Lab::Excercise &excercise) {
    for (auto const &historyIter : history) {
        if (std::get<Lab::Excercise>(historyIter) == excercise) {
            addHistoryValueToDateTotal(
                std::get<3>(historyIter) *
                    static_cast<double>(std::get<4>(historyIter)),
                std::chrono::days(), historyIter, map);
        }
    }
}

template <typename Rep, typename Period>
void populateMapWithRepValuesForPeriod(
    const Lab::historyVector &history, std::string_view valueType,
    std::chrono::duration<Rep, Period> /*unused*/,
    Lab::Analytics::analyticsMap &map) {
    for (auto const &historyIter : history) {
        const auto &excerciseType =
            std::get<Lab::Excercise>(historyIter).getType();
        if (std::find(excerciseType.cbegin(), excerciseType.cend(),
                      valueType) != excerciseType.cend()) {
            addHistoryValueToDateTotal(
                getCorrectValueFromTupleIndex(historyIter, excerciseType,
                                              valueType),
                std::chrono::duration<Rep, Period>(), historyIter, map);
        }
    }
}

template <typename Rep, typename Period>
void populateMapWithSetValuesForPeriod(
    const Lab::historyVector &history,
    std::chrono::duration<Rep, Period> /*unused*/,
    Lab::Analytics::analyticsMap &map) {
    for (auto const &historyIter : history) {
        addHistoryValueToDateTotal(1, std::chrono::duration<Rep, Period>(),
                                   historyIter, map);
    }
}

template <typename Rep, typename Period>
void populateMapWithVolumeValuesForPeriod(
    const Lab::historyVector &history,
    std::chrono::duration<Rep, Period> /*unused*/,
    Lab::Analytics::analyticsMap &map) {
    for (auto const &historyIter : history) {
        const auto &excerciseType =
            std::get<Lab::Excercise>(historyIter).getType();
        if (excerciseType == std::vector<std::string>({"weight", "reps"})) {
            addHistoryValueToDateTotal(
                std::get<3>(historyIter) *
                    static_cast<double>(std::get<4>(historyIter)),
                std::chrono::duration<Rep, Period>(), historyIter, map);
        }
    }
}

template <typename Rep, typename Period>
void populateMapWithWorkoutValuesForPeriod(
    const Lab::historyVector &history,
    std::chrono::duration<Rep, Period> /*unused*/,
    Lab::Analytics::analyticsMap &map) {
    std::vector<std::chrono::time_point<std::chrono::system_clock>> dayCounted;
    for (auto const &historyIter : history) {
        if (std::find(dayCounted.cbegin(), dayCounted.cend(),
                      std::get<dateTimePoint>(historyIter)) ==
            dayCounted.cend()) {
            addHistoryValueToDateTotal(1, std::chrono::duration<Rep, Period>(),
                                       historyIter, map);
            dayCounted.push_back(std::get<dateTimePoint>(historyIter));
        }
    }
}

template <typename Rep, typename Period>
void chooseValueType(const Lab::History &hist, std::string_view valueType,
                     std::chrono::duration<Rep, Period> /*unused*/,
                     Lab::Analytics::analyticsMap &map) {
    if (valueType == "reps") {
        populateMapWithRepValuesForPeriod(hist.getHistory(), valueType,
                                          std::chrono::duration<Rep, Period>(),
                                          map);
    } else if (valueType == "sets") {
        populateMapWithSetValuesForPeriod(
            hist.getHistory(), std::chrono::duration<Rep, Period>(), map);
    } else if (valueType == "volume") {
        populateMapWithVolumeValuesForPeriod(
            hist.getHistory(), std::chrono::duration<Rep, Period>(), map);
    } else if (valueType == "workouts") {
        populateMapWithWorkoutValuesForPeriod(
            hist.getHistory(), std::chrono::duration<Rep, Period>(), map);
    }
}
}  // namespace

std::map<unsigned long, double> Lab::Analytics::mapRepEstimates(
    unsigned long reps, double weight) {
    std::map<size_t, double> est;
    est[reps] = weight;
    if (reps != 1) {
        est[ONE_REP] = formula(weight, static_cast<double>(reps));
    }
    for (size_t index = 1; index <= TWELVE_REPS; ++index) {
        if (index != reps) {
            switch (index) {
                case TWO_REPS:
                    est[index] = est[ONE_REP] *
                                 ::formulaRepPercentages::TWO_REP_PERCENTAGE;
                    break;
                case THREE_REPS:
                    est[index] = est[ONE_REP] *
                                 ::formulaRepPercentages::THREE_REP_PERCENTAGE;
                    break;
                case FOUR_REPS:
                    est[index] = est[ONE_REP] *
                                 ::formulaRepPercentages::FOUR_REP_PERCENTAGE;
                    break;
                case FIVE_REPS:
                    est[index] = est[ONE_REP] *
                                 ::formulaRepPercentages::FIVE_REP_PERCENTAGE;
                    break;
                case SIX_REPS:
                    est[index] = est[ONE_REP] *
                                 ::formulaRepPercentages::SIX_REP_PERCENTAGE;
                    break;
                case SEVEN_REPS:
                    est[index] = est[ONE_REP] *
                                 ::formulaRepPercentages::SEVEN_REP_PERCENTAGE;
                    break;
                case EIGHT_REPS:
                    est[index] = est[ONE_REP] *
                                 ::formulaRepPercentages::EIGHT_REP_PERCENTAGE;
                    break;
                case NINE_REPS:
                    est[index] = est[ONE_REP] *
                                 ::formulaRepPercentages::NINE_REP_PERCENTAGE;
                    break;
                case TEN_REPS:
                    est[index] = est[ONE_REP] *
                                 ::formulaRepPercentages::TEN_REP_PERCENTAGE;
                    break;
                case ELEVEN_REPS:
                    est[index] = est[ONE_REP] *
                                 ::formulaRepPercentages::ELEVEN_REP_PERCENTAGE;
                    break;
                case TWELVE_REPS:
                    est[index] = est[ONE_REP] *
                                 ::formulaRepPercentages::TWELVE_REP_PERCENTAGE;
                    break;
                default:
                    break;
            }
        }
    }
    return est;
}

Lab::Analytics::analyticsMap Lab::Analytics::mapHighestValues(
    std::string_view type, const Excercise &excercise, const History &hist) {
    Lab::Analytics::analyticsMap map;

    const auto &excerciseType = excercise.getType();
    if ((type == "reps" || type == "weight" || type == "time" ||
         type == "distance") &&
        std::find(excerciseType.cbegin(), excerciseType.cend(), type) !=
            excerciseType.cend()) {
        populateMapWithHighestTypeValues(hist.getHistory(), map, excercise,
                                         excerciseType, type);
    } else if (type == "volume") {
        if (excercise.getType() ==
            std::vector<std::string>({"weight", "reps"})) {
            populateMapWithHighestVolumeValues(hist.getHistory(), map,
                                               excercise);
        }
    } else if (type == "pace") {
        if (excercise.getType() ==
            std::vector<std::string>({"distance", "time"})) {
            populateMapWithHighestPaceValues(hist.getHistory(), map, excercise);
        }
    } else if (type == "speed") {
        if (excercise.getType() ==
            std::vector<std::string>({"distance", "time"})) {
            populateMapWithHighestSpeedValues(hist.getHistory(), map,
                                              excercise);
        }
    }

    return map;
}

Lab::Analytics::analyticsRepForWeightMap Lab::Analytics::mapWeightForRep(
    const Excercise &excercise, const History &hist) {
    Lab::Analytics::analyticsRepForWeightMap map;

    if (excercise.getType() == std::vector<std::string>({"weight", "reps"})) {
        for (auto const &historyIter : hist.getHistory()) {
            if (std::get<Lab::Excercise>(historyIter) == excercise) {
                auto outerMapRepIndexForInnerMap =
                    map.find(std::get<4>(historyIter));

                if (outerMapRepIndexForInnerMap != map.end()) {
                    auto date = std::chrono::time_point_cast<std::chrono::days>(
                        std::get<dateTimePoint>(historyIter));
                    auto InnerMapDateIndex =
                        outerMapRepIndexForInnerMap->second.find(date);

                    if (InnerMapDateIndex ==
                        outerMapRepIndexForInnerMap->second.end()) {
                        outerMapRepIndexForInnerMap->second[date] =
                            std::get<3>(historyIter);
                    } else if (InnerMapDateIndex->second <
                               std::get<3>(historyIter)) {
                        InnerMapDateIndex->second = std::get<3>(historyIter);
                    }
                } else {
                    map[std::get<4>(historyIter)] = std::map<
                        std::chrono::time_point<std::chrono::system_clock>,
                        double>(
                        {{std::chrono::time_point_cast<std::chrono::days>(
                              std::get<dateTimePoint>(historyIter)),
                          std::get<3>(historyIter)}});
                }
            }
        }
    }
    return map;
}

Lab::Analytics::analyticsMap Lab::Analytics::mapTotalValues(
    std::string_view type, const Excercise &excercise, const History &hist) {
    Lab::Analytics::analyticsMap map;

    const auto &excerciseType = excercise.getType();
    if ((type == "reps" || type == "time" || type == "distance") &&
        std::find(excerciseType.cbegin(), excerciseType.cend(), type) !=
            excerciseType.cend()) {
        populateMapWithTotalTypeValues(hist.getHistory(), map, excercise,
                                       excerciseType, type);
    } else if (type == "volume") {
        if (excercise.getType() ==
            std::vector<std::string>({"weight", "reps"})) {
            populateMapWithTotalVolumeValues(hist.getHistory(), map, excercise);
        }
    }
    return map;
}

Lab::Analytics::analyticsMap Lab::Analytics::mapValuesPerPeriod(
    std::string_view valueType, std::string_view periodType,
    const History &hist) {
    Lab::Analytics::analyticsMap map;

    if (periodType == "day") {
        chooseValueType(hist, valueType, std::chrono::days(), map);
    } else if (periodType == "week") {
        chooseValueType(hist, valueType, std::chrono::weeks(), map);
    } else if (periodType == "month") {
        chooseValueType(hist, valueType, std::chrono::months(), map);
    } else if (periodType == "year") {
        chooseValueType(hist, valueType, std::chrono::years(), map);
    }

    return map;
}

Lab::Analytics::analyticsMap Lab::Analytics::constrainDate(
    const Lab::Analytics::analyticsMap &values,
    const std::chrono::time_point<std::chrono::system_clock> &startDate,
    const std::chrono::time_point<std::chrono::system_clock> &endDate) {
    Lab::Analytics::analyticsMap map;

    for (const auto &iter : values) {
        if (iter.first >= startDate && iter.first < endDate) {
            map[iter.first] = iter.second;
        }
    }

    return map;
}

Lab::Analytics::analyticsRepForWeightMap Lab::Analytics::constrainDate(
    const Lab::Analytics::analyticsRepForWeightMap &values,
    const std::chrono::time_point<std::chrono::system_clock> &startDate,
    const std::chrono::time_point<std::chrono::system_clock> &endDate) {
    Lab::Analytics::analyticsRepForWeightMap map;

    for (const auto &iter : values) {
        Lab::Analytics::analyticsMap innerMap;
        for (const auto &innerIter : iter.second) {
            if (innerIter.first >= startDate && innerIter.first < endDate) {
                innerMap[innerIter.first] = innerIter.second;
            }
        }
        map[iter.first] = innerMap;
    }
    return map;
}
