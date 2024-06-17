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
    Excercise(Excercise &&other)
        : name(std::move(other.name)),
          description(std::move(other.description)),
          muscleGroup(std::move(other.description)),
          musclesWorked(std::move(other.musclesWorked)),
          type(std::move(other.type)) {}
    Excercise(const Excercise &other)
        : name(other.name),
          description(other.description),
          muscleGroup(other.description),
          musclesWorked(other.musclesWorked),
          type(other.type) {}
    Excercise &operator=(Excercise &&other) {
        if (&other != this) {
            name = std::move(other.name);
            description = std::move(other.description);
            muscleGroup = std::move(other.description);
            musclesWorked = std::move(other.musclesWorked);
            type = std::move(other.type);
        }
        return *this;
    }
    Excercise &operator=(const Excercise &other) {
        if (&other != this) {
            name = other.name;
            description = other.description;
            muscleGroup = other.description;
            musclesWorked = other.musclesWorked;
            type = other.type;
        }
        return *this;
    }
    ~Excercise() = default;
    friend bool operator==(const Lab::Excercise &lhs,
                           const Lab::Excercise &rhs) {
        return (std::tie(lhs.name, lhs.description, lhs.muscleGroup,
                         lhs.musclesWorked, lhs.type) ==
                std::tie(rhs.name, rhs.description, rhs.muscleGroup,
                         rhs.musclesWorked, rhs.type));
    }

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
