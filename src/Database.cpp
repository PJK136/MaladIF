#include "Database.h"
#include "FileReader.h"
#include "CLI.h"
#include <numeric>
#include <algorithm>
#include <cmath>

#ifndef NDEBUG
    #include <CLI_utils.h>
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
                fingerprintMax.values[i] = 1.;
                fingerprintMin.values[i] = 0.;
                fingerprintEtendue.values[i] = 1.;
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

    std::pair<Fingerprint, std::string >loadedData(fileReader.nextFingerprint());

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

#ifndef NDEBUG
    std::cerr << "---- MEAN DATA BUILDER ----" << std::endl;
    for (const auto & diseaseAndBuilder : meanDataBuilder)
    {
        std::cerr << "DISEASE : " << diseaseAndBuilder.first << std::endl;
        std::cerr << "\tSUM : " << diseaseAndBuilder.second.sum << std::endl;
        std::cerr << "\tCOUNTS : " << diseaseAndBuilder.second.counts << std::endl;
        std::cerr << "\tSTRINGS :" << std::endl;
        for (const auto & attributeAndStrings : diseaseAndBuilder.second.stringValues)
        {
            std::cerr << "\t[" << attributeAndStrings.first << "]" << std::endl;
            for (const auto & stringAndCount : attributeAndStrings.second)
            {
                std::cerr << "\t\t" << stringAndCount.first << " -> " << stringAndCount.second << std::endl;
            }
        }
    }

#endif // NDEBUG


    //construction etendu
    fingerprintEtendue.values.resize(metadata.attributes.size());
    for (size_t i = 0; i < fingerprintEtendue.values.size(); i++)
    {
        auto diff = fingerprintMax.values[i] - fingerprintMin.values[i];
        if (std::holds_alternative<int>(diff))
            fingerprintEtendue.values[i] = (double) std::get<int>(diff);
        else if (std::holds_alternative<double>(diff))
            fingerprintEtendue.values[i] = std::get<double>(diff);
        else
            fingerprintEtendue.values[i] = std::monostate();
    }

    //construction meanData & stdDeviation
    for (const auto & diseaseAndBuilder : meanDataBuilder)
    {
        meanData[diseaseAndBuilder.first].values.resize(metadata.attributes.size());
        stdDeviation[diseaseAndBuilder.first].resize(metadata.attributes.size(), 0.);
        for (size_t i = 0; i < metadata.attributes.size(); ++i)
        {
            if (metadata.attributes[i].type == ID)
            {
                meanData[diseaseAndBuilder.first].values[i] = std::monostate();
            }
            else if (metadata.attributes[i].type == STRING)
            {
                auto &stringValues = diseaseAndBuilder.second.stringValues.at(metadata.attributes[i].name);
                meanData[diseaseAndBuilder.first].values[i] = std::max_element(stringValues.begin(), stringValues.end(),
                                                                               [](const std::pair<std::string,size_t> &p1, const std::pair<std::string,size_t> &p2) {
                                                                                   return p1.second < p2.second;
                                                                               })->first;
            }
            else
            {
                const FingerprintValue & sumValue = diseaseAndBuilder.second.sum.values[i];
                if (!std::holds_alternative<std::monostate>(sumValue))
                {
                    double sum = 0;
                    if (std::holds_alternative<int>(sumValue))
                        sum = std::get<int>(sumValue);
                    else if (std::holds_alternative<double>(sumValue))
                        sum = std::get<double>(sumValue);
                    meanData[diseaseAndBuilder.first].values[i] = sum / diseaseAndBuilder.second.counts[i];
                }
            }
        }

        //calcul ecart type ------------
        for (const Fingerprint & fingerprint : data[diseaseAndBuilder.first])
        {
            for (size_t i = 0; i < metadata.attributes.size(); ++i)
            {
                if (metadata.attributes[i].type != ID && metadata.attributes[i].type != STRING && !std::holds_alternative<std::monostate>(fingerprint.values[i]))
                {
                    double value;
                    if (metadata.attributes[i].type == BOOLEAN)
                        value = std::get<bool>(fingerprint.values[i]);
                    else if (metadata.attributes[i].type == INT)
                        value = std::get<int>(fingerprint.values[i]);
                    else //double
                        value = std::get<double>(fingerprint.values[i]);
                    double mean = std::get<double>(meanData[diseaseAndBuilder.first].values[i]);
                    stdDeviation[diseaseAndBuilder.first][i] += (value - mean) * (value - mean);
                }
            }
        }

        for (size_t i = 0; i < metadata.attributes.size(); ++i)
        {
             stdDeviation[diseaseAndBuilder.first][i] = std::sqrt(stdDeviation[diseaseAndBuilder.first][i] / diseaseAndBuilder.second.counts[i]);
        }
    }


#ifndef NDEBUG
    std::cerr << "---- MEAN DATA ----" << std::endl;
    for (const auto & diseaseAndMeanFp : meanData)
    {
        std::cerr << "[" << diseaseAndMeanFp.first << "] -> " << diseaseAndMeanFp.second << std::endl;
        std::cerr << std::string(diseaseAndMeanFp.first.size() + 6, ' ') << stdDeviation[diseaseAndMeanFp.first] << std::endl;
    }
#endif // NDEBUG

    return true;
}

