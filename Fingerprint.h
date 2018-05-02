#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include <vector>
#include <variant>
#include <string>

struct Fingerprint
{
    std::vector<std::variant<std::monostate,bool,int,double,std::string>> values;
};

#endif // FINGERPRINT_H
