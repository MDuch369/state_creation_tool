#ifndef _STATE_LIST_H_
#define _STATE_LIST_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <memory>
#include "state.h"

class State_list {
// data
    std::vector<std::unique_ptr<State>> states;
public: // TODO make some of the functons private
// functions 
    ~State_list();
    // creating an array of states info //? TODO refactor using regex
    std::ifstream::pos_type create_state(std::ifstream &, std::unique_ptr<State> & , std::string &);
    std::ifstream::pos_type add_homelands(std::ifstream &, std::unique_ptr<State> & , std::string &);
    std::ifstream::pos_type add_claims(std::ifstream &, std::unique_ptr<State> & , std::string &);
    void save_states(const std::filesystem::path &);
    
    // state info saving
    std::ifstream::pos_type save_gold_fields(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_rubber(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_oil(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_resources(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_capped_resources(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_subsistence_building(std::ifstream &, std::string &, std::string &);
    std::ifstream::pos_type save_state(std::ifstream &, std::unique_ptr<State> & , std::string &);
    void browse_file(std::ifstream &/* , std::vector<State> & */); 
    void save_state_info(const std::filesystem::path &, const std::filesystem::path *);
    
    //   pops saving
    std::ifstream::pos_type save_pop(std::ifstream &/* , std::vector<State> & */, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &, const int &);
    void save_state_pops(const std::filesystem::path &, const std::filesystem::path *);
    
    // building saving
    std::ifstream::pos_type save_building(std::ifstream &/* , std::vector<State> & */, const std::string &, const std::string &, const std::string &, const int &, const int &, const std::vector<std::string> &);
    void save_state_builds(const std::filesystem::path &, const std::filesystem::path *);

// public:
    // getters
    inline std::vector<std::unique_ptr<State>>& getStates() {return this->states;}
    inline size_t getSize() {return this->states.size();}

    void add_state(std::unique_ptr<State>&&);

};
#endif