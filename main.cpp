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

// creating an array of states info
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
                if(line.find("add_homeland", 0) != std::string::npos) {
                    break;
                }
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
    // std::string line;
    // int id{};
    std::filesystem::path regions{"map_data/state_regions"};

    std::filesystem::path regs[15]{};
    for(int i {}; i < 15; i++){regs[i] = path / regions / files[i].filename();}

    for(const auto &file : regs) {
        std::string line;
        std::ifstream  src(file, std::ios::binary);
        while(getline(src, line)) {
            if(line.find("STATE", 0) != std::string::npos) {
                int cap_res[10]{};
                std::string name{data_name(line)}; 
                getline(src, line);
                std::string id{data(line)};
                getline(src, line);
                std::string subsist{data(line)};
                getline(src, line);
                // std::vector<std::string> provs{};
                // data_vector(provs, line, 6);
                getline(src, line);
                std::vector<std::string> traits{};
                variable_string_vector(traits, line);
                // getline(src, line);
                // ! TODO implement copying of hubs here
                while(getline(src, line)) { if(line.find("arable_land", 0) != std::string::npos) {break;} }
                int ar_land{};
                ar_land = data_int(line);
                getline(src, line);
                std::vector<std::string> ar_res{};
                variable_string_vector(ar_res, line);
                getline(src, line);
                while(getline(src, line)){
                    if(line.find("bg_coal_mining", 0) != std::string::npos){cap_res[0] = data_int(line);}
                    if(line.find("bg_iron_mining", 0) != std::string::npos){cap_res[1] = data_int(line);}
                    if(line.find("bg_sulfur_mining", 0) != std::string::npos){cap_res[2] = data_int(line);}
                    if(line.find("bg_logging", 0) != std::string::npos){cap_res[3] = data_int(line);}
                    if(line.find("bg_whaling", 0) != std::string::npos){cap_res[4] = data_int(line);}
                    if(line.find("bg_fishing", 0) != std::string::npos){cap_res[5] = data_int(line);}
                    if(line.find("bg_gold_fields", 0) != std::string::npos){
                        getline(src, line);
                        getline(src, line);
                        if(line.find("undiscovered_amount", 0) != std::string::npos) {cap_res[6] = data_int(line);}
                        getline(src, line);
                        if(line.find("discovered_amount", 0) != std::string::npos) {cap_res[7] = data_int(line);}
                    }
                    if(line.find("bg_rubber", 0) != std::string::npos){
                        getline(src, line);
                        if(line.find("undiscovered_amount", 0) != std::string::npos) {cap_res[8] = data_int(line);}
                    }
                    if(line.find("bg_oil_extraction", 0) != std::string::npos) {
                        getline(src, line);
                        if(line.find("undiscovered_amount", 0) != std::string::npos) {cap_res[9] = data_int(line);}
                    }
                    // getline(src, line);
                    if(line.find("naval_exit_id", 0) != std::string::npos) {std::string nav_ex{data(line)};}
                    if(line.find("", 0) != std::string::npos) {break;}
                // ! TODO add naval exit saving 
                }
                // saving the data
                for (State &st : states) {
                    // std::ofstream  dst("debug_state_list.txt", std::ios::binary | std::ios::app);
                    if(st.getName() == name) {
                        st.setId(id);
                        st.setSub(subsist);
                        st.setTraits(traits);
                        st.setLand(ar_land);
                        st.setArRes(ar_res);
                        st.setRes(cap_res);
                    }
                    // dst.close();
                }
            }
        }
        // src.close();
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
                getline(src, line);   
            }
            if(line.find("building", 0) != std::string::npos) {
                type = data_name(line);
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
        std::cout << "Name of the state to be created number " << i + 1 << ": " << std::endl;
        std::cin >> name;
        // std::cout << std::endl;
        std::cout << "Id of the state to be created number " << i + 1 << ": " << std::endl;
        std::cin >> id;
        // std::cout << std::endl;
        std::cout << "Provinces to transfer to state number " << i + 1 << ": "/* << std::endl*/;
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

// file browsing 
unsigned int find_states(const std::string &path, const std::string &prov) {
    unsigned int cur_line{};
    std::string line{};
    std::ifstream  src(path + "common/history/states/00_states.txt", std::ios::binary);

    while(getline(src, line)) {
        cur_line++;
        if (line.find(prov, 0) != std::string::npos) {
            break;
        }
    }
    src.close();
    return cur_line;
}
//  move inside class
// std::string find_file(std::filesystem::path *files, const std::string &prov ) {
//     std::string line{};
//     // std::ifstream  src(file, std::ios::binary);
//     for(int i{}; i < 15; i++) {
//         std::ifstream src(files[i], std::ios::binary);
//         while(getline(src, line)) {
//             if (line.find(prov, 0) != std::string::npos) {
//                 // break;
//                 return files[i].filename();
//             }
//         }
//         src.close();
//     }
// }
// state data 
    // TODO all the funcions below should be moved inside class in next version
// void print_state_region(State &state){
//     std::ofstream  dst("output/map_data/state_regions/to.txt", std::ios::binary | std::ios::app);
//     dst << state.getName() << "= {" << std::endl
//         << "\tid = " << state.getId() << std::endl
//         << "\tsubsistence_building = " << state.getSub() << std::endl
//         << "\tprovinces = { ";
//     for (std::string s : state.getProvs()) {
//         dst << "\"x" << s << "\" ";
//     }
//     dst << "}" << std::endl 
//         << "\ttraits = { "; 
//     for (std::string s : state.getTraits()) {
//         dst << "\"" << s << "\" ";
//     }
//     dst << "}" << std::endl 
//         << "\tcity = " << state.getCity() << std::endl;
//     if(state.getPort() != "") {
//         dst<< "\tport = " << state.getPort() << std::endl;
//     }
//     dst << "\tfarm = " << state.getFarm() << std::endl
//         << "\tmine = " << state.getMine() << std::endl
//         << "\twood = " << state.getWood() << std::endl
//         << "\tarable_land = " << state.getLand() << std::endl
//         << "\tarable_resources = { ";
//     for (std::string s : state.getResources()) {
//         dst << "\"" << s << "\" ";
//     } 
//     dst << "}" << std::endl
//         << "\tcapped = {" << std::endl;
//     if(state.getIron() != 0) {
//         dst << "\t\tbg_iron_mining = " << state.getIron() << std::endl; 
//     }
//     if(state.getCoal() != 0) {
//         dst << "\t\tbg_coal_mining = " << state.getCoal() << std::endl; 
//     }
//     if(state.getLead() != 0) {
//         dst << "\t\tbg_lead_mining = " << state.getLead() << std::endl; 
//     }
//     if(state.getSulfur() != 0) {
//         dst << "\t\tbg_sulfur_mining = " << state.getSulfur() << std::endl; 
//     }
//     if(state.getLog() != 0) {
//         dst << "\t\tbg_logging = " << state.getLog() << std::endl; 
//     }
//     if(state.getFish() != 0) {
//         dst << "\t\tbg_fishing = " << state.getFish() << std::endl; 
//     }
//     if(state.getWhale() != 0) {
//         dst << "\t\tbg_whaling = " << state.getWhale() << std::endl; 
//     }
//     if(state.getOil() != 0) {
//         dst << "\t\tbg_oil_extraction = " << state.getOil() << std::endl; 
//     }
//     if(state.getRubber() != 0) {
//         dst << "\t\tbg_logging = " << state.getRubber() << std::endl; 
//     }
//     if(state.getGold() != 0) {
//         dst << "\t\tbg_gold_mining = " << state.getGold() << std::endl; 
//     }
//     if(state.getDiscGold() != 0) {
//         dst << "\t\tbg_whaling = " << state.getDiscGold() << std::endl; 
//     }
//     dst << "\t}" << std::endl;
//     if(state.getNavalExit() != 0) {
//        dst << "\tnaval_exit_id = " << state.getNavalExit() << std::endl;
//     }
//     dst << "}" << std::endl << std::endl;
// }
// void print_pops(State &state){
//     std::ofstream  dst("output/common/history/pops/to_pops.txt", std::ios::binary | std::ios::app);
//     dst << "\ts:" << state.getName() << " = {" << std::endl
//         << "\t\tregion_state:ABC = {" << std::endl;
//     for(int i{}; i < state.getPops().size(); i++){
//         dst << "\t\t\tcreate_pop = {" << std::endl;
//         if (state.getPops()[i].getType() != "") {
//             dst << "\t\t\t\tpop_type = " << state.getPops()[i].getType() << std::endl;
//         }
//         dst << "\t\t\t\tculture = " << state.getPops()[i].getCult() << std::endl;
//         if (state.getPops()[i].getRel() != "") {
//             dst << "\t\t\t\treligion = " << state.getPops()[i].getRel() << std::endl;
//         }
//         dst << "\t\t\t\tsize = " << state.getPops()[i].getSize() << std::endl
//             << "\t\t\t}" << std::endl;
//     }
//     dst << "\t\t}" << std::endl << "\t}" << std::endl;
// }
// void print_buildings(State &state){
//     std::ofstream  dst("output/common/history/buildings/to_buildings.txt", std::ios::binary | std::ios::app);
//     int size{state.getBuildings().size()};
//     dst << "\ts:" << state.getName() << " = {" << std::endl;
//     dst << "\t\tregion_state:" << state.getBuildings()[0].getRegion() <<" = {" << std::endl;
//     for(int i{}; i < size; i++){
//         std::string reg{state.getBuildings()[i].getRegion()};
//         if(state.getBuildings()[i].getLvl() > 0 ) {
//             if(i > 0 && reg != state.getBuildings()[i-1].getRegion()) {
//                 dst << "\t\tregion_state:" << reg <<" = {" << std::endl;
//             }
//             dst << "\t\t\tcreate_building = {" << std::endl;
//             dst << "\t\t\t\tbuilding = " << state.getBuildings()[i].getType() << std::endl;
//             dst << "\t\t\t\tlevel = " << state.getBuildings()[i].getLvl() << std::endl;
//             dst << "\t\t\t\treserves = " << state.getBuildings()[i].getRes() << std::endl;
//             dst << "\t\t\t\tactivate_production_methods = " << state.getBuildings()[i].getProd() << std::endl << "\t\t\t}" << std::endl;
//         }
//         if(i < (size - 1) && reg != state.getBuildings()[i+1].getRegion() ) {
//             dst << "\t\t}" << std::endl;
//         }
//         if(i == size - 1) {
//             dst << "\t\t}" << std::endl;
//         }
//     }
//     dst << "\t}" << std::endl;
// }
// void print_state(State &state) {
//     std::ofstream  dst("output/common/history/to_states.txt", std::ios::binary | std::ios::app);
//     std::string placeholder{"[ABC]"};
//     dst << "\ts:" << state.getName() << " = {" << std::endl;
//     dst << "\t\tcreate_state = {" << std::endl
//         << "\t\t\tcountry = c:" << placeholder << std::endl
//         << "\t\t\towned_provinces = { ";
//     for(std::string s : state.getProvs()) {
//         dst << s << " ";
//     }
//     dst << "}" << std::endl
//         <<  "\t\t}" << std::endl;
//     for(std::string t : state.getHomelands()) {
//         dst << "\t\tadd_homeland = " << t << std::endl;
//     }
//     dst << "\t}" << std::endl;
// }
// double calculate_ratio(State &state, const std::vector<std::string> &provs) {
//     double i{provs.size()}, j{state.getProvs().size()};
//     double result{i / j};
//     return result;
// }

    // TODO refactor
// void calculate_pops(State &donor, State &state, const double &ratio) {
//     for(int i{}; i < donor.getPops().size(); i++) {
//         state.add_pop(donor.getPops()[i].getCult(), donor.getPops()[i].getRel(), donor.getPops()[i].getType(), donor.getPops()[i].getSize() * ratio);
//     }
// }
// void calculate_remaining_pops(State &donor, State &remain, State &state) {
//     for(int i{}; i < donor.getPops().size(); i++) {
//         // remain.setPopSize(i, donor.getPops()[i].getSize() - state.getPops()[i].getSize());
//         remain.add_pop(donor.getPops()[i].getCult(), donor.getPops()[i].getRel(), donor.getPops()[i].getType(), donor.getPops()[i].getSize() - state.getPops()[i].getSize());
//     }
// }
// // void calculate_buildings(State &donor, State &state, const double &ratio) {
// //     for(int i{}; i < donor.getBuildings().size(); i++) {
// //         double lvl = donor.getBuildings()[i].getLvl() * ratio;
// //         state.add_building(donor.getBuildings()[i].getType(), donor.getBuildings()[i].getRegion(), donor.getBuildings()[i].getDlc(), donor.getBuildings()[i].getProd(), donor.getBuildings()[i].getRes(), lvl);
// //     }
// // }
// // void calculate_remaining_buildings(State &donor, State &remain, State &state) {
// //     for(int i{}; i < donor.getBuildings().size(); i++) {
// //         // remain.setBuildingsize(i, donor.getBuildings()[i].getSize() - state.getBuildings()[i].getSize());
// //         remain.add_building(donor.getBuildings()[i].getType(), donor.getBuildings()[i].getRegion(), donor.getBuildings()[i].getDlc(), donor.getBuildings()[i].getProd(), donor.getBuildings()[i].getRes(), donor.getBuildings()[i].getLvl() - state.getBuildings()[i].getLvl());
// //     }
// // }
// State calculate_resources(State &donor, const double &ratio) {
//     unsigned int res[12]{};
//     res[0] = donor.getLand() * ratio;
//     if(donor.getCoal() != 0){
//         res[1] = donor.getCoal() * ratio;
//     } 
//     if(donor.getIron() != 0){
//         res[2] = donor.getIron() * ratio;
//     }
//     if(donor.getLead() != 0){
//         res[3] = donor.getLead() * ratio;
//     }
//     if(donor.getSulfur() != 0){
//         res[4] = donor.getSulfur() * ratio;
//     }
//     if(donor.getLog() != 0){
//         res[5] = donor.getLog() * ratio;
//     }
//     if(donor.getFish() != 0){
//         res[6] = donor.getFish() * ratio;
//     }
//     if(donor.getWhale() != 0){
//         res[7] = donor.getWhale() * ratio;
//     }
//     if(donor.getOil() != 0){
//         res[8] = donor.getOil() * ratio;
//     }
//     if(donor.getRubber() != 0){
//         res[9] = donor.getRubber() * ratio;
//     }
//     if(donor.getGold() != 0){
//         res[10] = donor.getGold() * ratio;
//     }
//     if(donor.getDiscGold() != 0){
//         res[11] = donor.getDiscGold() * ratio;
//     }
//     State St(res);
//     return St;
// }
// State calculate_remaining_resources(State &donor, State &state) {
//         unsigned int res[12]{};
//     res[0] = donor.getLand() - state.getLand();
//     if(donor.getCoal() != 0){
//         res[1] = donor.getCoal() - state.getCoal();
//     } 
//     if(donor.getIron() != 0){
//         res[2] = donor.getIron() - state.getIron();
//     }
//     if(donor.getLead() != 0){
//         res[3] = donor.getLead() - state.getLead();
//     }
//     if(donor.getSulfur() != 0){
//         res[4] = donor.getSulfur() - state.getSulfur();
//     }
//     if(donor.getLog() != 0){
//         res[5] = donor.getLog() - state.getLog();
//     }
//     if(donor.getFish() != 0){
//         res[6] = donor.getFish() - state.getFish();
//     }
//     if(donor.getWhale() != 0){
//         res[7] = donor.getWhale() - state.getWhale();
//     }
//     if(donor.getOil() != 0){
//         res[8] = donor.getOil() - state.getOil();
//     }
//     if(donor.getRubber() != 0){
//         res[9] = donor.getRubber() - state.getRubber();
//     }
//     if(donor.getGold() != 0){
//         res[10] = donor.getGold() - state.getGold();
//     }
//     if(donor.getDiscGold() != 0){
//         res[11] = donor.getDiscGold() - state.getDiscGold();
//     }
//     State St(res);
//     return St;
// }

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



int main() {
// variables
    std::vector<State> states{};
    std::filesystem::path files[15], input{}, output{}, test[15];
    std::vector<std::string> provinces{};
    std::vector<State_transfer> tr_states{};
    std::string filename,/*path{"input/files"},*/ new_state_name{/*"NEW_STATE"*/}/*, strat_reg{}*/;
    int new_state_id{/*666*/}; 
    double provs_ratio; // TODO for next version change this to be member of state class
    
    check_i_o_file(input, output);
    file_list(input, files);

    save_states(input, states);
    save_state_info(input, states, files);
    save_state_pops(input, states, files);
    save_state_builds(input, states, files);
    
    tr_states = new_state_info();
    // save_provinces(provinces); 
    // filename = find_file(files, provinces[0]);

    for(int i {}; i < tr_states.size(); i++){
        tr_states[i].find_origin_states(states, tr_states);
    }
    for(int i {}; i < tr_states.size(); i++){
        tr_states[i].calculate_resources(states);
    }
    // for(State_transfer &trs : tr_states) {
        // trs.find_origin_states(states, tr_states);
    // }

    State Old_state(find_states(files[14], provinces[0]), files[14]);
    // provs_ratio = calculate_ratio(Old_state, provinces);
    // Old_state.create_pops("input/pops/" + filename);
    // Old_state.create_buildings("input/buildings/" + filename);
    // Old_state.create_homelands("input/00_states.txt");
    // State New_state = calculate_resources(Old_state, provs_ratio);
    // State Remaining_state = calculate_remaining_resources(Old_state, New_state);

// setting new states info
    // New_state.setName(new_state_name);
    // // New_state.setId(new_state_id);
    // New_state.setProvs(provinces);
    // New_state.copy_state_info(Old_state);
    // Remaining_state.setName(Old_state.getName());
    // Remaining_state.setId(Old_state.getId());
    // Remaining_state.setProvs(Old_state.getProvs());
    // Remaining_state.calculate_remaining_provs(New_state);
    // Remaining_state.copy_state_info(Old_state);
// calculating pops
    // calculate_pops(Old_state, New_state, provs_ratio);
    // calculate_remaining_pops(Old_state, Remaining_state, New_state);
// calculating buildings
    // calculate_buildings(Old_state, New_state, provs_ratio);
    // calculate_remaining_buildings(Old_state, Remaining_state, New_state);
// printing states
    // Old_state.print_state_region(); //for debugging purposes
    // Remaining_state.print_state_region();
    // New_state.print_state_region();
    // Old_state.print_pops(); //for debugging purposes
    // Remaining_state.print_pops();
    // New_state.print_pops();
    // Old_state.print_buildings(); //for debugging purposes
    // Remaining_state.print_buildings();
    // New_state.print_buildings();
    // Old_state.print_state(); //for debugging purposes
    // Remaining_state.print_state();
    // New_state.print_state();


    // debug( provinces );
    
    return 0;
}