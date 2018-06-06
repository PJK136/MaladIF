#ifndef DATABASE_H
#define DATABASE_H

#include "Fingerprint.h"
#include "Diagnosis.h"
#include "Metadata.h"
#include "FileReader.h"

#include <vector>
#include <list>
#include <utility>
#include <string>
#include <list>
#include <unordered_map>

class Database
{
    public:
        enum Error {
            OK,
            EMPTY_FILE,
            CANT_OPEN,
            NO_METADATA,
            INVALID,
            NOT_FOUND,
            NO_DATA
        };
        Database();
        ~Database();

        bool loadMetadata(const std::string & filename);
        bool loadData(const std::string & filename);
        std::list<Fingerprint> getDiseaseCharacteristics(const std::string & disease) const;
        std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> diagnose(const std::string & filename) const;
        void displayDataBase();

        inline Error error() const { return err; }

    private:
        void addFingerprint(const Fingerprint & fingerprint, const std::string & disease);
        std::vector<Diagnosis> diagnose(const Fingerprint & fingerprint) const;
        double fingerprintMatch(const Fingerprint & fp1, const Fingerprint & fp2) const;
        void setError(FileReader::Error frErr) const;

        mutable Error err;
        std::unordered_map<std::string, std::list<Fingerprint>> data;
        Metadata metadata;
        Fingerprint fingerprintMax;
        Fingerprint fingerprintMin;
        Fingerprint fingerprintEtendue;
};

#endif // DATABASE_H
