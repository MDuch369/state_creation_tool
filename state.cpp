#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "state.h"

State::State(){}

State::State(const unsigned int &cur_line, const std::string &file) {
    unsigned int copy_line{}, len{};
    std::string line{}, d{};
    std::ifstream  src(file, std::ios::binary);
    while(getline(src, line)) {
        copy_line++;
        if(copy_line == cur_line - 3) { 
            this->name = line.substr(0, line.find (" ") + 1);
        }
        if(copy_line == cur_line - 2) { 
            this->id = data_int(line);
         }
        if(copy_line == cur_line - 1) {
            this->sub = data(line);
        }
        if(copy_line == cur_line) {
            data_vector(this->provs, line, 7);
        }
        // if(copy_line == cur_line + 1) {
        //     data_vector(this->traits, line, 7);
        // }
        // if(copy_line == cur_line + 2) {
        //     this->city = data(line);
        // }
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