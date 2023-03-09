#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include "state.h"

// constructors
/* State::State() {}
State::State(const std::filesystem::path &path) {
    std::string line{};
    std::ifstream  src(path / "common/history/states/00_states.txt", std::ios::binary);   
}
State::State(const std::string &name) :name{name} {} 
State::Country::Country(const std::string &name):country{name} {}
State::Country::Country(const std::string &name, const std::vector<std::string> &pr) :country{name}, provs{pr} {}
State::Country::Country(const std::string &name, const std::string &type, const std::vector<std::string> &pr) :country{name}, type {type}, provs{pr} {}
State::Country::Pop::Pop(const std::string &cult, const std::string &rel, const std::string &t, const int &s)
    : culture{cult}, religion{rel}, type{t}, size{s} {}
State::Country::Building::Building(const std::string &type, const int &lvl, const int &res, const std::vector<std::string> &pr) 
    : type{type}, prod{pr}, level{lvl}, reserves{res} {}
 */

Origin_state::Country *const State::create_country(const std::string &name, std::vector<std::string> &pr) {
    State::Country country(name, pr);
    State::Country *const country_ptr = &country;
    this->countries.push_back(country);
    return country_ptr;
}

/**** STATE TRANSFER CLASS ****/

// constructor
Transfer_state::Transfer_state(const std::string &name, const std::string &id, const std::string &provs) {
    std::string prov, tmp; 
    this->name = name;
    this->id = id;
    std::getline(std::cin, prov);
    prov.erase (std::remove(prov.begin(), prov.end(), '\"'), prov.end());
    prov.erase (std::remove(prov.begin(), prov.end(), ' '), prov.end());
    transform(prov.begin(), prov.end(), prov.begin(), toupper);
    std::stringstream ss(prov);
    while(getline(ss, tmp, 'X')){
        if(tmp != "") {
        this->transfer_provs.push_back(tmp);
        }
    }
}
Transfer_state::Transfer_state(const std::string &name, const std::string &id, const std::vector<std::string> &provs) {
    this->name = name;
    this->id = id;
    this->transfer_provs = provs;
}

// void Transfer_state::create_country(const std::string &name, std::vector<std::string> &pr) {
//     this->countries.emplace_back(name, pr);
// }

// data calculating/copying
// Transfer_state::Country *const State::create_country(const std::string &name, std::vector<std::string> &pr) {

