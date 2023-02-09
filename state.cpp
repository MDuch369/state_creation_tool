#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include "state.h"

// constructors
State::State() {}
State::State(const std::filesystem::path &path) {
    std::string line{};
    std::ifstream  src(path / "common/history/states/00_states.txt", std::ios::binary);   
}
// State::State(const unsigned int &cur_line, const std::string &file) { // copies data from map_data/state_regions
//     unsigned int copy_line{}, len{};
//     std::string line{};
//     std::ifstream  src(file, std::ios::binary);
//     this->file_name = file;
//     while(getline(src, line)) {
//         copy_line++;
//         if (copy_line >= cur_line - 3) {
//             if((copy_line > cur_line) && (line[0] == 'S')) {
//                 break;
//             }
//             if(copy_line == cur_line - 3) { 
//                 this->name = line.substr(0, line.find ("=") - 1);
//             }
//             if(compare_string("id", line)) { 
//                 this->id = data_int(line);
//             }
//             if(compare_string("subsistence_building", line)) {
//                 this->sub = data(line);
//             }
//             // if(compare_string("provinces", line)) {
//             //     data_vector(this->provs, line, 6);
//             // }
//             if(compare_string("impassable", line)) {
//                 data_vector(this->im_provs, line, 6);
//             }
//             if(compare_string("traits", line)) {
//                 variable_string_vector(this->traits, line);
//             }
//             if(compare_string("city", line)) {
//                 this->city = data(line);
//             }
//             if(compare_string("port", line)) {
//                 this->port = data(line);
//             }
//             if(compare_string("farm", line)) {
//                 this->farm = data(line);
//             }
//             if(compare_string("mine", line)) {
//                 this->mine = data(line);
//             }
//             if(compare_string("wood", line)) {
//                 this->wood = data(line);
//             }
//             if(compare_string("arable_land", line)) {
//                 this->ar_land = data_int(line);
//             }
//             if(compare_string("arable_resources", line)) {
//                 variable_string_vector(this->ar_res, line);
//             }
//             if(compare_string("bg_coal_mining", line)) {
//                 this->coal = data_int(line);
//             }
//             if(compare_string("bg_iron_mining", line)) {
//                 this->iron = data_int(line);
//             }
//             if(compare_string("bg_sulfur_mining", line)) {
//                 this->sulfur = data_int(line);
//             }
//             if(compare_string("bg_logging", line)) {
//                 this->log = data_int(line);
//             }
//             if(compare_string("bg_whaling", line)) {
//                 this->whale = data_int(line);
//             }
//             if(compare_string("bg_fishing", line)) {
//                 this->fish = data_int(line);
//             }
//             if(compare_string("resource", line)) {
//                 getline(src, line);
//                 if(compare_string("bg_gold_fields", line)) {
//                     getline(src, line);
//                     if(compare_string("undiscovered_amount", line)) {
//                         this->gold = data_int(line);
//                     }
//                     if(compare_string("discovered_amount", line)) {
//                         this->disc_gold = data_int(line);
//                     }                    
//                 }
//                 if(compare_string("bg_rubber", line)) {
//                     getline(src, line);
//                     if(compare_string("undiscovered_amount", line)) {
//                         this->rubber = data_int(line);
//                     }
//                 }
//                 if(compare_string("bg_oil_extraction", line)) {
//                     getline(src, line);
//                     if(compare_string("undiscovered_amount", line)) {
//                         this->oil = data_int(line);
//                     }                    
//                 }                
//             }                                                                                                             
//             if(compare_string("naval_exit_id", line)) {
//                 this->naval_exit = data_int(line);
//             }
//         }
//     }
//     src.close();
// }
// State::State(const unsigned int res[]) 
//     :ar_land{res[0]}, coal{res[1]}, iron{res[2]}, lead{res[3]}, sulfur{res[4]}, log{res[5]}, fish{res[6]}, whale{res[7]}, oil{res[8]}, rubber{res[9]}, gold{res[10]}, disc_gold{res[11]} {}
State::State(const std::string &name) :name{name} {} 
State::Country::Country(const std::string &name):country{name} {}
State::Country::Country(const std::string &name, const std::vector<std::string> &pr) :country{name}, provs{pr} {}
State::Country::Country(const std::string &name, const std::string &type, const std::vector<std::string> &pr) :country{name}, type {type}, provs{pr} {}
State::Country::Pop::Pop(const std::string &cult, const std::string &rel, const std::string &t, const int &s)
    : culture{cult}, religion{rel}, type{t}, size{s} {}
