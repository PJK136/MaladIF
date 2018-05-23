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
    std::pair<Fingerprint, std::string> nextFingerprint();
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
    int i = 0;

    while (!sLine.eof())
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
        Fingerprint fi;
        return std::make_pair(fi, "");
    }
    if (line.back() == '\r') {
        line.pop_back();
    }
    std::stringstream sLine(line);
    std::string attribute;
    std::string disease;
    Fingerprint fi;
    fi.values = std::vector(metadata.attributes.size(), std::variant<std::monostate,bool,int,double,std::string>());

    int i = 0;

    while (!sLine.eof())
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
    return std::make_pair(fi,disease);
}

void afficheVariant(std::variant<std::monostate,bool,int,double,std::string> v)
{
    if (std::holds_alternative<std::monostate>(v)) {
        std::cout << "monostate";
    } else if (std::holds_alternative<bool>(v)) {
        std::cout << std::get<1>(v);
    } else if (std::holds_alternative<int>(v)) {
        std::cout << std::get<2>(v);
    } else if (std::holds_alternative<double>(v)) {
        std::cout << std::get<3>(v);
    } else if (std::holds_alternative<std::string>(v)) {
        std::cout << std::get<4>(v);
    }
}

void FileReader::afficherFI(std::pair<Fingerprint, std::string> data)
{
    std::cout<<"Maladie : "<<data.second<<std::endl;
    auto it = data.first.values.begin();
    while (it != data.first.values.end()) {
        afficheVariant(*it);
        std::cout << " | ";
        it++;
    }
    std::cout<<std::endl;
}

void FileReader::affiche()
{
    std::cout << "Metadata : " << std::endl;
    afficheMetadonnees(metadata);
    std::cout << std::endl;
    std::cout << "AssociatedIndex : " << std::endl;
    for (int i(0); i < associatedIndex.size(); i++)
        std::cout << associatedIndex[i] << " ";
    std::cout << std::endl;
}

void afficheMetadonnees(Metadata metadata)
{
    for (int i(0); i < metadata.attributes.size(); i++)
        std::cout << "Case " << i << " : " << metadata.attributes[i].name << " | " << metadata.attributes[i].type << " -> " << metadata.attributesIndex[metadata.attributes[i].name] << std::endl;
}

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
    int i = 0;

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
        metadata.attributesIndex[attributeName] = i;

        i++;
    }

    return metadata;
}
