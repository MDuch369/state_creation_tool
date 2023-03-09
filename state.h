#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "state_base.h"

class Origin_state : public State  {

public:
// constructors 
    Origin_state();
    Origin_state(const std::filesystem::path &);
    Origin_state(const unsigned int &, const std::string &);
    Origin_state(const unsigned int[]);
    Origin_state(const std::string &);

    Origin_state::Country *const create_country(const std::string &, std::vector<std::string> &);

// prov manipulation
    void remove_prov ();
    void calculate_remaining_provs(Origin_state &);

};

/**** REMNANT STATE CLASS ****/

class Remnant_state : public State  {
public:
    void remove_prov ();
    void calculate_remaining_provs(State &);

    // void create_country(const std::string &, std::vector<std::string> &);
    Remnant_state::Country *const create_country(const std::string &, std::vector<std::string> &);
};

/**** TRANSFER STATE CLASS ****/

class Transfer_state : public State{
// data
    int origin_pos{};
    std::string origin{};
    std::vector<std::string> transfer_provs;

// Functions:
    unsigned int find_states(const std::string &, std::vector<Transfer_state> &);

public:
// constructors
    Transfer_state(const std::string &, const std::string &, const std::string &); // TODO replace this constructor with the one below
    Transfer_state(const std::string &, const std::string &, const std::vector<std::string> &);  

// countries
    State::Country create_country(State::Country &, std::vector<std::string> &, const double &);
    // Transfer_state::Country *const create_country(const std::string &, std::vector<std::string> &);

// getters
    inline std::vector<std::string> getProvs() {return this->transfer_provs;}
    inline std::string getOrigin() {return this->origin;}

// transfer
    void find_origin_states(const std::vector<Origin_state> &, std::vector<Transfer_state> &);
    void calculate_resources(std::vector<Origin_state> &);
    void create_target_states(std::vector<Transfer_state> &);
    void create_remaining_states(std::vector<Remnant_state> &, std::vector<Origin_state> &);
    void calculate_remaining_resources(std::vector<Origin_state> &);

// debug functions
    void debug_print_provs();
};

#endif