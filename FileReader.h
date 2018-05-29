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
        FileReader();
        ~FileReader();

        bool open(const std::string & filename, const Metadata & metadata);
        std::pair<Fingerprint, std::string> nextFingerprint();

        static Metadata readMetadata(const std::string & filename);

        //void affiche();

    private:
        std::ifstream file;
        Metadata metadata;
        std::vector<int> associatedIndex; //association indices du fichier -> indices de la bdd
                                          //l'attribut disease est associe a la valeur -1
};

#endif // FILEREADER_H
