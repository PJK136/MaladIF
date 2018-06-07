#include "Database.h"
#include "FileReader.h"
#include "CLI.h"
#include <numeric>
#include <algorithm>

#ifndef NDEBUG
    #include <iostream>
#endif

constexpr double EPSILON = 0.1;

bool Database::loadMetadata(const std::string & filename)
{
    err = Database::Error::OK;

    Metadata metadataTemp = FileReader::readMetadata(filename);

    if(!FileReader::metadataError())
    {
        metadata = metadataTemp;
        fingerprintMax.values.resize(metadata.attributes.size());
        fingerprintMin.values.resize(metadata.attributes.size());
        fingerprintEtendue.values.resize(metadata.attributes.size());
        for(size_t i=0; i < metadata.attributes.size(); ++i)
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
        setError(FileReader::metadataError());
        return false;
    }
}

bool Database::loadData(const std::string & filename)
{
    err = Database::Error::OK;

    FileReader fileReader;
    bool success = fileReader.open(filename,metadata);

    if(!success)
    {
        setError(fileReader.error());
        return false;
    }

    std::pair <Fingerprint, std::string >loadedData(fileReader.nextFingerprint());

    if(fileReader.error())
    {
        setError(fileReader.error());
        return false;
    }

    while(!loadedData.first.values.empty())
    {
        addFingerprint(loadedData.first,loadedData.second);
        loadedData = fileReader.nextFingerprint();
        if (fileReader.error() != FileReader::Error::OK && fileReader.error() != FileReader::Error::EMPTY)
        {
            setError(fileReader.error());
            return false;
        }
    }

    return true;
}

void Database::addFingerprint(const Fingerprint & fingerprint, const std::string & disease)
{
    err = Database::Error::OK;

    data[disease].push_back(fingerprint);
    for(size_t i = 0; i < fingerprint.values.size(); i++)
    {
        if (metadata.attributes[i].type != STRING && !std::holds_alternative<std::monostate>(fingerprint.values[i]))
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
    err = Database::Error::OK;

    if (data.empty())
    {
        err = Database::Error::NO_DATA;
        return std::list<Fingerprint>{};
    }

    auto it = data.find(disease);
    if (it != data.end())
    {
        err = Database::Error::OK;
        return it->second;
    }

    err = Database::Error::NOT_FOUND;
    return std::list<Fingerprint>{};
}

std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> Database::diagnose(const std::string & filename) const
{
    err = Database::Error::OK;

    std::list<std::pair<Fingerprint,std::vector<Diagnosis>>> result;

    if (data.empty())
    {
        err = Database::Error::NO_DATA;
        return result;
    }

    FileReader fileReader;
    bool success = fileReader.open(filename, metadata);

    if(!success)
    {
        setError(fileReader.error());
        return result;
    }

    std::pair<Fingerprint, std::string> loadedData(fileReader.nextFingerprint());

    if(fileReader.error())
    {
        setError(fileReader.error());
        return result;
    }

    while(!loadedData.first.values.empty())
    {
        std::vector<Diagnosis> diagList = diagnose(loadedData.first);
        if (err)
        {
            return result;
        }
        result.emplace_back(loadedData.first, diagList);
        loadedData = fileReader.nextFingerprint();

        if (fileReader.error() != FileReader::Error::OK && fileReader.error() != FileReader::Error::EMPTY)
        {
            setError(fileReader.error());
            return result;
        }
    }

    return result;
}

std::vector<Diagnosis> Database::diagnose(const Fingerprint & fingerprint) const
{
    err = Database::Error::OK;

    #ifndef NDEBUG
    std::cerr << "diagnose : " << fingerprint << std::endl;
    #endif // NDEBUG
    std::vector<Diagnosis> diagnosisList;
    for (const auto & kv : data)
    {
        std::vector<double> values;
        for (const auto & fingerprintDB : kv.second)
        {
            #ifndef NDEBUG
            std::cerr << "on match avec : " << fingerprintDB << std::endl;
            #endif // NDEBUG
            double matchingValue = fingerprintMatch(fingerprint, fingerprintDB);
            #ifndef NDEBUG
            std::cerr << "fini" << std::endl;
            #endif // NDEBUG
            if (!err)
                values.push_back(matchingValue);
        }

        double sum = std::accumulate(values.begin(), values.end(), 0.);

        Diagnosis d;
        d.disease = kv.first;
        d.risk = sum / values.size();
        diagnosisList.push_back(d);
    }

    std::sort(diagnosisList.begin(), diagnosisList.end(),
    [](const Diagnosis &a, const Diagnosis &b) {
        return a.risk > b.risk;
    });

    err = Database::Error::OK;
    return diagnosisList;
}

double Database::fingerprintMatch(const Fingerprint & fp1, const Fingerprint & fp2) const
{
    err = Database::Error::OK;

    Fingerprint diff = fp1 - fp2;
    #ifndef NDEBUG
    std::cerr << "le diff a fonctionne" << std::endl;
    #endif // NDEBUG
    if (diff.values.empty())
    {
        #ifndef NDEBUG
        std::cerr << fp1 << std::endl << fp2 << std::endl;
        err = Database::Error::INVALID;
        #endif // NDEBUG
        return 0.;
    }
    double sum = 0.;
    size_t size = diff.values.size();
    for (size_t i = 0; i < diff.values.size(); ++i)
    {
        if (diff.values[i].index() != 0 && metadata.attributes[i].type != ID)
        {
            if (metadata.attributes[i].type != BOOLEAN)
            {
                double val = std::abs((diff.values[i].index() == 3) ? std::get<double>(diff.values[i]) : std::get<int>(diff.values[i]));
                #ifndef NDEBUG
                std::cerr << "fonctionne pour diff" << std::endl;
                #endif // NDEBUG
                double etendue = (fingerprintEtendue.values[i].index() == 3) ? std::get<double>(fingerprintEtendue.values[i]) : std::get<int>(fingerprintEtendue.values[i]);
                #ifndef NDEBUG
                std::cerr << "fonctionne pour etendue" << std::endl;
                #endif // NDEBUG
                sum += std::max(0., 1-((val / etendue)/EPSILON));
            }
            else
            {
                if (!std::get<int>(diff.values[i]))
                    sum += 1.;
                else
                    sum += 0.5;
            }
        }
        else
        {
            size -= 1;
        }
    }

    if (size)
        return (sum / size);
    else {
        err = Database::Error::INVALID;
        return 0;
    }
}

void Database::setError(FileReader::Error frErr) const
{
    switch (frErr)
    {
    case FileReader::OK :
        err = Database::Error::OK;
        break;
    case FileReader::CANT_OPEN :
        err = Database::Error::CANT_OPEN;
        break;
    case FileReader::EMPTY :
        err = Database::Error::EMPTY_FILE;
        break;
    case FileReader::NO_METADATA :
        err = Database::Error::NO_METADATA;
        break;
    default :
        err = Database::Error::INVALID;
        break;
    }
}

Database::Database() : err(Database::Error::OK), data(), metadata(), fingerprintMax(), fingerprintMin(), fingerprintEtendue()
{
    //ctor
}

Database::~Database()
{
    //dtor
}
