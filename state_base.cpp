#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem> 
#include "state_base.h"

// constructors
State::State() {}
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
    : type{type}, /*region{reg}, dlc{dlc}, */prod{pr}, level{lvl}, reserves{res} {}

// Countries
// void State::create_country(const std::string &name, std::vector<std::string> &pr) {
//     this->countries.emplace_back(name, pr);
// }

// pops
void State::create_pops(const std::string &co, const std::string &cul, const std::string &rel, const std::string &type, const int &size){
    for (State::Country &c : countries) {
        if(c.getName() == co) {
            c.getPops().emplace_back(cul, rel, type, size);
        }
    }
}

// buildings
void State::create_buildings(const std::string &co, const std::string &type, const int &lvl, const int &res, const std::vector<std::string> &pm) {
    for (State::Country &c : countries) {
        if(c.getName() == co) {
            c.getBuilds().emplace_back(type, lvl, res, pm);
        }
    }
}

// data calculating/copying
// std::string State::data( std::string &line){
//     line.erase (std::remove(line.begin(), line.end(), ' '), line.end());
//     int pos{line.find("=") + 1};
//     return line.substr(pos, line.find ("\n") - pos);
// }
// unsigned int State::data_int( std::string line) {
//     line.erase (std::remove(line.begin(), line.end(), ' '), line.end());
//     int pos{line.find("=") + 1};
//     std::string arg{line.substr(pos, line.find ("\n") - pos)};
//     return stoi(arg);
// }
// void State::data_vector(std::vector<std::string> &vec, const std::string &line, int len) {
//     int i{};
//     for ( char c : line ) {
//         if(c == 'x') {
//             vec.push_back(line.substr(i + 1, len));
//         }
//         i++; 
//     }
// }
// void State::variable_string_vector(std::vector<std::string> &t, std::string &line) {
//     int beg_pos{}, end_pos{};
//     line.erase (std::remove(line.begin(), line.end(), ' '), line.end());
//     beg_pos = line.find("\"") + 1;
//     end_pos = line.find('\"', beg_pos);

//     while( end_pos != std::string::npos ) {
//         t.push_back(line.substr(beg_pos, end_pos - beg_pos));
//         beg_pos = end_pos + 2;
//         end_pos = line.find("\"",beg_pos + 1);
//     }
// }
// bool State::compare_string(const std::string &str, std::string l) {
//     l.erase (std::remove(l.begin(), l.end(), '\t'), l.end());
//     l.erase (std::remove(l.begin(), l.end(), ' '), l.end());
//     return str == l.substr(0, str.size());
// }
void State::copy_state_info(State &st) {
    this->traits = st.getTraits();
    this->sub = st.getSub();
    this->ar_res = st.getResources();
    this->naval_exit = st.getNavalExit();
    this->homelands = st.getHomelands();

}
void State::create_homelands(const std::string &file) {
    std::string line;
    std::ifstream  src(file, std::ios::binary);

    while(getline(src, line)) {
        if(line.find(this->name, 0) != std::string::npos) {
            getline(src, line);
            while(true) {
                if(compare_string("s:STATE", line) == true) {
                    break;
                }
                getline(src, line);
                if(compare_string("add_homeland", line)) {
                    this->homelands.push_back(data(line));
                }
            }
        }
    }    
}

