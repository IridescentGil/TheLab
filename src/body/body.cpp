#include "body.h"

#include "database.h"

Body::Body(std::shared_ptr<DBConn> dBase) : db(dBase) {}
