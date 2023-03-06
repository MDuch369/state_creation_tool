#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include "state.h"


// data maniopulation
// TODO try to use function overloading here
std::string data(std::string &line){
    line.erase (std::remove(line.begin(), line.end(), ' '), line.end());
    int pos{line.find("=") + 1};
    return line.substr(pos, line.find ("\n") - pos);
}
std::string data(std::string &line, const char &delim){ //? TODO refactor using regex
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
/*bool find_line(const std::string &str, const std::string &line) {
    if(line.find(str, 0) != std::string::npos) {
        return true;
    }
    return false
} */

// text parsing
bool find_string(std::string &line, const std::string &str) {
    if(line.find(str, 0) != std::string::npos) { 
        return true; 
    }
    return false;
}
bool find_string(std::string &line, const std::string &str, const int &pos) {
    if(line.find(str, pos) != std::string::npos) { 
        return true; 
    }
    return false;
}
void find_name(std::ifstream &src, std::string &line, const std::string &str, std::string &result) { // ? use this function for other stuff
    if(line.find(str, 0) != std::string::npos) { 
        result = data_name(line);
    }
}
void find_data(std::ifstream &src, std::string &line, const std::string &str, std::string &result) { // ? use this function for other stuff
    if(line.find(str, 0) != std::string::npos) { 
        result = data(line);
    }
}
void find_data(std::ifstream &src, std::string &line, const std::string &str, const char &delim, std::string &result) { // ? use this function for other stuff
    if(line.find(str, 0) != std::string::npos) { 
        result = data(line, delim);
    }
}
// int find_data_int(std::ifstream &src, std::string &line, const std::string &str) { // ? use this function for other stuff
//     int ret{};
//     if(line.find(str, 0) != std::string::npos) { 
//         ret = data_int(line);
//     }
//     return ret;
// }
void find_data_int(std::ifstream &src, std::string &line, const std::string &str, int &result) { // ? use this function for other stuff
    if(line.find(str, 0) != std::string::npos) { 
        result = data_int(line);
    }
}

// input/output paths
void save_i_o_path() { // TODO add output path handling
    std::string line{};
    std::ofstream  in_dst("inputpath.txt", std::ios::binary | std::ios::app);
    // std::ofstream  out_dst("outputpath.txt", std::ios::binary | std::ios::app);
    std::cout << "Enter the game path: " << std::endl;
    std::getline(std::cin, line);
    in_dst << line << std::endl;
/*
    std::cout << "Enter path to the mod: " << std::endl;
    std::getline(std::cin, line);
    out = line.erase (std::remove(line.begin(), line.end(), '\"'), line.end());
    out_dst << "output: " << out << std::endl;
*/
}
void load_i_o_path(std::filesystem::path &in, std::filesystem::path &out) {
    std::string line{};
    std::ifstream  in_src("inputpath.txt", std::ios::binary);
    std::ifstream  out_src("outputpath.txt", std::ios::binary);
    while(getline(in_src, line)){
            in = line;
    }
    while(getline(out_src, line)){
            out = line;
    }
}
void check_i_o_file(std::filesystem::path &in, std::filesystem::path &out) { // TODO add output path handling
    if (std::filesystem::exists("inputpath.txt") /*&& std::filesystem::exists("outputpath.txt")*/){
        load_i_o_path(in, out);
    } else {
        save_i_o_path();
        load_i_o_path(in, out);
    }
}
void add_i_o_path(const char &io) {
    std::string line{};
    // std::filesystem::path path{};
    std::ofstream  in_dst("inputpath.txt", std::ios::binary | std::ios::app);
    std::ofstream  out_dst("ouputpath.txt", std::ios::binary | std::ios::app);
    std::cout << "Enter path to save: " << std::endl;
    // std::cin >> line;
    // std::getline(std::cin, line);
    std::getline(std::cin, line);
    in_dst << line << std::endl;
    // switch (io)
    // {
    // case 'i':
    //     in_dst << line << std::endl;
    //     break;
    // case 'o':
    //     out_dst << line << std::endl;
    //     break;
    // default:
    //     break;
    // }
    // return 1;
}
void list_i_o_paths(const char &io) {
    std::string line{};
    std::ifstream  in_src("inputpath.txt", std::ios::binary);
    std::ifstream  out_src("outputpath.txt", std::ios::binary);
    int in{}, out{};
    
    switch (io){
    case 'i':
        std::cout << "input paths: " << std::endl;
        while (getline(in_src, line)){
            std::cout << in <<  " " << line << std::endl;
            in++;
        } 
        break;
    case 'o':
        std::cout << "output paths: " << std::endl;
        while (getline(out_src, line)){
            std::cout << out << " " <<  line << std::endl;
            out++;
        }
        break;
    default:
        break;
    }
     
}
std::filesystem::path change_path(const char &io) { 
    int path_num{};
    std::string line{};
    std::ifstream  in_src("inputpath.txt", std::ios::binary);
    std::ifstream  out_src("outputpath.txt", std::ios::binary);
    std::cout << "select new path" << std::endl;

    list_i_o_paths(io);
    std::cin >> path_num;
    int i{};
    while (getline(in_src, line)) {
        if (i == path_num) {break;}
        i++;
    }
    
    return line;
}
void list_current_i_o_paths(std::filesystem::path &in, std::filesystem::path &out) {
    std::cout << "current input path: " << in << std::endl;
    std::cout << "current output path: " <<  out << std::endl;
}
// void file_list(const std::filesystem::path &path, std::filesystem::path *files) { // TODO sort the list of files
//     std::vector<std::filesystem::path>f;
//     int i{};
//     for (const std::filesystem::path& entry : std::filesystem::directory_iterator(path / "map_data/state_regions/")){
//         if(entry.filename() == "99_seas.txt") {continue;}
//         files[i] = entry;
//         i++;
//     }
// }
void /*change_*/file_list(const std::filesystem::path &path, std::filesystem::path *files) { // TODO sort the list of files
    std::vector<std::filesystem::path>f;
    int i{};
    for (const std::filesystem::path& entry : std::filesystem::directory_iterator(path / "map_data/state_regions/")){
        if(entry.filename() == "99_seas.txt") {continue;}
        files[i].assign(entry);
        i++;
    }
}

// menu options
bool menu(std::filesystem::path &in, std::filesystem::path &out) {
    char action{};
    std::cout << std::endl;
    std::cout << "Select an action: " << std::endl;
    std::cout << "1 - create states" << std::endl;
    std::cout << "2 - list current input and output path" << std::endl;
    std::cout << "3 - list saved input and output paths" << std::endl;
    std::cout << "4 - add new input path" << std::endl;
    std::cout << "5 - add new output path " << std::endl;
    std::cout << "6 - change current input path" << std::endl;
    std::cout << "7 - change current output path" << std::endl;
    // std::cout << "[e]xit" << std::endl;
    std::cin >> action; 
    std::cout << std::endl;
    switch (action) {
    case '1':
        return false;
    case '2':
        list_current_i_o_paths(in, out);
        return true;
    case '3':
        list_i_o_paths('i');
        list_i_o_paths('o');
        return true;
    case '4':
        add_i_o_path('i');
        // return add_i_o_path('i');
        return true;

    case '5':
        add_i_o_path('o');
        // return add_i_o_path('o');
        return true;
    case '6':
        (in = change_path('i'));
        return true;
    case '7':
        (out = change_path('o'));
        return true;
    // case 'e':
    //     exit();
    default:
        return true;
    }
}

// creating an array of states info //? TODO refactor using regex, create a class for the functions
std::ifstream::pos_type create_state(std::ifstream &src, std::vector<State> &states, std::string &line, const int &cur){
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
            states[cur].create_country(country, provs);
            provs.clear();
            getline(src, line);
            if(find_string(line, "#")) {
                continue;
            }
            if(find_string(line, "state_type")) {
                int pres{states[cur].getCountries().size() - 1};
                states[cur].getCountries()[pres].setCountryType(data(line));
            }
        }
        if(find_string(line, "add_homeland")) {
            break;
        }
    }
    return src.tellg();
}
std::ifstream::pos_type add_homelands(std::ifstream &src, std::vector<State> &states, std::string &line, const int &cur) {
    while(find_string(line, "add_homeland")) {
        states[cur].setHomeland(data(line));
        getline(src, line);
    }
    return src.tellg();
}
std::ifstream::pos_type add_claims(std::ifstream &src, std::vector<State> &states, std::string &line, const int &cur) {
    while(find_string(line, "add_claim")) {
        states[cur].setClaim(data(line));
        getline(src, line);
    } 
    return src.tellg();
}
void save_states(const std::filesystem::path &path, std::vector<State> &states) { 
    std::string line{}, country{}, type{};
    std::vector<std::string> provs{}, homelands{}, claims{};
    std::ifstream  src(path / "common/history/states/00_states.txt", std::ios::binary);  

    while(getline(src, line)) {
        if(find_string(line, "s:")) {
            std::string name{data_name(line)};
            states.emplace_back(name);
            int cur{states.size() - 1};
            src.seekg(create_state(src, states, line, cur));
            src.seekg(add_homelands(src, states, line, cur));
            src.seekg(add_claims(src, states, line, cur));
        }
    }
}
// state info saving
std::ifstream::pos_type save_gold_fields(std::ifstream &src, int *cap_res, std::string &line) {
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
std::ifstream::pos_type save_rubber(std::ifstream &src, int *cap_res, std::string &line) {
    while(getline(src, line)) {
        find_data_int(src, line, "undiscovered_amount", cap_res[10]);
        if(find_string(line, "}", 1)){
            getline(src, line);
            break;
        }
    }
    return src.tellg();
}
std::ifstream::pos_type save_oil(std::ifstream &src, int *cap_res, std::string &line) {
    while(getline(src, line)) {
        find_data_int(src, line, "undiscovered_amount", cap_res[11]);
        if(find_string(line, "}")){
            getline(src, line); break;
        }
    }
    return src.tellg();
}
std::ifstream::pos_type save_resources(std::ifstream &src, int *cap_res, std::string &line) {
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
std::ifstream::pos_type save_capped_resources(std::ifstream &src, int *cap_res, std::string &line) {
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
std::ifstream::pos_type save_subsistence_building(std::ifstream &src, std::string &line, std::string &subsist) {
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
std::ifstream::pos_type save_state(std::ifstream &src, State &st, std::string &line){
    int cap_res[12]{};
    std::string nav_ex{}, name{}, id{}, subsist{};
    std::vector<std::string> traits{}, ar_res{};
    while(getline(src, line)) {
        if(find_string(line, "id")) {
            id = data(line);
            st.setId(id);
            getline(src, line);
        }
        if(find_string(line, "subsistence_building")) {
            
            src.seekg(save_subsistence_building(src, line, subsist));
            st.setSub(subsist);
        }
        if(find_string(line, "traits")) {
            if(traits.empty() != true) {
                traits.clear();
            }
            variable_string_vector(traits, line);
            st.setTraits(traits);
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
            st.setArRes(ar_res);
            getline(src, line);
        }
        if(find_string(line, "capped_resources")) {
            src.seekg(save_capped_resources(src, cap_res, line));
        }
        if(find_string(line, "resource ")) {
            src.seekg(save_resources(src, cap_res, line));
        }
        st.setRes(cap_res);
        if(find_string(line, "naval_exit_id")) {
            nav_ex = data(line);
            st.setNavEx(nav_ex);
        }
        if(find_string(line, "}")){
            break;
        }
    }
    return src.tellg();
}
void browse_file(std::ifstream &src, std::vector<State> &states) { // TODO denest
    std::string line, name{};
    
    while(getline(src, line)) {
        if(find_string(line, "STATE")) {
            name = data_name(line);
            for (State &st : states) {
                if(st.getName() == name) {
                    src.seekg(save_state(src, st, line));
                }
            }  
        }
    }
}
void save_state_info(const std::filesystem::path &path, std::vector<State> &states, const std::filesystem::path *files) { 
    std::filesystem::path regions{"map_data/state_regions"};

    std::filesystem::path regs[15]{};
    for(int i {}; i < 15; i++){regs[i] = path / regions / files[i].filename();}

    for(const auto &file : regs) {
        std::ifstream  src(file, std::ios::binary);
        browse_file(src, states);
    }
} 
//   pops saving
std::ifstream::pos_type save_pop(std::ifstream &src, std::vector<State> &states, const std::string &name, const std::string &co, const std::string &cu, const std::string &r, const std::string &t, const int &s) { // ! TODO create a separate class for the parameters 
    for (State &st : states) {
        if(st.getName() == name) {
            st.create_pops(co, cu, r, t, s);
        }
    }
    return src.tellg();
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
                src.seekg(save_pop(src, states, name, country, cult, rel, type, size));
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
std::ifstream::pos_type save_building(std::ifstream &src, std::vector<State> &states, const std::string &name, const std::string &co, const std::string &t, const int &l, const int &r, std::vector<std::string> &pm) { // ! TODO create a separate class for the parameters 

    for (State &st : states) {
        if(st.getName() == name) {
            st.create_buildings(co, t, l, r, pm);
        }
    }
    return src.tellg();
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
                src.seekg(save_building(src, states, name, country, type, lvl, res, pm));
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

void save_provinces(std::vector<std::string> &provs) { // TODO move inside Transfer_State class
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

// debug functions //TODO create a class for this functions
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
    std::filesystem::path files[15], input{}, output{};
    std::vector<std::string> provinces{};
    std::vector<State_transfer> tr_states{}, tar_states{};
    std::string filename, new_state_name{}/*, strat_reg{}*/;
    int new_state_id{}; 
    
    check_i_o_file(input, output);
    file_list(input, files);

    while(menu(input, output)) {continue;} 

    save_states(input, states);
    save_state_info(input, states, files);
    save_state_pops(input, states, files);
    save_state_builds(input, states, files);
    

    tr_states = new_state_info();

    for(int i {}; i < tr_states.size(); i++){tr_states[i].find_origin_states(states, tr_states);}
    for(int i {}; i < tr_states.size(); i++){tr_states[i].calculate_resources(states);}
    for(int i {}; i < tr_states.size(); i++){tr_states[i].create_target_states(tar_states);}
    for(int i {}; i < tr_states.size(); i++){tr_states[i].create_remaining_states(remaining_states, states);}
    for(int i {}; i < tr_states.size(); i++){tr_states[i].calculate_remaining_resources(remaining_states);}
    for(int i {}; i < tar_states.size(); i++){ tar_states[i].print_entry();}
    for(int i {}; i < remaining_states.size(); i++) {remaining_states[i].print_entry();}
    
    return 0;
}