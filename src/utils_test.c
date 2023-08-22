#include "minunit.h"
#include "utils.c"
#include "utils.h"
#include <stdio.h>

char g_assertMsg[MU_MSGBUF_LEN];
int g_testsRun = 0;

static char *testAListAppend(void);
static char *testHTableSet(void);
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

static char *testAListAppend(void) {
    unsigned char buffer[Kilobyte(10)];
    Arena arena;
    AList list;

    ArenaInit(&arena, buffer, Kilobyte(10));
    AListInit(&list, &arena);

    for (int i = 0; i <= 100; ++i) {
        AListAppend(&list, i * 10);
    }

    MU_ASSERT_FMT(101 == AListSize(&list), "Expected size %d, but got %lu", 101,
                  AListSize(&list));
    MU_ASSERT_FMT(101 < list.capacity, "Expected capacity greater than %d, but got %lu",
                  101, list.capacity);

    MU_ASSERT_FMT(0 == AListGet(&list, 0), "At index %d, expected %d, but got %d", 0, 0,
                  AListGet(&list, 0));
    MU_ASSERT_FMT(100 == AListGet(&list, 10), "At index %d, expected %d, but got %d",
                  10, 100, AListGet(&list, 100));
    MU_ASSERT_FMT(1000 == AListGet(&list, 100), "At index %d, expected %d, but got %d",
                  100, 1000, AListGet(&list, 100));

    MU_PASS;
}

static char *testHTableSet(void) {
    unsigned char buffer[Kilobyte(100)];
    Arena arena;
    HTable table;
    const char *words[] = {"prison", "zombie", "policeman", "rifle", "ammo_bag"};

    ArenaInit(&arena, buffer, Kilobyte(100));
    HTableInit(&table, &arena);

    for (int i = 0; i < 100; ++i) {
        char customKey[64];
        int wordIdx = i % 5;
        snprintf(customKey, 64, "%s_%d", words[wordIdx], i);
        HTableSet(&table, customKey, i * 10);
    }

    MU_ASSERT_FMT(100 == table.size, "Expected size %d, but got %lu", 100, table.size);
    MU_ASSERT_FMT(100 < table.capacity,
                  "Expected capacity greater than %d, but got %lu", 100, table.size);

    const char *keys[] = {"prison_0", "zombie_6", "policeman_12", "rifle_53",
                          "ammo_bag_99"};
    int expectedVals[] = {0, 60, 120, 530, 990};

    for (int i = 0; i < 5; ++i) {
        int got = HTableGet(&table, keys[i]);
        int expect = expectedVals[i];
        MU_ASSERT_FMT(expect == got, "Expected %d, but got %d", expect, got);
    }

    MU_PASS;
}

static char *allTests(void) {
    MU_TEST(testAListAppend);
    MU_TEST(testHTableSet);
    MU_PASS;
}
