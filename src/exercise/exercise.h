#pragma once
///@file

#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace Lab {
class DBConn;

class Exercise {
   public:
    Exercise();
    Exercise(std::string name, std::string desc, const std::string &mGroup, const std::vector<std::string> &mWorked,
             const std::vector<std::string> &eType);
    Exercise(Exercise &&other) noexcept
        : name(std::move(other.name)),
          description(std::move(other.description)),
          muscleGroup(std::move(other.muscleGroup)),
          musclesWorked(std::move(other.musclesWorked)),
          type(std::move(other.type)) {}
    Exercise(const Exercise &other)
        : name(other.name),
          description(other.description),
          muscleGroup(other.muscleGroup),
          musclesWorked(other.musclesWorked),
          type(other.type) {}
    Exercise &operator=(Exercise &&other) noexcept {
        if (&other != this) {
            name = std::move(other.name);
            description = std::move(other.description);
            muscleGroup = std::move(other.muscleGroup);
            musclesWorked = std::move(other.musclesWorked);
            type = std::move(other.type);
        }
        return *this;
    }
    Exercise &operator=(const Exercise &other) {
        if (&other != this) {
            name = other.name;
            description = other.description;
            muscleGroup = other.muscleGroup;
            musclesWorked = other.musclesWorked;
            type = other.type;
        }
        return *this;
    }
    ~Exercise() = default;

    inline bool operator==(const Lab::Exercise &compare) const {
        return (std::tie(this->name, this->description, this->muscleGroup, this->musclesWorked, this->type) ==
                std::tie(compare.name, compare.description, compare.muscleGroup, compare.musclesWorked, compare.type));
    }
    inline bool operator!=(const Lab::Exercise &compare) const { return !(*this == compare); }
    inline bool operator<(const Lab::Exercise &compare) const {
        return (std::tie(this->name, this->description, this->muscleGroup, this->musclesWorked, this->type) <
                std::tie(compare.name, compare.description, compare.muscleGroup, compare.musclesWorked, compare.type));
    }

    std::tuple<std::string, std::string, std::string, std::vector<std::string>, std::vector<std::string>> getDetails()
        const;
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

    bool load(Lab::DBConn *database, const std::string &exercise);
    bool save(Lab::DBConn *database) const;

    bool isMuscleGroup(const std::string &mGroup);
    bool isMuscle(const std::vector<std::string> &mWorked);
    bool isType(const std::vector<std::string> &nType);

   private:
    std::string name;
    std::string description;
    std::string muscleGroup;
    std::vector<std::string> musclesWorked;
    std::vector<std::string> type;

    std::vector<std::string_view> mwCheck{"Neck", "Trapezius",  "Bicep",      "Tricep", "Forearm", "Pectoral",  "Abs",
                                          "Lats", "Upper-back", "Lower-back", "Quads",  "Glutes",  "Hamstring", "Calf"};
    /*
     Weight (kg), Reps, time (seconds),
     distance (meters).
     */
    std::vector<std::string_view> typeCheck{"weight", "reps", "time", "distance"};
    std::vector<std::string_view> groupCheck{"Arms", "Chest", "Back", "Core", "Legs", "Cardio"};
};

}  // namespace Lab
