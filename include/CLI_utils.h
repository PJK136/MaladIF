#ifndef CLI_UTILS_H
#define CLI_UTILS_H

#include "Diagnosis.h"
#include "Metadata.h"
#include "Fingerprint.h"

#include <iostream>

inline std::ostream &operator<<(std::ostream &stream, const Diagnosis &diagnosis)
{
    if (diagnosis.disease == "")
        stream << "Maladie : Aucune ; Risque : " << diagnosis.risk * 100 << " %";
    else
        stream << "Maladie : \"" << diagnosis.disease << "\" ; Risque : " << diagnosis.risk * 100 << " %";
    return stream;
}

inline std::ostream &operator<<(std::ostream &stream, const std::vector<Diagnosis> &diagnosisList)
{
    for (const Diagnosis &diagnosis : diagnosisList)
    {
        stream << "-> " << diagnosis << std::endl;
    }
    return stream;
}

inline std::ostream &operator<<(std::ostream &stream, const Metadata &metadata)
{
    for (size_t i(0); i < metadata.attributes.size(); i++)
        stream << "Case " << i << " : " << metadata.attributes.at(i).name << " | " << metadata.attributes.at(i).type
               << " -> " << metadata.attributesIndex.at(metadata.attributes.at(i).name) << std::endl;

    return stream;
}

inline std::ostream &operator<<(std::ostream &stream, const FingerprintValue &v)
{
    if (std::holds_alternative<std::monostate>(v)) {
        return stream << "null";
    } else if (std::holds_alternative<bool>(v)) {
        return stream << std::get<1>(v);
    } else if (std::holds_alternative<int>(v)) {
        return stream << std::get<2>(v);
    } else if (std::holds_alternative<double>(v)) {
        return stream << std::get<3>(v);
    } else if (std::holds_alternative<std::string>(v)) {
        return stream << std::get<4>(v);
    } else {
        return stream << "unsupported type";
    }
}

inline std::ostream &operator<<(std::ostream &stream, const Fingerprint &fi)
{
    if (!fi.values.empty())
    {
        stream << fi.values[0];
        for (size_t i = 1; i < fi.values.size(); i++)
            stream << " | " << fi.values[i];
    }

    return stream;
}

#endif //CLI_UTILS_H
