#include "Database.h"
#include "Diagnosis.h"

#include "gtest/gtest.h"

//analyse d'une empreinte présente dans les données
TEST(TU5, a1) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.startDiagnosis("diagnosis1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);

    Fingerprint fp;
    fp.values = {1,std::string("True"),2.12,13.,3.156,1236.};

    auto result = database.nextDiagnosis();
    EXPECT_EQ(database.error(), Database::Error::OK);
    EXPECT_EQ(result.first, fp);
    EXPECT_EQ(result.second.size(), 3);

    EXPECT_EQ(result.second[0].disease, "Maladie2");
    EXPECT_EQ(result.second[1].disease, "");
    EXPECT_EQ(result.second[2].disease, "Maladie1");

    result = database.nextDiagnosis();
    EXPECT_EQ(database.error(), Database::Error::EMPTY_FILE);
}

//analyse d'un fichier d'empreintes contenant plusieurs empreintes
TEST(TU5, a2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.startDiagnosis("diagnosis2.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);

    Fingerprint fp;
    fp.values = {1,std::string("True"),2.12,13.,3.156,1236.};

    auto result = database.nextDiagnosis();
    EXPECT_EQ(database.error(), Database::Error::OK);
    EXPECT_EQ(result.first, fp);
    EXPECT_EQ(result.second.size(), 3);

    EXPECT_EQ(result.second[0].disease, "Maladie2");
    EXPECT_EQ(result.second[1].disease, "");
    EXPECT_EQ(result.second[2].disease, "Maladie1");

    fp.values = {2,std::string("False"),1.1,14.3,std::monostate(),2367.};

    result = database.nextDiagnosis();
    EXPECT_EQ(database.error(), Database::Error::OK);
    EXPECT_EQ(result.first, fp);
    EXPECT_EQ(result.second.size(), 3);

    Diagnosis d;
    d.disease = "";
    d.risk = 1.;
    EXPECT_EQ(result.second[0], d);
}

//fichier d'emplreinte vide
TEST(TU5, b1) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_FALSE(database.startDiagnosis("empty.txt"));
    EXPECT_EQ(database.error(), Database::Error::EMPTY_FILE);
}

//fichier d'emplreinte inexistant
TEST(TU5, b2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_FALSE(database.startDiagnosis("doesntExist.txt"));
    EXPECT_EQ(database.error(), Database::Error::CANT_OPEN);
}

//fichier d'emplreinte illisible
TEST(TU5, b3) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_FALSE(database.startDiagnosis("data_bad1.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
}

//fichier d'empreintes avec des attributs en plus
TEST(TU5, b4) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_FALSE(database.startDiagnosis("diagnosis_bad1.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
}

//fichier d'empreintes avec des attributs en moins dans la déclaration des attributs
TEST(TU5, b5) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.startDiagnosis("diagnosis_bad2.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    database.nextDiagnosis();
    EXPECT_EQ(database.error(), Database::Error::INVALID);
}

//fichier d'empreintes avec des attributs en moins sur la première empreinte
TEST(TU5, c1) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.startDiagnosis("diagnosis_bad3.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    database.nextDiagnosis();
    EXPECT_EQ(database.error(), Database::Error::INVALID);
}

//fichier d'empreintes avec la 2ème empreinte non valide
TEST(TU5, c2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.startDiagnosis("diagnosis_bad4.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);

    Fingerprint fp;
    fp.values = {1,std::string("True"),2.12,13.,3.156,1236.};

    auto result = database.nextDiagnosis();
    EXPECT_EQ(result.first, fp);
    EXPECT_EQ(result.second.size(), 3);

    EXPECT_EQ(result.second[0].disease, "Maladie2");
    EXPECT_EQ(result.second[1].disease, "");
    EXPECT_EQ(result.second[2].disease, "Maladie1");

    database.nextDiagnosis();
    EXPECT_EQ(database.error(), Database::Error::INVALID);
}
