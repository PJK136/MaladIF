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
        std::cout << std::endl;
    }

}



CLI::Choice CLI::showMenu() const
{
    std::string choice;
    std::cout << "Bienvenue sur MaladIF"<<std::endl;
#ifndef NDEBUG
    std::cout << "(VERSION DEBUG)" << std::endl;
#endif // NDEBUG
    std::cout<<"1. (c)harger la base de données"<<std::endl;
    std::cout<<"2. (a)fficher caractéristique maladie"<<std::endl;
    std::cout<<"3. (d)emander diagnostiques"<<std::endl;
    std::cout<<"4. (q)uitter"<<std::endl;
    while(true)
    {
        std::getline(std::cin, choice);
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
            std::cout<<"Choix invalide."<<std::endl;
        }
    }

}

void CLI::loadDatabase()
{
    bool success;
    if (!alreadyLoaded) //des métadonnées sont déjà en mémoire
    {
        std::string filenameMetadata;
        std::cout<<"Entrez le nom du fichier de métadonnees :"<<std::endl;
        std::getline(std::cin, filenameMetadata);
        success = database.loadMetadata(filenameMetadata);
        if(!success)
        {
            printError(database.error());
            return;
        }
    }
    std::string filenameData;
    std::cout << "Entrez le nom du fichier des données :" << std::endl;
    std::getline(std::cin, filenameData);
    success = database.loadData(filenameData);
    if (!success)
    {
        printError(database.error());
        return;
    }
    alreadyLoaded = true;
}

void CLI::getDiseaseCharacteristics() const
{
    std::string disease;
    std::cout << "Entrez le nom de la maladie : " << std::endl;
    std::getline(std::cin, disease);
    std::list<Fingerprint> characs = database.getDiseaseCharacteristics(disease);
    if (database.error())
    {
        printError(database.error());
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
    std::getline(std::cin, filename);
    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> results = database.diagnose(filename);
    if (!database.error())
    {
        for (const auto & result : results)
        {
            std::cout << "Enpreinte [" << result.first << "] :" << std::endl << result.second << std::endl;
        }
    }
    else
    {
        printError(database.error());
    }
}

void CLI::printError(Database::Error error) const
{
    switch (error)
    {
    case Database::Error::EMPTY_FILE:
        std::cerr << "Le fichier est vide." << std::endl;
        break;
    case Database::Error::CANT_OPEN:
        std::cerr << "Impossible d'ouvrir le fichier." << std::endl;
        break;
    case Database::Error::NO_METADATA:
        std::cerr << "Les métadonées n'ont pas été chargées." << std::endl;
        break;
    case Database::Error::INVALID:
        std::cerr << "Le fichier est invalide." << std::endl;
        break;
    case Database::Error::NOT_FOUND:
        std::cerr << "La maladie n'est pas referencée." <<std::endl;
        break;
    case Database::Error::NO_DATA:
        std::cerr << "Les données n'ont pas été chargées." << std::endl;
        break;
    default:
        std::cerr << "Erreur inconnue (code d'erreur : " << database.error() << ")." << std::endl;
        break;
    }
}

CLI::CLI() : database(), alreadyLoaded(false)
{
    //ctor
}

CLI::~CLI()
{
    //dtor
}
