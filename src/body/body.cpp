#include "body.h"

#include "database.h"

Body::Body(std::shared_ptr<DBConn> dBase) : db(dBase) {}

Muscles Body::getCondition() {}

Measurements Body::getMeasurements() {}

unsigned short Body::getAge() {}

unsigned short Body::getHeight() {}

unsigned short Body::getWeight() {}

void Body::setMeasurement(Measurements nMes) {}

void Body::setCondition(Muscles nMus) {}

void Body::setAge(unsigned short nAge) {}

void Body::setWeight(unsigned short nWeight) {}

void Body::setHeight(unsigned short nHeight) {}

int Body::save() {}
