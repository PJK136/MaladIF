#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include <vector>
#include <variant>
#include <string>

struct Fingerprint
{
    std::vector<std::variant<std::monostate,bool,int,double,std::string>> values;
};

inline bool operator==(Fingerprint const& f1, Fingerprint const& f2)
{
    if(f1.values.size() != f2.values.size())
    {
        return false;
    }

    for (int i=0; i<f1.values.size(); i++)
    {
        if(f1.values[i] != f2.values[i])
        {
            return false;
        }
    }
    return true;
}

#endif // FINGERPRINT_H