void Database::addFingerprint(const Fingerprint & fingerprint, const std::string & disease)
{
    err = Database::Error::OK;

    data[disease].push_back(fingerprint);
    if (meanDataBuilder.find(disease) == meanDataBuilder.end()) //init si maladie non existante
    {
        meanDataBuilder[disease].sum.values.resize(fingerprint.values.size());
        meanDataBuilder[disease].counts.resize(fingerprint.values.size(), 0);
    }
    for(size_t i = 0; i < fingerprint.values.size(); i++)
    {
        if (std::holds_alternative<std::monostate>(fingerprint.values[i]))
            continue;

        if (metadata.attributes[i].type != STRING)
        {
            if (std::holds_alternative<std::monostate>(fingerprintMax.values[i]) || fingerprint.values[i] > fingerprintMax.values[i])
                fingerprintMax.values[i] = fingerprint.values[i];

            if (std::holds_alternative<std::monostate>(fingerprintMin.values[i]) || fingerprint.values[i] < fingerprintMin.values[i])
                fingerprintMin.values[i] = fingerprint.values[i];

            //meanDataBuilder
            if (std::holds_alternative<std::monostate>(meanDataBuilder[disease].sum.values[i])) //sum non initialise
                meanDataBuilder[disease].sum.values[i] = (std::holds_alternative<bool>(fingerprint.values[i])) ? (int) std::get<bool>(fingerprint.values[i]) : fingerprint.values[i];
            else //on ajoute
            {
                if (std::holds_alternative<bool>(fingerprint.values[i]))
                    meanDataBuilder[disease].sum.values[i] = std::get<int>(meanDataBuilder[disease].sum.values[i]) + std::get<bool>(fingerprint.values[i]);
                else if (std::holds_alternative<int>(fingerprint.values[i]))
                    meanDataBuilder[disease].sum.values[i] = std::get<int>(meanDataBuilder[disease].sum.values[i]) + std::get<int>(fingerprint.values[i]);
                else //contient <double>
                    meanDataBuilder[disease].sum.values[i] = std::get<double>(meanDataBuilder[disease].sum.values[i]) + std::get<double>(fingerprint.values[i]);
            }
        }
        else
        {
            meanDataBuilder[disease].stringValues[metadata.attributes[i].name][std::get<std::string>(fingerprint.values[i])] += 1;
        }

        meanDataBuilder[disease].counts[i] += 1;
    }
}

#ifndef NDEBUG
void Database::displayDatabase()
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
#endif

std::vector<std::string> Database::getDiseases() const
{
    err = Database::Error::OK;

    if (data.empty())
    {
        err = Database::Error::NO_DATA;
        return {};
    }

    std::vector<std::string> diseases;
    diseases.reserve(data.size());
    for (auto &d : data)
        diseases.push_back(d.first);

    std::sort(diseases.begin(), diseases.end());
    return diseases;
}

std::vector<Attribute> Database::getAttributes() const {
    return metadata.attributes;
}

Fingerprint Database::getDiseaseCharacteristics(const std::string & disease) const
{
    err = Database::Error::OK;

    if (data.empty())
    {
        err = Database::Error::NO_DATA;
        return {};
    }

    auto it = meanData.find(disease);
    if (it != meanData.end())
    {
        err = Database::Error::OK;
        return it->second;
    }

    err = Database::Error::NOT_FOUND;
    return {};
}

bool Database::startDiagnosis(const std::string & filename)
{
    err = Database::Error::OK;

    if (data.empty())
    {
        err = Database::Error::NO_DATA;
        return false;
    }

    diagnoseFile.close();
    bool success = diagnoseFile.open(filename, metadata);

    if(!success)
    {
        setError(diagnoseFile.error());
        return false;
    }

    return true;
}

std::pair<Fingerprint,std::vector<Diagnosis>> Database::nextDiagnosis()
{
    err = Database::Error::OK;

    std::pair<Fingerprint, std::string> loadedData(diagnoseFile.nextFingerprint());

    if (diagnoseFile.error())
    {
        setError(diagnoseFile.error());
        return {};
    }

    std::vector<Diagnosis> diagnosis(diagnose(loadedData.first));
    if (err)
    {
        return {};
    }

    return {loadedData.first, diagnosis};
}

std::vector<Diagnosis> Database::diagnose(const Fingerprint & fingerprint) const
{
    err = Database::Error::OK;

    #ifndef NDEBUG
    std::cerr << "diagnose : " << fingerprint << std::endl;
    #endif // NDEBUG
    std::vector<Diagnosis> diagnosisList;
    diagnosisList.reserve(meanData.size());
    for (const auto & diseaseAndMeanFp : meanData)
    {
        Diagnosis d;
        d.disease = diseaseAndMeanFp.first;
        d.risk = fingerprintMatch(fingerprint, diseaseAndMeanFp.second);
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
        if (!std::holds_alternative<std::monostate>(fingerprintEtendue.values[i]) &&
                !std::holds_alternative<std::monostate>(diff.values[i]) &&
                metadata.attributes[i].type != ID)
        {
            if (metadata.attributes[i].type != BOOLEAN)
            {
                double val = 0;
                if (std::holds_alternative<double>(diff.values[i]))
                    val = std::get<double>(diff.values[i]);
                else
                    val = std::get<int>(diff.values[i]);

                double etendue = std::get<double>(fingerprintEtendue.values[i]);

                sum += std::max(0., 1-((std::abs(val) / etendue)/EPSILON));
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

Database::Database() : err(Database::Error::OK), data(), meanDataBuilder(), meanData(),
                       stdDeviation(), metadata(), fingerprintMax(), fingerprintMin(),
                       fingerprintEtendue(), diagnoseFile()
{
    //ctor
}

Database::~Database()
{
    //dtor
}
