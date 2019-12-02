#include "rm.h"

#include <cstdio>

namespace recordmanager {

void RM_PrintError(int rc) {
  fprintf(stderr, "RM error %d.\n", rc);
}

}  // recordmanager