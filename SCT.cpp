#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <memory>
#include "parser.h"
#include "state.h"
#include "transfer_state.h"
#include "state_list.h"

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
void list_current_i_o_paths(const std::filesystem::path &in, const std::filesystem::path &out) {
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
    // std::vector<std::filesystem::path>f;
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
    // std::cout << "8 - debug - print state list" << std::endl;

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
    // case '8':
    //     (out = change_path('o'));
    //     return true;
    // case 'e':
    //     exit();
    default:
        return true;
    }
}

// transfer info input
/* State_list */void new_state_info(State_list &states) {
    int num{};
    std::string name{}, provs{}, id{};
    std::vector<std::string> prov_vec{};
    // State_list states{};
    // std::vector<Transfer_state> temp;
    std::cout << "Number of states to be created: " << std::endl;
    std::cin >> num;
    for(int i{}; i < num; i++) {
        std::cout << std::endl;
        std::cout << "Name of the state to be created number " << i + 1 << ": " << std::endl;
        std::cin >> name;
        std::cout << "Id of the state to be created number " << i + 1 << ": " << std::endl;
        std::cin >> id;
        std::cout << "Provinces to transfer to state number " << i + 1 << ": " << std::endl;
        std::cin >> std::ws;
        std::getline(std::cin, provs);
        data_vector(prov_vec, provs,6);
        std::cout << std::endl;
        // std::shared_ptr<State> state(new Transfer_state(name, id, provs));
        // states.add_state(std::move(state));
        states.add_state(std::shared_ptr<State>(new Transfer_state(name, id, prov_vec)));
    } 
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
    for(State &st : states) {
        dst << pos << " " << st.getName() << " " << st.getId() << std::endl;
        pos++;
    }
}



int main() {
// variables
    // std::vector<State> states{}, remnant_states{};
    State_list states{}, remnant_states{}, transfer_states{}, target_states{};
    std::filesystem::path files[15], input{}, output{};
    std::vector<std::string> provinces{};
    // std::vector<Transfer_state> transfer_states{}, target_states{};
    std::string /* filename, */ new_state_name{}/*, strat_reg{}*/;
    // int new_state_id{}; 
    
    check_i_o_file(input, output);
    file_list(input, files);

    while(menu(input, output)) {continue;} 

    states.save_states(input); // TODO make one functions call the others
    states.save_state_info(input, files);
    states.save_state_pops(input, files);
    states.save_state_builds(input, files);
    

    /* transfer_states =  */new_state_info(transfer_states);

    //  TODO refactor, make one function to call all the others
    for(int i {}; i < transfer_states.getStates().size(); i++){
        auto tr_ptr = std::dynamic_pointer_cast<Transfer_state>(transfer_states.getStates()[i]);
        tr_ptr->find_origin_states(states, transfer_states);
    }
    for(int i {}; i < transfer_states.getStates().size(); i++){
        auto tr_ptr = std::dynamic_pointer_cast<Transfer_state>(transfer_states.getStates()[i]);
        tr_ptr->calculate_resources(states);
    }
    for(int i {}; i < transfer_states.getStates().size(); i++){
        auto tr_ptr = std::dynamic_pointer_cast<Transfer_state>(transfer_states.getStates()[i]);
        tr_ptr->create_target_states(target_states);
        }
    for(int i {}; i < transfer_states.getStates().size(); i++){
        auto tr_ptr = std::dynamic_pointer_cast<Transfer_state>(transfer_states.getStates()[i]);
        tr_ptr->create_remaining_states(remnant_states, states);
    }
    for(int i {}; i < transfer_states.getStates().size(); i++){
        auto tr_ptr = std::dynamic_pointer_cast<Transfer_state>(transfer_states.getStates()[i]);
        tr_ptr->calculate_remaining_resources(remnant_states);
    }
    for(int i {}; i < target_states.getStates().size(); i++){ 
        target_states.getStates()[i]->print_entry();
    }
    for(int i {}; i < remnant_states.getStates().size(); i++) {
        remnant_states.getStates()[i]->print_entry();
    }
    
    return 0;
}