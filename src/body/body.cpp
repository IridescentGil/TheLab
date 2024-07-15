#include "body.h"

#include <chrono>
#include <utility>

#include "database.h"

namespace {
enum MEASUREMENT_DATABASE_DB_INDEXES {
    WEIGHT_DB_INDEX = 1,
    HEIGHT_DB_INDEX,
    AGE_DB_INDEX,
    NECK_DB_INDEX,
    SHOULDER_DB_INDEX,
    CHEST_DB_INDEX,
    WAIST_DB_INDEX,
    HIPS_DB_INDEX,
    UPPER_RIGHT_ARM_DB_INDEX,
    UPPER_LEFT_ARM_DB_INDEX,
    RIGHT_FOREARM_DB_INDEX,
    LEFT_FOREARM_DB_INDEX,
    RIGHT_THIGH_DB_INDEX,
    LEFT_THIGH_DB_INDEX,
    RIGHT_CALF_DB_INDEX,
    LEFT_CALF_DB_INDEX
};
enum CONDITION_DATABASE_DB_INDEXES {
    NECK_CONDITION_DB_INDEX = 1,
    TRAPEZIUS_CONDITION_DB_INDEX,
    BICEP_CONDITION_DB_INDEX,
    TRICEP_CONDITION_DB_INDEX,
    FOREARM_CONDITION_DB_INDEX,
    PECTORAL_CONDITION_DB_INDEX,
    ABS_CONDITION_DB_INDEX,
    LATS_CONDITION_DB_INDEX,
    UPPER_BACK_CONDITION_DB_INDEX,
    LOWER_BACK_CONDITION_DB_INDEX,
    QUADS_CONDITION_DB_INDEX,
    GLUTES_CONDITION_DB_INDEX,
    HAMSTRING_CONDITION_DB_INDEX,
    CALF_CONDITION_DB_INDEX
};
}  // namespace

Lab::Body::Body(std::shared_ptr<Lab::DBConn> dbBase) : db(std::move(dbBase)) {
    db->execMulti("SELECT * FROM bodyStats ORDER BY date DESC LIMIT 1");
    if (db->stepExec()) {
        weight = db->getColumn(WEIGHT_DB_INDEX);
        height = db->getColumn(HEIGHT_DB_INDEX);
        age = db->getColumn(AGE_DB_INDEX);
        measurement.neck = db->getColumn(NECK_DB_INDEX);
        measurement.shoulders = db->getColumn(SHOULDER_DB_INDEX);
        measurement.chest = db->getColumn(CHEST_DB_INDEX);
        measurement.waist = db->getColumn(WAIST_DB_INDEX);
        measurement.hips = db->getColumn(HIPS_DB_INDEX);
        measurement.upperArmRight = db->getColumn(UPPER_RIGHT_ARM_DB_INDEX);
        measurement.upperArmLeft = db->getColumn(UPPER_LEFT_ARM_DB_INDEX);
        measurement.forearmRight = db->getColumn(RIGHT_FOREARM_DB_INDEX);
        measurement.forearmLeft = db->getColumn(LEFT_FOREARM_DB_INDEX);
        measurement.thighRight = db->getColumn(RIGHT_THIGH_DB_INDEX);
        measurement.thighLeft = db->getColumn(LEFT_THIGH_DB_INDEX);
        measurement.calfRight = db->getColumn(RIGHT_CALF_DB_INDEX);
        measurement.calfLeft = db->getColumn(LEFT_CALF_DB_INDEX);
    } else {
        age = 0;
        height = 0;
        weight = 0;
    }
    db->execMulti("SELECT * FROM bodyCondition ORDER BY date DESC LIMIT 1");
    if (db->stepExec()) {
        condition.neck = db->getColumn(NECK_CONDITION_DB_INDEX);
        condition.trapezius = db->getColumn(TRAPEZIUS_CONDITION_DB_INDEX);
        condition.bicep = db->getColumn(BICEP_CONDITION_DB_INDEX);
        condition.tricep = db->getColumn(TRICEP_CONDITION_DB_INDEX);
        condition.forearm = db->getColumn(FOREARM_CONDITION_DB_INDEX);
        condition.pectoral = db->getColumn(PECTORAL_CONDITION_DB_INDEX);
        condition.abs = db->getColumn(ABS_CONDITION_DB_INDEX);
        condition.lats = db->getColumn(LATS_CONDITION_DB_INDEX);
        condition.upperBack = db->getColumn(UPPER_BACK_CONDITION_DB_INDEX);
        condition.lowerBack = db->getColumn(LOWER_BACK_CONDITION_DB_INDEX);
        condition.quads = db->getColumn(QUADS_CONDITION_DB_INDEX);
        condition.glutes = db->getColumn(GLUTES_CONDITION_DB_INDEX);
        condition.hamstring = db->getColumn(HAMSTRING_CONDITION_DB_INDEX);
        condition.calf = db->getColumn(CALF_CONDITION_DB_INDEX);
    }
}

