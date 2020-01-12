
#include "filesystem/utils/MyBitMap.h"
#include <unistd.h>
#include <vector>

char start_parse(const char *expr_input);

int yyparse();

int main(int args, char **argv) {
    filesystem::MyBitMap::initConst();
	chdir("/Users/xdbwk/Desktop/thu41/database/test_dir");
    system("rm -rf *");
    if (args > 1) {
        for (int i = 0; i < args - 1; ++i) {
            if (freopen(argv[i + 1], "r", stdin))
                yyparse();
            else {
                fprintf(stderr, "Open file %s failed\n", argv[i + 1]);
                return -1;
            }
        }
    }
    else {
        freopen("../generated.txt", "r", stdin);
        int rc = yyparse();
        while(rc) {
            rc = yyparse();
        }
        return 0;
    }
}