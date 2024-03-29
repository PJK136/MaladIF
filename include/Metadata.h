#ifndef METADATA_H_INCLUDED
#define METADATA_H_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>

enum AttributeType
{
    INVALID,
    ID,
    BOOLEAN,
    INT,
    DOUBLE,
    STRING
};

struct Attribute
{
    std::string name{};
    AttributeType type{INVALID};
};

struct Metadata
{
    std::vector<Attribute> attributes{};
    std::unordered_map<std::string, int> attributesIndex{};
};

#endif // METADATA_H_INCLUDED
