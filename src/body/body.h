#pragma once
///@file

#include "database.h"

namespace Lab {

struct Muscles {
    unsigned short neck = 100;
    unsigned short trapezius = 100;
    unsigned short bicep = 100;
    unsigned short tricep = 100;
    unsigned short forearm = 100;
    unsigned short pectoral = 100;
    unsigned short abs = 100;
    unsigned short lats = 100;
    unsigned short upperBack = 100;
    unsigned short lowerBack = 100;
    unsigned short quads = 100;
    unsigned short glutes = 100;
    unsigned short hamstring = 100;
    unsigned short calf = 100;
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
    Body(Lab::DBConn &);

    int save();

    unsigned short getHeight();
    unsigned short getWeight();
    unsigned short getAge();
    Muscles getCondition();
    Measurements getMeasurements();

    void setHeight(unsigned short);
    void setWeight(unsigned short);
    void setAge(unsigned short);
    void setCondition(Muscles);
    void setMeasurement(Measurements);

   private:
    Lab::DBConn &db;
    unsigned short height;
    unsigned short weight;
    unsigned short age;
    Muscles condition;
    Measurements measure;
    bool bodyEdit = false;
    bool condEdit = false;
    bool measEdit = false;
};
}  // namespace Lab
