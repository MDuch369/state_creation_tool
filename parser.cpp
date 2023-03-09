#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "parser.h"


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
bool compare_string(const std::string &str, std::string l) {
    l.erase (std::remove(l.begin(), l.end(), '\t'), l.end());
    l.erase (std::remove(l.begin(), l.end(), ' '), l.end());
    return str == l.substr(0, str.size());
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
void find_data_int(std::ifstream &src, std::string &line, const std::string &str, int &result) { // ? use this function for other stuff
    if(line.find(str, 0) != std::string::npos) { 
        result = data_int(line);
    }
}
