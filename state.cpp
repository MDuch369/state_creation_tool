#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include "state.h"

// constructors
State::State(const std::filesystem::path &path) {
    std::string line{};
    std::ifstream  src(path / "common/history/states/00_states.txt", std::ios::binary);   
}
State::State(const unsigned int &cur_line, const std::string &file) { // copies data from map_data/state_regions
    unsigned int copy_line{}, len{};
    std::string line{};
    std::ifstream  src(file, std::ios::binary);
    this->file_name = file;
    while(getline(src, line)) {
        copy_line++;
        if (copy_line >= cur_line - 3) {
            if((copy_line > cur_line) && (line[0] == 'S')) {
                break;
            }
            if(copy_line == cur_line - 3) { 
                this->name = line.substr(0, line.find ("=") - 1);
            }
            if(compare_string("id", line)) { 
                this->id = data_int(line);
            }
            if(compare_string("subsistence_building", line)) {
                this->sub = data(line);
            }
            if(compare_string("provinces", line)) {
                data_vector(this->provs, line, 6);
            }
            if(compare_string("impassable", line)) {
                data_vector(this->im_provs, line, 6);
            }
            if(compare_string("traits", line)) {
                variable_string_vector(this->traits, line);
            }
            if(compare_string("city", line)) {
                this->city = data(line);
            }
            if(compare_string("port", line)) {
                this->port = data(line);
            }
            if(compare_string("farm", line)) {
                this->farm = data(line);
            }
            if(compare_string("mine", line)) {
                this->mine = data(line);
            }
            if(compare_string("wood", line)) {
                this->wood = data(line);
            }
            if(compare_string("arable_land", line)) {
                this->land = data_int(line);
            }
            if(compare_string("arable_resources", line)) {
                variable_string_vector(this->resources, line);
            }
            if(compare_string("bg_coal_mining", line)) {
                this->coal = data_int(line);
            }
            if(compare_string("bg_iron_mining", line)) {
                this->iron = data_int(line);
            }
            if(compare_string("bg_sulfur_mining", line)) {
                this->sulfur = data_int(line);
            }
            if(compare_string("bg_logging", line)) {
                this->log = data_int(line);
            }
            if(compare_string("bg_whaling", line)) {
                this->whale = data_int(line);
            }
            if(compare_string("bg_fishing", line)) {
                this->fish = data_int(line);
            }
            if(compare_string("resource", line)) {
                getline(src, line);
                if(compare_string("bg_gold_fields", line)) {
                    getline(src, line);
                    if(compare_string("undiscovered_amount", line)) {
                        this->gold = data_int(line);
                    }
                    if(compare_string("discovered_amount", line)) {
                        this->disc_gold = data_int(line);
                    }                    
                }
                if(compare_string("bg_rubber", line)) {
                    getline(src, line);
                    if(compare_string("undiscovered_amount", line)) {
                        this->rubber = data_int(line);
                    }
                }
                if(compare_string("bg_oil_extraction", line)) {
                    getline(src, line);
                    if(compare_string("undiscovered_amount", line)) {
                        this->oil = data_int(line);
                    }                    
                }                
            }                                                                                                             
            if(compare_string("naval_exit_id", line)) {
                this->naval_exit = data_int(line);
            }
       
        }
    }
    src.close();
}
State::State(const unsigned int res[]) 
    :land{res[0]}, coal{res[1]}, iron{res[2]}, lead{res[3]}, sulfur{res[4]}, log{res[5]}, fish{res[6]}, whale{res[7]}, oil{res[8]}, rubber{res[9]}, gold{res[10]}, disc_gold{res[11]} {}
State::State(const std::string &name) :name{name} {} 
State::Pop::Pop(const std::string &cult, const std::string &rel, const std::string &t, const int &s)
    : culture{cult}, religion{rel}, type{t}, size{s} {}
State::Building::Building(const std::string &type, const std::string &reg, const std::string &dlc, const std::string &pr, const int &lvl, const int &res) 
    : type{type}, region{reg}, dlc{dlc}, prod{pr}, level{lvl}, reserves{res} {}
State::Country::Country(const std::string &name, const std::vector<std::string> &pr) :country{name}, provs{pr} {}

