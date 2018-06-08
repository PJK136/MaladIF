#include "CLI.h"
#include "CLI_utils.h"

void CLI::execute()
{
    while(true)
    {
        CLI::Choice choice = showMenu();
        switch (choice)
        {
            case LOAD_DATABASE:
                loadDatabase();
                break;
            case GET_DISEASE_CHARACTERISTICS:
                getDiseaseCharacteristics();
                break;
            case ASK_DIAGNOSIS:
                askDiagnosis();
                break;
            case EXIT:
                return;
            default:
                std::cout << "Ce choix n'existe pas." << std::endl;
                break;
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
    std::cout<<"1. (C)harger la base de données"<<std::endl;
    std::cout<<"2. (A)fficher caractéristique maladie"<<std::endl;
    std::cout<<"3. (D)emander diagnostic d'empreintes"<<std::endl;
    std::cout<<"4. (Q)uitter"<<std::endl;
    while(true)
    {
        std::cout << "Votre choix : " << std::flush;
        std::getline(std::cin, choice);
        std::cout << std::endl;
        if(strcmp_nocase(choice, "C") || choice == "1" )
        {
            return LOAD_DATABASE;
        }
        else if(strcmp_nocase(choice, "A") || choice == "2" )
        {
            return GET_DISEASE_CHARACTERISTICS;
        }
        else if(strcmp_nocase(choice, "D") || choice == "3")
        {
            return ASK_DIAGNOSIS;
        }
        else if(strcmp_nocase(choice, "Q") || choice == "4")
        {
            return EXIT;
        }
        else
        {
            std::cout<<"Choix invalide."<<std::endl;
        }
    }

}

void CLI::loadDatabase(std::string filenameMetadata, std::string filenameData)
{
    bool success;
    if (!alreadyLoaded) //des métadonnées sont déjà en mémoire
    {
        if (filenameMetadata.empty())
        {
            std::cout << "Entrez le nom du fichier de métadonnees :" << std::endl;
            std::getline(std::cin, filenameMetadata);
        }

        success = database.loadMetadata(filenameMetadata);
        if (!success) {
            printError(database.error());
            return;
        }
    }

    if (filenameData.empty())
    {
        std::cout << "Entrez le nom du fichier des données :" << std::endl;
        std::getline(std::cin, filenameData);
    }

    success = database.loadData(filenameData);
    if (!success)
    {
        printError(database.error());
        return;
    }
    alreadyLoaded = true;

    std::cout << "Base de données chargée !" << std::endl;
}

void CLI::getDiseaseCharacteristics() const
{
    std::string disease;
    std::cout << "Entrez le nom de la maladie : " << std::endl;
    std::getline(std::cin, disease);
    Fingerprint characs = database.getDiseaseCharacteristics(disease);
    std::cout << "[" << characs << "]" << std::endl;
}

void CLI::askDiagnosis(std::string filename) const
{
    if (filename.empty())
    {
        std::cout << "Entrez le nom du fichier d'empreintes : " << std::endl;
        std::getline(std::cin, filename);
    }

    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> results = database.diagnose(filename);
    if (!database.error())
    {
        for (const auto & result : results)
        {
            std::cout << "Empreinte [" << result.first << "] :" << std::endl << result.second << std::endl;
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
            std::cerr << "Les métadonnées n'ont pas été chargées." << std::endl;
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
            std::cerr << "Erreur inconnue (code d'erreur : " << error << ")." << std::endl;
            break;
    }
}

CLI::CLI() : database(), alreadyLoaded(false)
{

}

CLI::~CLI()
{
    //dtor
}
