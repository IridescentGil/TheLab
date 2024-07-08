#include "analytics.h"

#include <bits/chrono.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <type_traits>

namespace {
float formula(const float &weight, const float &reps) {
    const float UPPER = 36.0;
    const float LOWER = 37.0;

    return weight * (UPPER / (LOWER - reps));
}

namespace formulaRepPercentages {
const float TWO_REP_PERCENTAGE = 0.95;
const float THREE_REP_PERCENTAGE = 0.90;
const float FOUR_REP_PERCENTAGE = 0.88;
const float FIVE_REP_PERCENTAGE = 0.86;
const float SIX_REP_PERCENTAGE = 0.83;
const float SEVEN_REP_PERCENTAGE = 0.80;
const float EIGHT_REP_PERCENTAGE = 0.78;
const float NINE_REP_PERCENTAGE = 0.76;
const float TEN_REP_PERCENTAGE = 0.75;
const float ELEVEN_REP_PERCENTAGE = 0.72;
const float TWELVE_REP_PERCENTAGE = 0.70;
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

std::map<size_t, float> Lab::Analytics::mapRepEstimates(size_t reps,
                                                        float weight) {
    std::map<size_t, float> est;
    est[reps] = weight;
    if (reps != 1) {
        est[ONE_REP] = formula(weight, static_cast<float>(reps));
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

std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
Lab::Analytics::mapHighestValues(std::string_view type, const Excercise &exc,
                                 const History &hist) {
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t> map;

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
                        (typeIndexInTuple == 3 ? std::get<3>(historyIter)
                                               : std::get<4>(historyIter));
                } else {
                    index->second =
                        (index->second <
                         static_cast<size_t>((typeIndexInTuple == 3
                                                  ? std::get<3>(historyIter)
                                                  : std::get<4>(historyIter))))
                            ? (typeIndexInTuple == 3 ? std::get<3>(historyIter)
                                                     : std::get<4>(historyIter))
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
                            static_cast<long>(std::get<3>(historyIter)) *
                            std::get<4>(historyIter);
                    } else {
                        index->second =
                            (index->second <
                             (static_cast<size_t>(std::get<3>(historyIter) *
                                                  std::get<4>(historyIter))))
                                ? (static_cast<long>(std::get<3>(historyIter)) *
                                   std::get<4>(historyIter))
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
                            std::get<4>(historyIter) / std::get<3>(historyIter);
                    } else {
                        index->second =
                            (index->second <
                             static_cast<size_t>(std::get<4>(historyIter) /
                                                 std::get<3>(historyIter)))
                                ? (std::get<4>(historyIter) /
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
                            std::get<4>(historyIter) * ONE_HOUR_IN_SECONDS;
                    } else {
                        index->second =
                            (index->second <
                             static_cast<size_t>(
                                 static_cast<float>(std::get<3>(historyIter)) /
                                 std::get<4>(historyIter) *
                                 ONE_HOUR_IN_SECONDS))
                                ? (static_cast<float>(
                                       std::get<3>(historyIter)) /
                                   std::get<4>(historyIter) *
                                   ONE_HOUR_IN_SECONDS)
                                : index->second;
                    }
                }
            }
        }
    }

    return map;
}

std::map<size_t,
         std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>>
Lab::Analytics::mapWeightForRep(const Excercise &exc, const History &hist) {
    std::map<
        size_t,
        std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>>
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
                            (dateIndex->second <
                             static_cast<size_t>(std::get<3>(historyIter)))
                                ? std::get<3>(historyIter)
                                : dateIndex->second;
                    }
                } else {
                    map[std::get<4>(historyIter)] = std::map<
                        std::chrono::time_point<std::chrono::system_clock>,
                        size_t>(
                        {{std::chrono::time_point_cast<std::chrono::days>(
                              std::get<0>(historyIter)),
                          std::get<3>(historyIter)}});
                }
            }
        }
    }
    return map;
}

std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
Lab::Analytics::mapTotalValues(std::string_view type, const Excercise &exc,
                               const History &hist) {
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t> map;

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
                        (typeIndexInTuple == 3 ? std::get<3>(historyIter)
                                               : std::get<4>(historyIter));
                } else {
                    index->second +=
                        (typeIndexInTuple == 3 ? std::get<3>(historyIter)
                                               : std::get<4>(historyIter));
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
                            static_cast<long>(std::get<3>(historyIter)) *
                            std::get<4>(historyIter);
                    } else {
                        index->second +=
                            (static_cast<long>(std::get<3>(historyIter)) *
                             std::get<4>(historyIter));
                    }
                }
            }
        }
    }
    return map;
}

std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
Lab::Analytics::constrainDate(
    const std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
        &values,
    const std::chrono::time_point<std::chrono::system_clock> &startDate,
    const std::chrono::time_point<std::chrono::system_clock> &endDate) {
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t> map;

    for (const auto &iter : values) {
        if (iter.first >= startDate && iter.first < endDate) {
            map[iter.first] = iter.second;
        }
    }

    return map;
}

std::map<size_t,
         std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>>
Lab::Analytics::constrainDate(
    const std::map<size_t,
                   std::map<std::chrono::time_point<std::chrono::system_clock>,
                            size_t>> &values,
    const std::chrono::time_point<std::chrono::system_clock> &startDate,
    const std::chrono::time_point<std::chrono::system_clock> &endDate) {
    std::map<
        size_t,
        std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>>
        map;

    for (const auto &iter : values) {
        std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
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
