#ifndef CLI_H
#define CLI_H

#include "Database.h"

class CLI
{
    public:
        CLI();
        ~CLI();

        void execute();

    private:
        enum Choice {
            LOAD_DATABASE,
            GET_DISEASE_CHARACTERISTICS,
            ASK_DIAGNOSIS,
            EXIT
        };

        Choice showMenu() const;
        void loadDatabase();
        void getDiseaseCharacteristics() const;
        void askDiagnosis() const;

        Database database;
};

#endif // CLI_H
