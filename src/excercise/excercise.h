#pragma once
///@file

#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace Lab {

class Excercise {
   public:
    Excercise();
    Excercise(std::string name, std::string desc, const std::string &mGroup,
              const std::vector<std::string> &mWorked,
              const std::vector<std::string> &eType);
    Excercise(Excercise &&other) noexcept
        : name(std::move(other.name)),
          description(std::move(other.description)),
          muscleGroup(std::move(other.muscleGroup)),
          musclesWorked(std::move(other.musclesWorked)),
          type(std::move(other.type)) {}
    Excercise(const Excercise &other)
        : name(other.name),
          description(other.description),
          muscleGroup(other.muscleGroup),
          musclesWorked(other.musclesWorked),
          type(other.type) {}
    Excercise &operator=(Excercise &&other) noexcept {
        if (&other != this) {
            name = std::move(other.name);
            description = std::move(other.description);
            muscleGroup = std::move(other.muscleGroup);
            musclesWorked = std::move(other.musclesWorked);
            type = std::move(other.type);
        }
        return *this;
    }
    Excercise &operator=(const Excercise &other) {
        if (&other != this) {
            name = other.name;
            description = other.description;
            muscleGroup = other.muscleGroup;
            musclesWorked = other.musclesWorked;
            type = other.type;
        }
        return *this;
    }
    ~Excercise() = default;

    inline bool operator==(const Lab::Excercise &compare) const {
        return (std::tie(this->name, this->description, this->muscleGroup,
                         this->musclesWorked, this->type) ==
                std::tie(compare.name, compare.description, compare.muscleGroup,
                         compare.musclesWorked, compare.type));
    }

    inline bool operator!=(const Lab::Excercise &compare) const {
        return !(*this == compare);
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

    std::vector<std::string_view> mwCheck{
        "Neck",     "Trapezius", "Bicep",     "Tricep",     "Forearm",
        "Pectoral", "Abs",       "Lats",      "Upper-back", "Lower-back",
        "Quads",    "Glutes",    "Hamstring", "Calf"};
    std::vector<std::string_view> typeCheck{"weight", "reps", "time",
                                            "distance"};
    std::vector<std::string_view> groupCheck{"Arms", "Chest", "Back",
                                             "Core", "Legs",  "Cardio"};
};

}  // namespace Lab
