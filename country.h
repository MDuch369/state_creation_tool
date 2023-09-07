#ifndef _Country_H_
#define _Country_H_

#include "pop.h"
#include "building.h"
#include <vector>

class Country
{
private:
    std::string country, type{};
    std::vector<std::string> provs;
    std::vector<Pop> pops{};
    std::vector<Building> buildings{};
public:
    Country(const std::string &, const std::string &, const std::vector<std::string> &);
    ~Country();
    std::string getName(){return this->country;}
    std::string getType(){return this->type;}
    std::vector<Pop>& getPops(){return this->pops;}
    std::vector<Building>& getBuilds(){return this->buildings;}
    std::vector<std::string> &getProvs(){return this->provs;}
    void setCountryType(const std::string &);
};

Country::Country(const std::string &name, const std::string &type, const std::vector<std::string> &pr)
:country{name}, type {type}, provs{pr}
{
}

Country::~Country()
{
}


#endif 