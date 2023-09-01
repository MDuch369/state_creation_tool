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
    // :countries{}
/* {
    this->countries = {};
} */
Transfer_state::Transfer_state(const std::string &name, const std::string &id, const std::vector<std::string> &provs) {
    this->name = name;
    this->id = id;
    this->transfer_provs = provs;
}
Transfer_state::~Transfer_state() {}

State::Country Transfer_state::create_country(State::Country &country, const std::vector<std::string> &provs, const double &ratio) {
    State::Country new_country(country.getName(), country.getType(), provs);
    for(auto pop : country.getPops()) {
        new_country.getPops().emplace_back(pop.getCult(), pop.getRel(), pop.getType(), pop.getSize() * ratio);
    }
    for(auto build : country.getBuilds()) {
        new_country.getBuilds().emplace_back(build.getType(), build.getLvl() * ratio, build.getRes(), build.getProd());
    }
    return new_country;
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
/* void Transfer_state::find_origin_states( State_list &states, State_list &transfer_states) { // ? refactor, move to State_list class
    std::vector<std::string> p{}, diff_ori{};
    bool or_found{0}/* , state_end{0} *//*;

    for(std::shared_ptr<State> &st : states.getStates()) {
        for(State::Country co : st->getCountries()) {
            for(std::string or_pr : co.getProvs()) {
                for(std::string &pr : this->transfer_provs) { 
                    if(or_pr == pr) { // moves found provs into [p]rovince vector
                        if(this->origin == ""){
                            this->origin = st->getName();
                            // WIP converting the state to shared pointer 
                            std::shared_ptr<State> or_ptr(std::move(st));
                            this->origin_ptr = or_ptr;
                            // st = std::move(or_ptr);
                            // states.getStates().emplace_back(std::shared_ptr<State>(std::move(st)));
                            // std::shared_ptr<State> shared_ptr = std::shared_ptr<State>(std::move(st));
                        }
                        or_found = 1;
                        p.push_back(pr);
                        pr = ""; // ? find a better method to erase the entry
                    } 
                }
            }
            if(p.empty() != true){ // creates new transfer country from [p]rovince vector
                double ps = p.size(), cs = co.getProvs().size();// ? find a better method of calculating this
                this->countries.push_back(create_country(co, p, ps/cs));
                p.clear();
                // this->ratio.push_back(ps/cs);
                // for(auto pop : co.getPops()) {}
                // std::vector<State::Country::Pop>;
            }
        }
        if(or_found) {break;}
        // this->origin_pos++;
    }
    this->transfer_provs.erase(std::remove(this->transfer_provs.begin(), this->transfer_provs.end(), ""), this->transfer_provs.end());
    if(this->transfer_provs.empty() != true) { // creates another transfer state if this one still has provs left
        for(std::string pr : this->transfer_provs) {
            diff_ori.push_back(pr);
        }
        // ! TODO TEST THIS IMPLEMENTATION
        // std::shared_ptr<Transfer_state> new_transfer_state = std::dynamic_pointer_cast<Transfer_state>(transfer_states.add_state());
        // std::shared_ptr<Transfer_state> new_transfer_state = transfer_states.add_state(std::make_shared<Transfer_state>(*this));
        // transfer_states.add_state(std::make_shared<Transfer_state>(*this));
        // transfer_states.add_state(std::shared_ptr<State>(new Transfer_state(*this)));
        // transfer_states.add_state(*this);
        // std::shared_ptr<Transfer_state> new_transfer_state = std::dynamic_pointer_cast<Transfer_state>(transfer_states.getStates().back());
        std::shared_ptr<Transfer_state> new_transfer_state = std::dynamic_pointer_cast<Transfer_state>(transfer_states.add_state_ptr(*this));
        new_transfer_state->setName(this->name);
        new_transfer_state->setId(this->id);
        new_transfer_state->setProvs(diff_ori);
    }
}     */

void Transfer_state::copy_state_info(State &st) {
    this->traits = st.getTraits();
    this->sub = st.getSub();
    this->ar_res = st.getResources();
    this->naval_exit = st.getNavalExit();
    this->homelands = st.getHomelands();

}

// ! placeholder functions
void Transfer_state::create_pops(const std::string &co, const std::string &cul, const std::string &rel, const std::string &type, const int &size){
    for (State::Country &c : countries) {
        if(c.getName() == co) {
            c.getPops().emplace_back(cul, rel, type, size);
        }
    }
}
void Transfer_state::create_buildings(const std::string &co, const std::string &type, const int &lvl, const int &res, const std::vector<std::string> &pm) {
    for (State::Country &c : this->countries) {
        if(c.getName() == co) {
            c.getBuilds().emplace_back(type, lvl, res, pm);
        }
    }
}
std::shared_ptr<State::Country> Transfer_state::create_country(const std::string &name, std::vector<std::string> &pr){  
    this->countries.emplace_back(name, pr);
    std::shared_ptr<State::Country> country_ptr(new State::Country(name, pr));
    this->countries.push_back(*country_ptr);
    return country_ptr;
} 
void Transfer_state::setProvs(const std::vector<std::string> &transfer_provs)
{
    this->transfer_provs = transfer_provs;
}