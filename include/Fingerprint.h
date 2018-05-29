#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include <vector>
#include <variant>
#include <string>

typedef std::variant<std::monostate,bool,int,double,std::string> FingerprintValue;

struct Fingerprint
{
    std::vector<FingerprintValue> values{};
};

inline bool operator==(const Fingerprint &f1, const Fingerprint &f2)
{
    if (f1.values.size() != f2.values.size())
    {
        return false;
    }

    for (size_t i=0; i<f1.values.size(); i++)
    {
        if(f1.values[i] != f2.values[i])
        {
            return false;
        }
    }
    return true;
}

#endif // FINGERPRINT_H
