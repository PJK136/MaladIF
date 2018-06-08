#include "Database.h"
#include "CLI_utils.h"
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
    std::cout << fp << std::endl;
    Fingerprint fp2;
    fp2.values = {std::monostate(),std::string("None"),7.06,79,1.578,623.5};
    EXPECT_EQ(fp, fp2); //Problème différence très petite
}

//on recherche les empreintes n'ayant aucune maladie
TEST(TU4, a2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    Fingerprint fp (database.getDiseaseCharacteristics(""));
    std::cout << fp << std::endl;
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
