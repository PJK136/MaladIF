#include "Database.h"
#include "gtest/gtest.h"

//recherche d'une maladie répertoriée
TEST(TU4, a1) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    Fingerprint fp (database.getDiseaseCharacteristics("Maladie1"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    Fingerprint fp2;
    fp2.values = {std::monostate(),std::string("None"),(2.12+12.)/2.,79.,3.156,(1236.+11.)/2.};
    EXPECT_EQ(fp, fp2);
}

//on recherche les empreintes n'ayant aucune maladie
TEST(TU4, a2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    Fingerprint fp (database.getDiseaseCharacteristics(""));
    EXPECT_EQ(database.error(), Database::Error::OK);
    Fingerprint fp2;
    fp2.values = {std::monostate(),std::string("False"),1.1,14.3,13.2,2367.};
    EXPECT_EQ(fp, fp2);
}

//recherche d'une maladie non répertoriée
TEST(TU4, b) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    Fingerprint fp (database.getDiseaseCharacteristics("Inconnue"));
    EXPECT_EQ(database.error(), Database::Error::NOT_FOUND);
    ASSERT_EQ(fp.values.size(), 0);
}
