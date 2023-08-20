#include "minunit.h"

char g_assertMsg[MU_MSGBUF_LEN];
int g_testsRun = 0;

static char *allTests(void);

int main(void) {
    // run tests and exit
    char *testsResults = allTests();
    printf("Test count: %d\n", g_testsRun);

    if (testsResults != 0) {
        printf("Test failed\n");
        printf("%s\n", testsResults);
        return 1;
    }
    printf("All tests passed.\n");

    return 0;
}

static char *allTests(void) {
    MU_PASS;
}