// }
State::Country Transfer_state::create_country(State::Country &country, std::vector<std::string> &provs, const double &ratio) {
    State::Country new_country(country.getName(), country.getType(), provs);
    for(auto pop : country.getPops()) {
        new_country.getPops().emplace_back(pop.getCult(), pop.getRel(), pop.getType(), pop.getSize() * ratio);
    }
    for(auto build : country.getBuilds()) {
        new_country.getBuilds().emplace_back(build.getType(), build.getLvl() * ratio, build.getRes(), build.getProd());
    }
    return new_country;
}
void Transfer_state::find_origin_states(const std::vector<Origin_state> &states, std::vector<Transfer_state> &tr_st) { // ? refactor
    std::vector<std::string> p{}, diff_ori{};
    bool or_found{0}, state_end{0};

    for(Origin_state st : states) {
        for(State::Country co : st.getCountries()) {
            for(std::string or_pr : co.getProvs()) {
                for(std::string &pr : this->transfer_provs) { 
                    if(or_pr == pr) { // moves found provs into [p]rovince vector
                        if(this->origin == ""){
                            this->origin = st.getName();
                        }
                        or_found = 1;
                        p.push_back(pr);
                        pr = ""; // ? find a better method to erase the entry
                    } 
                }
            }
            if(p.empty() != true){ // creates new transfer country from [p]rovince vector
                double ps{p.size()}, cs{co.getProvs().size()};// ? find a better method of calculating this
                this->countries.push_back(create_country(co, p, ps/cs));
                p.clear();
                // this->ratio.push_back(ps/cs);
                // for(auto pop : co.getPops()) {}
                // std::vector<State::Country::Pop>;
            }
        }
        if(or_found) {break;}
        this->origin_pos++;
    }
    this->transfer_provs.erase(std::remove(this->transfer_provs.begin(), this->transfer_provs.end(), ""), this->transfer_provs.end());
    if(this->transfer_provs.empty() != true) { // creates another transfer state if this one still has provs left
        for(std::string pr : this->transfer_provs) {
            diff_ori.push_back(pr);
        }
        tr_st.emplace_back(this->name, this->id, diff_ori);
    }
}    
void Transfer_state::calculate_resources(std::vector<Origin_state> &states) {
    double origin_provs{}, provs{};
    double ratio{};
    Origin_state origin{states[this->origin_pos]};
    this->homelands = origin.getHomelands();
    this->claims = origin.getClaims();
    this->traits = origin.getTraits();
    this->sub = origin.getSub();
    this->ar_res = origin.getResources();
    this->naval_exit = origin.getNavalExit();
    for(State::Country co : origin.getCountries()) {
        origin_provs += co.getProvs().size();
    }
    for(State::Country co : this->getCountries()) {
        provs += co.getProvs().size();
    }
    ratio = provs/origin_provs;
    for(int i{}; i < 12; i++) {
        this->res[i] = origin.getRes()[i] * ratio;
    }
}
void Transfer_state::create_target_states(std::vector<Transfer_state> &target_st/*, std::vector<State> &origin_st*/) { //TODO denest
    bool target{};
    // Transfer_state trs{*this};
    // if (target_st.empty()) {target_st.emplace_back(*this);} 
    // else {
        for(State& st : target_st) {
            if(st.getName() == this->getName()) {
                for(State::Country co : this->getCountries()) { // country copying
                    for(State::Country& tar_co : st.getCountries()) {
                        if(co.getName() == tar_co.getName()) {
                            for(std::string prov : co.getProvs()) { // prov copying
                                bool present;
                                present = 0;
                                for(std::string tar_prov : tar_co.getProvs()) {
                                    if(prov == tar_prov) {
                                        present = 1;
                                        break;
                                    }
                                }
                                if(!present){
                                    tar_co.getProvs().push_back(prov);
                                } 
                            }
                            for(auto pop : co.getPops()) { // pop copying
                                bool present{};
                                for(auto tar_pop : tar_co.getPops()) {
                                    if(pop.getCult() == tar_pop.getCult() && pop.getRel() == tar_pop.getRel()) {
                                        tar_pop.setSize(tar_pop.getSize() + pop.getSize()); // TODO refactor using operator overlading
                                        present = 1;
                                        break;
                                    }
                                }
                                if(!present){tar_co.getPops().push_back(pop);}
                            }
                            for(auto build : co.getBuilds()) { // building copying
                                bool present{};
                                for(auto tar_build : tar_co.getBuilds()) {
                                    if(build.getType() == tar_build.getType()) {
                                        tar_build.setLvl(tar_build.getLvl() + build.getLvl()); // TODO refactor using operator overlading
                                        present = 1;
                                        break;
                                    }
                                }
                                if(!present){tar_co.getBuilds().push_back(build);}
                            }
                        }
                    }
                } 
                int res[12]{};
                for(int i{}; i < 11; i++) {res[i] = st.getRes()[i] + this->getRes()[i];}
                st.setRes(res);
                target = 1;
                break;
            }
        }
    // }
    if (!target) {target_st.emplace_back(*this);} 
}
void Transfer_state::create_remaining_states(std::vector<Remnant_state> &rem_st, std::vector<Origin_state> &ori_st){ // ? merge with calculate_remaining_resources
    bool found{};
    for(Remnant_state st : rem_st) {
        if (st.getName() == this->getOrigin()) {
            found = 1;
            break;
        }
    }
    if(!found) {rem_st.emplace_back(ori_st[this->origin_pos]);}
}
void Transfer_state::calculate_remaining_resources(std::vector<Origin_state> &rem_st/*, std::vector<State> &ori_st*/) { // TODO denest
    for(State& st : rem_st) {
        if(this->origin == st.getName()) {
            for(State::Country& co : st.getCountries()) { // country subtracting
                for(State::Country tr_co : this->getCountries()) {
                    if(co.getName() == tr_co.getName()) {
                        for(std::string& prov : co.getProvs()) { // prov subtracting
                            for(std::string tr_prov : tr_co.getProvs()) {
                                if(prov == tr_prov) { // ? TODO refactor using operator overlading
                                    prov = ""; // TODO erase empty provinces
                                    break;
                                }
                            }
                        }
                        // std::vector<std::string> provs{co.getProvs()};
                        
                        for(auto& pop : co.getPops()) { // pop subtracting
                            for(auto tr_pop : tr_co.getPops()) {
                                if(pop.getCult() == tr_pop.getCult() && pop.getRel() == tr_pop.getRel()) {
                                    pop.setSize(pop.getSize() - tr_pop.getSize()); // TODO refactor using operator overlading
                                    break;
                                }    
                            }
                        }
                        for(auto& build : co.getBuilds()) { // building subtracting
                            bool present{};
                            for(auto tr_build : tr_co.getBuilds()) {
                                if(build.getType() == tr_build.getType()) {
                                    build.setLvl(build.getLvl() - tr_build.getLvl()); // TODO refactor using operator overlading
                                    break;
                                }
                            }
                        }
                    }
                }
                co.getProvs().erase(std::remove(co.getProvs().begin(), co.getProvs().end(), ""), co.getProvs().end());
            }
            int res[/*st.getRes().size()*/12]{};
            for(int i{}; i < 11; i++) {res[i] = st.getRes()[i] - this->getRes()[i];}
            st.setRes(res);
            break;
        }
    }
}
