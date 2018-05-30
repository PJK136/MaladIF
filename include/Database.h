#ifndef DATABASE_H
#define DATABASE_H

#include "Fingerprint.h"
#include "Diagnosis.h"
#include "Metadata.h"



#include <vector>
#include <list>
#include <utility>
#include <string>
#include <list>
#include <unordered_map>

class Database
{
    public:
        Database();
        ~Database();

        bool loadMetadata(const std::string & filename);
        bool loadData(const std::string & filename);
        std::list<Fingerprint> getDiseaseCharacteristics(const std::string & disease) const;
        std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnose(const std::string & filename) const;
        void displayDataBase();

    private:
        void addFingerprint(const Fingerprint & fingerprint, const std::string & disease);
        std::vector<Diagnosis> diagnose(const Fingerprint & fingerprint) const;
        double fingerprintMatch(const Fingerprint & fp1, const Fingerprint & fp2) const;

        std::unordered_map<std::string, std::list<Fingerprint>> data;
        Metadata metadata;
        Fingerprint fingerprintMax;
        Fingerprint fingerprintMin;
        Fingerprint fingerprintEtendue;
};

#endif // DATABASE_H
