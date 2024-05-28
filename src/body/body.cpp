#include "body.h"

#include <chrono>

#include "database.h"

Lab::Body::Body(Lab::DBConn& dbBase) : db(dbBase) {
    db.execMulti("SELECT * FROM bodyStats ORDER BY date DESC LIMIT 1");
    if (db.stepExec()) {
        weight = db.getColumn(1);
        height = db.getColumn(2);
        age = db.getColumn(3);
        measure.neck = db.getColumn(4);
        measure.shoulders = db.getColumn(5);
        measure.chest = db.getColumn(6);
        measure.waist = db.getColumn(7);
        measure.hips = db.getColumn(8);
        measure.upperArmRight = db.getColumn(9);
        measure.upperArmLeft = db.getColumn(10);
        measure.forearmRight = db.getColumn(11);
        measure.forearmLeft = db.getColumn(12);
        measure.thighRight = db.getColumn(13);
        measure.thighLeft = db.getColumn(14);
        measure.calfRight = db.getColumn(15);
        measure.calfLeft = db.getColumn(16);
    } else {
        age = 0;
        height = 0;
        weight = 0;
    }
    db.execMulti("SELECT * FROM bodyCondition ORDER BY date DESC LIMIT 1");
    if (db.stepExec()) {
        condition.neck = db.getColumn(1);
        condition.trapezius = db.getColumn(2);
        condition.bicep = db.getColumn(3);
        condition.tricep = db.getColumn(4);
        condition.forearm = db.getColumn(5);
        condition.pectoral = db.getColumn(6);
        condition.abs = db.getColumn(7);
        condition.lats = db.getColumn(8);
        condition.upperBack = db.getColumn(9);
        condition.lowerBack = db.getColumn(10);
        condition.quads = db.getColumn(11);
        condition.glutes = db.getColumn(12);
        condition.hamstring = db.getColumn(13);
        condition.calf = db.getColumn(14);
    }
}

Lab::Muscles Lab::Body::getCondition() { return condition; }

Lab::Measurements Lab::Body::getMeasurements() { return measure; }

unsigned short Lab::Body::getAge() { return age; }

unsigned short Lab::Body::getHeight() { return height; }

unsigned short Lab::Body::getWeight() { return weight; }

void Lab::Body::setMeasurement(Measurements newMeasurements) {
    measure = newMeasurements;
    measEdit = true;
}

void Lab::Body::setCondition(Muscles newMuscles) {
    condition = newMuscles;
    condEdit = true;
}

void Lab::Body::setAge(unsigned short newAge) {
    age = newAge;
    bodyEdit = true;
}

void Lab::Body::setWeight(unsigned short newWeight) {
    weight = newWeight;
    bodyEdit = true;
}

void Lab::Body::setHeight(unsigned short newHeight) {
    height = newHeight;
    bodyEdit = true;
}

int Lab::Body::save() {
    auto epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
    if (bodyEdit || measEdit) {
        if (db.prepare(
                "INSERT INTO bodyStats (date, weight, height, age, "
                "neckMeasurement, "
                "shouldersMeasurement, chestMeasurement, waistMeasurement, "
                "hipsMeasurement, upperArmRightMeasurement, "
                "upperArmLeftMeasurement, "
                "forearmRightMeasurement, forearmLeftMeasurement, "
                "thighRightMeasurement, thighLeftMeasurement, "
                "calfRightMeasurement, "
                "calfLeftMeasurement) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
                "?, ?, ?, ?, ?, ?)",
                epoch, weight, height, age, measure.neck, measure.shoulders,
                measure.chest, measure.waist, measure.hips,
                measure.upperArmRight, measure.upperArmLeft,
                measure.forearmRight, measure.forearmLeft, measure.thighRight,
                measure.thighLeft, measure.calfRight, measure.calfLeft) == -1)
            return -1;
        if (db.execQuery() == -1) return -2;
        measEdit = false;
        bodyEdit = false;
    }
    if (condEdit) {
        if (db.prepare("INSERT INTO bodyCondition (date, neckCondition, "
                       "trapeziusCondition, "
                       "bicepsCondition, tricepsCondition, forearmsCondition, "
                       "pectoralsCondition, "
                       "absCondition, latsCondition, upperBackCondition, "
                       "lowerBackCondition, "
                       "quadsCondition, glutesCondition, hamstringsCondition, "
                       "calfsCondition) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
                       "?, ?, ?, ?, ?)",
                       epoch, condition.neck, condition.trapezius,
                       condition.bicep, condition.tricep, condition.forearm,
                       condition.pectoral, condition.abs, condition.lats,
                       condition.upperBack, condition.lowerBack,
                       condition.quads, condition.glutes, condition.hamstring,
                       condition.calf) == -1)
            return -3;
        if (db.execQuery() == -1) return -4;
        condEdit = false;
    }

    return 1;
}
