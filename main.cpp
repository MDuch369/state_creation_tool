#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "state.h"

// void check_i_o_data() {
//     for (const auto & entry : std::filesystem::directory_iterator(path)){
//         // std::cout << i << " " << files[i] << std::endl;
//         i++;
//     }
// }

void save_i_o_path(/*std::string &input, std::string &output*/) {
    std::filesystem::path in{}, out{};
    std::ofstream  dst("inoutpath.txt", std::ios::binary | std::ios::app);
    std::cout << "Enter the game path: " << std::endl;
    std::cin >> in;
    dst << "input: " << in << std::endl;
    std::cout << "Enter path to the mod: " << std::endl;
    std::cin >> out;
    dst << "output: " << out << std::endl;
}

void load_i_o_path(std::filesystem::path &in, std::filesystem::path &out) {
    std::string line{};
    std::ifstream  src("inoutpath.txt", std::ios::binary);
    while(getline(src, line)){
        if(line.find("input: ") != std::string::npos) {
            in = line.substr(8, std::string::npos);
        }
        if(line.find("output: ") != std::string::npos) {
            out = line.substr(9, std::string::npos);
        }
    }
}

// TODO sort the list of files
void file_list(const std::string &path, std::string *files) {
        int i{};
    for (const auto & entry : std::filesystem::directory_iterator(path)){
        files[i] = entry.path();
        // files[i] = files[i].substr(12, files[i].length() - 12);
        std::cout << i << " " << files[i] << std::endl;
        i++;
    }

}

void new_state_info(int &id, std::string &name) {
    std::cout << "Name of the state to be created: " << std::endl;
    std::cin >> name;
    std::cout << std::endl;
    std::cout << "Id of the state to be created: " << std::endl;
    std::cin >> id;
    std::cout << std::endl;
}

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

unsigned int find_state(const std::string &file, const std::string &prov) {
    unsigned int cur_line{};
    std::string line{};
    
    std::ifstream  src(file, std::ios::binary);

    while(getline(src, line)) {
        cur_line++;
        if (line.find(prov, 0) != std::string::npos) {
            break;
        }
    }
    src.close();
    return cur_line;
}

// void find_file() {}

// TODO all the funcions below should be moved inside class in next version
void print_state(State &state){
    std::ofstream  dst("output/map_data/state_regions/to.txt", std::ios::binary | std::ios::app);
    dst << state.getName() << "= {" << std::endl
        << "\tid = " << state.getId() << std::endl
        << "\tsubsistence_building = " << state.getSub() << std::endl
        << "\tprovinces = { ";
    for (std::string s : state.getProvs()) {
        dst << "\"x" << s << "\" ";
    }
    dst << "}" << std::endl 
        << "\ttraits = { "; 
    for (std::string s : state.getTraits()) {
        dst << "\"" << s << "\" ";
    }
    dst << "}" << std::endl 
        << "\tcity = " << state.getCity() << std::endl;
    if(state.getPort() != "") {
        dst<< "\tport = " << state.getPort() << std::endl;
    }
    dst << "\tfarm = " << state.getFarm() << std::endl
        << "\tmine = " << state.getMine() << std::endl
        << "\twood = " << state.getWood() << std::endl
        << "\tarable_land = " << state.getLand() << std::endl
        << "\tarable_resources = { ";
    for (std::string s : state.getResources()) {
        dst << "\"" << s << "\" ";
    } 
    dst << "}" << std::endl
        << "\tcapped = {" << std::endl;
    if(state.getIron() != 0) {
        dst << "\t\tbg_iron_mining = " << state.getIron() << std::endl; 
    }
    if(state.getCoal() != 0) {
        dst << "\t\tbg_coal_mining = " << state.getCoal() << std::endl; 
    }
    if(state.getLead() != 0) {
        dst << "\t\tbg_lead_mining = " << state.getLead() << std::endl; 
    }
    if(state.getSulfur() != 0) {
        dst << "\t\tbg_sulfur_mining = " << state.getSulfur() << std::endl; 
    }
    if(state.getLog() != 0) {
        dst << "\t\tbg_logging = " << state.getLog() << std::endl; 
    }
    if(state.getFish() != 0) {
        dst << "\t\tbg_fishing = " << state.getFish() << std::endl; 
    }
    if(state.getWhale() != 0) {
        dst << "\t\tbg_whaling = " << state.getWhale() << std::endl; 
    }
    if(state.getOil() != 0) {
        dst << "\t\tbg_oil_extraction = " << state.getOil() << std::endl; 
    }
    if(state.getRubber() != 0) {
        dst << "\t\tbg_logging = " << state.getRubber() << std::endl; 
    }
    if(state.getGold() != 0) {
        dst << "\t\tbg_gold_mining = " << state.getGold() << std::endl; 
    }
    if(state.getDiscGold() != 0) {
        dst << "\t\tbg_whaling = " << state.getDiscGold() << std::endl; 
    }
    dst << "\t}" << std::endl;
    if(state.getNavalExit() != 0) {
       dst << "\tnaval_exit_id = " << state.getNavalExit() << std::endl;
    }
    dst << "}" << std::endl << std::endl;

}

