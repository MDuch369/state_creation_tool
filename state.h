#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

class State {

    std::vector<std::string> provs, traits, resources;
    std::string name, sub, city, port, farm, mine, wood;
    unsigned int id, land, coal, iron, lead, sulfur, wood, fish, naval_exit;    

    std::string data(const std::string);
    unsigned int data_int(const std::string);
    void data_vector(std::vector<std::string>&, const std::string&, int);

public:
    State();
    State(const unsigned int &cur_line, const std::string &file);
};


#endif