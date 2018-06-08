#ifndef CLI_H
#define CLI_H

#include "Database.h"

class CLI
{
    public:
        CLI();
        ~CLI();

        void execute();
        void loadDatabase(std::string filenameMetadata = "", std::string filenameData = "");
        void askDiagnosis(std::string filename = "") const;

    private:
        enum Choice {
            LOAD_DATABASE,
            GET_DISEASES,
            GET_DISEASE_CHARACTERISTICS,
            ASK_DIAGNOSIS,
            EXIT
        };

        Choice showMenu() const;
        void getDiseases() const;
        void getDiseaseCharacteristics() const;
        void printError(Database::Error error) const;

        Database database;
        bool alreadyLoaded;
};

#endif // CLI_H
