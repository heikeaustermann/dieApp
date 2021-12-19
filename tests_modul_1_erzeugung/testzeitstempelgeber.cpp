#include "../modul_1_erzeugung/zeitstempelgeber.h"
#include <gtest/gtest.h>

TEST(FrakZeitstempelgeberTest,frakzeitstempel) {
    Zeitstempelgeber zeitstempelgeber(100,10,10,0.01);
    uint64_t zeitstempel=zeitstempelgeber.gebeZeitstempel();
    EXPECT_GE(zeitstempel,101);
    zeitstempel = zeitstempelgeber.gebeZeitstempel(1000000);
    EXPECT_GE(zeitstempel,1000102);
}

TEST(ReguzeitstempelgeberTest, einfacherDurchlauf) {
    Zeitstempelgeber zeitstempelgeber(1000,0,10);
    uint64_t zeitstempel = zeitstempelgeber.gebeZeitstempel();
    EXPECT_LE(zeitstempel,1010);
    zeitstempel = zeitstempelgeber.gebeZeitstempel(100000);
    EXPECT_LE(zeitstempel,101020);
    EXPECT_GE(zeitstempel,101000);
}