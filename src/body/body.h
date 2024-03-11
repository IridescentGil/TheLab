#ifndef BODY_H
#define BODY_H

#include <memory>
#include <string>

#include "databse.h"

struct Muscles {
    unsigned short neck;
    unsigned short trapezius;
    unsigned short bicep;
    unsigned short tricep;
    unsigned short forearm;
    unsigned short pectoral;
    unsigned short abs;
    unsigned short lats;
    unsigned short upperBack;
    unsigned short lowerBack;
    unsigned short quads;
    unsigned short glutes;
    unsigned short hamstring;
    unsigned short calf;
};

struct Measurements {
    unsigned short neck;
    unsigned short shoulders;
    unsigned short chest;
    unsigned short waist;
    unsigned short hips;
    unsigned short upperArmRight;
    unsigned short upperArmLeft;
    unsigned short foreaarmRight;
    unsigned short forearmLeft;
    unsigned short thighRight;
    unsigned short thighLeft;
    unsigned short calfRight;
    unsigned short calfLeft;
};

class Body {
   public:
    Body(std::shared_ptr<DBConn>);

    int save();

    unsigned short getHeight();
    unsigned short getWeight();
    unsigned short getAge();
    Muscles getCondition();

    void setHeight(unsigned short);
    void setWeight(unsigned short);
    void setAge(unsigned short);
    void setCondition(std::string);

   private:
    std::shared_ptr<DBConn> db;
    unsigned short height;
    unsigned short weight;
    unsigned short age;
    Muscles condition;
    Measurements measure;
};

#endif  // BODY_H
