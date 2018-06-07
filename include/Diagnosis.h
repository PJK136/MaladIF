#ifndef DIAGNOSIS_H
#define DIAGNOSIS_H

struct Diagnosis
{
    std::string disease{};
    double risk = 0.;
};

inline bool operator==(const Diagnosis &d1, const Diagnosis &d2)
{
    return (d1.disease == d2.disease) && (d1.risk == d2.risk);
}
#endif // DIAGNOSIS_H
