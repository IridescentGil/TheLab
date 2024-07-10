#include "analytics.h"

#include <bits/chrono.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <type_traits>

namespace {
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

std::map<std::chrono::time_point<std::chrono::system_clock>, double>
Lab::Analytics::mapHighestValues(std::string_view type, const Excercise &exc,
                                 const History &hist) {
    std::map<std::chrono::time_point<std::chrono::system_clock>, double> map;

    const auto &excerciseType = exc.getType();
    if ((type == "reps" || type == "weight" || type == "time" ||
         type == "distance") &&
        std::find(excerciseType.cbegin(), excerciseType.cend(), type) !=
            excerciseType.cend()) {
        size_t typeIndexInTuple =
            std::find(excerciseType.cbegin(), excerciseType.cend(), type) -
            excerciseType.cbegin() +
            3;  // In history tuple types are index 3 and 4

        for (auto const &historyIter : hist.getHistory()) {
            if (std::get<2>(historyIter) == exc) {
                auto date = std::chrono::time_point_cast<std::chrono::days>(
                    std::get<0>(historyIter));
                auto index = map.find(date);

                if (index == map.end()) {
                    map[date] =
                        (typeIndexInTuple == 3
                             ? std::get<3>(historyIter)
                             : static_cast<double>(std::get<4>(historyIter)));
                } else {
                    index->second = (index->second <
                                     ((typeIndexInTuple == 3
                                           ? std::get<3>(historyIter)
                                           : static_cast<double>(
                                                 std::get<4>(historyIter)))))
                                        ? (typeIndexInTuple == 3
                                               ? std::get<3>(historyIter)
                                               : static_cast<double>(
                                                     std::get<4>(historyIter)))
                                        : index->second;
                    ;
                }
            }
        }
    } else if (type == "volume") {
        if (exc.getType() == std::vector<std::string>({"weight", "reps"})) {
            for (auto const &historyIter : hist.getHistory()) {
                if (std::get<2>(historyIter) == exc) {
                    auto date = std::chrono::time_point_cast<std::chrono::days>(
                        std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] =
                            (std::get<3>(historyIter) *
                             static_cast<double>(std::get<4>(historyIter)));
                    } else {
                        index->second =
                            (index->second <
                             (std::get<3>(historyIter) *
                              static_cast<double>(std::get<4>(historyIter))))
                                ? (std::get<3>(historyIter) *
                                   static_cast<double>(
                                       std::get<4>(historyIter)))
                                : index->second;
                    }
                }
            }
        }
    } else if (type == "pace") {
        if (exc.getType() == std::vector<std::string>({"distance", "time"})) {
            for (auto const &historyIter : hist.getHistory()) {
                if (std::get<2>(historyIter) == exc) {
                    auto date = std::chrono::time_point_cast<std::chrono::days>(
                        std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] =
                            static_cast<double>(std::get<4>(historyIter)) /
                            std::get<3>(historyIter);
                    } else {
                        index->second =
                            (index->second <
                             static_cast<double>(std::get<4>(historyIter)) /
                                 std::get<3>(historyIter))
                                ? (static_cast<double>(
                                       std::get<4>(historyIter)) /
                                   std::get<3>(historyIter))
                                : index->second;
                    }
                }
            }
        }
    } else if (type == "speed") {
        if (exc.getType() == std::vector<std::string>({"distance", "time"})) {
            const float ONE_HOUR_IN_SECONDS = 3600;
            for (auto const &historyIter : hist.getHistory()) {
                if (std::get<2>(historyIter) == exc) {
                    auto date = std::chrono::time_point_cast<std::chrono::days>(
                        std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] =
                            static_cast<float>(std::get<3>(historyIter)) /
                            static_cast<double>(std::get<4>(historyIter)) *
                            ONE_HOUR_IN_SECONDS;
                    } else {
                        index->second =
                            (index->second <
                             std::get<3>(historyIter) /
                                 static_cast<double>(std::get<4>(historyIter)) *
                                 ONE_HOUR_IN_SECONDS)
                                ? (std::get<3>(historyIter) /
                                   static_cast<double>(
                                       std::get<4>(historyIter)) *
                                   ONE_HOUR_IN_SECONDS)
                                : index->second;
                    }
                }
            }
        }
    }

    return map;
}

std::map<unsigned long,
         std::map<std::chrono::time_point<std::chrono::system_clock>, double>>
