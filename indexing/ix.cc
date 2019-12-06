#include "ix.h"

#include <cstdio>
#include <cstring>

#include "return_code.h"

namespace indexing {

void IX_PrintError(int rc) { fprintf(stderr, "IX error %d.\n", rc); }


}