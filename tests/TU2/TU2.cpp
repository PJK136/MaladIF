#include "FileReader.h"

#include <iostream>
#include "gtest/gtest.h"

//Fichier de métadonnées et d'empreintes valides
TEST(TU2,a) {
    Metadata metadata(FileReader::readMetadata("metadata1.txt"));
    FileReader fr;
    bool open(fr.open("data1.txt",metadata));
    EXPECT_EQ(fr.error(), FileReader::Error::OK);
    ASSERT_TRUE(open);

    std::pair<Fingerprint, std::string> line(fr.nextFingerprint());
    EXPECT_EQ(fr.error(), FileReader::Error::OK);
    Fingerprint fp;
    fp.values= {1,std::string("True"),2.12,13.,3.156,1236.};
    EXPECT_EQ(line.first,fp);
    EXPECT_EQ(line.second,"Maladie1");

    line = fr.nextFingerprint();
    EXPECT_EQ(fr.error(), FileReader::Error::OK);
    line = fr.nextFingerprint();
    EXPECT_EQ(fr.error(), FileReader::Error::OK);
    Fingerprint fp2;
    fp2.values = {2,std::string("False"),1.1,14.3,13.2,2367.};

    EXPECT_EQ(line.first,fp2);
    EXPECT_EQ(line.second,"");

    line = fr.nextFingerprint();
    EXPECT_EQ(fr.error(), FileReader::Error::OK);
    Fingerprint fp3;
    fp3.values= {3,std::string("None"),12.,145.,std::monostate(),11.};

    EXPECT_EQ(line.first,fp3);
    EXPECT_EQ(line.second,"Maladie1");
}


//Le nom du fichier de métadonnées est invalide
TEST(TU2,b1) {
    Metadata metadata(FileReader::readMetadata("metadata123456.txt"));
    FileReader fr;
    ASSERT_FALSE(fr.open("data1.txt",metadata));
    EXPECT_EQ(fr.error(), FileReader::Error::NO_METADATA);
}

//Le fichier de métadonnées est vide
TEST(TU2,b2) {
    Metadata metadata(FileReader::readMetadata("empty.txt"));
    FileReader fr;
    ASSERT_FALSE(fr.open("data1.txt",metadata));
    EXPECT_EQ(fr.error(), FileReader::Error::NO_METADATA);
}

//Le fichier de métadonnées définit des attributs non renseignés dans les empreintes
TEST(TU2,b3) {
    Metadata metadata(FileReader::readMetadata("metadata3.txt"));
    FileReader fr;
    ASSERT_TRUE(fr.open("data2.txt",metadata));
    EXPECT_EQ(fr.error(), FileReader::Error::OK);
}

//Le fichier métadonnées est mal contruit : types non valides
TEST(TU2,b4) {
    Metadata metadata(FileReader::readMetadata("metadata_bad1.txt"));
    FileReader fr;
    ASSERT_FALSE(fr.open("data1.txt",metadata));
    EXPECT_EQ(fr.error(), FileReader::Error::NO_METADATA);
}

//Le fichier de métadonnées n'utilise pas le ";" comme séparateur
TEST(TU2,b5) {
    Metadata metadata(FileReader::readMetadata("metadata_bad3.txt"));
    FileReader fr;
    ASSERT_FALSE(fr.open("data1.txt",metadata));
    EXPECT_EQ(fr.error(), FileReader::Error::NO_METADATA);
}

//le fichier de métadonnées n'est pas lisible
TEST(TU2,b6) {
    Metadata metadata(FileReader::readMetadata("metadata_bad2.txt"));
    FileReader fr;
    ASSERT_FALSE(fr.open("data1.txt",metadata));
    EXPECT_EQ(fr.error(), FileReader::Error::NO_METADATA);
}

//Le fichier d'empreintes renseigné n'existe pas
TEST(TU2,c1) {
    Metadata metadata(FileReader::readMetadata("metadata1.txt"));
    FileReader fr;
    ASSERT_FALSE(fr.open("data123456.txt",metadata));
    EXPECT_EQ(fr.error(), FileReader::Error::CANT_OPEN);
}

//Le fichier d'empreintes est vide
TEST(TU2,c2) {
    Metadata metadata(FileReader::readMetadata("metadata1.txt"));
    FileReader fr;
    ASSERT_FALSE(fr.open("empty.txt",metadata));
    EXPECT_EQ(fr.error(), FileReader::Error::EMPTY);
}

//Le fichier d'empreintes n'est pas lisible
TEST(TU2,c3) {
    Metadata metadata(FileReader::readMetadata("metadata1.txt"));
    FileReader fr;
    ASSERT_FALSE(fr.open("data_bad1.txt",metadata));
    EXPECT_EQ(fr.error(), FileReader::Error::UNKNOWN_ATTRIBUTE);
}

//Le fichier de métadonnées ne définit pas des attributs renseignés dans les empreintes
TEST(TU2,d) {
    Metadata metadata(FileReader::readMetadata("metadata4.txt"));
    FileReader fr;
    ASSERT_FALSE(fr.open("data1.txt",metadata));
    EXPECT_EQ(fr.error(), FileReader::Error::UNKNOWN_ATTRIBUTE);
}

//Le fichier d'empreintes est invalide : pas le bon nombre d'attributs pour une empreinte
TEST(TU2,e) {
    Metadata metadata(FileReader::readMetadata("metadata1.txt"));
    FileReader fr;
    ASSERT_TRUE(fr.open("data_bad2.txt",metadata));
    std::pair<Fingerprint, std::string> next = fr.nextFingerprint();
    EXPECT_EQ(next.first,Fingerprint());
    EXPECT_EQ(next.second,"");
    EXPECT_EQ(fr.error(), FileReader::Error::ATTRIBUTES_MISSING);
}


