#include "country.h"

// Country::Country(const std::string &name, const std::vector<std::string> &pr, const std::string &type = "")
// :country{name}, type{type}, provs{pr}
// {
// }

Country::~Country()
{
}

// std::shared_ptr<Country> Country::create_country(const std::string &name, std::vector<std::string> &pr) {
    // std::shared_ptr<State::Country> country_ptr(new State::Country(name, pr));
    // this->countries.push_back(*country_ptr);
    // return country_ptr;
// }

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