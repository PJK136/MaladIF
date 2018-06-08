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
            case GET_DISEASES:
                getDiseases();
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
    std::cout<<"2. (L)ister les maladies répertoriées"<<std::endl;
    std::cout<<"3. (A)fficher les caractéristiques d'une maladie"<<std::endl;
    std::cout<<"4. (D)emander le diagnostic d'empreintes"<<std::endl;
    std::cout<<"5. (Q)uitter"<<std::endl;
    while(true)
    {
        std::cout << "Votre choix : " << std::flush;
        std::getline(std::cin, choice);
        std::cout << std::endl;
        if(strcmp_nocase(choice, "C") || choice == "1" )
        {
            return LOAD_DATABASE;
        }
        else if(strcmp_nocase(choice, "L") || choice == "2" )
        {
            return GET_DISEASES;
        }
        else if(strcmp_nocase(choice, "A") || choice == "3" )
        {
            return GET_DISEASE_CHARACTERISTICS;
        }
        else if(strcmp_nocase(choice, "D") || choice == "4")
        {
            return ASK_DIAGNOSIS;
        }
        else if(strcmp_nocase(choice, "Q") || choice == "5")
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

void CLI::getDiseases() const
{
    auto diseases = database.getDiseases();
    if (database.error())
    {
        printError(database.error());
        return;
    }

    std::cout << "Liste des maladies répertoriées : " << std::endl;
    for (auto &disease : diseases)
    {
        if (!disease.empty())
            std::cout << " - " << disease << std::endl;
    }
}

void CLI::getDiseaseCharacteristics() const
{
    if (!alreadyLoaded)
    {
        printError(Database::Error::NO_DATA);
        return;
    }

    std::string disease;
    std::cout << "Entrez le nom de la maladie : " << std::endl;
    std::getline(std::cin, disease);

    Fingerprint characs = database.getDiseaseCharacteristics(disease);
    if (database.error())
    {
        printError(database.error());
        return;
    }

    std::vector<Attribute> attributes(database.getAttributes());
    std::cout << "<";
    if (!attributes.empty())
    {
        std::cout << attributes[0].name;
        for (size_t i = 1; i < attributes.size(); i++)
            std::cout << " | " << attributes[i].name;
    }
    std::cout << ">" << std::endl;

    std::cout << "[" << characs << "]" << std::endl;
}

void CLI::askDiagnosis(std::string filename)
{
    if (!alreadyLoaded)
    {
        printError(Database::Error::NO_DATA);
        return;
    }

    if (filename.empty())
    {
        std::cout << "Entrez le nom du fichier d'empreintes : " << std::endl;
        std::getline(std::cin, filename);
    }

    if (database.startDiagnosis(filename))
    {
        auto result = database.nextDiagnosis();
        while (!database.error())
        {
            std::cout << std::endl;
            std::cout << "Empreinte [" << result.first << "] :\n";

            if (!result.second.empty())
            {
                std::cout << result.second[0];
                for (size_t i = 1; i < result.second.size() && result.second[i].risk >= 0.001; i++)
                    std::cout << "\n" << result.second[i];
            }

            std::cout << std::endl;

            result = database.nextDiagnosis();
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
            std::cerr << "La maladie n'est pas référencée." <<std::endl;
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
    //ctor
}

CLI::~CLI()
{
    //dtor
}
