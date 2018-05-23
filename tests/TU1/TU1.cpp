#include "FileReader.h"

#include <iostream>
#include "gtest/gtest.h"

TEST(TU1,a1) {
    Metadata metadata(FileReader::readMetadata("metadata1.txt"));
    ASSERT_FALSE(metadata.attributes.empty());
    ASSERT_EQ(metadata.attributes[0].name, "NoID");
    EXPECT_EQ(metadata.attributes[0].name, "NoID");
}

TEST(TU1, c1) {
    Metadata metadata(FileReader::readMetadata(""));
    ASSERT_TRUE(metadata.attributes.empty());
    ASSERT_TRUE(metadata.attributesIndex.empty());
}

TEST(TU1, c2) {
    Metadata metadata = FileReader::readMetadata("565e034b-6f73-4c0a-9b2d-dee4aae760eb.zer");
    ASSERT_TRUE(metadata.attributes.empty());
    ASSERT_TRUE(metadata.attributesIndex.empty());
}

TEST(TU1, c3) {
    Metadata metadata = FileReader::readMetadata("sdqf sdfjmlksqdf ~#{ßë‘dùmqélmùlé'mùlsflsdf");
    ASSERT_TRUE(metadata.attributes.empty());
    ASSERT_TRUE(metadata.attributesIndex.empty());
}