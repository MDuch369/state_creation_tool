#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>

// #include "origin_state.h"
#include "transfer_state.h"


// constructor
// Transfer_state::Transfer_state() {}
Transfer_state::Transfer_state(Transfer_state const&) {}

Transfer_state::Transfer_state(const std::string &name, const std::string &id, const std::vector<std::string> &provs) {
    this->name = name;
    this->id = id;
    this->transfer_provs = provs;
}

Transfer_state::~Transfer_state() {}

void Transfer_state::create_transfer_country(const std::shared_ptr<Country> &country, const std::vector<std::string> &provs, const double &ratio) {
    std::shared_ptr<Country> new_country = std::make_shared<Country>(country->getName(), provs, country->getType());

    for(std::shared_ptr<Pop> pop : country->getPops()) {
        new_country->add_pop(std::make_shared<Pop>(pop->getCult(), pop->getRel(), pop->getType(), pop->getSize() * ratio));
    }

    for(std::shared_ptr<Building> build : country->getBuilds()) {
        new_country->add_building(std::make_shared<Building>(build->getType(), build->getLvl() * ratio, build->getRes(), build->getProd()));
    }

}

/* void Transfer_state::new_transfer_country(const std::vector<std::string> &provs, const State::Country &country) { // [WIP] REFACTOR

    double ps = provs.size(), cs = country.getProvs().size();// ? find a better method of calculating this
    this->countries.push_back(create_country(country, provs, ps/cs));
    provs.clear();
    // this->ratio.push_back(ps/cs);
    // for(auto pop : co.getPops()) {}
    // std::vector<State::Country::Pop>;

} */

void Transfer_state::remove_transfer_provs(){
    this->transfer_provs.erase(std::remove(this->transfer_provs.begin(), this->transfer_provs.end(), ""), this->transfer_provs.end());
}

bool Transfer_state::check_transfer_provs(){
    return this->transfer_provs.empty();
}

/* std::vector<std::string> Transfer_state::add_different_origin_provs() {
    std::vector<std::string> diff_ori{};
    for(std::string pr : this->transfer_provs) {
        diff_ori.push_back(pr);
    }
    return diff_ori{};
} */

std::vector<std::string> Transfer_state::handle_transfer_provs(){
    std::vector<std::string> diff_ori{};

    this->remove_transfer_provs();
    if(this->check_transfer_provs() != true) {
        
        for(std::string pr : this->transfer_provs) {
            diff_ori.push_back(pr);
        }
    }
    return diff_ori;
}

// TODO denest
 std::vector<std::string> Transfer_state::find_origin_states( const std::vector<std::shared_ptr<State>> &origin_states) { // ? refactor, move to State_list class
    std::vector<std::string> provs{}, different_origin_provs{};
    bool origin_found{false};

    for(const std::shared_ptr<State> &state : origin_states) {
        
        for(const std::shared_ptr<Country> &country : state->getCountries()) {
            
            for(const std::string &origin_province : country->getProvs()) {
                
                for(std::string &province : this->transfer_provs) { 
                    
                    if(origin_province == province) { // moves found provs into provs vector
                       
                        if(this->origin == ""){
                            this->origin = state->getName();
                            this->origin_ptr = state;
                        }
                        origin_found = 1;
                        provs.push_back(province);
                        // province = ""; // ? find a better method to erase the entry
                        erase_province(province); // ! check if this implementation works as intended
                    } 
                }
            }
            
            if(provs.empty() != true){ // creates new transfer country from provs vector
                double provs_size = provs.size(), country_size = country->getProvs().size();// ? find a better method of calculating the ratio
                this->create_transfer_country(country, provs, provs_size/country_size);
                provs.clear();
            }
        }

        if(origin_found) {break;}    
    }

    return different_origin_provs;

    // this->transfer_provs.erase(std::remove(this->transfer_provs.begin(), this->transfer_provs.end(), ""), this->transfer_provs.end()); // erases empty strings // TODO find a better method of doing this
    
    // if(this->transfer_provs.empty() != true) { // creates another transfer state if this one still has provs left
        // 
        // for(std::string remaining_prov : this->transfer_provs) {
            // different_origin_provs.push_back(remaining_prov);
        // }
 
        // std::shared_ptr<State> new_transfer_state = std::make_shared<Transfer_state>(this->name, this->id, different_origin_provs); 
    // }
}     

// void Transfer_state::copy_state_info(State &st) {
    // this->traits = st.getTraits();
    // this->sub = st.getSub();
    // this->ar_res = st.getResources();
    // this->naval_exit = st.getNavalExit();
    // this->homelands = st.getHomelands();
// 
// }

// ! placeholder functions
// void Transfer_state::create_pops(const std::string &co, const std::string &cul, const std::string &rel, const std::string &type, const int &size){
    // for (State::Country &c : countries) {
        // if(c.getName() == co) {
            // c.getPops().emplace_back(cul, rel, type, size);
        // }
    // }
// }
// void Transfer_state::create_buildings(const std::string &co, const std::string &type, const int &lvl, const int &res, const std::vector<std::string> &pm) {
    // for (State::Country &c : this->countries) {
        // if(c.getName() == co) {
            // c.getBuilds().emplace_back(type, lvl, res, pm);
        // }
    // }
// }
// std::shared_ptr<State::Country> Transfer_state::create_country(const std::string &name, std::vector<std::string> &pr){  
    // this->countries.emplace_back(name, pr);
    // std::shared_ptr<State::Country> country_ptr(new State::Country(name, pr));
    // this->countries.push_back(*country_ptr);
    // return country_ptr;
// } 

void Transfer_state::setProvs(const std::vector<std::string> &transfer_provs)
{
    this->transfer_provs = transfer_provs;
}

void Transfer_state::setOrigin(const std::string &new_origin)
{
    this->origin = new_origin;
}

void Transfer_state::setOriginPtr(const std::shared_ptr<State> &new_origin_ptr)
{
    this->origin_ptr = new_origin_ptr;
}

void Transfer_state::erase_province(const std::string &province)
{
    this->transfer_provs.erase(std::remove(this->transfer_provs.begin(), this->transfer_provs.end(), province), this->transfer_provs.end()); 
    // std::erase(this->transfer_provs, province);
}