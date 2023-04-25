#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>

#include "state.h"
#include "transfer_state.h"


// constructor
// Transfer_state::Transfer_state() {}
Transfer_state::Transfer_state(Transfer_state const&) {}
/* Transfer_state::Transfer_state(const std::string &name, const std::string &id, const std::string &provs) {
    std::string prov, tmp; 
    this->name = name;
    this->id = id;
    std::getline(std::cin, prov);
    prov.erase (std::remove(prov.begin(), prov.end(), '\"'), prov.end()); // TODO extract this function
    prov.erase (std::remove(prov.begin(), prov.end(), ' '), prov.end());
    transform(prov.begin(), prov.end(), prov.begin(), toupper);
    std::stringstream ss(prov);
    while(getline(ss, tmp, 'X')){
        if(tmp != "") {
        this->transfer_provs.push_back(tmp);
        }
    }
} */
Transfer_state::Transfer_state(const std::string &name, const std::string &id, const std::vector<std::string> &provs) {
    this->name = name;
    this->id = id;
    this->transfer_provs = provs;
}
Transfer_state::~Transfer_state() {}

// void Transfer_state::create_country(const std::string &name, std::vector<std::string> &pr) {
//     this->countries.emplace_back(name, pr);
// }

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
// TODO denest
void Transfer_state::find_origin_states( State_list &states, State_list &transfer_states) { // ? refactor, move to State_list class
    std::vector<std::string> p{}, diff_ori{};
    bool or_found{0}/* , state_end{0} */;

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
        std::shared_ptr<Transfer_state> new_transfer_state = std::dynamic_pointer_cast<Transfer_state>(transfer_states.add_state());
        new_transfer_state->setName(this->name);
        new_transfer_state->setId(this->id);
        new_transfer_state->setProvs(diff_ori);
    }
}    
void Transfer_state::calculate_resources(State_list &states) {
    double origin_provs{}, provs{};
    double ratio{};
    // Origin_state origin{states[this->origin_pos]};

    this->homelands = this->origin_ptr->getHomelands();
    this->claims = this->origin_ptr->getClaims();
    this->traits = this->origin_ptr->getTraits();
    this->sub = this->origin_ptr->getSub();
    this->ar_res = this->origin_ptr->getResources();
    this->naval_exit = this->origin_ptr->getNavalExit();
    for(State::Country co : this->origin_ptr->getCountries()) {
        origin_provs += co.getProvs().size();
    }
    for(State::Country co : this->getCountries()) {
        provs += co.getProvs().size();
    }
    ratio = provs/origin_provs;
    for(int i{}; i < 12; i++) {
        this->res[i] = this->origin_ptr->getRes()[i] * ratio;
    }
}

void prov_copy(State::Country &tar_co, State::Country &co) {
    for(std::string prov : co.getProvs()) { // prov copying
        bool present;
        present = 0;
        // WIP replacing loop with any_of algorithm
        // if(std::any_of(tar_co.getProvs().begin(), tar_co.getProvs().end(), 
            // [&prov](const std::string &target) { return target == prov; }))
        // {
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
}
void pop_copy(State::Country &tar_co, State::Country &co) 
{
    for(auto pop : co.getPops()) { // pop copying
        bool present{};
        for(auto tar_pop : tar_co.getPops()) {
            if(pop.getCult() == tar_pop.getCult() && pop.getRel() == tar_pop.getRel()) {
                tar_pop.setSize(tar_pop.getSize() + pop.getSize()); // ? refactor using operator overlading
                present = 1;
                break;
            }
        }
        if(!present){tar_co.getPops().push_back(pop);}
    }
}
void building_copy(State::Country &tar_co, State::Country &co)
{
  for(auto build : co.getBuilds()) { // building copying
    bool present{};
    for(auto tar_build : tar_co.getBuilds()) {
        if(build.getType() == tar_build.getType()) {
            tar_build.setLvl(tar_build.getLvl() + build.getLvl()); // ? refactor using operator overlading
            present = 1;
            break;
        }
    }
    if(!present){tar_co.getBuilds().push_back(build);}
    }  
}

void Transfer_state::create_target_states(State_list &target_st/*, std::vector<State> &origin_st*/) { //TODO denest
    bool target{};
    // Transfer_state trs{*this};
    // if (target_st.empty()) {target_st.emplace_back(*this);} 
    // else {
        for(std::shared_ptr<State> &st : target_st.getStates()) {
            if(st->getName() == this->getName()) {
                for(State::Country co : this->getCountries()) { // country copying
                    for(State::Country& tar_co : st->getCountries()) {
                        if(co.getName() == tar_co.getName()) {
                            prov_copy(tar_co, co);
                            pop_copy(tar_co, co); 
                            building_copy(tar_co, co);
                        }
                    }
                } 
                int res[12]{};
                for(int i{}; i < 11; i++) {
                    res[i] = st->getRes()[i] + this->getRes()[i];
                }
                st->setRes(res);
                target = 1;
                break;
            }
        }
    if (!target) {
        target_st.add_state(std::make_shared<Transfer_state>(*this));
    } 
}

void Transfer_state::create_remaining_states(State_list &rem_st, const State_list &ori_st){ // ? merge with calculate_remaining_resources
    bool found{};
    for(std::shared_ptr<State> &st : rem_st.getStates()) {
        if (st->getName() == this->getOrigin()) {
            found = 1;
            break;
        }
    }
    if(!found) {
        // ! TODO test if this is corrrect
        rem_st.getStates().emplace_back(std::move(this->origin_ptr)); 
        /*         
        // std::shared_ptr<State> new_remnant_state{ori_st[this->origin_pos]};
        Remnant_state new_remnant_state{ori_st[this->origin_pos]};
        Remnant_state *new_remnant_state_ptr = &new_remnant_state;
        // new_remnant_state.State = ori_st.getStates()[this->origin_pos].State;
        // rem_st.getStates().emplace_back(ori_st.getStates()[this->origin_pos]);
        rem_st.push_back(new_remnant_state_ptr); 
        */
    }
}
void Transfer_state::calculate_remaining_resources(State_list &rem_st/*, std::vector<State> &ori_st*/) { // TODO denest
    for(std::shared_ptr<State> &st : rem_st.getStates()) {
        if(this->origin == st->getName()) {
            for(State::Country& co : st->getCountries()) { // country subtracting
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
                            // bool present{};
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
            for(int i{}; i < 11; i++) {res[i] = st->getRes()[i] - this->getRes()[i];}
            st->setRes(res);
            break;
        }
    }
}
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
/*     State::Country country(name, pr);
    State::Country *const country_ptr = &country;
    this->countries.push_back(country);
    return country_ptr; */
} 
void Transfer_state::setProvs(const std::vector<std::string> &transfer_provs)
{
    this->transfer_provs = transfer_provs;
}