#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "state.h"


// data maniopulation
// TODO try to use polymorphism here
std::string data(std::string &line){
    line.erase (std::remove(line.begin(), line.end(), ' '), line.end());
    int pos{line.find("=") + 1};
    return line.substr(pos, line.find ("\n") - pos);
}
std::string data(std::string &line, const char &delim){
    line.erase (std::remove(line.begin(), line.end(), ' '), line.end());
    line.erase (std::remove(line.begin(), line.end(), '='), line.end()); 
    line.erase (std::remove(line.begin(), line.end(), '{'), line.end());
    line.erase (std::remove(line.begin(), line.end(), '\t'), line.end());
    int pos{line.find(delim) + 1};
    return line.substr(pos, line.find ("\n") - pos);
}
unsigned int data_int( std::string line) {
    line.erase (std::remove(line.begin(), line.end(), ' '), line.end());
    int pos{line.find("=") + 1};
    std::string arg{line.substr(pos, line.find ("\n") - pos)};
    return stoi(arg);
}
void data_vector(std::vector<std::string> &vec, const std::string &line, int len) {
    int i{};

    for ( char c : line ) {
        if(c == 'x') {
            vec.push_back(line.substr(i + 1, len));
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
std::string data_name(std::string &line ) {
    int pos{};
    line.erase (std::remove(line.begin(), line.end(), ' '), line.end());
    line.erase (std::remove(line.begin(), line.end(), '='), line.end()); 
    line.erase (std::remove(line.begin(), line.end(), '{'), line.end());
    line.erase (std::remove(line.begin(), line.end(), '\t'), line.end());
    line.erase (std::remove(line.begin(), line.end(), '\"'), line.end());
    pos = line.find("_") + 1;
    std::string name{line.substr(pos, line.find ("\n") - pos)};
    return name;
}

// input/output paths
void save_i_o_path(/*std::string &input, std::string &output*/) { // TODO refactor
    std::filesystem::path in{}, out{};
    std::string line{};
    std::ofstream  dst("inoutpath.txt", std::ios::binary | std::ios::app);
    std::cout << "Enter the game path: " << std::endl;
    std::getline(std::cin, line);
    in = line;
    dst << "input: " << in << std::endl;
    std::cout << "Enter path to the mod: " << std::endl;
    std::getline(std::cin, line);
    out = line;
    dst << "output: " << out << std::endl;
}
void load_i_o_path(std::filesystem::path &in, std::filesystem::path &out) { // TODO refactor
    std::string line{};
    std::ifstream  src("inoutpath.txt", std::ios::binary);
    while(getline(src, line)){
        if(line.find("input: ") != std::string::npos) {
            // TODO find a better way to do this
            in = line.substr(8, line.length() - 10);
        }
        if(line.find("output: ") != std::string::npos) {
            // TODO find a better way to do this
            out = line.substr(9, line.length() - 11);
        }
    }
}
void check_i_o_file(std::filesystem::path &in, std::filesystem::path &out) {
    if (std::filesystem::exists("inoutpath.txt")){
        load_i_o_path(in, out);
    } else {
        save_i_o_path();
        load_i_o_path(in, out);
    }
}
void file_list(const std::filesystem::path &path, std::filesystem::path *files) { // TODO sort the list of files
    std::vector<std::filesystem::path>f;
    int i{};
    for (const std::filesystem::path& entry : std::filesystem::directory_iterator(path / "map_data/state_regions/")){
        if(entry.filename() == "99_seas.txt") {continue;}
        files[i] = entry;
        i++;
    }
}

// creating an array of states info //? TODO refactor using regex
void save_states(const std::filesystem::path &path, std::vector<State> &states) { // TODO tidy up (remove unnecesary strings in entries)
    std::string line{}, country{}, type{};
    std::vector<std::string> provs{}, homelands{}, claims{};
    std::ifstream  src(path / "common/history/states/00_states.txt", std::ios::binary);  

    while(getline(src, line)) {
        if(line.find("s:", 0) != std::string::npos) {
            std::string name{data_name(line)};
            states.emplace_back(name);
            int cur{states.size() - 1};
            while(getline(src, line)) {
                if(line.find("create_state", 0) != std::string::npos) {
                    getline(src, line);
                    country = data(line, ':');
                    while(getline(src, line)) {
                        data_vector(provs, line, 6);
                        if(line.find("}", 0) != std::string::npos){break;}
                    }
                    states[cur].create_country(country, provs);
                    provs.clear();
                    getline(src, line);
                    if(line.find("#", 0) != std::string::npos) {continue;}
                    if(line.find("state_type", 0) != std::string::npos) {
                        int pres{states[cur].getCountries().size() - 1};
                        states[cur].getCountries()[pres].setCountryType(data(line));
                    }
                }
                if(line.find("add_homeland", 0) != std::string::npos) {break;}
            }
            while(line.find("add_homeland", 0) != std::string::npos) {
                states[cur].setHomeland(data(line));
                getline(src, line);
            }
            while(line.find("add_claim", 0) != std::string::npos) {
                states[cur].setClaim(data(line));
                getline(src, line);
            }
        }
    }
}
void save_state_info(const std::filesystem::path &path, std::vector<State> &states, const std::filesystem::path *files) {
    std::filesystem::path regions{"map_data/state_regions"};

    std::filesystem::path regs[15]{};
    for(int i {}; i < 15; i++){regs[i] = path / regions / files[i].filename();}

    for(const auto &file : regs) {
        std::string line;
        std::ifstream  src(file, std::ios::binary);
        // int cap_res[12]{};
        std::string nav_ex{}, name{}, id{}, subsist{};
        std::vector<std::string> traits{}, ar_res{};
        while(getline(src, line)) {
            if(line.find("STATE") != std::string::npos) {
                name = data_name(line);
                for (State &st : states) {
                    int cap_res[12]{};
                    bool exit{};
                    if(st.getName() == name) {
                        while(getline(src, line)) {
                            if(line.find("id", 0) != std::string::npos) {
                                id = data(line);
                                st.setId(id);
                                getline(src, line);
                            }
                            if(line.find("subsistence_building", 0) != std::string::npos) {
                                subsist = data(line);
                                st.setSub(subsist);
                                while(getline(src, line)){
                                    if(line.find("}", 0) != std::string::npos) {
                                        getline(src, line); 
                                        if(line.find("impassable", 0) != std::string::npos) { // TODO implement impassable provs handling
                                            getline(src, line); 
                                            break;
                                        }
                                        break;
                                    }
                                }
                            }
                            if(line.find("traits", 0) != std::string::npos) {
                                if(traits.empty() != true) {traits.clear();}
                                variable_string_vector(traits, line);
                                st.setTraits(traits);
                                getline(src, line);
                            }
                            // ? TODO implement copying of hubs here
                            // TEMP CODE
                            if(line.find("city", 0) != std::string::npos) {getline(src, line);}
                            if(line.find("port", 0) != std::string::npos) {getline(src, line);}
                            if(line.find("farm", 0) != std::string::npos) {getline(src, line);}
                            if(line.find("mine", 0) != std::string::npos) {getline(src, line);}
                            if(line.find("wood", 0) != std::string::npos) {getline(src, line);}
                            // END OF TEMP CODE
                            if(line.find("arable_land", 0) != std::string::npos) {
                                cap_res[0] = data_int(line);
                                getline(src, line);
                            } 
                            if(line.find("arable_resources", 0) != std::string::npos) {
                                if(ar_res.empty() != true) {ar_res.clear();}
                                variable_string_vector(ar_res, line);
                                st.setArRes(ar_res);
                                getline(src, line);
                            }
                            if(line.find("capped_resources", 0) != std::string::npos) {
                                while(getline(src, line)) {
                                    if(line.find("bg_coal_mining", 0) != std::string::npos){cap_res[1] = data_int(line); getline(src, line);}
                                    if(line.find("bg_iron_mining", 0) != std::string::npos){cap_res[2] = data_int(line); getline(src, line);}
                                    if(line.find("bg_lead_mining", 0) != std::string::npos){cap_res[3] = data_int(line); getline(src, line);}
                                    if(line.find("bg_sulfur_mining", 0) != std::string::npos){cap_res[4] = data_int(line); getline(src, line);}
                                    if(line.find("bg_logging", 0) != std::string::npos){cap_res[5] = data_int(line); getline(src, line);}
                                    if(line.find("bg_fishing", 0) != std::string::npos){cap_res[6] = data_int(line); getline(src, line);}
                                    if(line.find("bg_whaling", 0) != std::string::npos){cap_res[7] = data_int(line); getline(src, line);}
                                    if(line.find("}", 1) != std::string::npos){getline(src, line); break;}
                                }
                            }
                            if(line.find("resource ", 0) != std::string::npos) {
                                while(getline(src, line)){
                                    if(line.find("bg_gold_fields", 0) != std::string::npos) {
                                        while(getline(src, line)) {
                                            if(line.find("undiscovered_amount", 0) != std::string::npos) {cap_res[8] = data_int(line);}
                                            if(line.find("discovered_amount", 0) != std::string::npos) {cap_res[9] = data_int(line);}
                                            if(line.find("}", 1) != std::string::npos){getline(src, line); break;}
                                        }
                                    }    
                                    if(line.find("bg_rubber", 0) != std::string::npos) {
                                        while(getline(src, line)) {
                                            if(line.find("undiscovered_amount", 0) != std::string::npos) {cap_res[10] = data_int(line);}
                                            if(line.find("}", 1) != std::string::npos){getline(src, line); break;}
                                        }
                                    }
                                    if(line.find("bg_oil_extraction", 0) != std::string::npos) {
                                        while(getline(src, line)) {
                                            if(line.find("undiscovered_amount", 0) != std::string::npos) {cap_res[11] = data_int(line);}
                                            if(line.find("}", 1) != std::string::npos){getline(src, line); break;}
                                        }
                                    }
                                    if(line.find("}", 0) != std::string::npos){break;}
                                }
                            }
                            st.setRes(cap_res);
                            
                            if(line.find("naval_exit_id", 0) != std::string::npos) {
                                nav_ex = data(line);
                                st.setNavEx(nav_ex);
                                // getline(src, line);
                            }
                            // if(line.size() == 1);
                            if(line.find("}", 0) != std::string::npos){
                                // getline(src, line);
                                exit = 1;
                                break;
                            }
                        }
                    }
                    if(exit) {break;}
                }  
            }
        }
    }
}   
void save_state_pops(const std::filesystem::path &path, std::vector<State> &states, const std::filesystem::path *files) {
    std::filesystem::path pops{"common/history/pops"};
    std::filesystem::path ps[15]{};
    for(int i {}; i < 15; i++){ps[i] = path / pops / files[i].filename();}
    
    for(const auto &file : ps) { 
        std::ifstream  src(file, std::ios::binary);
        std::string line;
        std::string name{}, country{};
        getline(src, line);
        while(getline(src, line)) { 
            int size{};
            std::string type{}, cult{}, rel{};
            if(line.find("s:", 0) != std::string::npos) { 
                name = data_name(line);
                getline(src, line);
            }
            if(line.find("region_state", 0) != std::string::npos) { 
                country = data(line, ':');
                getline(src, line);
                getline(src, line);   
            }
            if(line.find("pop_type", 0) != std::string::npos) {
                type = data(line);
                getline(src, line);
            }
            if(line.find("culture", 0) != std::string::npos) {
                cult = data(line);
                getline(src, line);
            }
            if(line.find("religion", 0) != std::string::npos) {
                rel = data(line);
                getline(src, line);
            }
            if(line.find("size", 0) != std::string::npos) {
                size = data_int(line);
                for (State &st : states) {
                    if(st.getName() == name) {
                        st.create_pops(country, cult, rel, type, size);
                    }
                }
                size = 0;
                type = "";
                cult = "";
                rel = "";
                getline(src, line);
            }
        }
    }
}
void save_state_builds(const std::filesystem::path &path, std::vector<State> &states, const std::filesystem::path *files) {
    std::filesystem::path builds{"common/history/buildings"};
    std::filesystem::path blds[15]{};
    for(int i {}; i < 15; i++){blds[i] = path / builds / files[i].filename();}
    
    for(const auto &file : blds) { 
        std::ifstream  src(file, std::ios::binary);
        std::string line;
        std::string name{}, country{};
        getline(src, line);
        while(getline(src, line)) { 
            int lvl{}, res{};
            std::string type{};
            std::vector<std::string> pm{};
            if(line.find("s:", 0) != std::string::npos) { 
                name = data_name(line);
                getline(src, line);
            }
            if(line.find("region_state", 0) != std::string::npos) { 
                country = data(line, ':');
                getline(src, line);
                // getline(src, line);
            }
            if(line.find("create_building", 0) != std::string::npos){getline(src, line);}   
            if(line.find("building", 0) != std::string::npos) {
                type = data(line);
                getline(src, line);
            }
            if(line.find("level", 0) != std::string::npos) {
                lvl = data_int(line);
                getline(src, line);
            }
            if(line.find("reserves", 0) != std::string::npos) {
                res = data_int(line);
                getline(src, line);
            }
            if(line.find("activate_production_methods", 0) != std::string::npos) {
                variable_string_vector(pm, line);
                for (State &st : states) {
                    if(st.getName() == name) {
                        st.create_buildings(country, type, lvl, res, pm);
                    }
                }
                type = "";
                lvl = 0;
                res = 0;
                pm.clear();
                getline(src, line);
            }
        }
    }
}

// transfer info input
std::vector<State_transfer> new_state_info() {
    int num{};
    std::string name{}, provs{}, id{};
    std::vector<State_transfer> states;
    std::cout << "Number of states to be created: " << std::endl;
    std::cin >> num;
    for(int i{}; i < num; i++) {
        std::cout << std::endl;
        std::cout << "Name of the state to be created number " << i + 1 << ": " << std::endl;
        std::cin >> name;
        std::cout << "Id of the state to be created number " << i + 1 << ": " << std::endl;
        std::cin >> id;
        std::cout << "Provinces to transfer to state number " << i + 1 << ": ";
        std::getline(std::cin, provs);
        std::cout << std::endl;
        states.emplace_back(name, id, provs);
    } 
    return states;
}
// TODO move inside Transfer_State class
void save_provinces(std::vector<std::string> &provs) {
    std::string prov, tmp; 
    std::cout<<"Enter provinces to transfer: "<<std::endl;
    std::getline(std::cin, prov);
    prov.erase (std::remove(prov.begin(), prov.end(), '\"'), prov.end());
    prov.erase (std::remove(prov.begin(), prov.end(), ' '), prov.end());
    transform(prov.begin(), prov.end(), prov.begin(), toupper);
    std::stringstream ss(prov);
    while(getline(ss, tmp, 'X')){
        if(tmp != "") {
        provs.push_back(tmp);
        }
    }
}

// debug functions 
// void debug(const std::vector<std::string> &provs) {
//     for(std::string s : provs) {
//         std::cout<< s << std::endl;
//     }
// }
void debug_print_file_list(const std::filesystem::path *files) {
    for (int i{}; i < 16; i++){
        std::cout << i << " " << files[i] << std::endl;
    }
}
void debug_print_state_pos(std::vector<State> &states) {
    std::ofstream  dst("output/debug/state_pos_list.txt", std::ios::binary);
    int pos{};
    for(State st : states) {
        dst << pos << " " << st.getName() << " " << st.getId() << std::endl;
        pos++;
    }
}



int main() {
// variables
    std::vector<State> states{}, remaining_states{};
    std::filesystem::path files[15], input{}, output{}/*, test[15]*/;
    std::vector<std::string> provinces{};
    std::vector<State_transfer> tr_states{}, tar_states{};
    std::string filename,/*path{"input/files"},*/ new_state_name{/*"NEW_STATE"*/}/*, strat_reg{}*/;
    int new_state_id{/*666*/}; 
    
    check_i_o_file(input, output);
    file_list(input, files);

    save_states(input, states);
    save_state_info(input, states, files);
    save_state_pops(input, states, files);
    save_state_builds(input, states, files);
    
    // debug_print_state_pos(states);

    tr_states = new_state_info();
    // save_provinces(provinces); 
    // filename = find_file(files, provinces[0]);

    for(int i {}; i < tr_states.size(); i++){tr_states[i].find_origin_states(states, tr_states);}
    for(int i {}; i < tr_states.size(); i++){tr_states[i].calculate_resources(states);}
    for(int i {}; i < tr_states.size(); i++){tr_states[i].create_target_states(tar_states);}
    for(int i {}; i < tr_states.size(); i++){tr_states[i].create_remaining_states(remaining_states, states);}
    for(int i {}; i < tr_states.size(); i++){tr_states[i].calculate_remaining_resources(remaining_states);}
    for(int i {}; i < tar_states.size(); i++){ // target state printing
        tar_states[i].print_state_region();
        tar_states[i].print_pops();
        tar_states[i].print_buildings();
        tar_states[i].print_state();
    }
    for(int i {}; i < remaining_states.size(); i++) { // state remains printing
        remaining_states[i].print_state_region();
        remaining_states[i].print_pops();
        remaining_states[i].print_buildings();
        remaining_states[i].print_state();
    }

    // debug( provinces );
    
    return 0;
}