# include "../modul_4_parameter/entropieberechner.h"
# include <gtest/gtest.h>

TEST(Entropieberechner, nulleingabe) {
    double entropie = Entropieberechner::berechne("");
    EXPECT_EQ(entropie,0);
}

TEST(Entropieberechner, einseingabe) {
    double entropie = Entropieberechner::berechne("1");
    EXPECT_EQ(entropie,0);
}

TEST(Entropieberechner, beispiel) {
    double entropie = Entropieberechner::berechne("00AA00BB11");
    EXPECT_NEAR(entropie,0.240241,0.0001);
}

TEST(Entropieberechner, beispiel2) {
    double entropie = Entropieberechner::berechne("01020203030304040404");
    EXPECT_NEAR(entropie,0.230805,0.0001);
}