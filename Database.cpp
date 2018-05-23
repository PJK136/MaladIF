#include "Database.h"



bool Database::loadMetadata(const std::string & filename)
{

    try
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
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        return false;
    }
}


bool Database::loadData(const std::string & filename)
{
    FileReader fileReader;
    bool success = fileReader.open(filename,metadata);

    if(!success)
        return false;

    std::pair <Fingerprint, std::string >loadedData(fileReader.nextFingerprint());

    //Si c'est vide (ces lignes sont a supprimer au cas ou c'est pris en compte dans open)
    if(!loadedData.first.values.empty())
    {
        return false;
    }
    //--

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


Database::Database()
{
    //ctor
}

Database::~Database()
{
    //dtor
}