void Lab::Body::setMeasurement(const Measurements &newMeasurements) {
    measurement = newMeasurements;
    measurementValuesEdited = true;
}

void Lab::Body::setCondition(const Muscles &newMuscles) {
    condition = newMuscles;
    conditionValuesEdited = true;
}

void Lab::Body::setAge(const unsigned short &newAge) {
    age = newAge;
    bodyValuesEdited = true;
}

void Lab::Body::setWeight(const unsigned short &newWeight) {
    weight = newWeight;
    bodyValuesEdited = true;
}

void Lab::Body::setHeight(const unsigned short &newHeight) {
    height = newHeight;
    bodyValuesEdited = true;
}

int Lab::Body::save() {
    auto epoch =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    if (bodyValuesEdited || measurementValuesEdited) {
        if (db->prepare("INSERT INTO bodyStats (date, weight, height, age, "
                        "neckMeasurement, "
                        "shouldersMeasurement, chestMeasurement, waistMeasurement, "
                        "hipsMeasurement, upperArmRightMeasurement, "
                        "upperArmLeftMeasurement, "
                        "forearmRightMeasurement, forearmLeftMeasurement, "
                        "thighRightMeasurement, thighLeftMeasurement, "
                        "calfRightMeasurement, "
                        "calfLeftMeasurement) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
                        "?, ?, ?, ?, ?, ?)",
                        epoch, weight, height, age, measurement.neck, measurement.shoulders, measurement.chest,
                        measurement.waist, measurement.hips, measurement.upperArmRight, measurement.upperArmLeft,
                        measurement.forearmRight, measurement.forearmLeft, measurement.thighRight,
                        measurement.thighLeft, measurement.calfRight, measurement.calfLeft) == -1) {
            return -1;
        }
        if (db->execQuery() == -1) {
            return -2;
        }
        measurementValuesEdited = false;
        bodyValuesEdited = false;
    }
    if (conditionValuesEdited) {
        if (db->prepare("INSERT INTO bodyCondition (date, neckCondition, "
                        "trapeziusCondition, "
                        "bicepsCondition, tricepsCondition, forearmsCondition, "
                        "pectoralsCondition, "
                        "absCondition, latsCondition, upperBackCondition, "
                        "lowerBackCondition, "
                        "quadsCondition, glutesCondition, hamstringsCondition, "
                        "calfsCondition) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
                        "?, ?, ?, ?, ?)",
                        epoch, condition.neck, condition.trapezius, condition.bicep, condition.tricep,
                        condition.forearm, condition.pectoral, condition.abs, condition.lats, condition.upperBack,
                        condition.lowerBack, condition.quads, condition.glutes, condition.hamstring,
                        condition.calf) == -1) {
            return -3;
        }
        if (db->execQuery() == -1) {
            return -4;
        }
        conditionValuesEdited = false;
    }

    return 1;
}
