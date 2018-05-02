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

    private:
        std::ifstream file;
};

#endif // FILEREADER_H
