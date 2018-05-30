#ifndef CLI_H
#define CLI_H

#include "Database.h"
#include <iostream>

class CLI
{
    public:
        CLI();
        ~CLI();

        void execute();

    private:
        enum Choice {
            LOAD_DATABASE,
            GET_DISEASE_CHARACTERISTICS,
            ASK_DIAGNOSIS,
            EXIT
        };

        Choice showMenu() const;
        void loadDatabase();
        void getDiseaseCharacteristics() const;
        void askDiagnosis() const;

        Database database;
};

inline std::ostream &operator<<(std::ostream &stream, const Diagnosis &diagnosis)
{
    stream << "Disease : " << diagnosis.disease << "; risk : " << diagnosis.risk * 100 << "%";
    return stream;
}

inline std::ostream &operator<<(std::ostream &stream, const std::vector<Diagnosis> &diagnosisList)
{
    for (const Diagnosis &diagnosis : diagnosisList)
    {
        stream << diagnosis << std::endl;
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
        return stream << "monostate";
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


#endif // CLI_H
