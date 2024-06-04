#include "excercise.h"

#include <algorithm>

Lab::Excercise::Excercise(std::string name, std::string desc,
                          std::string mGroup, std::vector<std::string> mWorked,
                          std::vector<std::string> eType)
    : name(name), description(desc) {
    if (isMuscle(mWorked)) musclesWorked = mWorked;
    if (isType(eType)) type = eType;
    if (isMuscleGroup(mGroup)) muscleGroup = mGroup;
}

std::tuple<std::string, std::string, std::string, std::vector<std::string>,
           std::vector<std::string>>
Lab::Excercise::getDetails() const {
    return std::make_tuple(name, description, muscleGroup, musclesWorked, type);
}

void Lab::Excercise::setName(const std::string &newName) { name = newName; }

void Lab::Excercise::setDescription(const std::string &newDesc) {
    description = newDesc;
}

int Lab::Excercise::setMuscleGroup(const std::string &newMG) {
    if (isMuscleGroup(newMG))
        muscleGroup = newMG;
    else
        return -1;

    return 0;
}

int Lab::Excercise::setMusclesWorked(const std::vector<std::string> &newMW) {
    if (isMuscle(newMW))
        musclesWorked = newMW;
    else
        return -1;

    return 0;
}

int Lab::Excercise::setType(const std::vector<std::string> &newType) {
    if (isType(newType))
        type = newType;
    else
        return -1;

    return 0;
}

bool Lab::Excercise::isMuscleGroup(const std::string &mGroup) {
    if (std::find(groupCheck.cbegin(), groupCheck.cend(), mGroup) ==
        groupCheck.cend())
        return false;

    return true;
}
bool Lab::Excercise::isMuscle(const std::vector<std::string> &mWorked) {
    if (mWorked.size() == 0) return false;
    for (auto it = mWorked.cbegin(); it != mWorked.cend(); it++) {
        if (std::find(mwCheck.cbegin(), mwCheck.cend(), *it) == mwCheck.cend())
            return false;
    }

    return true;
}

bool Lab::Excercise::isType(const std::vector<std::string> &nType) {
    if (nType.size() > 2 || nType.size() == 0) return false;
    for (auto it = nType.cbegin(); it != nType.cend(); it++) {
        if (std::find(typeCheck.cbegin(), typeCheck.cend(), *it) ==
            typeCheck.cend())
            return false;
    }

    return true;
}
