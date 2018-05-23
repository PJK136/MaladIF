#ifndef DATABASE_H
#define DATABASE_H

#include "Fingerprint.h"
#include "Diagnosis.h"
#include "Metadata.h"

#include <vector>
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
        std::list<Fingerprint> getDiseaseCharacteristics(const std::string & disease);
        std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnose(const std::string & filename);

    private:
        void addFingerprint(Fingerprint fingerprint, const std::string & disease);
        std::vector<Diagnosis> diagnose(Fingerprint fingerprint);

        std::unordered_map<std::string, std::list<Fingerprint>> data;
        Metadata metadata;
};

#endif // DATABASE_H