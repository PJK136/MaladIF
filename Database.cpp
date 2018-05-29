#include "Database.h"
#include "FileReader.h"
#include "CLI.h"
#include <iostream>

bool Database::loadMetadata(const std::string & filename)
{
    Metadata metadataTemp = FileReader::readMetadata(filename);
    if(!metadataTemp.attributes.empty())
    {
        metadata = metadataTemp;
        return true;
    }
    else
    {
        return false;
    }

}


bool Database::loadData(const std::string & filename)
{
    FileReader fileReader;
    bool success = fileReader.open(filename,metadata);

    if(!success)
    {
        std::cerr << "Erreur lors de l'ouverture du fichier" << std::endl;
        return false;
    }

    std::pair <Fingerprint, std::string >loadedData(fileReader.nextFingerprint());


    if(loadedData.first.values.empty())
    {
        std::cerr << "Fichier vide" << std::endl;
        std::cerr << "Taille de metadata" << metadata.attributes.size()<< std::endl;
        return false;
    }

    while(!loadedData.first.values.empty())
    {
        addFingerprint(loadedData.first,loadedData.second);
        loadedData = fileReader.nextFingerprint();
    }

    return true;
}

void Database::addFingerprint(const Fingerprint & fingerprint, const std::string & disease)
{
    data[disease].push_back(fingerprint);
}

//TOOD : À déplacer
void Database::displayDataBase()
{

    for (const auto & entry : data)
    {
        std::cout<< entry.first << " : ";
        for(const auto & fingerprint : entry.second)
        {
            std::cout<<" [";
            for(const auto & value : fingerprint.values)
            {
                std::cout << " " << value;
            }
            std::cout<<" ] ";
        }
        std::cout<<std::endl;
    }

    std::cout << std::endl;
}


std::list<Fingerprint> Database::getDiseaseCharacteristics(const std::string & disease)
{
    auto it = data.find(disease);
    return it != data.end() ? it->second : std::list<Fingerprint>{};
}

std::vector<Diagnosis> Database::diagnose(const Fingerprint & fingerprint)
{
    return {};
}


Database::Database() : data(), metadata()
{
    //ctor
}

Database::~Database()
{
    //dtor
}
