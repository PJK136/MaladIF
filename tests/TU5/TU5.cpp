#include "Database.h"
#include "Diagnosis.h"

#include "gtest/gtest.h"

//analyse d'une empreinte pr�sente dans les donn�es
TEST(TU5, a1) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnosis (database.diagnose("diagnosis1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_EQ(diagnosis.size(), 1);
    Fingerprint fp;
    fp.values = {1,std::string("True"),2.12,13.,3.156,1236.};
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>>::iterator it = diagnosis.begin();
    EXPECT_EQ((*it).first, fp);
    EXPECT_EQ(((*it).second).size(), 3);
    std::vector<Diagnosis>::iterator it2 = (*it).second.begin();
    Diagnosis d;
    d.disease = "Maladie2";
    d.risk = 1.;
    EXPECT_EQ(*it2, d);
    it2++;
    d.disease = "Maladie1";
    d.risk = 0.5;
    EXPECT_EQ(*it2, d);
    it2++;
    EXPECT_EQ((*it2).disease, "");
}

//analyse d'un fichier d'empreintes contenant plusieurs empreintes
TEST(TU5, a2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnosis (database.diagnose("diagnosis2.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_EQ(diagnosis.size(), 2);
    Fingerprint fp;

    fp.values = {1,std::string("True"),2.12,13.,3.156,1236.};
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>>::iterator it = diagnosis.begin();
    EXPECT_EQ((*it).first, fp);
    EXPECT_EQ(((*it).second).size(), 3);
    std::vector<Diagnosis>::iterator it2 = (*it).second.begin();
    Diagnosis d;
    d.disease = "Maladie2";
    d.risk = 1.;
    EXPECT_EQ(*it2, d);
    it2++;
    d.disease = "Maladie1";
    d.risk = 0.5;
    EXPECT_EQ(*it2, d);
    it2++;
    EXPECT_EQ((*it2).disease, "");

    fp.values = {2,std::string("False"),1.1,14.3,std::monostate(),2367.};
    it++;
    EXPECT_EQ((*it).first, fp);
    EXPECT_EQ(((*it).second).size(), 3);
    std::vector<Diagnosis>::iterator it3 = (*it).second.begin();
    d.disease = "";
    d.risk = 1.;
    EXPECT_EQ(*it3, d);
}

//fichier d'emplreinte vide
TEST(TU5, b1) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnosis (database.diagnose("empty.txt"));
    EXPECT_EQ(database.error(), Database::Error::EMPTY_FILE);
    ASSERT_EQ(diagnosis.size(), 0);
}

//fichier d'emplreinte inexistant
TEST(TU5, b2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnosis (database.diagnose("doesntExist.txt"));
    EXPECT_EQ(database.error(), Database::Error::CANT_OPEN);
    ASSERT_EQ(diagnosis.size(), 0);
}

//fichier d'emplreinte illisible
TEST(TU5, b3) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnosis (database.diagnose("data_bad1.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
    ASSERT_EQ(diagnosis.size(), 0);
}

//fichier d'empreintes avec des attributs en plus
TEST(TU5, b4) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnosis (database.diagnose("diagnosis_bad1.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
    ASSERT_EQ(diagnosis.size(), 0);
}

//fichier d'empreintes avec des attributs en moins dans la d�claration des attributs
TEST(TU5, b5) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnosis (database.diagnose("diagnosis_bad2.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
    ASSERT_EQ(diagnosis.size(), 0);
}

//fichier d'empreintes avec des attributs en moins sur la premi�re empreinte
TEST(TU5, c1) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnosis (database.diagnose("diagnosis_bad3.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
    ASSERT_EQ(diagnosis.size(), 0);
}

//fichier d'empreintes avec la 2�me empreinte non valide
TEST(TU5, c2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnosis (database.diagnose("diagnosis_bad4.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
    ASSERT_EQ(diagnosis.size(), 1);

    Fingerprint fp;
    fp.values = {1,std::string("True"),2.12,13.,3.156,1236.};
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>>::iterator it = diagnosis.begin();
    EXPECT_EQ((*it).first, fp);
    EXPECT_EQ(((*it).second).size(), 3);
    std::vector<Diagnosis>::iterator it2 = (*it).second.begin();
    Diagnosis d;
    d.disease = "Maladie2";
    d.risk = 1.;
    EXPECT_EQ(*it2, d);
    it2++;
    d.disease = "Maladie1";
    d.risk = 0.5;
    EXPECT_EQ(*it2, d);
    it2++;
    EXPECT_EQ((*it2).disease, "");
}
