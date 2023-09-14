#include "country.h"

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