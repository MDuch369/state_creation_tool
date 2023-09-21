#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>

#include "parser.h"
#include "origin_state.h"
#include "transfer_state.h"
#include "state_list.h"

// State_list::~State_list() {};

// creating an array of states info //? TODO refactor using regex
std::ifstream::pos_type State_list::create_state(std::ifstream &src, std::shared_ptr<State> &state, std::string &line) {
    std::string country{};
    std::vector<std::string> provs{};
    
    while(getline(src, line)) {
       
        if(find_string(line, "create_state")) {
            getline(src, line);
            country = data(line, ':');
            while(getline(src, line)) { // ? extract function
                data_vector(provs, line, 6);
                if(find_string(line, "}")){
                    break;
                }
            }

            std::shared_ptr<Country> pres = std::make_shared<Country>(country, provs);
            provs.clear();
            getline(src, line);
            if(find_string(line, "#")) {
                continue;
            }

            if(find_string(line, "state_type")) {
                pres->setCountryType(data(line));
            }

            state->add_country(pres);
        }

        if(find_string(line, "add_homeland")) {
            break;
        }
    }
    return src.tellg();
}

std::ifstream::pos_type State_list::add_homelands(std::ifstream &src, std::shared_ptr<State> &state, std::string &line) {
    while(find_string(line, "add_homeland")) {
        state->addHomeland(data(line));
        getline(src, line);
    }
    return src.tellg();
}

std::ifstream::pos_type State_list::add_claims(std::ifstream &src, std::shared_ptr<State> &state, std::string &line) {
    while(find_string(line, "add_claim")) {
        state->addClaim(data(line));
        getline(src, line);
    } 
    return src.tellg();
}

void State_list::save_states(const std::filesystem::path &path) { 
    std::string line{}, country{}, type{};
    std::vector<std::string> provs{}, homelands{}, claims{};
    std::ifstream  src(path / "common/history/states/00_states.txt", std::ios::binary);  

    while(getline(src, line)) { // ? move this function inside state class, and override it in subclasses
        if(find_string(line, "s:")) {
            std::shared_ptr<State> state = this->states.emplace_back(std::make_shared<Origin_state>(data_name(line)));
            src.seekg(create_state(src, state, line));
            src.seekg(add_homelands(src, state, line));
            src.seekg(add_claims(src, state, line));
        }
    }
}

