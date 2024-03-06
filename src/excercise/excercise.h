#ifndef EXCERCISE_H
#define EXCERCISE_H

#include <string>
#include <tuple>
#include <vector>

class Excercise {
   public:
    Excercise(std::string name, std::string desc, std::string mGroup,
              std::vector<std::string> mWorked, std::vector<std::string> eType)
        : name(name),
          description(desc),
          muscleGroup(mGroup),
          musclesWorked(mWorked),
          type(eType) {}

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
    void setMuscleGroup(std::string);
    void setMusclesWorked(std::vector<std::string>);
    void setType(std::vector<std::string>);

   private:
    std::string name;
    std::string description;
    std::string muscleGroup;
    std::vector<std::string> musclesWorked;
    std::vector<std::string> type;
};

#endif  // !EXCERCISE_H
