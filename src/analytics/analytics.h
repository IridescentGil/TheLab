#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <cstddef>
#include <memory>

#include "excercise.h"
#include "history.h"

namespace analytics {
namespace excercise {
size_t repMax(size_t n, Excercise exc, std::shared_ptr<History> hist);
std::vector<size_t> estimateRepMax(size_t n, Excercise exc,
                                   std::shared_ptr<History> hist);
std::vector<size_t> maxReps(Excercise exc, std::shared_ptr<History> hist);
std::vector<size_t> maxWeight(Excercise exc, std::shared_ptr<History> hist);
std::vector<size_t> maxVolume(Excercise exc, std::shared_ptr<History> hist);
std::vector<size_t> maxWeightForRep(Excercise exc,
                                    std::shared_ptr<History> hist);
std::vector<size_t> workoutVolume(
    std::chrono::time_point<std::chrono::system_clock> date,
    std::shared_ptr<History> hist);
std::vector<size_t> workoutReps(
    std::chrono::time_point<std::chrono::system_clock> date,
    std::shared_ptr<History> hist);
}  // namespace excercise
namespace workout {
std::vector<size_t> volumePerW(size_t months, std::shared_ptr<History> hist);
std::vector<size_t> setsPerW(size_t months, std::shared_ptr<History> hist);
std::vector<size_t> repsPerW(size_t months, std::shared_ptr<History> hist);
std::vector<size_t> workoutsPerPeriod(std::string type, size_t timeframe,
                                      std::shared_ptr<History> hist);
std::vector<size_t> volumePerPeriod(std::string type, size_t timeframe,
                                    std::shared_ptr<History> hist);
std::vector<size_t> setsPerPeriod(std::string type, size_t timeframe,
                                  std::shared_ptr<History> hist);
std::vector<size_t> repsPerPeriod(std::string type, size_t timeframe,
                                  std::shared_ptr<History> hist);
std::vector<size_t> workoutDurationPerPeriod(std::string type, size_t timeframe,
                                             std::shared_ptr<History> hist);
}  // namespace workout
}  // namespace analytics

#endif  // !ANALYTICS_H
