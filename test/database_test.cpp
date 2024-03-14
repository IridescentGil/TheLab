#include "database.h"

#include <gtest/gtest.h>

#include <memory>

class DatabaseTest : public testing::Test {
   protected:
    void SetUp() override {}

    DBConn db1;
    DBConn db2;
    DBConn db3{"test.db"};
    DBConn db4{"test.db"};
    std::shared_ptr<DBConn> db5 = std::make_shared<DBConn>();
    std::shared_ptr<DBConn> db6;
};

TEST_F(DatabaseTest, NewDBTest) {}

TEST_F(DatabaseTest, SetDBTest) {}

TEST_F(DatabaseTest, PrepareIndexFunctionTest) {}

TEST_F(DatabaseTest, PrepareVariadicFunctionTest) {}

TEST_F(DatabaseTest, StepExecFunctionTest) {}

TEST_F(DatabaseTest, SameDBTest) {}

TEST_F(DatabaseTest, SharedPtrDBTest) {}

TEST_F(DatabaseTest, SanatizeQueryTest) {}