// pops
void State::create_pops(const std::string &file/*, const std::string &name, std::vector<State::Pop> &vec*/) {
    std::string line, cul, rel{}, t{};
    int s;
    std::ifstream  src(file, std::ios::binary);
    
    // TODO implement handling of different region states
    while(getline(src, line)){
        if(line.find(this->name, 0) != std::string::npos) {
            getline(src, line);
            while(true) {
                getline(src, line);
                if(compare_string("s:STATE", line) == true) {
                    break;
                }
                if(compare_string("create_pop", line)) {
                    getline(src, line);
                    if(compare_string("pop_type", line)) {
                        t = data(line);
                        getline(src, line);
                    }
                    if(compare_string("culture", line)) {
                        cul = data(line);
                        getline(src, line);
                    }
                    if(compare_string("religion", line)) {
                        rel = data(line);
                        getline(src, line);
                    }
                    if(compare_string("size", line)) {
                        s = data_int(line);
                        getline(src, line);
                    }
                    if(compare_string("}", line)) {
                       this->pops.emplace_back(cul, rel, t, s);
                       rel = "";
                    }
                }
            }
        }
    }
}
void State::add_pop(const std::string &cul, const std::string &rel, const std::string &type, const int &size) {
    this->pops.emplace_back(cul, rel, type, size);
}

// buildings
void State::create_buildings(const std::string &file){
    std::string line, reg, pm, build, dlc{};
    int level, res;
    std::ifstream  src(file, std::ios::binary);
    
    while(getline(src, line)) {
        if(line.find(this->name, 0) != std::string::npos) {
            getline(src, line);
            while(true) {
                if(compare_string("region_state", line) == true){
                    int pos{line.find(":") + 1};
                    reg = line.substr(pos, line.find ("=") - pos);
                }
                getline(src, line);
                if(compare_string("s:STATE", line) == true) {
                    break;
                }
                if(compare_string("create_building", line)) {
                    getline(src, line);
                    if(compare_string("building", line)) {
                        build = data(line);
                        getline(src, line);
                    }
                    if(compare_string("level", line)) {
                        level = data_int(line);
                        getline(src, line);
                    }
                    if(compare_string("reserves", line)) {
                        res = data_int(line);
                        getline(src, line);
                    }
                    if(compare_string("activate_production_methods", line)) {
                        pm = data(line);
                        getline(src, line);
                    }
                    if(compare_string("}", line)) {
                       this->buildings.emplace_back(build, reg, dlc, pm, level, res);
                       res = 0;
                       pm = "";
                    }
                }
            }
        }
    }
}
void State::add_building(const std::string &type, const std::string &reg, const std::string &dlc, const std::string &pr, const int &res, const int &lvl) {
    this->buildings.emplace_back(type, reg, dlc, pr, lvl, res);
}

