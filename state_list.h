#ifndef _STATE_LIST_H_
#define _STATE_LIST_H_

#include <iostream>
#include <vector>
#include "state.h"

const class State_list {

// data
    std::vector<State> states;

// functions 
    // data manipulation
    // TODO try to use function overloading here
    std::string data(std::string &);
    std::string data(std::string &, const char &);
    unsigned int data_int( std::string);
    void data_vector(std::vector<std::string> &, const std::string &, int);
    void variable_string_vector(std::vector<std::string> &, std::string &);
    std::string data_name(std::string &);
    
    // text parsing
    bool find_string(std::string &, const std::string &, const int &pos = 0);
    void find_name(std::ifstream &, std::string &, const std::string &, std::string &);
    void find_data(std::ifstream &, std::string &, const std::string &, std::string &);
    void find_data(std::ifstream &, std::string &, const std::string &, const char &, std::string &);
    void find_data_int(std::ifstream &, std::string &, const std::string &, int &);

    // creating an array of states info //? TODO refactor using regex, create a class for the functions
    std::ifstream::pos_type create_state(std::ifstream &, std::vector<State> &, std::string &, const int &);
    std::ifstream::pos_type add_homelands(std::ifstream &, std::vector<State> &, std::string &, const int &);
    std::ifstream::pos_type add_claims(std::ifstream &, std::vector<State> &, std::string &, const int &);
    void save_states(const std::filesystem::path &, std::vector<State> &);
    
    // state info saving
    std::ifstream::pos_type save_gold_fields(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_rubber(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_oil(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_resources(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_capped_resources(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_subsistence_building(std::ifstream &, std::string line, std::string &);
    std::ifstream::pos_type save_state(std::ifstream &, State &, std::string &);
    void browse_file(std::ifstream &, std::vector<State> &); 
    void save_state_info(const std::filesystem::path &, std::vector<State> &, const std::filesystem::path *);
    
    //   pops saving
    std::ifstream::pos_type save_pop(std::ifstream &, std::vector<State> &, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &, const int &);
    void save_state_pops(const std::filesystem::path &, std::vector<State> &, const std::filesystem::path *);
    
    // building saving
    std::ifstream::pos_type save_building(std::ifstream &, std::vector<State> &, const std::string &, const std::string &, const std::string &, const int &, const int &, std::vector<std::string> &);
    void save_state_builds(const std::filesystem::path &, std::vector<State> &, const std::filesystem::path *);

};
#endif