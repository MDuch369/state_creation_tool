#ifndef _STATE_LIST_H_
#define _STATE_LIST_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <memory>

#include "state.h"
#include "origin_state.h"
#include "transfer_state.h"


class State_list {
// data
    std::vector<std::shared_ptr<State>> states;
public: // TODO make some of the functons private
// functions 
    // ~State_list();
    // creating an array of states info //? TODO refactor using regex
    std::ifstream::pos_type create_state(std::ifstream &, std::shared_ptr<State> & , std::string &);
    std::ifstream::pos_type add_homelands(std::ifstream &, std::shared_ptr<State> & , std::string &);
    std::ifstream::pos_type add_claims(std::ifstream &, std::shared_ptr<State> & , std::string &);
    void save_states(const std::filesystem::path &);
    
    // state info saving // ? move into state
    std::ifstream::pos_type save_gold_fields(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_rubber(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_oil(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_resources(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_capped_resources(std::ifstream &, int *, std::string &);
    std::ifstream::pos_type save_subsistence_building(std::ifstream &, std::string &, std::string &);
    std::ifstream::pos_type save_state(std::ifstream &, std::shared_ptr<State> & , std::string &);
    void browse_file(std::ifstream &); 
    void save_state_info(const std::filesystem::path &, const std::filesystem::path *);
    
    //   pops saving // ? move into country
    std::ifstream::pos_type save_pop(std::ifstream &, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &, const int &);

    void save_state_pops(const std::filesystem::path &, const std::filesystem::path *);
    
    // building saving // ? move into country
    std::ifstream::pos_type save_building(std::ifstream &, const std::string &, const std::string &, const std::string &, const int &, const int &, const std::vector<std::string> &);

    void save_state_builds(const std::filesystem::path &, const std::filesystem::path *);

    // getters
    inline std::vector<std::shared_ptr<State>>& getStates() {
        return this->states;
    }

    inline size_t getSize() {
        return this->states.size();
    }

    void calculate_resources();

    void create_target_states(State_list &);

    void create_remaining_states(State_list &);
    
    void calculate_remaining_resources(State_list &);

    void add_state(std::shared_ptr<State> );
   
    // WIP State adding function returning a shared pointer
    // std::shared_ptr<State> add_state_ptr(const State &state);
    // std::shared_ptr<State> add_state(const std::string &, const std::string &, const std::vector<std::string> &);
    std::shared_ptr<State> emplace_transfer_state(std::shared_ptr<State> &);

// transfer
    void find_origin_states(State_list &);

};
#endif