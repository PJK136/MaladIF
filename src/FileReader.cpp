#include "FileReader.h"

#include <sstream>
#include <iostream>
#include <vector>

FileReader::FileReader() :
    file(),
    metadata(),
    associatedIndex()
{
#ifdef DEBUG
    std::cerr << "Appel au constructeur de FileReader" << std::endl;
#endif // DEBUG
}

FileReader::~FileReader()
{
#ifdef DEBUG
    std::cerr << "Appel au destructeur de FileReader" << std::endl;
#endif // DEBUG
}

bool FileReader::open(const std::string & filename, const Metadata & mdata)
{
    file.open(filename);
    if (!file)
    {
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
                std::cerr << "Erreur de lecture des données : l'attribut n'existe pas : " << attribute << std::endl;
                associatedIndex.clear();
                return false;
            }
        }
        i++;
    }

    if (!sLine.eof())
    {
        std::cerr << "Erreur de lecture des données : il y a plus d'attributs dans les données que dans les métadonnées." << std::endl;
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
        return std::make_pair(Fingerprint(), "");
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
                continue;
            }

            AttributeType type = metadata.attributes[associatedIndex[i]].type;
            if (type == ID || type == INT) {
                try
                {
                    fi.values[associatedIndex[i]] = std::stoi(attribute);
                }
                catch (const std::invalid_argument & ia)
                {
                    std::cerr << "Invalid argument : " << ia.what() << std::endl;
                    fi.values[associatedIndex[i]] = std::monostate();
                }
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
                    std::cerr << "Invalid boolean : " << attribute << std::endl;
                    fi.values[associatedIndex[i]] = std::monostate();
                }
            } else if (type == DOUBLE) {
                try
                {
                    fi.values[associatedIndex[i]] = std::stod(attribute);
                }
                catch (const std::invalid_argument & ia)
                {
                    std::cerr << "Invalid argument : " << ia.what() << std::endl;
                    fi.values[associatedIndex[i]] = std::monostate();
                }
            } else if (type == STRING) {
                fi.values[associatedIndex[i]] = attribute;
            }
        }
        i++;
    }

    if (!sLine.eof())
        std::cerr << "Erreur de lecture des données : il y a plus d'attributs dans les données que dans les métadonnées." << std::endl;

    return std::make_pair(fi,disease);
}

/* TODO : à enlever
void FileReader::affiche()
{
    std::cout << "Metadata : " << metadata << std::endl;
    std::cout << std::endl;
    std::cout << "AssociatedIndex : " << std::endl;
    for (size_t i(0); i < associatedIndex.size(); i++)
        std::cout << associatedIndex[i] << " ";
    std::cout << std::endl;
}*/

Metadata FileReader::readMetadata(const std::string & filename)
{
    std::ifstream file(filename);
    Metadata metadata;

    if (!file)
    {
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
            std::cerr << "Erreur de lecture des métadonnées : type non valide : " << attributeType << std::endl;
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
