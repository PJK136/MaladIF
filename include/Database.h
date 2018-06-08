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
        enum Error
        {
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
        std::vector<std::string> getDiseases() const;
        std::vector<Attribute> getAttributes() const;
        Fingerprint getDiseaseCharacteristics(const std::string & disease) const;
        bool startDiagnosis(const std::string & filename);
        std::pair<Fingerprint,std::vector<Diagnosis>> nextDiagnosis();

#ifndef NDEBUG
        void displayDatabase();
#endif

        inline Error error() const { return err; }

    private:
        //key = attributeName -> value = { value -> count }
        typedef std::unordered_map<std::string, std::unordered_map<std::string, size_t>> StringValues;

        struct MeanFingerprintBuilder
        {
            Fingerprint sum{};
            size_t fingerprintCount{0};
            StringValues stringValues{};
        };
        void addFingerprint(const Fingerprint & fingerprint, const std::string & disease);
        std::vector<Diagnosis> diagnose(const Fingerprint & fingerprint) const;
        double fingerprintMatch(const Fingerprint & fp1, const Fingerprint & fp2) const;
        void setError(FileReader::Error frErr) const;

        mutable Error err;
        //key = disease name
        std::unordered_map<std::string, std::list<Fingerprint>> data;
        std::unordered_map<std::string, MeanFingerprintBuilder> meanDataBuilder;
        std::unordered_map<std::string, Fingerprint> meanData;
        Metadata metadata;
        Fingerprint fingerprintMax;
        Fingerprint fingerprintMin;
        Fingerprint fingerprintEtendue;

        FileReader diagnoseFile;
};

#endif // DATABASE_H
