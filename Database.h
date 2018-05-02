#ifndef DATABASE_H
#define DATABASE_H
#include "FingerPrint.h"

class Database
{
    public:
        Database();
        bool loadMetadata(std::string filename);
        bool loadData(std::string filename);
        std::list<FingerPrint> getDiseaseCharacteristics(std::string disease);
        std::list<std::pair<FingerPrint,std::vector<Diagnosis>>> diagnose(std::string filename);
    private:
        void addFingerPrint(FingerPrint fingerprint,std::string disease);
        std::vector<Diagnosis> diagnose(FingerPrint fingerprint);
};

#endif // DATABASE_H