// state info saving
std::ifstream::pos_type State_list::save_gold_fields(std::ifstream &src, int *cap_res, std::string &line) {
    while(getline(src, line)) {
        find_data_int(src, line, "undiscovered_amount", cap_res[8]);
        find_data_int(src, line, "discovered_amount", cap_res[9]);
        if(find_string(line, "}", 1)){
            getline(src, line);
            break;
        }
    }
    return src.tellg();
}
std::ifstream::pos_type State_list::save_rubber(std::ifstream &src, int *cap_res, std::string &line) {
    while(getline(src, line)) {
        find_data_int(src, line, "undiscovered_amount", cap_res[10]);
        if(find_string(line, "}", 1)){
            getline(src, line);
            break;
        }
    }
    return src.tellg();
}
std::ifstream::pos_type State_list::save_oil(std::ifstream &src, int *cap_res, std::string &line) {
    while(getline(src, line)) {
        find_data_int(src, line, "undiscovered_amount", cap_res[11]);
        if(find_string(line, "}", 1)){
            getline(src, line); 
            break;
        }
    }
    return src.tellg();
}
std::ifstream::pos_type State_list::save_resources(std::ifstream &src, int *cap_res, std::string &line) {
    // std::string line;
    while(getline(src, line)){
        if(find_string(line, "bg_gold_fields")) {
            src.seekg(save_gold_fields(src, cap_res, line));
        }    
        if(find_string(line, "bg_rubber")) {
            src.seekg(save_rubber(src, cap_res, line));
        }
        if(find_string(line, "bg_oil_extraction")) {
            src.seekg(save_oil(src, cap_res, line));
        }
        if(find_string(line, "}")){
            break;
        }
    }
    return src.tellg();
}
std::ifstream::pos_type State_list::save_capped_resources(std::ifstream &src, int *cap_res, std::string &line) {
    // std::string line;
    // for(int i = 1; i < 8; i++) 
    while(getline(src, line)) {
        find_data_int(src, line, "bg_coal_mining", cap_res[1]);
        find_data_int(src, line, "bg_iron_mining", cap_res[2]);
        find_data_int(src, line, "bg_lead_mining", cap_res[3]);
        find_data_int(src, line, "bg_sulfur_mining", cap_res[4]);
        find_data_int(src, line, "bg_logging", cap_res[5]);
        find_data_int(src, line, "bg_fishing", cap_res[6]);
        find_data_int(src, line, "bg_whaling", cap_res[7]);
        if(find_string(line, "}", 1)){
            getline(src, line);
            break;
        }
    }
    return src.tellg();
}
std::ifstream::pos_type State_list::save_subsistence_building(std::ifstream &src, std::string &line, std::string &subsist) {
    // std::string line;
    subsist = data(line);
    while(getline(src, line)){
        if(find_string(line, "}")) {
            getline(src, line); 
            if(find_string(line, "impassable")) { // TODO implement impassable provs handling
                getline(src, line); 
                break;
            }
            break;
        }
    } 
    return src.tellg();
}
std::ifstream::pos_type State_list::save_state(std::ifstream &src, std::shared_ptr<State> & st, std::string &line){
    int cap_res[12]{};
    std::string nav_ex{}, name{}, id{}, subsist{};
    std::vector<std::string> traits{}, ar_res{};
    
    while(getline(src, line)) {
        if(find_string(line, "id")) {
            id = data(line);
            st->setId(id);
            getline(src, line);
        }
        if(find_string(line, "subsistence_building")) {
            
            src.seekg(save_subsistence_building(src, line, subsist));
            st->setSub(subsist);
        }
        if(find_string(line, "traits")) {
            if(traits.empty() != true) {
                traits.clear();
            }
            variable_string_vector(traits, line);
            st->setTraits(traits);
            getline(src, line);
        }
        // ? TODO implement copying of hubs here
        // TEMP CODE
        if(find_string(line, "city")) {
            getline(src, line);
        }
        if(find_string(line, "port")) {
            getline(src, line);
        }
        if(find_string(line, "farm")) {
            getline(src, line);
        }
        if(find_string(line, "mine")) {
            getline(src, line);
        }
        if(find_string(line, "wood")) {
            getline(src, line);
        }
        // END OF TEMP CODE
        if(find_string(line, "arable_land")) {
            cap_res[0] = data_int(line);
            getline(src, line);
        } 
        if(find_string(line, "arable_resources")) {
            if(ar_res.empty() != true) {
                ar_res.clear();
            }
            variable_string_vector(ar_res, line);
            st->setArRes(ar_res);
            getline(src, line);
        }
        if(find_string(line, "capped_resources")) {
            src.seekg(save_capped_resources(src, cap_res, line));
        }
        if(find_string(line, "resource ")) {
            src.seekg(save_resources(src, cap_res, line));
        }
        st->setRes(cap_res);
        if(find_string(line, "naval_exit_id")) {
            nav_ex = data(line);
            st->setNavExit(nav_ex);
        }
        if(find_string(line, "}")){
            break;
        }
    }
    return src.tellg();
}

void State_list::browse_file(std::ifstream &src) { // TODO denest
    std::string line, name{};
    
    while(getline(src, line)) {
        if(find_string(line, "STATE")) {
            name = data_name(line);
            for (std::shared_ptr<State> &st : this->states) {
                if(st->getName() == name) {
                    src.seekg(save_state(src, st, line));
                }
            }  
        }
    }
}

void State_list::save_state_info(const std::filesystem::path &path/* , std::vector<State> &states */, const std::filesystem::path *files) { 
    std::filesystem::path regions{"map_data/state_regions"};

    std::filesystem::path regs[15]{};
    for(int i {}; i < 15; i++){
        regs[i] = path / regions / files[i].filename();
    }

    for(const auto &file : regs) {
        std::ifstream  src(file, std::ios::binary);
        browse_file(src/* , this->states */);
    }
} 

//   pops saving
std::ifstream::pos_type State_list::save_pop(std::ifstream &src, const std::string &name, const std::string &country_name, const std::string &cu, const std::string &r, const std::string &t, const int &s) { // ! TODO create a separate class for the parameters 
    
    for (std::shared_ptr<State> &st : this->states) { 
        if(st->getName() == name) {
            for(std::shared_ptr<Country> country : st->getCountries()) {
                if (country->getName() == country_name) {
                    country->add_pop(std::make_shared<Pop>(cu, r, t, s));
                }
            }
        }
    }
    return src.tellg();
}

