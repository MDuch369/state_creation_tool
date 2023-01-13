#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

class State {

    std::vector<std::string> provs, traits, resources, im_provs;
    std::string name, sub, city, port, farm, mine, wood;
    unsigned int id, land, coal, iron, lead, sulfur, log, fish, whale, oil, rubber, gold, disc_gold, naval_exit;    

    std::string data(const std::string);
    unsigned int data_int(const std::string);
    void data_vector(std::vector<std::string>&, const std::string&, int);
    bool compare_string(const std::string&, std::string);

public:
    State();
    State(const unsigned int &, const std::string &);
};


#endif