// data calculating/copying
std::string State::data( std::string &line){
    line.erase (std::remove(line.begin(), line.end(), ' '), line.end());
    int pos{line.find("=") + 1};
    return line.substr(pos, line.find ("\n") - pos);
}
unsigned int State::data_int( std::string line) {
    line.erase (std::remove(line.begin(), line.end(), ' '), line.end());
    int pos{line.find("=") + 1};
    std::string arg{line.substr(pos, line.find ("\n") - pos)};
    return stoi(arg);
}
void State::data_vector(std::vector<std::string> &vec, const std::string &line, int len) {
    int i{};

    for ( char c : line ) {
        if(c == 'x') {
            vec.push_back(line.substr(i + 1, len));
        }
        i++; 
    }
}
void State::variable_string_vector(std::vector<std::string> &t, std::string &line) {
    int beg_pos{}, end_pos{};
    line.erase (std::remove(line.begin(), line.end(), ' '), line.end());
    beg_pos = line.find("\"") + 1;
    end_pos = line.find('\"', beg_pos);

    while( end_pos != std::string::npos ) {
        t.push_back(line.substr(beg_pos, end_pos - beg_pos));
        beg_pos = end_pos + 2;
        end_pos = line.find("\"",beg_pos + 1);
    }
}
bool State::compare_string(const std::string &str, std::string l) {
    l.erase (std::remove(l.begin(), l.end(), '\t'), l.end());
    l.erase (std::remove(l.begin(), l.end(), ' '), l.end());
    return str == l.substr(0, str.size());
}
void State::copy_state_info(State &st) {
    this->traits = st.getTraits();
    this->sub = st.getSub();
    this->resources = st.getResources();
    this->naval_exit = st.getNavalExit();
    this->homelands = st.getHomelands();

}
void State::calculate_remaining_provs(State &st) {
    for (std::string a : st.getProvs()) {
        for(int i{}; i < provs.size(); i++) {
            if (a == provs[i]) {
                provs.erase(provs.begin() + i);
            }
        }
    }
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
    std::ofstream  dst("output/map_data/state_regions/to.txt", std::ios::binary | std::ios::app);
    dst << this->name << "= {" << std::endl
        << "\tid = " << this->id << std::endl
        << "\tsubsistence_building = " << this->sub << std::endl
        << "\tprovinces = { ";
    for (std::string s : this->provs) {
        dst << "\"x" << s << "\" ";
    }
    dst << "}" << std::endl 
        << "\ttraits = { "; 
    for (std::string s : this->traits) {
        dst << "\"" << s << "\" ";
    }
    dst << "}" << std::endl 
        << "\tcity = " << this->city << std::endl;
    if(this->port != "") {
        dst<< "\tport = " << this->port << std::endl;
    }
    dst << "\tfarm = " << this->farm << std::endl
        << "\tmine = " << this->mine << std::endl
        << "\twood = " << this->wood << std::endl
        << "\tarable_land = " << this->land << std::endl
        << "\tarable_resources = { ";
    for (std::string s : this->resources) {
        dst << "\"" << s << "\" ";
    } 
    dst << "}" << std::endl
        << "\tcapped = {" << std::endl;
    if(this->iron != 0) {
        dst << "\t\tbg_iron_mining = " << this->iron << std::endl; 
    }
    if(this->coal != 0) {
        dst << "\t\tbg_coal_mining = " << this->coal << std::endl; 
    }
    if(this->lead != 0) {
        dst << "\t\tbg_lead_mining = " << this->lead << std::endl; 
    }
    if(this->sulfur != 0) {
        dst << "\t\tbg_sulfur_mining = " << this->sulfur << std::endl; 
    }
    if(this->log != 0) {
        dst << "\t\tbg_logging = " << this->log << std::endl; 
    }
    if(this->fish != 0) {
        dst << "\t\tbg_fishing = " << this->fish << std::endl; 
    }
    if(this->whale != 0) {
        dst << "\t\tbg_whaling = " << this->whale << std::endl; 
    }
    if(this->oil != 0) {
        dst << "\t\tbg_oil_extraction = " << this->oil << std::endl; 
    }
    if(this->rubber != 0) {
        dst << "\t\tbg_logging = " << this->rubber << std::endl; 
    }
    if(this->gold != 0) {
        dst << "\t\tbg_gold_mining = " << this->gold << std::endl; 
    }
    if(this->disc_gold != 0) {
        dst << "\t\tbg_whaling = " << this->disc_gold << std::endl; 
    }
    dst << "\t}" << std::endl;
    if(this->naval_exit != 0) {
       dst << "\tnaval_exit_id = " << this->naval_exit << std::endl;
    }
    dst << "}" << std::endl << std::endl;
}
void State::print_pops() {
        std::ofstream  dst("output/common/history/pops/to_pops.txt", std::ios::binary | std::ios::app);
    dst << "\ts:" << this->name << " = {" << std::endl
        << "\t\tregion_state:abc = {" << std::endl;
    for(int i{}; i < this->pops.size(); i++){
        dst << "\t\t\tcreate_pop = {" << std::endl;
        if (this->pops[i].getType() != "") {
            dst << "\t\t\t\tpop_type = " << this->pops[i].getType() << std::endl;
        }
        dst << "\t\t\t\tculture = " << this->pops[i].getCult() << std::endl;
        if (this->pops[i].getRel() != "") {
            dst << "\t\t\t\treligion = " << this->pops[i].getRel() << std::endl;
        }
        dst << "\t\t\t\tsize = " << this->pops[i].getSize() << std::endl
            << "\t\t\t}" << std::endl;
    }
    dst << "\t\t}" << std::endl << "\t}" << std::endl;
}
void State::print_buildings(){
    std::ofstream  dst("output/common/history/buildings/to_buildings.txt", std::ios::binary | std::ios::app);
    int size{this->buildings.size()};
   
    dst << "\ts:" << this->name << " = {" << std::endl;
    dst << "\t\tregion_state:" << this->buildings[0].getRegion() <<" = {" << std::endl;

    for(int i{}; i < size; i++){
        std::string reg{this->buildings[i].getRegion()};
        if(this->buildings[i].getLvl() > 0 ) {
            if(i > 0 && reg != this->buildings[i-1].getRegion()) {
                dst << "\t\tregion_state:" << reg <<" = {" << std::endl;
            }
            dst << "\t\t\tcreate_building = {" << std::endl;
            dst << "\t\t\t\tbuilding = " << this->buildings[i].getType() << std::endl;
            dst << "\t\t\t\tlevel = " << this->buildings[i].getLvl() << std::endl;
            dst << "\t\t\t\treserves = " << this->buildings[i].getRes() << std::endl;
            dst << "\t\t\t\tactivate_production_methods = " << this->buildings[i].getProd() << std::endl << "\t\t\t}" << std::endl;
        }
        if(i < (size - 1) && reg != this->buildings[i+1].getRegion() ) {
            dst << "\t\t}" << std::endl;
        }
        if(i == size - 1) {
            dst << "\t\t}" << std::endl;
        }
    }
    dst << "\t}" << std::endl;
}
void State::print_state() {
    std::ofstream  dst("output/common/history/to_states.txt", std::ios::binary | std::ios::app);
    std::string placeholder{"[abc]"};
    dst << "\ts:" << this->name << " = {" << std::endl;
    dst << "\t\tcreate_state = {" << std::endl
        << "\t\t\tcountry = c:" << placeholder << std::endl
        << "\t\t\towned_provinces = { ";
    for(std::string s : this->provs) {
        dst << s << " ";
    }
    dst << "}" << std::endl
        <<  "\t\t}" << std::endl;
    for(std::string t : this->homelands) {
        dst << "\t\tadd_homeland = " << t << std::endl;
    }
    dst << "\t}" << std::endl;
}

