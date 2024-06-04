#pragma once
///@file

#include <string>
#include <tuple>
#include <vector>

namespace Lab {

class Excercise {
   public:
    Excercise() : name(""), description(""), muscleGroup(""){};
    Excercise(std::string name, std::string desc, std::string mGroup,
              std::vector<std::string> mWorked, std::vector<std::string> eType);

    std::tuple<std::string, std::string, std::string, std::vector<std::string>,
               std::vector<std::string>>
    getDetails() const;
    std::string getName() const { return name; };
    std::string getDescription() const { return description; };
    std::string getMuscleGroup() const { return muscleGroup; };
    std::vector<std::string> getMusclesWorked() const { return musclesWorked; };
    std::vector<std::string> getType() const { return type; };

    void setName(const std::string &newName);
    void setDescription(const std::string &newDesc);
    int setMuscleGroup(const std::string &newMG);
    int setMusclesWorked(const std::vector<std::string> &newMW);
    int setType(const std::vector<std::string> &newType);

    bool isMuscleGroup(const std::string &mGroup);
    bool isMuscle(const std::vector<std::string> &mWorked);
    bool isType(const std::vector<std::string> &nType);

   private:
    std::string name;
    std::string description;
    std::string muscleGroup;
    std::vector<std::string> musclesWorked;
    std::vector<std::string> type;

    const std::vector<std::string> mwCheck{
        "Neck",     "Trapezius", "Bicep",     "Tricep",     "Forearm",
        "Pectoral", "Abs",       "Lats",      "Upper-back", "Lower-back",
        "Quads",    "Glutes",    "Hamstring", "Calf"};
    const std::vector<std::string> typeCheck{"weight", "reps", "time",
                                             "distance"};
    const std::vector<std::string> groupCheck{"Arms", "Chest", "Back",
                                              "Core", "Legs",  "Cardio"};
};

}  // namespace Lab
