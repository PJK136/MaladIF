#include "Database.h"

#include "gtest/gtest.h"

//fichier de métadonnées valide
TEST(TU3, a1) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
}

//fichier de métadonnées valide
TEST(TU3, a2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata2.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
}

//fichier de métadonnées valide
TEST(TU3, a3) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata3.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
}

//fichier de métadonnées valide
TEST(TU3, a4) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata4.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
}

//Le fichier contient des types mal définis
TEST(TU3, b1) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("metadata_bad1.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
}

//le fichier n'est pas lisible
TEST(TU3, b2) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("metadata_bad2.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
}

//le fichier n'existe pas
TEST(TU3, b3) {
    Database database;
    ASSERT_FALSE(database.loadMetadata(""));
    EXPECT_EQ(database.error(), Database::Error::CANT_OPEN);
}

//le nom du fichier est incorrect
TEST(TU3, b4) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("565e034b-6f73-4c0a-9b2d-dee4aae760eb.zer"));
    EXPECT_EQ(database.error(), Database::Error::CANT_OPEN);
}

//le nom du fichier est incorrect
TEST(TU3, b5) {
    Database database;
    ASSERT_FALSE(database.loadMetadata("sdqf sdfjmlksqdf ~#{ßë‘dùmqélmùlé'mùlsflsdf"));
    EXPECT_EQ(database.error(), Database::Error::CANT_OPEN);
}

//le nom du fichier est un chemin vers un dossier
TEST(TU3, b6) {
    Database database;
    ASSERT_FALSE(database.loadMetadata(".."));
    //Sous Linux, on peut ouvrir un dossier avec fstream alors que sous Windows non
    //Donc le code d'erreur change selon l'OS
    EXPECT_NE(database.error(), Database::Error::OK);
}

//le fichier est vide
TEST(TU3, b7) {
   Database database;
   ASSERT_FALSE(database.loadMetadata("empty.txt"));
   EXPECT_EQ(database.error(), Database::Error::EMPTY_FILE);
}

//fichiers de métadonnées et de données valides
TEST(TU3, c) {
    Database database;
    EXPECT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    EXPECT_TRUE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
}

//le fichier de données est vide
TEST(TU3, d1) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_FALSE(database.loadData("empty.txt"));
    EXPECT_EQ(database.error(), Database::Error::EMPTY_FILE);
}

//fichier de données inexistant
TEST(TU3, d2) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_FALSE(database.loadData("doesntExist.txt"));
    EXPECT_EQ(database.error(), Database::Error::CANT_OPEN);
}

//ficher de données avec un nom incorrect
TEST(TU3, d3) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_FALSE(database.loadData("jmlksqdf ~#{."));
    EXPECT_EQ(database.error(), Database::Error::CANT_OPEN);
}

//ficher de données invalide car nombre de caractéristiques renseignées invalide
TEST(TU3, d4) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_FALSE(database.loadData("data_bad2.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
}

//ficher de données invalide car caractères non lisibles
TEST(TU3, d5) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_FALSE(database.loadData("data_bad1.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
}

//ficher de données invalide car renseigne un attribut non renseigné dans les métadonnées
TEST(TU3, d6) {
    Database database;
    ASSERT_TRUE(database.loadMetadata("metadata1.txt"));
    EXPECT_EQ(database.error(), Database::Error::OK);
    ASSERT_FALSE(database.loadData("data_bad3.txt"));
    EXPECT_EQ(database.error(), Database::Error::INVALID);
}

//chargement des données sans avoir charger les métadonnées
TEST(TU3, e) {
Database database;
    ASSERT_FALSE(database.loadData("data1.txt"));
    EXPECT_EQ(database.error(), Database::Error::NO_METADATA);
}