State::Country::Building::Building(const std::string &type, const int &lvl, const int &res, const std::vector<std::string> &pr) 
    : type{type}, /*region{reg}, dlc{dlc}, */prod{pr}, level{lvl}, reserves{res} {}

// Countries
void State::create_country(const std::string &name, std::vector<std::string> &pr) {
    this->countries.emplace_back(name, pr);
}

// pops
void State::create_pops(const std::string &co, const std::string &cul, const std::string &rel, const std::string &type, const int &size){
    for (State::Country &c : countries) {
        if(c.getName() == co) {
            c.getPops().emplace_back(cul, rel, type, size);
        }
    }
}
// void State::add_pop(const std::string &cul, const std::string &rel, const std::string &type, const int &size) {
//     this->pops.emplace_back(cul, rel, type, size);
// }

// buildings
void State::create_buildings(const std::string &co, const std::string &type, const int &lvl, const int &res, const std::vector<std::string> &pm) {
    for (State::Country &c : countries) {
        if(c.getName() == co) {
            c.getBuilds().emplace_back(type, lvl, res, pm);
        }
    }
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
    this->ar_res = st.getResources();
    this->naval_exit = st.getNavalExit();
    this->homelands = st.getHomelands();

}
// void State::calculate_remaining_provs(State &st) {
//     for (std::string a : st.getProvs()) {
//         for(int i{}; i < provs.size(); i++) {
//             if (a == provs[i]) {
//                 provs.erase(provs.begin() + i);
//             }
//         }
//     }
// }
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
// void State::print_state_region(){
//     std::ofstream  dst("output/map_data/state_regions/to.txt", std::ios::binary | std::ios::app);
//     dst << this->name << "= {" << std::endl
//         << "\tid = " << this->id << std::endl
//         << "\tsubsistence_building = " << this->sub << std::endl
//         << "\tprovinces = { ";
//     // for (std::string s : this->provs) {
//     //     dst << "\"x" << s << "\" ";
//     // }
//     dst << "}" << std::endl 
//         << "\ttraits = { "; 
//     for (std::string s : this->traits) {
//         dst << "\"" << s << "\" ";
//     }
//     dst << "}" << std::endl 
//         << "\tcity = " << this->city << std::endl;
//     if(this->port != "") {
//         dst<< "\tport = " << this->port << std::endl;
//     }
//     dst << "\tfarm = " << this->farm << std::endl
//         << "\tmine = " << this->mine << std::endl
//         << "\twood = " << this->wood << std::endl
//         << "\tarable_land = " << this->ar_land << std::endl
//         << "\tarable_resources = { ";
//     for (std::string s : this->ar_res) {
//         dst << "\"" << s << "\" ";
//     } 
//     dst << "}" << std::endl
//         << "\tcapped = {" << std::endl;
//     if(this->iron != 0) {
//         dst << "\t\tbg_iron_mining = " << this->iron << std::endl; 
//     }
//     if(this->coal != 0) {
//         dst << "\t\tbg_coal_mining = " << this->coal << std::endl; 
//     }
//     if(this->lead != 0) {
//         dst << "\t\tbg_lead_mining = " << this->lead << std::endl; 
//     }
//     if(this->sulfur != 0) {
//         dst << "\t\tbg_sulfur_mining = " << this->sulfur << std::endl; 
//     }
//     if(this->log != 0) {
//         dst << "\t\tbg_logging = " << this->log << std::endl; 
//     }
//     if(this->fish != 0) {
//         dst << "\t\tbg_fishing = " << this->fish << std::endl; 
//     }
//     if(this->whale != 0) {
//         dst << "\t\tbg_whaling = " << this->whale << std::endl; 
//     }
//     if(this->oil != 0) {
//         dst << "\t\tbg_oil_extraction = " << this->oil << std::endl; 
//     }
//     if(this->rubber != 0) {
//         dst << "\t\tbg_logging = " << this->rubber << std::endl; 
//     }
//     if(this->gold != 0) {
//         dst << "\t\tbg_gold_mining = " << this->gold << std::endl; 
//     }
//     if(this->disc_gold != 0) {
//         dst << "\t\tbg_whaling = " << this->disc_gold << std::endl; 
//     }
//     dst << "\t}" << std::endl;
//     if(this->naval_exit != 0) {
//        dst << "\tnaval_exit_id = " << this->naval_exit << std::endl;
//     }
//     dst << "}" << std::endl << std::endl;
// void State::print_pops() {
// }
//         std::ofstream  dst("output/common/history/pops/to_pops.txt", std::ios::binary | std::ios::app);
//     dst << "\ts:" << this->name << " = {" << std::endl
//         << "\t\tregion_state:abc = {" << std::endl;
//     for(int i{}; i < this->pops.size(); i++){
//         dst << "\t\t\tcreate_pop = {" << std::endl;
//         if (this->pops[i].getType() != "") {
//             dst << "\t\t\t\tpop_type = " << this->pops[i].getType() << std::endl;
//         }
//         dst << "\t\t\t\tculture = " << this->pops[i].getCult() << std::endl;
//         if (this->pops[i].getRel() != "") {
//             dst << "\t\t\t\treligion = " << this->pops[i].getRel() << std::endl;
//         }
//         dst << "\t\t\t\tsize = " << this->pops[i].getSize() << std::endl
//             << "\t\t\t}" << std::endl;
//     }
//     dst << "\t\t}" << std::endl << "\t}" << std::endl;
// }
// void State::print_buildings(){
//     std::ofstream  dst("output/common/history/buildings/to_buildings.txt", std::ios::binary | std::ios::app);
//     int size{this->buildings.size()};
//     dst << "\ts:" << this->name << " = {" << std::endl;
//     dst << "\t\tregion_state:" << this->buildings[0].getRegion() <<" = {" << std::endl;
//     for(int i{}; i < size; i++){
//         std::string reg{this->buildings[i].getRegion()};
//         if(this->buildings[i].getLvl() > 0 ) {
//             if(i > 0 && reg != this->buildings[i-1].getRegion()) {
//                 dst << "\t\tregion_state:" << reg <<" = {" << std::endl;
//             }
//             dst << "\t\t\tcreate_building = {" << std::endl;
//             dst << "\t\t\t\tbuilding = " << this->buildings[i].getType() << std::endl;
//             dst << "\t\t\t\tlevel = " << this->buildings[i].getLvl() << std::endl;
//             dst << "\t\t\t\treserves = " << this->buildings[i].getRes() << std::endl;
//             dst << "\t\t\t\tactivate_production_methods = " << this->buildings[i].getProd() << std::endl << "\t\t\t}" << std::endl;
//         }
//         if(i < (size - 1) && reg != this->buildings[i+1].getRegion() ) {
//             dst << "\t\t}" << std::endl;
//         }
//         if(i == size - 1) {
//             dst << "\t\t}" << std::endl;
//         }
//     }
//     dst << "\t}" << std::endl;
// }
void State::print_state() {
    std::ofstream  dst("output/common/history/to_states.txt", std::ios::binary | std::ios::app);
    std::string placeholder{"[abc]"};
    dst << "\ts:" << this->name << " = {" << std::endl;
    dst << "\t\tcreate_state = {" << std::endl
        << "\t\t\tcountry = c:" << placeholder << std::endl
        << "\t\t\towned_provinces = { ";
    // for(std::string s : this->provs) {
    //     dst << s << " ";
    // }
    dst << "}" << std::endl
        <<  "\t\t}" << std::endl;
    for(std::string t : this->homelands) {
        dst << "\t\tadd_homeland = " << t << std::endl;
    }
    dst << "\t}" << std::endl;
}

