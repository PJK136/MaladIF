#ifndef FILEREADER_H
#define FILEREADER_H

#include "Metadata.h"
#include "Fingerprint.h"

#include <string>
#include <utility>
#include <fstream>

class FileReader
{
    public:
        enum Error {
            OK,
            CANT_OPEN,
            UNKNOWN_ATTRIBUTE,
            TOO_MANY_ATTRIBUTES,
            INVALID_VALUE,
            INVALID_TYPE
        };

        FileReader();
        ~FileReader();

        bool open(const std::string & filename, const Metadata & metadata);
        std::pair<Fingerprint, std::string> nextFingerprint();

        static Metadata readMetadata(const std::string & filename);
        static inline Error metadataError() { return metadataErr; }

        inline Error error() { return err; }

    private:
        Error err;
        std::ifstream file;
        Metadata metadata;
        std::vector<int> associatedIndex; //association indices du fichier -> indices de la bdd
                                          //l'attribut disease est associe a la valeur -1

        static Error metadataErr;
};

#endif // FILEREADER_H
