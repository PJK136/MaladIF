#ifndef FINGERPRINT_H
#define FINGERPRINT_H

struct Fingerprint
{
    std::vector<std::variant<std::monostate,bool,int,double,std::string>> values;
};

#endif // FINGERPRINT_H
