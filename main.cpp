#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>

class State {

    std::vector<std::string> provs{}, traits{}, resources{};
    std::string name, sub{}, city{}, port{}, farm{}, mine{}, wood{};
    unsigned int id, land{}, coal{}, iron{}, lead{}, sulfur{}, wood{}, fish{}, naval_exit{};    

    State(std::string name, unsigned int id ) {
        this->name = name;
        this->id = id;
    }

};

void save_provinces(std::vector<std::string> &provs) {
    std::string prov, tmp; 
    
    std::cout<<"Enter provinces to transfer: "<<std::endl;
    std::getline(std::cin, prov);
    prov.erase (std::remove(prov.begin(), prov.end(), ' '), prov.end());
    transform(prov.begin(), prov.end(), prov.begin(), toupper);

    std::stringstream ss(prov);

    while(getline(ss, tmp, 'X')){
        provs.push_back(tmp);

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

// void copy_state(const unsigned int &cur_line, const std::string &file) {
//     unsigned int copy_line{};
//     std::string line{};
//     std::ifstream  src(file, std::ios::binary);
//     std::ofstream  dst("to.txt", std::ios::binary);
//         while(getline(src, line)) {
//         copy_line++;
//         if(copy_line >= cur_line - 3) {
//             if(copy_line > cur_line && line[0] == 'S') {
//                 break;
//             }
//             dst<<line<<"\n";
//         }

//         line = {};
//     }
//     src.close();
//     dst.close();
// } 

void create_state(const std::vector<std::string> &pr, const std::string &state_name, const unsigned int &state_id){
    std::string str_place{"\"PLACEHOLDER\""};
    unsigned int int_place{123};
    
    std::ofstream  dst("to.txt", std::ios::binary);
    dst << "STATE_" << state_name << " = {" << std::endl
        << "    id = " << state_id << std::endl
        << "    subsistence_building = " << str_place << std::endl
        << "    provinces = { ";
    for (std::string s : pr) {
        dst << "\"" << s << "\" ";
    }
    dst << " }" << std::endl 
        << "    traits = { " << str_place << " }" << std::endl 
        << "    city = " << str_place << std::endl
        << "    port = " << str_place << std::endl
        << "    farm = " << str_place << std::endl
        << "    mine = " << str_place << std::endl
        << "    wood = " << str_place << std::endl
        << "    arable land = " << int_place << std::endl
        << "    arable_resources = { " << str_place << " }" << std::endl
        << "    capped = {" << std::endl
        << "        " << str_place << " = " << int_place << std::endl
        << "        " << str_place << " = " << int_place << std::endl
        << "    }" << std::endl
        << " naval_exit_id = " << int_place << std::endl
        << "}" << std::endl << std::endl;

}

//  TODO finish
void create_map_data(const std::string &state_name, const int &state_id, const std::vector<std::string> &provs) {}

//  TODO finish
void create_states_files(const std::string &state_name, const std::vector<std::string> &provs) {}

//  TODO finish
void create_pops_file(const int &provs_ratio) {}

//  TODO finish
std::string find_files(const std::vector<std::string> &provs) {}

// std::string get_file_name(const std::vector<std::string> pr ) {
//     std::ifstream  src("from.ogv", std::ios::binary);
//     for (std::string s : pr) {

//     }
// }

void debug(const std::vector<std::string> &provs) {
    for(std::string s : provs) {
        std::cout<< s << std::endl;
    }
}


int main() {

    std::string input_file;
    std::vector<std::string> provinces = {};
    int init_num_provs{}, transfered_provs{}, remaining_provs{};
    double provs_ratio{};

    save_provinces( provinces );

    debug( provinces );
    
    return 0;
}