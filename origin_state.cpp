#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>

#include "origin_state.h"

/**** ORIGIN STATE CLASS ****/

// constructors
Origin_state::Origin_state() {}
Origin_state::~Origin_state() {}

Origin_state::Origin_state(const std::string &n)
{
    name = n;
}

// Origin_state::Origin_state(const std::string &name)/*  :name{name}  */{
    // this->State::name = name;
// } 
/* State::State(const std::filesystem::path &path) {
    std::string line{};
    std::ifstream  src(path / "common/history/states/00_states.txt", std::ios::binary);   
}
State::Country::Country(const std::string &name):country{name} {}
State::Country::Country(const std::string &name, const std::vector<std::string> &pr) :country{name}, provs{pr} {}
State::Country::Country(const std::string &name, const std::string &type, const std::vector<std::string> &pr) :country{name}, type {type}, provs{pr} {}
State::Country::Pop::Pop(const std::string &cult, const std::string &rel, const std::string &t, const int &s)
    : culture{cult}, religion{rel}, type{t}, size{s} {}
State::Country::Building::Building(const std::string &type, const int &lvl, const int &res, const std::vector<std::string> &pr) 
    : type{type}, prod{pr}, level{lvl}, reserves{res} {}
 */

// std::shared_ptr<Country> Origin_state::create_country(const std::string &name, std::vector<std::string> &pr) {
    // this->countries.emplace_back(name, pr);
    // std::shared_ptr<State::Country> country_ptr(new State::Country(name, pr));
    // this->countries.push_back(*country_ptr);
    // return country_ptr;
// /*     State::Country country(name, pr);
    // State::Country *const country_ptr = &country;
    // this->countries.push_back(country);
    // return country_ptr; */
// }
// 
// void Origin_state::create_pops(const std::string &co, const std::string &cul, const std::string &rel, const std::string &type, const int &size){
    // for (Country &c : this->countries) {
        // if(c.getName() == co) {
            // c.getPops().emplace_back(cul, rel, type, size);
        // }
    // }
// }
// void Origin_state::create_buildings(const std::string &co, const std::string &type, const int &lvl, const int &res, const std::vector<std::string> &pm) {
    // for (State::Country &c : this->countries) {
        // if(c.getName() == co) {
            // c.getBuilds().emplace_back(type, lvl, res, pm);
        // }
    // }
// }

