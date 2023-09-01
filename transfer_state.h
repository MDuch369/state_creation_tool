#ifndef _TRANSFER_STATE_H_
#define _TRANSFER_STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "state.h"
// #include "state_list.h"

class Transfer_state : public State{
// data
    int origin_pos{};
    std::string origin{};
    std::vector<std::string> transfer_provs;
    /* const */ std::shared_ptr<State> origin_ptr{};

// Functions:
    unsigned int find_states(const std::string &, std::vector<Transfer_state> &);

public:
// constructors
    Transfer_state(Transfer_state const&);
    Transfer_state(const std::string &, const std::string &, const std::vector<std::string> &);  
    // Transfer_state();
    // Transfer_state(const std::string &, const std::string &, const std::string &);

// destructor
    ~Transfer_state() override;

// countries
    // State::Country create_country(State::Country &, std::vector<std::string> &, const double &); // ? merge the two functions
    State::Country create_country(State::Country &, const std::vector<std::string> &, const double &);
    std::shared_ptr<State::Country> create_country(const std::string &, std::vector<std::string> &) override; // this function exist so the class is not abstract

    void create_pops(const std::string &, const std::string &, const std::string &, const std::string &, const int &) override; 
    void create_buildings(const std::string &, const std::string &, const int &, const int &, const std::vector<std::string> &) override;

// getters
    inline std::vector<std::string> getProvs() {return this->transfer_provs;}
    inline std::string getOrigin() {return this->origin;}
    inline std::shared_ptr<State> getOriginPtr() {return this->origin_ptr;}

// transfer
    // void find_origin_states(State_list &, State_list &);
    void copy_state_info(State &);

    void setProvs(const std::vector<std::string> &);
    void remove_transfer_provs();
    bool check_transfer_provs();
    // std::vector<std::string> add_different_origin_provs();
    std::vector<std::string> handle_transfer_provs();


// debug functions
    void debug_print_provs();
};

#endif