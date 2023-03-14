#ifndef _STATE_LIST_H_
#define _STATE_LIST_H_

#include <iostream>
#include <vector>
#include <sstream>
#include "state.h"

class State_list {
// data
    std::vector<State*> states;
public: // TODO make some of the functons private
// functions 

    // creating an array of states info //? TODO refactor using regex, create a class for the functions
    std::ifstream::pos_type create_state(std::ifstream &, State* , std::string &);
    std::ifstream::pos_type add_homelands(std::ifstream &, State* , std::string &);
    std::ifstream::pos_type add_claims(std::ifstream &, State* , std::string &);
    void save_states(const std::filesystem::path &);
    
    // state info saving
    std::ifstream::pos_type save_gold_fields(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_rubber(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_oil(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_resources(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_capped_resources(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_subsistence_building(std::ifstream &, std::string &, std::string &);
    std::ifstream::pos_type save_state(std::ifstream &, State* , std::string &);
    void browse_file(std::ifstream &/* , std::vector<State> & */); 
    void save_state_info(const std::filesystem::path &, const std::filesystem::path *);
    
    //   pops saving
    std::ifstream::pos_type save_pop(std::ifstream &/* , std::vector<State> & */, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &, const int &);
    void save_state_pops(const std::filesystem::path &, const std::filesystem::path *);
    
    // building saving
    std::ifstream::pos_type save_building(std::ifstream &/* , std::vector<State> & */, const std::string &, const std::string &, const std::string &, const int &, const int &, std::vector<std::string> &);
    void save_state_builds(const std::filesystem::path &, const std::filesystem::path *);

public:
    // getters
    inline std::vector<State*>& getStates() {return this->states;}
    inline size_t getSize() {return this->states.size();}

};
#endif