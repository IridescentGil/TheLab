#pragma once
///@file

#include <chrono>
#include <cstddef>
#include <map>
#include <string_view>

#include "excercise.h"
#include "history.h"

///@brief Lab classes, namespaces and functions
namespace Lab {
namespace Analytics {
std::map<unsigned long, double> mapRepEstimates(unsigned long reps,
                                                double weight);

/**
@brief Map the highest value of a type on particular days
@param type  1 of ["reps", "weight", "volume", "time", "pace", "speed",
"distance"]
@param exc The excercise to look through the history for
@param hist History to search through
@return A map containing the dates and values of @param type
 */
std::map<std::chrono::time_point<std::chrono::system_clock>, double>
mapHighestValues(std::string_view type, const Excercise &exc,
                 const History &hist);

/**
 @brief Map the most weight used on certain days per certain amount of reps
 @return A map cointaining the amount of reps and a maps containing dates and
 weight used
 */
std::map<unsigned long,
         std::map<std::chrono::time_point<std::chrono::system_clock>, double>>
mapWeightForRep(const Excercise &exc, const History &hist);

/**
@brief Map the total values of a type on particular days in a date
@param type 1 of ["reps", "volume", "distance", "time"]
@param exc The excercise to look through the history for
@param hist History to search through
@return A map containing the dates and values of @param type
*/
std::map<std::chrono::time_point<std::chrono::system_clock>, double>
mapTotalValues(std::string_view type, const Excercise &exc,
               const History &hist);

/**
@brief Map the total values of a type during a period
@param valueType 1 of ["workouts", "volume", "sets", "reps", "workout_duration"]
@param periodType 1 of ["day", "week", "month", "year"]
@return A map of dates in relation to @param periodType and the values of @param
Valuetype
*/
template <typename Rep, typename Period>
std::map<std::chrono::time_point<std::chrono::system_clock>, double>
mapValuesPerPeriod(std::string_view valueType,
                   std::chrono::duration<Rep, Period> periodType,
                   const History &hist);

std::map<std::chrono::time_point<std::chrono::system_clock>, double>
constrainDate(
    const std::map<std::chrono::time_point<std::chrono::system_clock>, double>
        &values,
    const std::chrono::time_point<std::chrono::system_clock> &startDate,
    const std::chrono::time_point<std::chrono::system_clock> &endDate);

std::map<unsigned long,
         std::map<std::chrono::time_point<std::chrono::system_clock>, double>>
constrainDate(
    const std::map<unsigned long,
                   std::map<std::chrono::time_point<std::chrono::system_clock>,
                            double>> &values,
    const std::chrono::time_point<std::chrono::system_clock> &startDate,
    const std::chrono::time_point<std::chrono::system_clock> &endDate);
}  // namespace Analytics
}  // namespace Lab

template <typename Rep, typename Period>
std::map<std::chrono::time_point<std::chrono::system_clock>, double>
Lab::Analytics::mapValuesPerPeriod(
    std::string_view valueType, std::chrono::duration<Rep, Period> /*unused*/,
    const History &hist) {
    std::map<std::chrono::time_point<std::chrono::system_clock>, double> map;

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
                std::chrono::time_point<std::chrono::system_clock,
                                        std::chrono::duration<Rep, Period>>
                    date = std::chrono::time_point_cast<
                        std::chrono::duration<Rep, Period>>(
                        std::get<0>(historyIter));
                auto index = map.find(date);

                if (index == map.end()) {
                    map[date] =
                        (valueTypeIndexInTuple == 3
                             ? std::get<3>(historyIter)
                             : static_cast<double>(std::get<4>(historyIter)));
                } else {
                    index->second +=
                        (valueTypeIndexInTuple == 3
                             ? std::get<3>(historyIter)
                             : static_cast<double>(std::get<4>(historyIter)));
                }
            }
        }
    } else if (valueType == "sets") {
        for (auto const &historyIter : hist.getHistory()) {
            std::chrono::time_point<std::chrono::system_clock,
                                    std::chrono::duration<Rep, Period>>
                date = std::chrono::time_point_cast<
                    std::chrono::duration<Rep, Period>>(
                    std::get<0>(historyIter));
            auto index = map.find(date);

            if (index == map.end()) {
                map[date] = 1;
            } else {
                index->second += 1;
            }
        }
    } else if (valueType == "volume") {
        for (auto const &historyIter : hist.getHistory()) {
            if (std::get<2>(historyIter).getType() ==
                std::vector<std::string>({"weight", "reps"})) {
                std::chrono::time_point<std::chrono::system_clock,
                                        std::chrono::duration<Rep, Period>>
                    date = std::chrono::time_point_cast<
                        std::chrono::duration<Rep, Period>>(
                        std::get<0>(historyIter));
                auto index = map.find(date);

                if (index == map.end()) {
                    map[date] = std::get<3>(historyIter) *
                                static_cast<double>(std::get<4>(historyIter));
                } else {
                    index->second +=
                        (std::get<3>(historyIter) *
                         static_cast<double>(std::get<4>(historyIter)));
                }
            }
        }
    } else if (valueType == "workouts") {
        std::vector<std::chrono::time_point<std::chrono::system_clock>>
            dayCounted;
        for (auto const &historyIter : hist.getHistory()) {
            if (std::find(dayCounted.cbegin(), dayCounted.cend(),
                          std::get<0>(historyIter)) == dayCounted.cend()) {
                std::chrono::time_point<std::chrono::system_clock,
                                        std::chrono::duration<Rep, Period>>
                    date = std::chrono::time_point_cast<
                        std::chrono::duration<Rep, Period>>(
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
    return map;
}
