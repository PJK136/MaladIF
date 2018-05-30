#include "Database.h"
#include "FileReader.h"
#include "CLI.h"
#include <iostream>

constexpr double EPSILON = 0.5;

bool Database::loadMetadata(const std::string & filename)
{
    Metadata metadataTemp = FileReader::readMetadata(filename);
    if(!metadataTemp.attributes.empty())
    {
        metadata = metadataTemp;
        fingerprintMax.values.resize(metadata.attributes.size());
        fingerprintMin.values.resize(metadata.attributes.size());
        fingerprintEtendue.values.resize(metadata.attributes.size());
        for(unsigned int i=0;i < metadata.attributes.size() ;++i)
        {
            if(metadata.attributes[i].type == STRING)
            {
                fingerprintMax.values[i] = 1;
                fingerprintMin.values[i] = 0;
                fingerprintEtendue.values[i] = 1;
            }
        }
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
    for(unsigned int i = 0; i < fingerprint.values.size(); i++)
    {
        if (metadata.attributes[i].type != STRING)
        {
            if (std::holds_alternative<std::monostate>(fingerprintMax.values[i]) || fingerprint.values[i] > fingerprintMax.values[i])
            {
                fingerprintMax.values[i] = fingerprint.values[i];
                fingerprintEtendue.values[i] = fingerprintMax.values[i] - fingerprintMin.values[i];
            }
            if (std::holds_alternative<std::monostate>(fingerprintMin.values[i]) || fingerprint.values[i] < fingerprintMin.values[i])
            {
                fingerprintMin.values[i] = fingerprint.values[i];
                fingerprintEtendue.values[i] = fingerprintMax.values[i] - fingerprintMin.values[i];
            }
        }
    }
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

    std::cout<<"Min [";
    for(const auto & value : fingerprintMin.values)
    {
        std::cout << " " << value;
    }
    std::cout<<" ] ";

    std::cout << std::endl;

    std::cout<<"Max [";
    for(const auto & value : fingerprintMax.values)
    {
        std::cout << " " << value;
    }
    std::cout<<" ] ";

    std::cout << std::endl;

    std::cout<<"Etendue [";
    for(const auto & value : fingerprintEtendue.values)
    {
        std::cout << " " << value;
    }
    std::cout<<" ] ";

    std::cout << std::endl;
}

std::list<Fingerprint> Database::getDiseaseCharacteristics(const std::string & disease) const
{
    auto it = data.find(disease);
    return it != data.end() ? it->second : std::list<Fingerprint>{};
}

std::vector<Diagnosis> Database::diagnose(const Fingerprint & fingerprint) const
{
    std::vector<Diagnosis> diagnosisList;
    for (const auto & kv : data)
    {
        std::vector<double> values;
        for (const auto & fingerprintDB : kv.second)
        {
            values.push_back(fingerprintMatch(fingerprint, fingerprintDB));
        }
        double sum = 0.;
        for (double value : values)
        {
            sum += value;
        }
        Diagnosis d;
        d.disease = kv.first;
        d.risk = sum / values.size();
        diagnosisList.push_back(d);
    }
    return diagnosisList;
}

double Database::fingerprintMatch(const Fingerprint & fp1, const Fingerprint & fp2) const
{
    Fingerprint diff = fp1 - fp2;
    double sum = 0.;
    size_t size = diff.values.size();
    for (size_t i = 0; i < diff.values.size(); ++i)
    {
        if (diff.values[i].index() != 0 && metadata.attributes[i].type != ID)
        {
            double val = std::abs((diff.values[i].index() == 3) ? std::get<double>(diff.values[i]) : (double) std::get<int>(diff.values[i]));
            double etendue = (fingerprintEtendue.values[i].index() == 3) ? std::get<double>(fingerprintEtendue.values[i]) : (double) std::get<int>(fingerprintEtendue.values[i]);
            if ((val / etendue) < EPSILON)
            {
                sum += 1.;
            }
        }
        else
        {
            size -= 1;
        }
    }
    return (sum / size);
}

Database::Database() : data(), metadata(), fingerprintMax(), fingerprintMin(), fingerprintEtendue()
{
    //ctor
}

Database::~Database()
{
    //dtor
}
