#include "excercise.h"

#include <algorithm>

Excercise::Excercise(std::string name, std::string desc, std::string mGroup,
                     std::vector<std::string> mWorked,
                     std::vector<std::string> eType)
    : name(name), description(desc) {
    if (isMuscle(mWorked)) musclesWorked = mWorked;
    if (isType(eType)) type = eType;
    if (isMuscleGroup(mGroup)) muscleGroup = mGroup;
}

std::tuple<std::string, std::string, std::string, std::vector<std::string>,
           std::vector<std::string>>
Excercise::getDetails() const {
    return std::make_tuple(name, description, muscleGroup, musclesWorked, type);
}

void Excercise::setName(std::string newName) { name = newName; }

void Excercise::setDescription(std::string newDesc) { description = newDesc; }

int Excercise::setMuscleGroup(std::string newMG) {
    if (isMuscleGroup(newMG))
        muscleGroup = newMG;
    else
        return -1;

    return 0;
}

int Excercise::setMusclesWorked(std::vector<std::string> newMW) {
    if (isMuscle(newMW))
        musclesWorked = newMW;
    else
        return -1;

    return 0;
}

int Excercise::setType(std::vector<std::string> newType) {
    if (isType(newType))
        type = newType;
    else
        return -1;

    return 0;
}

bool Excercise::isMuscleGroup(std::string mGroup) {
    if (std::find(groupCheck.cbegin(), groupCheck.cend(), mGroup) ==
        groupCheck.cend())
        return false;

    return true;
}
bool Excercise::isMuscle(std::vector<std::string> mWorked) {
    if (mWorked.size() == 0) return false;
    for (auto it = mWorked.cbegin(); it != mWorked.cend(); it++) {
        if (std::find(mwCheck.cbegin(), mwCheck.cend(), *it) == mwCheck.cend())
            return false;
    }

    return true;
}

bool Excercise::isType(std::vector<std::string> nType) {
    if (nType.size() > 2 || nType.size() == 0) return false;
    for (auto it = nType.cbegin(); it != nType.cend(); it++) {
        if (std::find(typeCheck.cbegin(), typeCheck.cend(), *it) ==
            typeCheck.cend())
            return false;
    }

    return true;
}
