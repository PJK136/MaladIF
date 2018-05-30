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


TEST(TU3, b1) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("metadata_bad1.txt"));
}

TEST(TU3, b2) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("metadata_bad2.txt"));
}

TEST(TU3, b3) {
    Database database;
    ASSERT_FALSE(database.loadMetadata(""));
}

TEST(TU3, b4) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("565e034b-6f73-4c0a-9b2d-dee4aae760eb.zer"));
}

TEST(TU3, b5) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("sdqf sdfjmlksqdf ~#{ßë‘dùmqélmùlé'mùlsflsdf"));
}

TEST(TU3, b6) {
    Database database;
    ASSERT_FALSE(database.loadMetadata(".."));
}