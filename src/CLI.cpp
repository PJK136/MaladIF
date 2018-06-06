#include "CLI.h"



void CLI::execute()
{


    while(true)
    {
        CLI::Choice choice = showMenu();
        if(choice == LOAD_DATABASE )
        {
            loadDatabase();

        }
        else if(choice == GET_DISEASE_CHARACTERISTICS)
        {
            getDiseaseCharacteristics();

        }
        else if(choice == ASK_DIAGNOSIS)
        {
            askDiagnosis();

        }
        else
        {
            return;
        }
    }

}



CLI::Choice CLI::showMenu() const
{
    std::string choice;
    std::cout<<"Bienvenue sur MaladIF"<<std::endl;
    std::cout<<"1-(c)harger la base de donnees"<<std::endl;
    std::cout<<"2-(a)fficher caracteristique maladie"<<std::endl;
    std::cout<<"3-(d)emander diagnostiques"<<std::endl;
    std::cout<<"4-(q)uitter"<<std::endl;
    while(true)
    {
        std::cin>>choice;
        if(choice == "c" || choice == "1" )
        {
            return LOAD_DATABASE;
        }
        else if(choice == "a" || choice == "2" )
        {
            return GET_DISEASE_CHARACTERISTICS;
        }
        else if(choice == "d" || choice == "3")
        {
            return ASK_DIAGNOSIS;
        }
        else if(choice == "q" || choice == "4")
        {
            return EXIT;
        }
        else
        {
            std::cout<<"Choix invalide"<<std::endl;
        }
    }

}

void CLI::loadDatabase()
{
    std::string filenameMetadata;
    std::cout<<"Entrez le nom du fichier de Metadonnees"<<std::endl;
    std::cin>>filenameMetadata;
    bool success = database.loadMetadata(filenameMetadata);
    if(!success)
    {
        std::cout<<"invalide, erreur : "<<database.error() << std::endl;
        return;
    }
    std::string filenameData;
    std::cout << "Entrez le nom du fichier des donnees" << std::endl;
    std::cin >> filenameData;
    success = database.loadData(filenameData);
    if (!success)
    {
        std::cout<<"invalide, erreur : "<<database.error() << std::endl;
        return;
    }
    database.displayDataBase();
}

void CLI::getDiseaseCharacteristics() const
{
    std::string disease;
    std::cout << "Entrez le nom de la maladie : " << std::endl;
    std::cin >> disease;
    std::list<Fingerprint> characs = database.getDiseaseCharacteristics(disease);
    if (database.error() == Database::Error::NOT_FOUND)
    {
        std::cout << "Maladie non trouvee : " << disease << std::endl;
    }
    else if (database.error() == Database::Error::NO_DATA)
    {
        std::cout << "Base de donnees non chargees" << std::endl;
    }
    else if (database.error())
    {
        std::cout << "Bravo ! Vous avez reussi a declencher une erreur qu'on a meme pas prevu !" << std::endl;
    }
    else
    {
        for (Fingerprint fp : characs)
        {
            std::cout << fp << std::endl;
        }
    }
}

void CLI::askDiagnosis() const
{
    std::string filename;
    std::cout << "Entrez le nom du fichier d'empreintes : " << std::endl;
    std::cin >> filename;
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> results = database.diagnose(filename);
    if (!database.error())
    {
        for (const auto & result : results)
        {
            std::cout << "--> [" << result.first << "] :" << std::endl << result.second << std::endl;
        }
    }
    else
    {
        std::cout << "Erreur : " << database.error() << std::endl;
    }
}

CLI::CLI() : database()
{
    //ctor
}

CLI::~CLI()
{
    //dtor
}
