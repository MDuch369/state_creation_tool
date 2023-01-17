#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "state.h"

State::State(){}

State::State(const unsigned int &cur_line, const std::string &file) {
    unsigned int copy_line{}, len{};
    std::string line{}, d{};
    std::ifstream  src(file, std::ios::binary);
    while(getline(src, line)) {
        copy_line++;
        if (copy_line >= cur_line - 3) {
            if(compare_string("STATE", line)) { 
                this->name = line.substr(0, line.find (" ") + 1);
            }
            if(compare_string("id", line)) { 
                this->id = data_int(line);
            }
            if(compare_string("subsistence_building", line)) {
             this->sub = data(line);
            }
            if(compare_string("provinces", line)) {
                data_vector(this->provs, line, 7);
            }
            if(compare_string("impassable", line)) {
                data_vector(this->im_provs, line, 7);
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
            if(compare_string("traits", line)) {
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
            //      TODO FINISH DISCOVERABLE RESOURCES
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

std::string data(const std::string &line){
    int pos{line.find("=")};
    return line.substr(pos, line.find ("\n") - pos);
}

unsigned int data_int(const std::string &line) {
    int pos{line.find("=")};
    return stoi(line.substr(pos, line.find ("\n") - pos));
}

void data_vector(std::vector<std::string> &vec, const std::string &line, int len) {
    int i{}, j{};

    for ( char c : line ) {
        if(c == 'x') {
            vec[j] = line.substr(i, len);
            j++;
        }
        i++; 
    }
}

void variable_string_vector(std::vector<std::string> &t, std::string &line) {
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

bool compare_string(const std::string &str, std::string l) {
    l.erase (std::remove(l.begin(), l.end(), ' '), l.end());
    return str == l.substr(0, str.size());
}



// void save_data ( const std::string &line, std::ifstream src) {
//     getline(src, line);
// }
