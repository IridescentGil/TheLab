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
std::map<size_t, float> mapRepEstimates(size_t reps, float weight);

/**
@brief Map the highest value of a type on particular days
@param type  1 of ["reps", "weight", "volume", "time", "pace", "speed",
"distance"]
@param exc The excercise to look through the history for
@param hist History to search through
@return A map containing the dates and values of @param type
 */
std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
mapHighestValues(std::string_view type, const Excercise &exc,
                 const History &hist);

/**
 @brief Map the most weight used on certain days per certain amount of reps
 @return A map cointaining the amount of reps and a maps containing dates and
 weight used
 */
std::map<size_t,
         std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>>
mapWeightForRep(const Excercise &exc, const History &hist);

/**
@brief Map the total values of a type on particular days in a date
@param type 1 of ["reps", "volume", "distance", "time"]
@param exc The excercise to look through the history for
@param hist History to search through
@return A map containing the dates and values of @param type
*/
std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
mapTotalValues(std::string_view type, const Excercise &exc,
               const History &hist);

/**
@brief Map the total values of a type during a period
@param valueType 1 of ["workouts", "volume", "sets", "reps", "workout_duration"]
@param periodType 1 of ["day", "week", "month", "year"]
@return A map of dates in relation to @param periodType and the values of @param
Valuetype
*/
std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
mapValuesPerPeriod(std::string_view valueType, std::string_view periodType,
                   const History &hist);

std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>
constrainDate(
    std::map<std::chrono::time_point<std::chrono::system_clock>, size_t> values,
    const std::chrono::time_point<std::chrono::system_clock> &startDate,
    const std::chrono::time_point<std::chrono::system_clock> &endDate);

std::map<size_t,
         std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>>
constrainDate(
    std::map<
        size_t,
        std::map<std::chrono::time_point<std::chrono::system_clock>, size_t>>
        values,
    const std::chrono::time_point<std::chrono::system_clock> &startDate,
    const std::chrono::time_point<std::chrono::system_clock> &endDate);
}  // namespace Analytics
}  // namespace Lab
