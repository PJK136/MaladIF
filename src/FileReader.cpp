#include "FileReader.h"

#include <sstream>
#include <vector>

#ifndef NDEBUG
    #include <iostream>
#endif

FileReader::Error FileReader::metadataErr = OK;

FileReader::FileReader() :
    err(OK),
    file(),
    metadata(),
    associatedIndex()
{
#ifndef NDEBUG
    std::cerr << "Appel au constructeur de FileReader" << std::endl;
#endif // NDEBUG
}

FileReader::~FileReader()
{
#ifndef NDEBUG
    std::cerr << "Appel au destructeur de FileReader" << std::endl;
#endif // NDEBUG
}

bool FileReader::open(const std::string & filename, const Metadata & mdata)
{
    file.open(filename);
    if (!file)
    {
        err = CANT_OPEN;
        return false;
    }

    if (file.eof())
    {
        err = EMPTY;
        return false;
    }

    associatedIndex.resize(mdata.attributes.size() + 1);

    std::string line;
    getline(file, line);
    if (line.back() == '\r') {
        line.pop_back();
    }
    std::stringstream sLine(line);
    std::string attribute;
    size_t i = 0;

    while (!sLine.eof() && i < associatedIndex.size())
    {
        getline(sLine, attribute, ';');
        if (attribute == "Disease")
        {
            associatedIndex[i] = -1;
        }
        else
        {
            auto it = mdata.attributesIndex.find(attribute);
            if (it != mdata.attributesIndex.end())
            {
                associatedIndex[i] = it->second;
            }
            else
            {
                err = UNKNOWN_ATTRIBUTE;
#ifndef NDEBUG
                std::cerr << "L'attribut suivant n'existe pas : " << attribute << std::endl;
#endif
                associatedIndex.clear();
                return false;
            }
        }
        i++;
    }

    if (!sLine.eof())
    {
        err = TOO_MANY_ATTRIBUTES;
        associatedIndex.clear();
        return false;
    }

    //Si les données ne contiennent pas de colonne Disease, on peut enlever la place réservée dans associatedIndex
    if (i == mdata.attributes.size())
    {
        associatedIndex.pop_back();
    }

    metadata.attributes = mdata.attributes;
    metadata.attributesIndex = mdata.attributesIndex;

    return true;
}

std::pair<Fingerprint, std::string> FileReader::nextFingerprint()
{
    std::string line;
    getline(file, line);
    if (line.empty())
    {
        err = EMPTY;
        return {};
    }
    if (line.back() == '\r') {
        line.pop_back();
    }
    std::stringstream sLine(line);
    std::string attribute;
    std::string disease;
    Fingerprint fi;
    fi.values = std::vector(metadata.attributes.size(), std::variant<std::monostate,bool,int,double,std::string>());

    size_t i = 0;

    while (!sLine.eof() && i < associatedIndex.size())
    {
        getline(sLine, attribute, ';');
        if (associatedIndex[i] == -1)
        {
            disease = attribute;
        }
        else
        {
            if (attribute.empty())
            {
                fi.values[associatedIndex[i]] = std::monostate();
            }
            else
            {
                AttributeType type = metadata.attributes[associatedIndex[i]].type;
                try
                {
                    if (type == ID || type == INT) {
                            fi.values[associatedIndex[i]] = std::stoi(attribute);
                    } else if (type == BOOLEAN) {
                        if (attribute == "True" || attribute == "true")
                        {
                            fi.values[associatedIndex[i]] = true;
                        }
                        else if (attribute == "False" || attribute == "false")
                        {
                            fi.values[associatedIndex[i]] = false;
                        }
                        else
                        {
                            throw std::invalid_argument(attribute);
                        }
                    } else if (type == DOUBLE) {
                        fi.values[associatedIndex[i]] = std::stod(attribute);
                    } else if (type == STRING) {
                        fi.values[associatedIndex[i]] = attribute;
                    }
                }
                catch (const std::invalid_argument &ia)
                {
                    err = INVALID_VALUE;
                    fi.values[associatedIndex[i]] = std::monostate();
#ifndef NDEBUG
                    std::cerr << "Invalid argument : " << ia.what() << std::endl;
#endif
                    return {};
                }
            }
        }
        i++;
    }

    if (!sLine.eof())
        err = TOO_MANY_ATTRIBUTES;

    if (sLine.eof() && i < associatedIndex.size())
    {
        err = ATTRIBUTES_MISSING;
        associatedIndex.clear();
        return {};
    }

    return std::make_pair(fi,disease);
}

Metadata FileReader::readMetadata(const std::string & filename)
{
    std::ifstream file(filename);
    Metadata metadata;

    if (!file)
    {
        metadataErr = CANT_OPEN;
        return metadata;
    }

    std::string line;

    std::string attributeName;
    std::string attributeType;
    
    getline(file, line);

    while (!file.eof())
    {
        getline(file, line);
        if (line.empty())
        {
            break;
        }
        if (line.back() == '\r') {
            line.pop_back();
        }
        std::stringstream sLine(line);
        getline(sLine, attributeName, ';');
        getline(sLine, attributeType);

        AttributeType type;

        if (attributeType == "ID")
            type = ID;
        else if (attributeType == "boolean")
            type = BOOLEAN;
        else if (attributeType == "int")
            type = INT;
        else if (attributeType == "double")
            type = DOUBLE;
        else if (attributeType == "string")
            type = STRING;
        else
        {
            metadataErr = INVALID_TYPE;
#ifndef NDEBUG
            std::cerr << "Erreur de lecture des métadonnées : type non valide : " << attributeType << std::endl;
#endif
            return Metadata();
        }

        Attribute attribute;
        attribute.name = attributeName;
        attribute.type = type;

        metadata.attributes.push_back(attribute);
        metadata.attributesIndex[attributeName] = metadata.attributes.size() - 1;
    }

    return metadata;
}
