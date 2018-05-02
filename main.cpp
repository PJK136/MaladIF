#include <iostream>

//#include "CLI.h"
#include "FileReader.h"

int main() {
    Metadata m = FileReader::readMetadata("Metadata.txt");

    afficheMetadonnees(m);

    FileReader fr;
    fr.affiche();
    fr.open("Data.txt", m);
    fr.affiche();
    std::string vide;
    std::cin >> vide;
    return 0;
}
