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
    d.risk = 0.5;
    EXPECT_EQ(*it2, d);
}