void State_list::save_state_pops(const std::filesystem::path &path, const std::filesystem::path *files) {
    std::filesystem::path pops{"common/history/pops"};
    std::filesystem::path ps[15]{};
    for(int i {}; i < 15; i++){ps[i] = path / pops / files[i].filename();}
    
    for(const auto &file : ps) { 
        std::ifstream  src(file, std::ios::binary);
        std::string line;
        std::string name{}, country_name{};
        getline(src, line);
        int size{};
        std::string type{}, cult{}, rel{};

        while(getline(src, line)) { 
            find_name(src, line, "s:", name);
            find_data(src, line, "region_state", ':', country_name);
            find_data(src, line, "pop_type", type);
            find_data(src, line, "culture", cult);
            find_data(src, line, "religion", rel);
            if(find_string(line, "size")) { // ! TODO extract this as a function
                size = data_int(line);
                src.seekg(save_pop(src, name, country_name, cult, rel, type, size));
                size = 0;
                type = "";
                cult = "";
                rel = "";
                getline(src, line);
            }
        }
    }
}

// building saving
std::ifstream::pos_type State_list::save_building(std::ifstream &src, const std::string &name, const std::string &country_name, const std::string &type, const int &lvl, const int &res, const std::vector<std::string> &prod_meth) { // ! TODO create a separate class for the parameters 

    for (std::shared_ptr<State> &st : this->states) {
        if(st->getName() == name) {
            for(std::shared_ptr<Country> country : st->getCountries()) {
                if (country->getName() == country_name) {
                    country->add_building(std::make_shared<Building>(type, lvl, res, prod_meth));
                }
            }
        }
    }
    return src.tellg();
}

void State_list::save_state_builds(const std::filesystem::path &path, const std::filesystem::path *files) {
    std::filesystem::path builds{"common/history/buildings"};
    std::filesystem::path blds[15]{};
    for(int i {}; i < 15; i++){blds[i] = path / builds / files[i].filename();}
    
    for(const auto &file : blds) { 
        std::ifstream  src(file, std::ios::binary);
        std::string line;
        std::string name{}, country{};
        getline(src, line);
        int lvl{}, res{};
        std::string type{};
        std::vector<std::string> pm{};
        while(getline(src, line)) { 
            find_name(src, line, "s:", name);
            find_data(src, line, "region_state", ':', country);
            if(find_string(line, "create_building")){ // ? TODO make this a function
                getline(src, line);
            }   
            find_data(src, line,"building=", type);
            find_data_int(src, line, "level", lvl);
            find_data_int(src, line, "reserves", res);
            if(find_string(line, "activate_production_methods")) { // ! TODO extract this as a function
                variable_string_vector(pm, line);
                src.seekg(save_building(src, name, country, type, lvl, res, pm));
                type = "";
                lvl = 0;
                res = 0;
                pm.clear();
                getline(src, line);
            }
        }
    }
}

// state transfer
void State_list::calculate_resources() {
    double num_origin_provs{}, num_provs{};
    double ratio{};

    for (std::shared_ptr<State> &state : this->states) {
        std::shared_ptr<Transfer_state> transfer_state = std::dynamic_pointer_cast<Transfer_state>(state);

        std::shared_ptr<State> origin_ptr = transfer_state->getOriginPtr();

        for(const std::string &homeland : origin_ptr->getHomelands()) {
            transfer_state->addHomeland(homeland);
        }

        for(const std::string &claim : origin_ptr->getClaims()) {
            transfer_state->addClaim(claim);
        }

        transfer_state->setTraits(origin_ptr->getTraits());
        transfer_state->setSub(origin_ptr->getSub());
        transfer_state->setArRes(origin_ptr->getResources());
        transfer_state->setNavExit(origin_ptr->getNavalExit());
        
        for(std::shared_ptr<Country> country : origin_ptr->getCountries()) {
            num_origin_provs += country->getProvs().size();
        }

        for(std::shared_ptr<Country> country : transfer_state->getCountries()) {
            num_provs += country->getProvs().size();
        }

        ratio = num_provs/num_origin_provs;
       
        int res[12]; 
        for(int i{}; i < 12; i++) {
            res[i] = origin_ptr->getRes()[i] * ratio;
        }
        
        transfer_state->setRes(res); 
    }
}

// ? roll these functions into one
void prov_copy(std::shared_ptr<Country> &target_country, std::shared_ptr<Country> &country) {
   
    for(std::string province : country->getProvs()) { // prov copying
        bool present{false};
        // WIP replacing loop with any_of algorithm
        // if(std::any_of(tar_co.getProvs().begin(), tar_co.getProvs().end(), 
            // [&prov](const std::string &target) { return target == prov; }))
        // {
        for(std::string target_prov : target_country->getProvs()) {
            if(province == target_prov) {
                present = true;
                break;
            }
        }

        if(!present){
            target_country->getProvs().push_back(province);
        } 
    }
}

