#include "analytics.h"

#include <bits/chrono.h>

#include <algorithm>
#include <chrono>
#include <type_traits>

std::map<size_t, float> Lab::Analytics::mapRepEstimates(size_t reps,
                                                        float weight) {
    std::map<size_t, float> est;
    est[reps] = weight;
    if (reps != 1) est[1] = weight * (36.0 / (37.0 - reps));
    for (size_t index = 1; index <= 12; index++) {
        if (index != reps) {
            switch (index) {
                case 2:
                    est[index] = est[1] * 0.95;
                    break;
                case 3:
                    est[index] = est[1] * 0.90;
                    break;
                case 4:
                    est[index] = est[1] * 0.88;
                    break;
                case 5:
                    est[index] = est[1] * 0.86;
                    break;
                case 6:
                    est[index] = est[1] * 0.83;
                    break;
                case 7:
                    est[index] = est[1] * 0.80;
                    break;
                case 8:
                    est[index] = est[1] * 0.78;
                    break;
                case 9:
                    est[index] = est[1] * 0.76;
                    break;
                case 10:
                    est[index] = est[1] * 0.75;
                    break;
                case 11:
                    est[index] = est[1] * 0.72;
                    break;
                case 12:
                    est[index] = est[1] * 0.70;
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

    if ((type == "reps" || type == "weight" || type == "time" ||
         type == "distance") &&
        std::find(exc.getType().cbegin(), exc.getType().cend(), type) !=
            exc.getType().cend()) {
        size_t typeIndexInTuple =
            std::find(exc.getType().cbegin(), exc.getType().cend(), type) -
            exc.getType().cbegin() +
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
                            std::get<3>(historyIter) * std::get<4>(historyIter);
                    } else {
                        index->second =
                            (index->second <
                             static_cast<size_t>(std::get<3>(historyIter) *
                                                 std::get<4>(historyIter)))
                                ? (std::get<3>(historyIter) *
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
            float oneHourInSeconds = 3600;
            for (auto const &historyIter : hist.getHistory()) {
                if (std::get<2>(historyIter) == exc) {
                    auto date = std::chrono::time_point_cast<std::chrono::days>(
                        std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] =
                            static_cast<float>(std::get<3>(historyIter)) /
                            std::get<4>(historyIter) * oneHourInSeconds;
                    } else {
                        index->second =
                            (index->second <
                             static_cast<size_t>(
                                 static_cast<float>(std::get<3>(historyIter)) /
                                 std::get<4>(historyIter) * oneHourInSeconds))
                                ? (static_cast<float>(
                                       std::get<3>(historyIter)) /
                                   std::get<4>(historyIter) * oneHourInSeconds)
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
                }
            } else {
                map[std::get<4>(historyIter)] =
                    std::map<std::chrono::time_point<std::chrono::system_clock>,
                             size_t>(
                        {{std::get<0>(historyIter), std::get<3>(historyIter)}});
            }
        }
    }
    return map;
}

std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
Lab::Analytics::mapTotalValues(std::string_view type, const Excercise &exc,
                               const History &hist) {
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t> map;

    if ((type == "reps" || type == "time" || type == "distance") &&
        std::find(exc.getType().cbegin(), exc.getType().cend(), type) !=
            exc.getType().cend()) {
        size_t typeIndexInTuple =
            std::find(exc.getType().cbegin(), exc.getType().cend(), type) -
            exc.getType().cbegin() +
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
                            std::get<3>(historyIter) * std::get<4>(historyIter);
                    } else {
                        index->second += (std::get<3>(historyIter) *
                                          std::get<4>(historyIter));
                    }
                }
            }
        }
    }
    return map;
}