// Setters
void State::setName(const std::string &n) {this->name = n;}
void State::setId(const std::string &i) {this->id = i;}
void State::setSub(const std::string &s){this->sub = s;}
void State::setTraits(const std::vector<std::string> &t){this->traits = t;}
void State::setHubs(const std::string h[5]){for(int i{}; i < 5; i++) {this->hubs[i] = h[i];} }
void State::setLand(const int &l){this->ar_land = l;}
void State::setArRes(const std::vector<std::string> &r) {this->ar_res = r;}
void State::setRes(const int r[11]){ for(int i{}; i < 11; i++) {this->res[i] = r[i];} }
// void State::setProvs(const std::vector<std::string> &p) {this->provs = p;}
// void State::setProv(const int &i, const std::string &s) {this->provs[i] = s;}
// void State::setPopSize(const int &i, const int &size) {this->pops[i].setSize(size);}
void State::Country::Pop::setSize(const int &size) {this->size = size;}
void State::Country::Building::setLvl(const int &level) {this->level = level;}
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
//     this->ar_res = r;
// }
void State::setHomeland(const std::string &home) {this->homelands.push_back(home);}
void State::setClaim(const std::string &claim) {this->claims.push_back(claim);}
void State::setArable(const int &ar) { this->ar_land = ar;}
void State::Country::setCountryType(const std::string &type) {this->type = type;}




