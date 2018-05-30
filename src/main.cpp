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
    bool succ = database.loadData("data1.txt");
    std::cout<<"apres charg"<<std::endl;
    std::cout<<succ<<std::endl;
    database.displayDataBase();

    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> results = database.diagnose("data2.txt");
    for (const auto & result : results)
    {
        std::cout << "--> [" << result.first << "] :" << std::endl << result.second << std::endl;
    }

//    Fingerprint fp;
//    fp.values = {1, std::string("True"), 2.12, 13.0, 3.156, 1236, true};
//    std::cout << "match : " << database.fingerprintMatch(fp, fp) << std::endl;
//    std::vector<Diagnosis> diag = database.diagnose(fp);
//    std::cout << diag << std::endl;

    return 0;
}