void pop_copy(std::shared_ptr<Country> &target_country, std::shared_ptr<Country> &country) 
{
    for(std::shared_ptr<Pop> pop : country->getPops()) { // pop copying
        bool present{false};
        
        for(std::shared_ptr<Pop> target_pop : target_country->getPops()) {
            
            if(pop->getCult() == target_pop->getCult() && pop->getRel() == target_pop->getRel()) {
                target_pop->setSize(target_pop->getSize() + pop->getSize()); // ? refactor using operator overlading
                present = true;
                break;
            }
        }

        if(!present){
            target_country->add_pop(pop);
        }
    }
}

void building_copy(std::shared_ptr<Country> &target_country, std::shared_ptr<Country> &country)
{
    for(std::shared_ptr<Building> build : country->getBuilds()) { // building copying
        bool present{false};

        for(std::shared_ptr<Building> target_build : target_country->getBuilds()) {

            if(build->getType() == target_build->getType()) {
                target_build->setLvl(target_build->getLvl() + build->getLvl()); // ? refactor using operator overlading
                present = true;
                break;
            }
        }

        if(!present){
            target_country->add_building(build);
        }
    }  
}


//TODO denest
void State_list::create_target_states(State_list &target_states/*, std::vector<State> &origin_st*/) { 
    bool target{};

    for(std::shared_ptr<State> &transfer_state : this->states) {

        for(std::shared_ptr<State> &state : target_states.getStates()) {

            if(state->getName() == transfer_state->getName()) {

                for(std::shared_ptr<Country> country : transfer_state->getCountries()) { // country copying

                    for(std::shared_ptr<Country> &target_country : state->getCountries()) {

                        if(country->getName() == target_country->getName()) {
                            prov_copy(target_country, country);
                            pop_copy(target_country, country); 
                            building_copy(target_country, country);
                        }
                    }
                }

                int res[12]{};
                for(int i{}; i < 11; i++) {
                    res[i] = state->getRes()[i] + transfer_state->getRes()[i];
                }

                state->setRes(res);
                target = 1;
                break;
            }
        }
        
        // adds copy of transfer_state to state list
        if (!target) { // ! that does not look right
            target_states.add_state(transfer_state);
            
            // std::shared_ptr<Transfer_state> transfer_state = std::dynamic_pointer_cast<Transfer_state>(transfer_state);
            // target_states.add_state(std::make_shared<Transfer_state>(*transfer_state));
        } 
    }
}

// ! this functions now handles a whole list of states, instead of one, so it should be carefully checked if it works properly
void State_list::create_remaining_states(State_list &remaining_states){ // ? merge with calculate_remaining_resources
    bool found{false};
    
    for(std::shared_ptr<State> &state : this->states) {
        std::shared_ptr<Transfer_state> transfer_state = std::dynamic_pointer_cast<Transfer_state>(state);
        
        for(std::shared_ptr<State> &remaining_state : remaining_states.getStates()) {
           
            if (remaining_state->getName() == transfer_state->getOrigin()) {
                found = true;
                break;
            }
        }
       
        if(!found) {
            // ! TODO test if this is corrrect
            remaining_states.add_state(std::move(transfer_state->getOriginPtr())); 
        }
    }
}


void subtract_prov(std::shared_ptr<Country> &country, std::shared_ptr<Country> &transfer_country) {
    
    for(std::string& prov : country->getProvs()) { // prov subtracting
        
        for(std::string transfer_prov : transfer_country->getProvs()) {
            
            if(prov == transfer_prov) { // ? TODO refactor using operator overlading
                prov = ""; // TODO erase empty provinces
                break;
            }
        }
    }
}

void subtract_pop(std::shared_ptr<Country> &country, std::shared_ptr<Country> &transfer_country) {
   
    for(std::shared_ptr<Pop> &pop : country->getPops()) { // pop subtracting
       
        for(std::shared_ptr<Pop> transfer_pop : transfer_country->getPops()) {
            
            if(pop->getCult() == transfer_pop->getCult() && pop->getRel() == transfer_pop->getRel()) {
                pop->setSize(pop->getSize() - transfer_pop->getSize()); // TODO refactor using operator overlading
                break;
            }    
        }
    }
}

void subtract_buildings(std::shared_ptr<Country> &country, std::shared_ptr<Country> &transfer_country) {
    
    for(std::shared_ptr<Building> &build : country->getBuilds()) { // building subtracting
        // bool present{};
        
        for(std::shared_ptr<Building> transfer_build : transfer_country->getBuilds()) {
            
            if(build->getType() == transfer_build->getType()) {
                build->setLvl(build->getLvl() - transfer_build->getLvl()); // TODO refactor using operator overlading
                break;
            }
        }
    }
}

