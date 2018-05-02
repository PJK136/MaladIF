#include "FileReader.h"

#include <sstream>
#include <iostream>

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
