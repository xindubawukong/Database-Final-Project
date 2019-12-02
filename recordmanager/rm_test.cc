#include "rm.h"
#include "gtest/gtest.h"
#include "../return_code.h"

namespace recordmanager {

TEST(TestPrintError, SimpleTest) {
  RM_PrintError(1);
}

}  // namespace recordmanager