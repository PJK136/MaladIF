#include "Database.h"

#include "gtest/gtest.h"

//recherche d'une maladie répertoriée
TEST(TU4, a1) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<Fingerprint> fp (database.getDiseaseCharacteristics("Maladie1"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_EQ(fp.size(), 2);
    Fingerprint fp2;
    fp2.values = {1,std::string("True"),2.12,13.,3.156,1236.};
    std::list<Fingerprint>::iterator it = fp.begin();
    EXPECT_EQ(*it, fp2);
    it++;
    fp2.values = {3,std::string("None"),12.,145.,std::monostate(),11.};
    EXPECT_EQ(*it, fp2);
}

//on recherche les empreintes n'ayant aucune maladie
TEST(TU4, a2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<Fingerprint> fp (database.getDiseaseCharacteristics(""));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_EQ(fp.size(), 1);
     Fingerprint fp2;
    fp2.values = {2,std::string("False"),1.1,14.3,13.2,2367.};
    std::list<Fingerprint>::iterator it = fp.begin();
    EXPECT_EQ(*it, fp2);
}

//recherche d'une maladie non répertoriée
TEST(TU4, b) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<Fingerprint> fp (database.getDiseaseCharacteristics("Inconnue"));
    EXPECT_EQ(database.error(), Database::Error::NOT_FOUND);
    ASSERT_EQ(fp.size(), 0);
}