void subtract_country(std::shared_ptr<State> &remaining_state, std::shared_ptr<Transfer_state> &transfer_state){
    
    for(std::shared_ptr<Country>& country : remaining_state->getCountries()) { // country subtracting
        
        for(std::shared_ptr<Country> transfer_country : transfer_state->getCountries()) {
           
            if(country->getName() == transfer_country->getName()) {
                subtract_prov(country, transfer_country);
                subtract_pop(country, transfer_country);
                subtract_buildings(country, transfer_country);
                // std::vector<std::string> provs{co.getProvs()};
            }
        }

        // auto prov_list = country->getProvs();
        // std::string value{};
        // std::remove(prov_list.begin(), prov_list.end(), value);
        country->getProvs().erase(std::remove(country->getProvs().begin(), country->getProvs().end(), ""), country->getProvs().end()); 
    }
}

// ? TODO denest
void State_list::calculate_remaining_resources(State_list &remaining_states) {
   
    for(std::shared_ptr<State> &state : this->states) {
        
        for(std::shared_ptr<State> &remaining_state : remaining_states.getStates()) {
            
            // TODO check if origin_ptr can be used instead
            std::shared_ptr<Transfer_state> transfer_state = std::dynamic_pointer_cast<Transfer_state>(state);
            
            if(transfer_state->getOrigin() == remaining_state->getName()) {
                subtract_country(remaining_state, transfer_state);
                int res[12]{};
                
                for(int i{}; i < 11; i++) {
                    res[i] = remaining_state->getRes()[i] - transfer_state->getRes()[i];
                }

                remaining_state->setRes(res);
                break;
            }
        }
    }
}

// ? consolidate the functions
void State_list::add_state( std::shared_ptr<State> state) {
    this->states.emplace_back(state);
}

/* void State_list::add_state(const State &state) {
    
    this->states.emplace_back(state);
} */

/* std::shared_ptr<State> State_list::add_state_ptr(const State &state)
{
    std::shared_ptr<State> new_state = std::make_shared<State>(state);
    // std::shared_ptr<State> new_state{state};
    this->states.push_back(new_state);
    return new_state;
} */

/* std::shared_ptr<State> State_list::add_state(const std::string &name, const std::string &id, const std::vector<std::string> &provs)
{
    this->states.emplace_back(name, id, provs);
    return std::shared_ptr<State>();
} */

// HACK - this entire function will propably cause an enormous  mess
std::shared_ptr<State> State_list::emplace_transfer_state(std::shared_ptr<State> &transfer_st) {
    this->states.emplace_back(transfer_st);
    std::shared_ptr<Transfer_state> new_transfer_state = std::dynamic_pointer_cast<Transfer_state>(this->states.back());
    return new_transfer_state;
}

// TODO denest
void State_list::find_origin_states(State_list &origin_states) { // ? refactor/denest
    std::vector<std::string> provs{}, different_origin_provs{};
    bool origin_found{false};

    for( std::shared_ptr<State> &state : this->states) { // ! this will probably have to be changed into for loop, due to adding antries into the list
        std::shared_ptr<Transfer_state> transfer_state = std::dynamic_pointer_cast<Transfer_state>(state);
        
        for(const std::shared_ptr<State> &origin_state : origin_states.getStates()) {
           
            for(const std::shared_ptr<Country> &origin_country : origin_state->getCountries()) {
               
                for(const std::string &origin_province : origin_country->getProvs()) {
                    
                    for(std::string &province : transfer_state->getProvs()) { 
                        
                        if(origin_province == province) { // moves found provs into [p]rovince vector
                           
                            if(transfer_state->getOrigin() == ""){
                                transfer_state->setOrigin(origin_state->getName());
                                transfer_state->setOriginPtr(origin_state);
                            }

                            origin_found = true;
                            provs.push_back(province);
                            province = ""; // ? find a better method to erase the entry
                            // transfer_state->erase_province(province); // ! check if this implementation works as intended
                        } 
                    }
                }
               
                if(provs.empty() != true){ // creates new transfer country from provs vector
                    double provs_size = provs.size(), country_size = origin_country->getProvs().size();// ? find a better method of calculating the ratio
                    transfer_state->create_transfer_country(origin_country, provs, provs_size/country_size);
                    provs.clear();
                }
            }
        }

        state = transfer_state;

        if(origin_found) {
            break;
        }

        // this->origin_pos++;
        different_origin_provs = transfer_state->handle_transfer_provs();


        if(transfer_state->check_transfer_provs() != true) { // creates another transfer state if this one still has provs left
            this->states.push_back(std::make_shared<Transfer_state>(transfer_state->getName(), transfer_state->getId(), different_origin_provs));
        }
    }
}    