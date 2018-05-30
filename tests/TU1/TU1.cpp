#include "FileReader.h"

#include <iostream>
#include "gtest/gtest.h"

//test avec un fichier correct
TEST(TU1, a1) {
    Metadata metadata(FileReader::readMetadata("metadata1.txt"));
    EXPECT_EQ(FileReader::metadataError(), FileReader::Error::OK);
    ASSERT_FALSE(metadata.attributes.empty());
    ASSERT_FALSE(metadata.attributesIndex.empty());
    EXPECT_EQ(metadata.attributes.size(), metadata.attributesIndex.size());
    EXPECT_EQ(metadata.attributes[0].name, "NoID");
    EXPECT_EQ(metadata.attributes[0].type, ID);
    EXPECT_EQ(metadata.attributes[1].name, "A1");
    EXPECT_EQ(metadata.attributes[1].type, STRING);
    EXPECT_EQ(metadata.attributes[2].name, "A2");
    EXPECT_EQ(metadata.attributes[2].type, DOUBLE);
    EXPECT_EQ(metadata.attributes[3].name, "A3");
    EXPECT_EQ(metadata.attributes[3].type, DOUBLE);
    EXPECT_EQ(metadata.attributes[4].name, "A4");
    EXPECT_EQ(metadata.attributes[4].type, DOUBLE);
    EXPECT_EQ(metadata.attributes[5].name, "AZ51");
    EXPECT_EQ(metadata.attributes[5].type, DOUBLE);
    EXPECT_EQ(metadata.attributesIndex["NoID"], 0);
    EXPECT_EQ(metadata.attributesIndex["A1"], 1);
    EXPECT_EQ(metadata.attributesIndex["A2"], 2);
    EXPECT_EQ(metadata.attributesIndex["A3"], 3);
    EXPECT_EQ(metadata.attributesIndex["A4"], 4);
    EXPECT_EQ(metadata.attributesIndex["AZ51"], 5);
}

//test avec un fichier correct
TEST(TU1, a2) {
    Metadata metadata(FileReader::readMetadata("metadata2.txt"));
    EXPECT_EQ(FileReader::metadataError(), FileReader::Error::OK);
    ASSERT_FALSE(metadata.attributes.empty());
    ASSERT_FALSE(metadata.attributesIndex.empty());
    EXPECT_EQ(metadata.attributes.size(), metadata.attributesIndex.size());
    EXPECT_EQ(metadata.attributes[0].name, "abc");
    EXPECT_EQ(metadata.attributes[0].type, STRING);
    EXPECT_EQ(metadata.attributes[1].name, "def");
    EXPECT_EQ(metadata.attributes[1].type, ID);
    EXPECT_EQ(metadata.attributes[2].name, "ghi");
    EXPECT_EQ(metadata.attributes[2].type, BOOLEAN);
    EXPECT_EQ(metadata.attributes[3].name, "jkl");
    EXPECT_EQ(metadata.attributes[3].type, DOUBLE);
    EXPECT_EQ(metadata.attributes[4].name, "mno");
    EXPECT_EQ(metadata.attributes[4].type, INT);
    EXPECT_EQ(metadata.attributesIndex["abc"], 0);
    EXPECT_EQ(metadata.attributesIndex["def"], 1);
    EXPECT_EQ(metadata.attributesIndex["ghi"], 2);
    EXPECT_EQ(metadata.attributesIndex["jkl"], 3);
    EXPECT_EQ(metadata.attributesIndex["mno"], 4);
}

//fichier avec des types invalides
TEST(TU1, b1) {
    Metadata metadata(FileReader::readMetadata("metadata_bad1.txt"));
    EXPECT_EQ(FileReader::metadataError(), FileReader::Error::INVALID_TYPE);
    EXPECT_TRUE(metadata.attributes.empty());
    EXPECT_TRUE(metadata.attributesIndex.empty());
}

//le fichier n'est pas lisible
TEST(TU1, b2) {
    Metadata metadata(FileReader::readMetadata("metadata_bad2.txt"));
    EXPECT_EQ(FileReader::metadataError(), FileReader::Error::INVALID_TYPE);
    EXPECT_TRUE(metadata.attributes.empty());
    EXPECT_TRUE(metadata.attributesIndex.empty());
}

//le fichier est vide
TEST(TU1, b3) {
    Metadata metadata(FileReader::readMetadata("empty.txt"));
    EXPECT_EQ(FileReader::metadataError(), FileReader::Error::EMPTY);
    EXPECT_TRUE(metadata.attributes.empty());
    EXPECT_TRUE(metadata.attributesIndex.empty());
}

//le fichier n'existe pas
TEST(TU1, c1) {
    Metadata metadata(FileReader::readMetadata(""));
    EXPECT_EQ(FileReader::metadataError(), FileReader::Error::CANT_OPEN);
    EXPECT_TRUE(metadata.attributes.empty());
    EXPECT_TRUE(metadata.attributesIndex.empty());
}

//nom du fichier invalide
TEST(TU1, c2) {
    Metadata metadata = FileReader::readMetadata("565e034b-6f73-4c0a-9b2d-dee4aae760eb.zer");
    EXPECT_EQ(FileReader::metadataError(), FileReader::Error::CANT_OPEN);
    EXPECT_TRUE(metadata.attributes.empty());
    EXPECT_TRUE(metadata.attributesIndex.empty());
}

//nom du fichier mal formé
TEST(TU1, c3) {
    Metadata metadata = FileReader::readMetadata("sdqf sdfjmlksqdf ~#{ßë‘dùmqélmùlé'mùlsflsdf");
    EXPECT_EQ(FileReader::metadataError(), FileReader::Error::CANT_OPEN);
    EXPECT_TRUE(metadata.attributes.empty());
    EXPECT_TRUE(metadata.attributesIndex.empty());
}

//nom du fichier mal formé
TEST(TU1, c4) {
    Metadata metadata = FileReader::readMetadata("..");
    EXPECT_EQ(FileReader::metadataError(), FileReader::Error::CANT_OPEN);
    EXPECT_TRUE(metadata.attributes.empty());
    EXPECT_TRUE(metadata.attributesIndex.empty());
}
