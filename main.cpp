#include <iostream>

#include "CLI.h"
#include "CLI.h"
#include "FileReader.h"

int main() {
    Metadata m = FileReader::readMetadata("metadata3.txt");

    afficheMetadonnees(m);

    FileReader fr;
    fr.affiche();
    std::cout << fr.open("data2.txt", m) << std::endl;
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