void print_pops(State &state){
    std::ofstream  dst("output/common/history/pops/to_pops.txt", std::ios::binary | std::ios::app);
    dst << "\ts:" << state.getName() << " = {" << std::endl
        << "\t\tregion_state:ABC = {" << std::endl;
    for(int i{}; i < state.getPops().size(); i++){
        dst << "\t\t\tcreate_pop = {" << std::endl;
        if (state.getPops()[i].getType() != "") {
            dst << "\t\t\t\tpop_type = " << state.getPops()[i].getType() << std::endl;
        }
        dst << "\t\t\t\tculture = " << state.getPops()[i].getCult() << std::endl;
        if (state.getPops()[i].getRel() != "") {
            dst << "\t\t\t\treligion = " << state.getPops()[i].getRel() << std::endl;
        }
        dst << "\t\t\t\tsize = " << state.getPops()[i].getSize() << std::endl
            << "\t\t\t}" << std::endl;
    }
    dst << "\t\t}" << std::endl << "\t}" << std::endl;

}

double calculate_ratio(State &state, const std::vector<std::string> &provs) {
    double i{provs.size()}, j{state.getProvs().size()};
    double result{i / j};
    return result;
}
// TODO refactor
void calculate_pops(State &donor, State &state, const double &ratio) {
    // const std::vector<State::Pop> *ptr = state.getPopsPtr();
    for(int i{}; i < donor.getPops().size(); i++) {
        state.add_pop(donor.getPops()[i].getCult(), donor.getPops()[i].getRel(), donor.getPops()[i].getType(), donor.getPops()[i].getSize() * ratio);
    }
}

void calculate_remaining_pops(State &donor, State &remain, State &state) {
    for(int i{}; i < donor.getPops().size(); i++) {
        // remain.setPopSize(i, donor.getPops()[i].getSize() - state.getPops()[i].getSize());
        remain.add_pop(donor.getPops()[i].getCult(), donor.getPops()[i].getRel(), donor.getPops()[i].getType(), donor.getPops()[i].getSize() - state.getPops()[i].getSize());
    }
}

State calculate_resources(State &donor, const double &ratio) {
    unsigned int res[12]{};
    res[0] = donor.getLand() * ratio;
    if(donor.getCoal() != 0){
        res[1] = donor.getCoal() * ratio;
    } 
    if(donor.getIron() != 0){
        res[2] = donor.getIron() * ratio;
    }
    if(donor.getLead() != 0){
        res[3] = donor.getLead() * ratio;
    }
    if(donor.getSulfur() != 0){
        res[4] = donor.getSulfur() * ratio;
    }
    if(donor.getLog() != 0){
        res[5] = donor.getLog() * ratio;
    }
    if(donor.getFish() != 0){
        res[6] = donor.getFish() * ratio;
    }
    if(donor.getWhale() != 0){
        res[7] = donor.getWhale() * ratio;
    }
    if(donor.getOil() != 0){
        res[8] = donor.getOil() * ratio;
    }
    if(donor.getRubber() != 0){
        res[9] = donor.getRubber() * ratio;
    }
    if(donor.getGold() != 0){
        res[10] = donor.getGold() * ratio;
    }
    if(donor.getDiscGold() != 0){
        res[11] = donor.getDiscGold() * ratio;
    }
    State St(res);
    return St;
}