std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
Lab::Analytics::mapValuesPerPeriod(std::string_view valueType,
                                   std::string_view periodType,
                                   const History &hist) {
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t> map;
    //@param valueType 1 of ["workouts", "volume", "sets", "reps",
    //"workout_duration"]
    //@param periodType 1 of ["day", "week", "month", "year"]
    if (periodType == "days") {
        if ((valueType == "reps")) {
            for (auto const &historyIter : hist.getHistory()) {
                if (std::find(std::get<2>(historyIter).getType().cbegin(),
                              std::get<2>(historyIter).getType().cend(),
                              valueType) !=
                    std::get<2>(historyIter).getType().cend()) {
                    size_t valueTypeIndexInTuple =
                        std::find(std::get<2>(historyIter).getType().cbegin(),
                                  std::get<2>(historyIter).getType().cend(),
                                  valueType) -
                        std::get<2>(historyIter).getType().cbegin() +
                        3;  // In history tuple valueTypes are index 3 and 4
                    auto date = std::chrono::time_point_cast<std::chrono::days>(
                        std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] = (valueTypeIndexInTuple == 3
                                         ? std::get<3>(historyIter)
                                         : std::get<4>(historyIter));
                    } else {
                        index->second += (valueTypeIndexInTuple == 3
                                              ? std::get<3>(historyIter)
                                              : std::get<4>(historyIter));
                    }
                }
            }
        } else if (valueType == "volume") {
            for (auto const &historyIter : hist.getHistory()) {
                if (std::get<2>(historyIter).getType() ==
                    std::vector<std::string>({"weight", "reps"})) {
                    auto date = std::chrono::time_point_cast<std::chrono::days>(
                        std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] =
                            std::get<3>(historyIter) * std::get<4>(historyIter);
                    } else {
                        index->second += (std::get<3>(historyIter) *
                                          std::get<4>(historyIter));
                    }
                }
            }
        } else if (valueType == "sets") {
            std::vector<std::chrono::time_point<std::chrono::system_clock>>
                dayCounted;
            for (auto const &historyIter : hist.getHistory()) {
                if (std::find(dayCounted.cbegin(), dayCounted.cend(),
                              std::get<0>(historyIter)) == dayCounted.cend()) {
                    auto date = std::chrono::time_point_cast<std::chrono::days>(
                        std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] = 1;
                    } else {
                        index->second += 1;
                    }
                    dayCounted.push_back(std::get<0>(historyIter));
                }
            }
        }
    }
    if (periodType == "weeks") {
        if ((valueType == "reps")) {
            for (auto const &historyIter : hist.getHistory()) {
                if (std::find(std::get<2>(historyIter).getType().cbegin(),
                              std::get<2>(historyIter).getType().cend(),
                              valueType) !=
                    std::get<2>(historyIter).getType().cend()) {
                    size_t valueTypeIndexInTuple =
                        std::find(std::get<2>(historyIter).getType().cbegin(),
                                  std::get<2>(historyIter).getType().cend(),
                                  valueType) -
                        std::get<2>(historyIter).getType().cbegin() +
                        3;  // In history tuple valueTypes are index 3 and 4
                    auto date =
                        std::chrono::time_point_cast<std::chrono::weeks>(
                            std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] = (valueTypeIndexInTuple == 3
                                         ? std::get<3>(historyIter)
                                         : std::get<4>(historyIter));
                    } else {
                        index->second += (valueTypeIndexInTuple == 3
                                              ? std::get<3>(historyIter)
                                              : std::get<4>(historyIter));
                    }
                }
            }
        } else if (valueType == "volume") {
            for (auto const &historyIter : hist.getHistory()) {
                if (std::get<2>(historyIter).getType() ==
                    std::vector<std::string>({"weight", "reps"})) {
                    auto date =
                        std::chrono::time_point_cast<std::chrono::weeks>(
                            std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] =
                            std::get<3>(historyIter) * std::get<4>(historyIter);
                    } else {
                        index->second += (std::get<3>(historyIter) *
                                          std::get<4>(historyIter));
                    }
                }
            }
        } else if (valueType == "sets") {
            std::vector<std::chrono::time_point<std::chrono::system_clock>>
                dayCounted;
            for (auto const &historyIter : hist.getHistory()) {
                if (std::find(dayCounted.cbegin(), dayCounted.cend(),
                              std::get<0>(historyIter)) == dayCounted.cend()) {
                    auto date =
                        std::chrono::time_point_cast<std::chrono::weeks>(
                            std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] = 1;
                    } else {
                        index->second += 1;
                    }
                    dayCounted.push_back(std::get<0>(historyIter));
                }
            }
        }
    }
    if (periodType == "months") {
        if ((valueType == "reps")) {
            for (auto const &historyIter : hist.getHistory()) {
                if (std::find(std::get<2>(historyIter).getType().cbegin(),
                              std::get<2>(historyIter).getType().cend(),
                              valueType) !=
                    std::get<2>(historyIter).getType().cend()) {
                    size_t valueTypeIndexInTuple =
                        std::find(std::get<2>(historyIter).getType().cbegin(),
                                  std::get<2>(historyIter).getType().cend(),
                                  valueType) -
                        std::get<2>(historyIter).getType().cbegin() +
                        3;  // In history tuple valueTypes are index 3 and 4
                    auto date =
                        std::chrono::time_point_cast<std::chrono::months>(
                            std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] = (valueTypeIndexInTuple == 3
                                         ? std::get<3>(historyIter)
                                         : std::get<4>(historyIter));
                    } else {
                        index->second += (valueTypeIndexInTuple == 3
                                              ? std::get<3>(historyIter)
                                              : std::get<4>(historyIter));
                    }
                }
            }
        } else if (valueType == "volume") {
            for (auto const &historyIter : hist.getHistory()) {
                if (std::get<2>(historyIter).getType() ==
                    std::vector<std::string>({"weight", "reps"})) {
                    auto date =
                        std::chrono::time_point_cast<std::chrono::months>(
                            std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] =
                            std::get<3>(historyIter) * std::get<4>(historyIter);
                    } else {
                        index->second += (std::get<3>(historyIter) *
                                          std::get<4>(historyIter));
                    }
                }
            }
        } else if (valueType == "sets") {
            std::vector<std::chrono::time_point<std::chrono::system_clock>>
                dayCounted;
            for (auto const &historyIter : hist.getHistory()) {
                if (std::find(dayCounted.cbegin(), dayCounted.cend(),
                              std::get<0>(historyIter)) == dayCounted.cend()) {
                    auto date =
                        std::chrono::time_point_cast<std::chrono::months>(
                            std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] = 1;
                    } else {
                        index->second += 1;
                    }
                    dayCounted.push_back(std::get<0>(historyIter));
                }
            }
        }
    }
    if (periodType == "years") {
        if ((valueType == "reps")) {
            for (auto const &historyIter : hist.getHistory()) {
                if (std::find(std::get<2>(historyIter).getType().cbegin(),
                              std::get<2>(historyIter).getType().cend(),
                              valueType) !=
                    std::get<2>(historyIter).getType().cend()) {
                    size_t valueTypeIndexInTuple =
                        std::find(std::get<2>(historyIter).getType().cbegin(),
                                  std::get<2>(historyIter).getType().cend(),
                                  valueType) -
                        std::get<2>(historyIter).getType().cbegin() +
                        3;  // In history tuple valueTypes are index 3 and 4
                    auto date =
                        std::chrono::time_point_cast<std::chrono::years>(
                            std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] = (valueTypeIndexInTuple == 3
                                         ? std::get<3>(historyIter)
                                         : std::get<4>(historyIter));
                    } else {
                        index->second += (valueTypeIndexInTuple == 3
                                              ? std::get<3>(historyIter)
                                              : std::get<4>(historyIter));
                    }
                }
            }
        } else if (valueType == "volume") {
            for (auto const &historyIter : hist.getHistory()) {
                if (std::get<2>(historyIter).getType() ==
                    std::vector<std::string>({"weight", "reps"})) {
                    auto date =
                        std::chrono::time_point_cast<std::chrono::years>(
                            std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] =
                            std::get<3>(historyIter) * std::get<4>(historyIter);
                    } else {
                        index->second += (std::get<3>(historyIter) *
                                          std::get<4>(historyIter));
                    }
                }
            }
        } else if (valueType == "sets") {
            std::vector<std::chrono::time_point<std::chrono::system_clock>>
                dayCounted;
            for (auto const &historyIter : hist.getHistory()) {
                if (std::find(dayCounted.cbegin(), dayCounted.cend(),
                              std::get<0>(historyIter)) == dayCounted.cend()) {
                    auto date =
                        std::chrono::time_point_cast<std::chrono::years>(
                            std::get<0>(historyIter));
                    auto index = map.find(date);

                    if (index == map.end()) {
                        map[date] = 1;
                    } else {
                        index->second += 1;
                    }
                    dayCounted.push_back(std::get<0>(historyIter));
                }
            }
        }
    }
    return map;
}

std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
Lab::Analytics::constrainDate(
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t> values,
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
    std::map<
        size_t,
        std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>>
        values,
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