// Setters
void State::setName(const std::string &n) {this->name = n;}
void State::setId(const int &i) {this->id = i;}
void State::setProvs(const std::vector<std::string> &p) {this->provs = p;}
void State::setProv(const int &i, const std::string &s) {this->provs[i] = s;}
void State::setPopSize(const int &i, const int &size) {this->pops[i].setSize(size);}
void State::Pop::setSize(const int &size) {this->size = size;}
// void State::setTraits(const std::vector<std::string> &tr) {
//     this->traits = tr;
// }
// void save_data ( const std::string &line, std::ifstream src) {
//     getline(src, line);
// }
// void State::setSub(const std::string &s){
//     this->sub = s;
// }
// void State::setTraits(const std::vector<std::string> &t){
//     this->traits = t;
// }
// void State::setResources(const std::vector<std::string> &r){
//     this->resources = r;
// }
void State::setHomeland(const std::string &home) {this->homelands.push_back(home);}
void State::setClaim(const std::string &claim) {this->claims.push_back(claim);}
void State::Country::setCountryType(const std::string &type) {this->type = type;}

// State_transfer class

// constructor
// State_transfer::State_transfer(const std::string &name, const int &id, std::string &provs) {
//     std::string prov, tmp; 
//     this->name = name;
//     this->id = id;
    
//     std::getline(std::cin, prov);
//     prov.erase (std::remove(prov.begin(), prov.end(), '\"'), prov.end());
//     prov.erase (std::remove(prov.begin(), prov.end(), ' '), prov.end());
//     transform(prov.begin(), prov.end(), prov.begin(), toupper);

//     std::stringstream ss(prov);

//     while(getline(ss, tmp, 'X')){
//         if(tmp != "") {
//         this->provs.push_back(tmp);
//         }
//     }
// }

// // data calculating/copying
// double State_transfer::calculate_ratio(const std::vector<std::string> &pr) {
//     // double i{pr.size()}, j{this->provs.size()};
//     // double result{pr.size() / this->provs.size()};
//     return pr.size() / this->provs.size();
// }
//     // ! TODO finish
// unsigned int State_transfer::find_states(const std::string &path, std::vector<State_transfer> &states) {
//     std::vector<std::string> diff_origin_st{};
//     // std::string *pr = &this->provs[0]; 
//     auto pr = provs.begin();
//     unsigned int cur_line{};
//     std::string line{};
//     // bool mult_donor_st{false};
//     std::ifstream  src(path + "common/history/states/00_states.txt", std::ios::binary);

//     while(getline(src, line)) {
//         cur_line++;
//         if (line.find(*pr, 0) != std::string::npos) {
//             pr++;
//             while(pr != provs.end()) {
//                 if ((line.find(*pr, 0) == std::string::npos)) {
//                     diff_origin_st.push_back(*pr);
//                     pr = provs.erase(pr);
//                 } else { pr++; }
//             }          
//             break;
//         }
//     }
//     states.emplace_back(this->name, this->id, diff_origin_st);
//     src.close();
//     return cur_line;
// }