State calculate_remaining_resources(State &donor, State &state) {
        unsigned int res[12]{};
    res[0] = donor.getLand() - state.getLand();
    if(donor.getCoal() != 0){
        res[1] = donor.getCoal() - state.getCoal();
    } 
    if(donor.getIron() != 0){
        res[2] = donor.getIron() - state.getIron();
    }
    if(donor.getLead() != 0){
        res[3] = donor.getLead() - state.getLead();
    }
    if(donor.getSulfur() != 0){
        res[4] = donor.getSulfur() - state.getSulfur();
    }
    if(donor.getLog() != 0){
        res[5] = donor.getLog() - state.getLog();
    }
    if(donor.getFish() != 0){
        res[6] = donor.getFish() - state.getFish();
    }
    if(donor.getWhale() != 0){
        res[7] = donor.getWhale() - state.getWhale();
    }
    if(donor.getOil() != 0){
        res[8] = donor.getOil() - state.getOil();
    }
    if(donor.getRubber() != 0){
        res[9] = donor.getRubber() - state.getRubber();
    }
    if(donor.getGold() != 0){
        res[10] = donor.getGold() - state.getGold();
    }
    if(donor.getDiscGold() != 0){
        res[11] = donor.getDiscGold() - state.getDiscGold();
    }
    State St(res);
    return St;
}

// void add_state_info(State &donor, State &state) {
//     state.setName(new_state_name)
// }

// void debug(const std::vector<std::string> &provs) {
//     for(std::string s : provs) {
//         std::cout<< s << std::endl;
//     }
// }



int main() {
// variables
    std::filesystem::path input{}, output{};
    std::string files[15]{};
    std::vector<std::string> provinces{};
    std::string path{"input/files"}, new_state_name{/*"NEW_STATE"*/}/*, strat_reg{}*/;
    int new_state_id{/*666*/}; 
    double provs_ratio; // TODO for next version change this to be member of state class
    
    if (std::filesystem::exists("inoutpath.txt")){
        load_i_o_path(input, output);
    } else {
        save_i_o_path();
        load_i_o_path(input, output);
    }
    file_list(path, files);
    save_provinces(provinces);
    new_state_info(new_state_id, new_state_name);
    // find_state(input_file, provinces[0]);
    // ! temporary args
    State Old_state(find_state(files[0], provinces[0]), files[0]);
    // State New_state(Old_state);
    provs_ratio = calculate_ratio(Old_state, provinces);
    // ! temporary args
    Old_state.copy_pops("input/pops/05_north_america.txt", "STATE_LOUISIANA");
    // calculate_resources(Old_state, provs_ratio);
    State New_state = calculate_resources(Old_state, provs_ratio);
    State Remaining_state = calculate_remaining_resources(Old_state, New_state);

// setting new states info
    New_state.setName(new_state_name);
    New_state.setId(new_state_id);
    New_state.setProvs(provinces);
    New_state.copy_state_info(Old_state);
    Remaining_state.setName(Old_state.getName());
    Remaining_state.setId(Old_state.getId());
    Remaining_state.setProvs(Old_state.getProvs());
    Remaining_state.calculate_remaining_provs(New_state);
    Remaining_state.copy_state_info(Old_state);

// calculating pops
    calculate_pops(Old_state, New_state, provs_ratio);
    calculate_remaining_pops(Old_state, Remaining_state, New_state);

// printing states
    print_state(Old_state); //for debugging purposes
    print_state(Remaining_state);
    print_state(New_state);
    print_pops(Old_state); //for debugging purposes
    print_pops(Remaining_state);
    print_pops(New_state);


    // debug( provinces );
    
    return 0;
}