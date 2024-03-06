#include "excercise.h"

std::tuple<std::string, std::string, std::string, std::vector<std::string>,
           std::vector<std::string>>
Excercise::getDetails() const {
    return std::make_tuple(name, description, muscleGroup, musclesWorked, type);
}
