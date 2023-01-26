#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "state.h"

// constructors
State::State(const unsigned int &cur_line, const std::string &file) {
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
State::Pop::Pop(const std::string &cult, const std::string &rel, const std::string &t, const int &s)
    : culture{cult}, religion{rel}, type{t}, size{s} {}
State::Building::Building(const std::string &type, const std::string &reg, const std::string &dlc, const std::string &pr_m, const int &lvl, const int &res) 
    : type{type}, region{reg}, dlc{dlc}, prod_med{pr_m}, level{lvl}, reserves{res} {}
// pops
void State::copy_pops(const std::string &file/*, const std::string &name, std::vector<State::Pop> &vec*/) {
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
// Setters
void State::setName(const std::string &n) {
    this->name = n;
}
void State::setId(const int &i) {
    this->id = i;
}
void State::setProvs(const std::vector<std::string> &p) {
    this->provs = p;
}
void State::setProv(const int &i, const std::string &s) {
    this->provs[i] = s;
}
void State::setPopSize(const int &i, const int &size) {
    this->pops[i].setSize(size);
}
void State::Pop::setSize(const int &size) {
    this->size = size;
}
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