// debug functions
// void State::debug_print_provs(){
//     std::ofstream  dst("provs_debug", std::ios::binary | std::ios::app);
//     for(State::Country co : this->getCountries()){
//         int i{};
//         for (char c : co.getProvs()[0]) {
//             dst << c;
//             i++;
//         }
//     dst << " " << i << std::endl;    
//     }
// }

/**** STATE TRANSFER CLASS ****/

// constructor
State_transfer::State_transfer(const std::string &name, const std::string &id, const std::string &provs) {
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
        this->provs.push_back(tmp);
        }
    }
}
State_transfer::State_transfer(const std::string &name, const std::string &id, const std::vector<std::string> &provs) {
    this->name = name;
    this->id = id;
    this->provs = provs;
}

// data calculating/copying
State::Country State_transfer::create_transfer_country(State::Country &country, std::vector<std::string> &provs, const double &ratio) {
    State::Country new_country(country.getName(), country.getType(), provs);
    for(auto pop : country.getPops()) {
        new_country.getPops().emplace_back(pop.getCult(), pop.getRel(), pop.getType(), pop.getSize() * ratio);
    }
    for(auto build : country.getBuilds()) {
        new_country.getBuilds().emplace_back(build.getType(), build.getLvl() * ratio, build.getRes(), build.getProd());
    }
    return new_country;
}
void State_transfer::find_origin_states(const std::vector<State> &states, std::vector<State_transfer> &tr_st) { // ? refactor
    std::vector<std::string> p{}, diff_ori{};
    bool or_found{0}, state_end{0};

    for(State st : states) {
        for(State::Country co : st.getCountries()) {
            for(std::string or_pr : co.getProvs()) {
                for(std::string &pr : this->provs) {
                    if(or_pr == pr) {
                        if(this->origin == ""){
                            this->origin = st.getName();
                        }
                        or_found = 1;
                        p.push_back(pr);
                        pr = ""; // ? find a better method to erase the entry
                    } 
                }
            }
            if(p.empty() != true){
                double ps{p.size()}, cs{co.getProvs().size()};// ? find a better method of calculating this
                this->countries.push_back(create_transfer_country(co, p, ps/cs));
                p.clear();
                // this->ratio.push_back(ps/cs);
                // for(auto pop : co.getPops()) {}
                // std::vector<State::Country::Pop>;
            }
        }
        if(or_found) {break;}
        this->origin_pos++;
    }
    this->provs.erase(std::remove(this->provs.begin(), this->provs.end(), ""), this->provs.end());
    if(this->provs.empty() != true) {
        for(std::string pr : this->provs) {
            diff_ori.push_back(pr);
        }
        tr_st.emplace_back(this->name, this->id, diff_ori);
    }
}    
void State_transfer::calculate_resources(std::vector<State> &states) {
    int origin_provs{}, provs{};
    double ratio{};
    State origin{states[this->origin_pos]};
    this->homelands = origin.getHomelands();
    this->claims = origin.getClaims();
    this->traits = origin.getTraits();
    this->ar_res = origin.getResources();
    this->naval_exit = origin.getNavalExit();
    for(State::Country co : origin.getCountries()) {
        origin_provs += co.getProvs().size();
    }
    for(State::Country co : this->getCountries()) {
        provs += co.getProvs().size();
    }
    ratio = provs/origin_provs;
    // this->ar_land = origin.getLand() * ratio;
    for(int i{}; i < 11; i++) {
        this->res[i] = origin.getRes()[i] * ratio;
    }
}
void State_transfer::create_target_states(std::vector<State_transfer> &target_st/*, std::vector<State> &origin_st*/) {
    bool target{};
    // State_transfer trs{*this};
    if (target_st.empty()) {target_st.emplace_back(*this);} // ! TODO finish the constructor
    else {
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
                // st.setArable(st.getLand() + this->getLand()); // TODO refactor using operator overloading
                int res[11]{};
                for(int i{}; i < 11; i++) {res[i] = st.getRes()[i] + this->getRes()[i];}
                st.setRes(res);
                target = 1;
                break;
            }
        }
    }
    if (!target) {target_st.emplace_back(*this);} // ! TODO finish the constructor
}
void State_transfer::create_remaining_states(std::vector<State> &rem_st, std::vector<State> &ori_st){ // ? merge with calculate_remaining_resources
    bool found{};
    for(State st : rem_st) {
        if (st.getName() == this->getName()) {
            found = 1;
            break;
        }
    }
    if(!found) {rem_st.emplace_back(ori_st[this->origin_pos]);}
}
void State_transfer::calculate_remaining_resources(std::vector<State> &rem_st/*, std::vector<State> &ori_st*/) {
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

                        for(auto pop : co.getPops()) { // pop subtracting
                            for(auto tr_pop : tr_co.getPops()) {
                                if(pop.getCult() == tr_pop.getCult() && pop.getRel() == tr_pop.getRel()) {
                                    pop.setSize(pop.getSize() - tr_pop.getSize()); // TODO refactor using operator overlading
                                    break;
                                }    
                            }
                        }
                        for(auto build : co.getBuilds()) { // building subtracting
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
            }
            int res[11]{};
            for(int i{}; i < 11; i++) {res[i] = st.getRes()[i] - this->getRes()[i];}
            st.setRes(res);
            break;
        }
    }
}

    // ! TODO finish
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

/**** TARGET STATE CONSTRUCTOR ****/
// State::State(State_transfer &trs) :name{trs.getName()}, id{trs.getId()}, traits{trs.getTraits()}, ar_res{trs.getResources()}, homelands{trs.getHomelands()},  claims{trs.getClaims()}, sub{trs.getSub()}, ar_land{trs.getLand()}, res{*trs.getRes()}, countries{trs.getCountries()} {}

// debug functions
// void State_transfer::debug_print_provs() {
//     std::ofstream  dst("provs_debug", std::ios::binary | std::ios::app);
//     int i{};
//     for (char c : this->getProvs()[0]) {
//         dst << c;
//         i++;
//     }
//     dst << " " << i << std::endl;
// }