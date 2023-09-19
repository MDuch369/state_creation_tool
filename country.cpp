#include "country.h"

Country::~Country()
{
}

void Country::add_pop(const std::shared_ptr<Pop> &pop) 
{
    this->pops.push_back(pop);
}

void Country::add_building(const std::shared_ptr<Building> &building) 
{
    this->buildings.push_back(building);
}

void Country::setCountryType(const std::string &t){
    this->type = t;
}