// data printing
void State::print_state_region(){ 
    // std::ofstream  dst("output/map_data/state_regions/to.txt", std::ios::binary | std::ios::app);
    std::ofstream  dst("new.txt", std::ios::binary | std::ios::app);
    dst << "STATE_" << this->name << " = {" << std::endl
        << "\tid = " << this->id << std::endl
        << "\tsubsistence_building = " << this->sub << std::endl
        << "\tprovinces = { ";
    for (State::Country co : this->getCountries()) {
        for (std::string prov : co.getProvs()) {dst << "\"x" << prov << "\" ";}
    }
    dst << "}" << std::endl; 
    if (this->traits.empty() != true) {
        dst << "\ttraits = { "; 
        for (std::string s : this->traits) {dst << "\"" << s << "\" ";}
        dst << "}" << std::endl; 
    }
    dst << "\tcity = " << this->hubs[0] << std::endl;
    if(this->hubs[1] != "") {dst<< "\tport = " << this->hubs[1] << std::endl;}
    dst << "\tfarm = " << this->hubs[2] << std::endl
        << "\tmine = " << this->hubs[3] << std::endl
        << "\twood = " << this->hubs[4] << std::endl
        << "\tarable_land = " << this->res[0] << std::endl
        << "\tarable_resources = { ";
    for (std::string s : this->ar_res) {dst << "\"" << s << "\" ";} 
    dst << "}" << std::endl
        << "\tcapped_resources = {" << std::endl;
    if(this->res[1] != 0) {dst << "\t\tbg_coal_mining = " << this->res[1] << std::endl; }
    if(this->res[2] != 0) {dst << "\t\tbg_iron_mining = " << this->res[2] << std::endl; }
    if(this->res[3] != 0) {dst << "\t\tbg_lead_mining = " << this->res[3] << std::endl; }
    if(this->res[4] != 0) {dst << "\t\tbg_sulfur_mining = " << this->res[4] << std::endl; }
    if(this->res[5] != 0) {dst << "\t\tbg_logging = " << this->res[5] << std::endl; }
    if(this->res[6] != 0) {dst << "\t\tbg_fishing = " << this->res[6] << std::endl; }
    if(this->res[7] != 0) {dst << "\t\tbg_whaling = " << this->res[7] << std::endl; }
    dst << "\t}" << std::endl;
    if(this->res[8] != 0) {
        dst << "\tresource = {" << std::endl 
            << "\t\ttype = \"bg_gold_fields\"" << std::endl
            << "\t\tdepleted_type = \"bg_gold_mining\"" << std::endl
            << "\t\tundiscovered_amount =" << this->res[8] << std::endl;
        if(this->res[9] != 0) {dst << "\t\tdiscovered_amount = " << this->res[9] << std::endl;}
        dst << "\t}" << std::endl;
    }
    if(this->res[10] != 0) {
        dst << "\tresource = {" << std::endl
            << "\t\ttype = \"bg_rubber\"" << std::endl
            << "\t\tundiscovered_amount =" << this->res[10] << std::endl;
        dst << "\t}" << std::endl;
    }
    if(this->res[11] != 0) {
        dst << "\tresource = {" << std::endl
            << "\t\ttype = \"bg_oil_extraction\"" << std::endl
            << "\t\tundiscovered_amount =" << this->res[11] << std::endl;
        dst << "\t}" << std::endl;
    }
    if(this->naval_exit != "") {dst << "\tnaval_exit_id = " << this->naval_exit << std::endl;}
    dst << "}" << std::endl << std::endl;
}
void State::print_pops() { 
    // std::ofstream  dst("output/common/history/pops/new_pops.txt", std::ios::binary | std::ios::app);
    std::ofstream  dst("new_pops.txt", std::ios::binary | std::ios::app);
    dst << "\ts:STATE_" << this->name << " = {" << std::endl;
    for(State::Country co : this->getCountries()) {
        if(co.getPops().empty()) {continue;}
        dst << "\t\tregion_state:" << co.getName() << " = {" << std::endl;
        for(auto pop : co.getPops()) {
            if(pop.getSize() == 0) {continue;}
            dst << "\t\t\tcreate_pop = {" << std::endl;
            if (pop.getType() != "") {dst << "\t\t\t\tpop_type = " << pop.getType() << std::endl;}
            dst << "\t\t\t\tculture = " << pop.getCult() << std::endl;
            if (pop.getRel() != "") {dst << "\t\t\t\treligion = " << pop.getRel() << std::endl;}
            dst << "\t\t\t\tsize = " << pop.getSize() << std::endl
                << "\t\t\t}" << std::endl;
        }
        dst << "\t\t}" << std::endl; 
    }
    dst << "\t}" << std::endl;
}
void State::print_buildings(){ 
    // std::ofstream  dst("output/common/history/buildings/new_buildings.txt", std::ios::binary | std::ios::app);
    std::ofstream  dst("new_buildings.txt", std::ios::binary | std::ios::app);
    // int size{this->buildings.size()};
    dst << "\ts:STATE_" << this->name << " = {" << std::endl;
    for(State::Country co : this->getCountries()) {
        if(co.getBuilds().empty()) {continue;}
        dst << "\t\tregion_state:" << co.getName() << " = {" << std::endl;
        for(auto build : co.getBuilds()) {
            if(build.getLvl() == 0) {continue;}
            dst << "\t\t\tcreate_building = {" << std::endl;
            dst << "\t\t\t\tbuilding = " << build.getType() << std::endl;
            dst << "\t\t\t\tlevel = " << build.getLvl() << std::endl;
            dst << "\t\t\t\treserves = " << build.getRes() << std::endl;
            dst << "\t\t\t\tactivate_production_methods = { ";
            for(std::string prod : build.getProd()) {dst << "\"" << prod << "\" ";}
            dst << "} " << std::endl << "\t\t\t}" << std::endl;
        }
        dst << "\t\t}" << std::endl;
    }
    dst << "\t}" << std::endl;
    // for(int i{}; i < size; i++){
    //     std::string reg{this->buildings[i].getRegion()};
    //     if(this->buildings[i].getLvl() > 0 ) {
    //         if(i > 0 && reg != this->buildings[i-1].getRegion()) {
    //             dst << "\t\tregion_state:" << reg <<" = {" << std::endl;
    //         }
    //     }
    //     if(i < (size - 1) && reg != this->buildings[i+1].getRegion() ) {
    //         dst << "\t\t}" << std::endl;
    //     }
    //     if(i == size - 1) {
    //         dst << "\t\t}" << std::endl;
    //     }
    // }
    // dst << "\t}" << std::endl;
}
void State::print_state() {
    // std::ofstream  dst("output/common/history/states/new_states.txt", std::ios::binary | std::ios::app);
    std::ofstream  dst("new_states.txt", std::ios::binary | std::ios::app);
    dst << "\ts:STATE_" << this->name << " = {" << std::endl;
    for(State::Country co : this->countries) {
        if(co.getProvs().empty()) {continue;}
        dst << "\t\tcreate_state = {" << std::endl
            << "\t\t\tcountry = c:" << co.getName() << std::endl
            << "\t\t\towned_provinces = { ";
        for(std::string s : co.getProvs()) {dst << "x" << s << " ";}
        dst << "}" << std::endl << "\t\t}" << std::endl;
    }
    for(std::string t : this->homelands) {
        dst << "\t\tadd_homeland = " << t << std::endl;
    }
    for(std::string c : this->claims) {
        dst << "\t\tadd_claim = " << c << std::endl;
    }
    dst << "\t}" << std::endl;
}
void State::print_entry() {
    this->print_state_region();
    this->print_pops();
    this->print_buildings();
    this->print_state();
}

// Setters
void State::setName(const std::string &n) {this->name = n;}
void State::setId(const std::string &i) {this->id = i;}
void State::setSub(const std::string &s){this->sub = s;}
void State::setTraits(const std::vector<std::string> &t){this->traits = t;}
void State::setHubs(const std::string h[5]){for(int i{}; i < 5; i++) {this->hubs[i] = h[i];} }
void State::setArRes(const std::vector<std::string> &r) {this->ar_res = r;}
void State::setRes(const int r[/*12*/]){ for(int i{}; i < 12; i++) {this->res[i] = r[i];} }
void State::setHomeland(const std::string &home) {this->homelands.push_back(home);}
void State::setClaim(const std::string &claim) {this->claims.push_back(claim);}
void State::setNavEx(const std::string &nx){this->naval_exit = nx;}
void State::Country::setCountryType(const std::string &type) {this->type = type;}
void State::Country::Pop::setSize(const int &size) {this->size = size;}
void State::Country::Building::setLvl(const int &level) {this->level = level;}