Lab::Analytics::mapWeightForRep(const Excercise &exc, const History &hist) {
    std::map<
        unsigned long,
        std::map<std::chrono::time_point<std::chrono::system_clock>, double>>
        map;

    if (exc.getType() == std::vector<std::string>({"weight", "reps"})) {
        for (auto const &historyIter : hist.getHistory()) {
            if (std::get<2>(historyIter) == exc) {
                auto repIndex = map.find(std::get<4>(historyIter));

                if (repIndex != map.end()) {
                    auto date = std::chrono::time_point_cast<std::chrono::days>(
                        std::get<0>(historyIter));
                    auto dateIndex = repIndex->second.find(date);

                    if (dateIndex == repIndex->second.end()) {
                        repIndex->second[date] = std::get<3>(historyIter);
                    } else {
                        dateIndex->second =
                            (dateIndex->second < std::get<3>(historyIter))
                                ? std::get<3>(historyIter)
                                : dateIndex->second;
                    }
                } else {
                    map[std::get<4>(historyIter)] = std::map<
                        std::chrono::time_point<std::chrono::system_clock>,
                        double>(
                        {{std::chrono::time_point_cast<std::chrono::days>(
                              std::get<0>(historyIter)),
                          std::get<3>(historyIter)}});
                }
            }
        }
    }
    return map;
}

std::map<std::chrono::time_point<std::chrono::system_clock>, double>
Lab::Analytics::mapTotalValues(std::string_view type, const Excercise &exc,
                               const History &hist) {
    std::map<std::chrono::time_point<std::chrono::system_clock>, double> map;

    const auto &excerciseType = exc.getType();
    if ((type == "reps" || type == "time" || type == "distance") &&
        std::find(excerciseType.cbegin(), excerciseType.cend(), type) !=
            excerciseType.cend()) {
        size_t typeIndexInTuple =
            std::find(excerciseType.cbegin(), excerciseType.cend(), type) -
            excerciseType.cbegin() +
            3;  // In history tuple types are index 3 and 4

        for (auto const &historyIter : hist.getHistory()) {
            if (std::get<2>(historyIter) == exc) {
                auto date = std::chrono::time_point_cast<std::chrono::days>(
                    std::get<0>(historyIter));
                auto index = map.find(date);

                if (index == map.end()) {
                    map[date] =
                        (typeIndexInTuple == 3
                             ? std::get<3>(historyIter)
                             : static_cast<double>(std::get<4>(historyIter)));
                } else {
                    index->second +=
                        (typeIndexInTuple == 3
                             ? std::get<3>(historyIter)
                             : static_cast<double>(std::get<4>(historyIter)));
                }
            }
        }
    } else if (type == "volume") {
        if (exc.getType() == std::vector<std::string>({"weight", "reps"})) {
            for (auto const &historyIter : hist.getHistory()) {
                if (std::get<2>(historyIter) == exc) {
                    auto date = std::chrono::time_point_cast<std::chrono::days>(
                        std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] =
                            std::get<3>(historyIter) *
                            static_cast<double>(std::get<4>(historyIter));
                    } else {
                        index->second +=
                            (std::get<3>(historyIter) *
                             static_cast<double>(std::get<4>(historyIter)));
                    }
                }
            }
        }
    }
    return map;
}

std::map<std::chrono::time_point<std::chrono::system_clock>, double>
Lab::Analytics::constrainDate(
    const std::map<std::chrono::time_point<std::chrono::system_clock>, double>
        &values,
    const std::chrono::time_point<std::chrono::system_clock> &startDate,
    const std::chrono::time_point<std::chrono::system_clock> &endDate) {
    std::map<std::chrono::time_point<std::chrono::system_clock>, double> map;

    for (const auto &iter : values) {
        if (iter.first >= startDate && iter.first < endDate) {
            map[iter.first] = iter.second;
        }
    }

    return map;
}

std::map<unsigned long,
         std::map<std::chrono::time_point<std::chrono::system_clock>, double>>
Lab::Analytics::constrainDate(
    const std::map<unsigned long,
                   std::map<std::chrono::time_point<std::chrono::system_clock>,
                            double>> &values,
    const std::chrono::time_point<std::chrono::system_clock> &startDate,
    const std::chrono::time_point<std::chrono::system_clock> &endDate) {
    std::map<
        unsigned long,
        std::map<std::chrono::time_point<std::chrono::system_clock>, double>>
        map;

    for (const auto &iter : values) {
        std::map<std::chrono::time_point<std::chrono::system_clock>, double>
            innerMap;
        for (const auto &innerIter : iter.second) {
            if (innerIter.first >= startDate && innerIter.first < endDate) {
                innerMap[innerIter.first] = innerIter.second;
            }
        }
        map[iter.first] = innerMap;
    }
    return map;
}
