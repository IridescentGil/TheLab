#pragma once
///@file

#include "database.h"

namespace Lab {
namespace Constants {
static const unsigned short FULLY_HEALED = 100;
}

struct Muscles {
    unsigned short neck = Constants::FULLY_HEALED;
    unsigned short trapezius = Constants::FULLY_HEALED;
    unsigned short bicep = Constants::FULLY_HEALED;
    unsigned short tricep = Constants::FULLY_HEALED;
    unsigned short forearm = Constants::FULLY_HEALED;
    unsigned short pectoral = Constants::FULLY_HEALED;
    unsigned short abs = Constants::FULLY_HEALED;
    unsigned short lats = Constants::FULLY_HEALED;
    unsigned short upperBack = Constants::FULLY_HEALED;
    unsigned short lowerBack = Constants::FULLY_HEALED;
    unsigned short quads = Constants::FULLY_HEALED;
    unsigned short glutes = Constants::FULLY_HEALED;
    unsigned short hamstring = Constants::FULLY_HEALED;
    unsigned short calf = Constants::FULLY_HEALED;
};

struct Measurements {
    unsigned short neck = 0;
    unsigned short shoulders = 0;
    unsigned short chest = 0;
    unsigned short waist = 0;
    unsigned short hips = 0;
    unsigned short upperArmRight = 0;
    unsigned short upperArmLeft = 0;
    unsigned short forearmRight = 0;
    unsigned short forearmLeft = 0;
    unsigned short thighRight = 0;
    unsigned short thighLeft = 0;
    unsigned short calfRight = 0;
    unsigned short calfLeft = 0;
};

class Body {
   public:
    Body() : height(0), weight(0), age(0) {};
    Body(Lab::DBConn *dbBase);

    /**
     * @brief Update database if any values have been changed
     * @return An int 1 for success, -1 and -3 for statement preparation errors
     * and -2 and -4 for statement execution errors
     */
    int save();

    unsigned short getHeight() const { return height; };
    unsigned short getWeight() const { return weight; };
    unsigned short getAge() const { return age; };
    Muscles getCondition() const { return condition; };
    Measurements getMeasurements() const { return measurement; };

    void setHeight(const unsigned short &newHeight);
    void setWeight(const unsigned short &newWeight);
    void setAge(const unsigned short &newAge);
    void setCondition(const Muscles &newMuscles);
    void setMeasurement(const Measurements &newMeasurements);

   private:
    Lab::DBConn *db{};
    unsigned short height;
    unsigned short weight;
    unsigned short age;
    Muscles condition;
    Measurements measurement;
    bool bodyValuesEdited = false;
    bool conditionValuesEdited = false;
    bool measurementValuesEdited = false;
};
}  // namespace Lab
