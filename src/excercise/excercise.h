#pragma once
///@file

#include <string>
#include <tuple>
#include <vector>

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

    void setName(std::string);
    void setDescription(std::string);
    int setMuscleGroup(std::string);
    int setMusclesWorked(std::vector<std::string>);
    int setType(std::vector<std::string>);

    bool isMuscleGroup(std::string);
    bool isMuscle(std::vector<std::string>);
    bool isType(std::vector<std::string>);

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
