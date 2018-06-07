#ifndef DIAGNOSIS_H
#define DIAGNOSIS_H

struct Diagnosis
{
    std::string disease{};
    double risk = 0.;
};

inline bool operator==(const Diagnosis &d1, const Diagnosis &d2)
{
    if ( (d1.disease == d2.disease) && (d1.risk == d2.risk) )
    {
        return true;
    }
    return false;
}
#endif // DIAGNOSIS_H
