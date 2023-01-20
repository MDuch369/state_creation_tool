#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "state.h"

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

void print_state(State &state){
    std::ofstream  dst("output/files/to.txt", std::ios::binary | std::ios::app);
    dst << state.getName() << "= {" << std::endl
        << "    id = " << state.getId() << std::endl
        << "    subsistence_building = " << state.getSub() << std::endl
        << "    provinces = { ";
    for (std::string s : state.getProvs()) {
        dst << "\"" << s << "\" ";
    }
    dst << "}" << std::endl 
        << "    traits = { "; 
    for (std::string s : state.getTraits()) {
        dst << "\"" << s << "\" ";
    }
    dst << "}" << std::endl 
        << "    city = " << state.getCity() << std::endl;
    if(state.getPort() != "") {
        dst<< "    port = " << state.getPort() << std::endl;
    }
    dst << "    farm = " << state.getFarm() << std::endl
        << "    mine = " << state.getMine() << std::endl
        << "    wood = " << state.getWood() << std::endl
        << "    arable_land = " << state.getLand() << std::endl
        << "    arable_resources = { ";
    for (std::string s : state.getResources()) {
        dst << "\"" << s << "\" ";
    } 
    dst << "}" << std::endl
        << "    capped = {" << std::endl;
    if(state.getIron() != 0) {
        dst << "        bg_iron_mining = " << state.getIron() << std::endl; 
    }
    if(state.getCoal() != 0) {
        dst << "        bg_coal_mining = " << state.getCoal() << std::endl; 
    }
    if(state.getLead() != 0) {
        dst << "        bg_lead_mining = " << state.getLead() << std::endl; 
    }
    if(state.getSulfur() != 0) {
        dst << "        bg_sulfur_mining = " << state.getSulfur() << std::endl; 
    }
    if(state.getLog() != 0) {
        dst << "        bg_logging = " << state.getLog() << std::endl; 
    }
    if(state.getFish() != 0) {
        dst << "        bg_fishing = " << state.getFish() << std::endl; 
    }
    if(state.getWhale() != 0) {
        dst << "        bg_whaling = " << state.getWhale() << std::endl; 
    }
    // if(state.getOil() != 0) {
    //     dst << "        bg_oil_extraction = " << state.getOil() << std::endl; 
    // }
    // if(state.getRubber() != 0) {
    //     dst << "        bg_logging = " << state.getRubber() << std::endl; 
    // }
    // if(state.getGold() != 0) {
    //     dst << "        bg_gold_mining = " << state.getGold() << std::endl; 
    // }
    // if(state.getDiscGold() != 0) {
    //     dst << "        bg_whaling = " << state.getDiscGold() << std::endl; 
    // }
    dst << "    }" << std::endl;
    if(state.getNavalExit() != 0) {
       dst << "    naval_exit_id = " << state.getNavalExit() << std::endl;
    }
    dst << "}" << std::endl << std::endl;

}

void print_pops(State &state){
    std::ofstream  dst("output/files/to_pops.txt", std::ios::binary | std::ios::app);
    dst << "    s:" << state.getName() << " = {" << std::endl
        << "        region_state:ABC = {" << std::endl;
    for(int i{}; i < state.getPops().size(); i++){
        dst << "            create_pop = {" << std::endl
            << "                culture = " << state.getPops()[i].getCult() << std::endl;
        if (state.getPops()[i].getRel() != "") {
            dst << "                religion = " << state.getPops()[i].getRel() << std::endl;
        }
        dst << "                size = " << state.getPops()[i].getSize() << std::endl
            << "            }" << std::endl;
    }
    dst << "        }" << std::endl << "    }" << std::endl;

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
        state.add_pop(donor.getPops()[i].getCult(), donor.getPops()[i].getRel(), donor.getPops()[i].getSize(), ratio);
    }
}

// void debug(const std::vector<std::string> &provs) {
//     for(std::string s : provs) {
//         std::cout<< s << std::endl;
//     }
// }


int main() {

    std::string files[15]{};
    std::vector<std::string> provinces{};
    std::string path = "input/files";
    double provs_ratio;
    // int init_num_provs{}, transfered_provs{}, remaining_provs{};

    int i{};
    for (const auto & entry : std::filesystem::directory_iterator(path)){
        files[i] = entry.path();
        // files[i] = files[i].substr(12, files[i].length() - 12);
        std::cout << i << " " << files[i] << std::endl;
        i++;
    }

    save_provinces(provinces);
    // find_state(input_file, provinces[0]);
    State Old_state(find_state(files[0], provinces[1]), files[0]);
    State New_state(Old_state);
    provs_ratio = calculate_ratio(Old_state, provinces);
    Old_state.copy_pops("input/pops/05_north_america.txt", "STATE_LOUISIANA");
    calculate_pops(Old_state, New_state, provs_ratio);
    print_state(Old_state);
    print_state(New_state);
    print_pops(Old_state);
    print_pops(New_state);


    // debug( provinces );
    
    return 0;
}