#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include <vector>
#include <variant>
#include <string>
#include <iostream>

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

inline FingerprintValue operator-(const FingerprintValue &f1, const FingerprintValue &f2)
{
    if(f1.index() == 0 || f2.index() == 0)
    {
        return std::monostate();
    }
    else if(f1.index() == 4 && f2.index() == 4)
    {
        return (int)(f1 != f2);
    }
    else if (f1.index() == 2 && f2.index() == 2)
    {
        return std::get<int>(f1) - std::get<int>(f2);
    }
    else if((f1.index() == 3 || f1.index() == 2) && (f2.index() == 3 || f2.index() == 2))
    {
        double d1 = (f1.index() == 3) ? std::get<double>(f1) : (double)(std::get<int>(f1));
        double d2 = (f2.index() == 3) ? std::get<double>(f2) : (double)(std::get<int>(f2));
        return d1 - d2;
    }
    else if (f1.index() == 1 && f2.index() == 1)
    {
        return (int)(f1 != f2);
    }

    return std::monostate();
}

inline Fingerprint operator-(const Fingerprint &f1, const Fingerprint &f2)
{
    Fingerprint diff;
    if (f1.values.size() != f2.values.size())
    {
        return diff;
    }

    diff.values.resize(f1.values.size());
    for (unsigned int i = 0; i < f1.values.size(); ++i)
    {
        diff.values[i] = f1.values[i] - f2.values[i];
    }
    return diff;
}

#endif // FINGERPRINT_H
