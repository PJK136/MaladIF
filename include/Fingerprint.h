#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include <vector>
#include <variant>
#include <numeric>
#include <string>

typedef std::variant<std::monostate,bool,int,double,std::string> FingerprintValue;

namespace FingerprintUtil
{
    inline bool isNumeric(const FingerprintValue &value)
    {
        return std::holds_alternative<double>(value) ||
                std::holds_alternative<int>(value) ||
                 std::holds_alternative<bool>(value);
    }

    inline double asDouble(const FingerprintValue &value)
    {
        if (std::holds_alternative<double>(value))
            return std::get<double>(value);
        else if (std::holds_alternative<int>(value))
            return std::get<int>(value);
        else if (std::holds_alternative<bool>(value))
            return std::get<bool>(value);
        else
            return std::numeric_limits<double>::quiet_NaN();
    }
}

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
    if (f1.index() == f2.index())
    {
        if (std::holds_alternative<bool>(f1))
        {
            return (int)(f1 != f2);
        }
        else if (std::holds_alternative<int>(f1))
        {
            return std::get<int>(f1) - std::get<int>(f2);
        }
        else if (std::holds_alternative<double>(f1))
        {
            return std::get<double>(f1) - std::get<double>(f2);
        }
        else if(std::holds_alternative<std::string>(f1))
        {
            return (int)(f1 != f2);
        }
    }
    else
    {
        if (FingerprintUtil::isNumeric(f1) && FingerprintUtil::isNumeric(f2))
            return FingerprintUtil::asDouble(f1) - FingerprintUtil::asDouble(f2);
        else
            return std::monostate();
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
    for (size_t i = 0; i < f1.values.size(); ++i)
    {
        diff.values[i] = f1.values[i] - f2.values[i];
    }
    return diff;
}

inline bool strcmp_nocase(std::string const& a, std::string const& b)
{
    if (a.length() == b.length()) {
        return std::equal(b.begin(), b.end(), a.begin(),
                          [](unsigned char a, unsigned char b)
                          {
                              return std::tolower(a) == std::tolower(b);
                          });
    }
    else {
        return false;
    }
}

#endif // FINGERPRINT_H
