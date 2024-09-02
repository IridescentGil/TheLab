#include "excercise.h"

#include <algorithm>
#include <sstream>
#include <utility>

#include "database.h"

namespace {
enum EXCERCISE_DATABASE_INDEXES {
    EXCERCISE_NAME_DB_INDEX = 0,
    EXCERCISE_DESCRIPTION_DB_INDEX,
    MUSCLE_GROUP_DB_INDEX,
    MUSCLES_WORKED_DB_INDEX,
    EXCERCISE_TYPE_DB_INDEX
};
}  // namespace

Lab::Excercise::Excercise() {}

Lab::Excercise::Excercise(std::string newName, std::string desc, const std::string &mGroup,
                          const std::vector<std::string> &mWorked, const std::vector<std::string> &eType)
    : name(std::move(newName)), description(std::move(desc)) {
    if (isMuscle(mWorked)) {
        musclesWorked = mWorked;
    }
    if (isType(eType)) {
        type = eType;
    }
    if (isMuscleGroup(mGroup)) {
        muscleGroup = mGroup;
    }
}

std::tuple<std::string, std::string, std::string, std::vector<std::string>, std::vector<std::string>>
Lab::Excercise::getDetails() const {
    return std::make_tuple(name, description, muscleGroup, musclesWorked, type);
}

void Lab::Excercise::setName(const std::string &newName) { name = newName; }

void Lab::Excercise::setDescription(const std::string &newDesc) { description = newDesc; }

int Lab::Excercise::setMuscleGroup(const std::string &newMG) {
    if (isMuscleGroup(newMG)) {
        muscleGroup = newMG;
    } else {
        return -1;
    }

    return 0;
}

int Lab::Excercise::setMusclesWorked(const std::vector<std::string> &newMW) {
    if (isMuscle(newMW)) {
        musclesWorked = newMW;
    } else {
        return -1;
    }

    return 0;
}

int Lab::Excercise::setType(const std::vector<std::string> &newType) {
    if (isType(newType)) {
        type = newType;
    } else {
        return -1;
    }

    return 0;
}

bool Lab::Excercise::isMuscleGroup(const std::string &mGroup) {
    return std::find(groupCheck.cbegin(), groupCheck.cend(), mGroup) != groupCheck.cend();
}
bool Lab::Excercise::isMuscle(const std::vector<std::string> &mWorked) {
    if (mWorked.empty()) {
        return false;
    }
    for (auto it = mWorked.cbegin(); it != mWorked.cend(); ++it) {
        if (std::find(mwCheck.cbegin(), mwCheck.cend(), *it) == mwCheck.cend()) {
            return false;
        }
    }

    return true;
}

bool Lab::Excercise::isType(const std::vector<std::string> &nType) {
    if (nType.size() > 2 || nType.empty()) {
        return false;
    }
    for (auto it = nType.cbegin(); it != nType.cend(); ++it) {
        if (std::find(typeCheck.cbegin(), typeCheck.cend(), *it) == typeCheck.cend()) {
            return false;
        }
    }

    return true;
}

bool Lab::Excercise::load(Lab::DBConn *database, const std::string &excercise) {
    database->prepare("SELECT * FROM excercises WHERE name = ?", excercise);
    while (database->retrieve_next_row()) {
        std::vector<std::string> loadedMusclesWorked;
        std::vector<std::string> loadedExcerciseType;
        std::istringstream musclesWorkedStream(database->get_column(MUSCLES_WORKED_DB_INDEX));
        std::istringstream excerciseTypeStream(database->get_column(EXCERCISE_TYPE_DB_INDEX));
        for (std::string str; std::getline(musclesWorkedStream, str, ',');) {
            loadedMusclesWorked.push_back(str);
        }
        for (std::string str; std::getline(excerciseTypeStream, str, ',');) {
            loadedExcerciseType.push_back(str);
        }
        *this = Lab::Excercise(database->get_column(EXCERCISE_NAME_DB_INDEX),
                               database->get_column(EXCERCISE_DESCRIPTION_DB_INDEX),
                               database->get_column(MUSCLE_GROUP_DB_INDEX), loadedMusclesWorked, loadedExcerciseType);
        return true;
    }
    return false;
};

bool Lab::Excercise::save(Lab::DBConn *database) const {
    std::string musclesWorkedString;
    std::string typeString;
    for (const auto &iter : musclesWorked) {
        musclesWorkedString += iter;
        if (*(musclesWorked.end() - 1) != iter) {
            musclesWorkedString += ",";
        }
    }
    for (const auto &iter : type) {
        typeString += iter;
        if (*(type.end() - 1) != iter) {
            typeString += ",";
        }
    }

    database->prepare("SELECT name FROM excercises WHERE name = ?", name);
    if (database->retrieve_next_row()) {
        if (database->prepare("UPDATE excercises SET name = ?, description = ?, muscleGroup = ?, "
                              "musclesTargeted = ?, type = ? WHERE name = ?",
                              name, description, muscleGroup, musclesWorkedString, typeString, name) == -1) {
            return false;
        }
        if (database->exec_prepared() == -1) {
            return false;
        }

    } else {
        if (database->prepare("INSERT INTO excercises (name, description, muscleGroup, "
                              "musclesTargeted, type) VALUES (?, ?, ?, ?, ?)",
                              name, description, muscleGroup, musclesWorkedString, typeString) == -1) {
            return false;
        }
        if (database->exec_prepared() == -1) {
            return false;
        }
    }
    return true;
}
