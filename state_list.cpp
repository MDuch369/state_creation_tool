#include <vector>
#include <iostream>
#include <fstream>
#include "parser.h"
#include "state.h"
#include "state_list.h"

// creating an array of states info //? TODO refactor using regex
std::ifstream::pos_type State_list::create_state(std::ifstream &src, Origin_state *const state, std::string &line){
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
            // State::Country *const pres{state->create_country(country, provs)};
            auto *const pres{state->create_country(country, provs)};
            provs.clear();
            getline(src, line);
            if(find_string(line, "#")) {
                continue;
            }
            if(find_string(line, "state_type")) {
                // int pres{this->states[cur].getCountries().size() - 1};
                // this->states[cur].getCountries()[pres].setCountryType(data(line));
                pres->setCountryType(data(line));
            }
        }
        if(find_string(line, "add_homeland")) {
            break;
        }
    }
    return src.tellg();
}
std::ifstream::pos_type State_list::add_homelands(std::ifstream &src, Origin_state *const state, std::string &line) {
    while(find_string(line, "add_homeland")) {
        state->setHomeland(data(line));
        getline(src, line);
    }
    return src.tellg();
}
std::ifstream::pos_type State_list::add_claims(std::ifstream &src, Origin_state *const state, std::string &line) {
    while(find_string(line, "add_claim")) {
        state->setClaim(data(line));
        getline(src, line);
    } 
    return src.tellg();
}
void State_list::save_states(const std::filesystem::path &path/* , std::vector<State> &states */) { 
    std::string line{}, country{}, type{};
    std::vector<std::string> provs{}, homelands{}, claims{};
    std::ifstream  src(path / "common/history/states/00_states.txt", std::ios::binary);  

    while(getline(src, line)) { // TODO move this function inside state class, and override it in subclasses
        if(find_string(line, "s:")) {
            std::string name{data_name(line)};
            Origin_state state{name};
            Origin_state *const st_ptr = &state; 
            this->states.push_back(st_ptr);
            // int cur{this->states.size() - 1};

            src.seekg(create_state(src, st_ptr, line));
            src.seekg(add_homelands(src, st_ptr, line));
            src.seekg(add_claims(src, st_ptr, line));
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
        if(find_string(line, "}")){
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
std::ifstream::pos_type State_list::save_state(std::ifstream &src, State* const st, std::string &line){
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
            st->setNavEx(nav_ex);
        }
        if(find_string(line, "}")){
            break;
        }
    }
    return src.tellg();
}
void State_list::browse_file(std::ifstream &src/* , std::vector<State> &states */) { // TODO denest
    std::string line, name{};
    
    while(getline(src, line)) {
        if(find_string(line, "STATE")) {
            name = data_name(line);
            for (State* const st : this->states) {
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
    for(int i {}; i < 15; i++){regs[i] = path / regions / files[i].filename();}

    for(const auto &file : regs) {
        std::ifstream  src(file, std::ios::binary);
        browse_file(src/* , this->states */);
    }
} 

//   pops saving
std::ifstream::pos_type State_list::save_pop(std::ifstream &src/* , std::vector<State> &states */, const std::string &name, const std::string &co, const std::string &cu, const std::string &r, const std::string &t, const int &s) { // ! TODO create a separate class for the parameters 
    for (State* const st : this->states) {
        if(st->getName() == name) {
            st->create_pops(co, cu, r, t, s);
        }
    }
    return src.tellg();
}
void State_list::save_state_pops(const std::filesystem::path &path/* , std::vector<State> &states */, const std::filesystem::path *files) {
    std::filesystem::path pops{"common/history/pops"};
    std::filesystem::path ps[15]{};
    for(int i {}; i < 15; i++){ps[i] = path / pops / files[i].filename();}
    
    for(const auto &file : ps) { 
        std::ifstream  src(file, std::ios::binary);
        std::string line;
        std::string name{}, country{};
        getline(src, line);
        int size{};
        std::string type{}, cult{}, rel{};
        while(getline(src, line)) { 
            find_name(src, line, "s:", name);
            find_data(src, line, "region_state", ':', country);
            find_data(src, line, "pop_type", type);
            find_data(src, line, "culture", cult);
            find_data(src, line, "religion", rel);
            if(find_string(line, "size")) { // ! TODO extract this as a function
                size = data_int(line);
                src.seekg(save_pop(src/* , this->states */, name, country, cult, rel, type, size));
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
std::ifstream::pos_type State_list::save_building(std::ifstream &src/* , std::vector<State> &states */, const std::string &name, const std::string &co, const std::string &t, const int &l, const int &r, std::vector<std::string> &pm) { // ! TODO create a separate class for the parameters 

    for (State* const st : this->states) {
        if(st->getName() == name) {
            st->create_buildings(co, t, l, r, pm);
        }
    }
    return src.tellg();
}
void State_list::save_state_builds(const std::filesystem::path &path/* , std::vector<State> &states */, const std::filesystem::path *files) {
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
            find_data(src, line,"building", type);
            find_data_int(src, line, "level", lvl);
            find_data_int(src, line, "reserves", res);
            if(find_string(line, "activate_production_methods")) { // ! TODO extract this as a function
                variable_string_vector(pm, line);
                src.seekg(save_building(src/* , this->states */, name, country, type, lvl, res, pm));
                type = "";
                lvl = 0;
                res = 0;
                pm.clear();
                getline(src, line);
            }
        }
    }
}
