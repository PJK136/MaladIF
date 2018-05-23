#include <iostream>

#include "CLI.h"
#include "CLI.h"
#include "FileReader.h"

int main() {
    Metadata m = FileReader::readMetadata("Metadata.txt");

    afficheMetadonnees(m);

    FileReader fr;
    fr.affiche();
    fr.open("Data.txt", m);
    fr.affiche();
    std::pair<Fingerprint, std::string> fi;
    fi = fr.nextFingerprint();
    while(!fi.first.values.empty()){
        fr.afficherFI(fi);
        fi = fr.nextFingerprint();
    }
    std::string vide;
    std::cin >> vide;
    return 0;
}
