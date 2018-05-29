#include "FileReader.h"

#include <iostream>
#include "gtest/gtest.h"

TEST(TU2,a) {
    Metadata metadata(FileReader::readMetadata("metadata1.txt"));
    FileReader fr;
    bool open(fr.open("data2.txt",metadata));
    ASSERT_TRUE(open);

    std::pair<Fingerprint, std::string> line(fr.nextFingerprint());
    Fingerprint fp;
    fp.values.push_back(1);
    fp.values.push_back(std::string("True"));
    fp.values.push_back(2.12);
    fp.values.push_back(13.);
    fp.values.push_back(3.156);
    fp.values.push_back(1236.);
    EXPECT_EQ(line.first,fp);
    EXPECT_EQ(line.second,"Maladie1");

    line = fr.nextFingerprint();
    line = fr.nextFingerprint();
    Fingerprint fp2;
    fp2.values.push_back(2);
    fp2.values.push_back(std::string("False"));
    fp2.values.push_back(1.1);
    fp2.values.push_back(14.3);
    fp2.values.push_back(13.2);
    fp2.values.push_back(2367.);

    EXPECT_EQ(line.first,fp2);
    EXPECT_EQ(line.second,"");
}

TEST(TU2,b1) {
    Metadata metadata(FileReader::readMetadata("metadata123456.txt"));
    FileReader fr;
    bool open(fr.open("data2.txt",metadata));
    ASSERT_FALSE(open);
}

TEST(TU2,b2) {
    Metadata metadata(FileReader::readMetadata("metadata_empty.txt"));
    FileReader fr;
    bool open(fr.open("data2.txt",metadata));
    ASSERT_FALSE(open);
}

TEST(TU2,b3) {
    Metadata metadata(FileReader::readMetadata("metadata3.txt"));
    FileReader fr;
    bool open(fr.open("data2.txt",metadata));
    ASSERT_TRUE(open);
}

TEST(TU2,b4) {
    Metadata metadata(FileReader::readMetadata("metadata4.txt"));
    FileReader fr;
    bool open(fr.open("data2.txt",metadata));
    ASSERT_FALSE(open);
}
