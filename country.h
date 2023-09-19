#ifndef _COUNTRY_H_
#define _COUNTRY_H_

#include "pop.h"
#include "building.h"

#include <vector>
#include <memory>

class Country
{
private:
    std::string country, type;

    std::vector<std::string> provs;

    std::vector<std::shared_ptr<Pop>> pops{};

    std::vector<std::shared_ptr<Building>> buildings{};

public:
    Country(const std::string &name, const std::vector<std::string> &pr, const std::string &type = "")
    :country{name}, type{type}, provs{pr}
    {
    }
    
    ~Country();
    
    void add_pop(const std::shared_ptr<Pop> &);
    
    void add_building(const std::shared_ptr<Building> &);

    std::string getName(){
        return this->country;
    }

    std::string getType(){
        return this->type;
    }

    std::vector<std::shared_ptr<Pop>> getPops(){
        return this->pops;
    }

    std::vector<std::shared_ptr<Building>> getBuilds(){
        return this->buildings;
    }

    std::vector<std::string> &getProvs(){
        return this->provs;
    }

    void setCountryType(const std::string &);
};

#endif 