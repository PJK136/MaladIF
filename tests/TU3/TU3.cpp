#include "Database.h"

#include "gtest/gtest.h"

TEST(TU3, a1) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
}

TEST(TU3, a2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata2.txt"));
}

TEST(TU3, a3) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata3.txt"));
}

TEST(TU3, a4) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata4.txt"));
}

//Le fichier contient des types mal définis
TEST(TU3, b1) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("metadata_bad1.txt"));
}

//le fichier n'est pas lisible
TEST(TU3, b2) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("metadata_bad2.txt"));
}

//le fichier n'existe pas
TEST(TU3, b3) {
    Database database;
    ASSERT_FALSE(database.loadMetadata(""));
}

//le nom du fichier est incorrect
TEST(TU3, b4) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("565e034b-6f73-4c0a-9b2d-dee4aae760eb.zer"));
}

//le nom du fichier est incorrect
TEST(TU3, b5) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("sdqf sdfjmlksqdf ~#{ßë‘dùmqélmùlé'mùlsflsdf"));
}

//le nom du fichier est incorrect
TEST(TU3, b6) {
    Database database;
    ASSERT_FALSE(database.loadMetadata(".."));
}

//le fichier est vide
TEST(TU3, b7) {
   Database database;
   ASSERT_FALSE(database.loadMetadata("empty.txt"));

}
