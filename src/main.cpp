#include <iostream>

#include "CLI.h"
#include "CLI.h"
#include "Database.h"

int main() {
    /*
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
    std::cin >> vide;*/

    Database database;
    bool success = database.loadMetadata("metadata1.txt");
    std::cout<<"avant charg"<<std::endl;
    database.displayDataBase();
    std::cout<<success<<std::endl;
    bool succ = database.loadData("Data.txt");
    std::cout<<"apres charg"<<std::endl;
    std::cout<<succ<<std::endl;
    database.displayDataBase();
    return 0;
}

