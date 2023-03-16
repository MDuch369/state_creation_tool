#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "state_base.h"
#include "state_list.h"

/**** ORIGIN STATE CLASS ****/

class Origin_state : public State  {

public:
// constructors 
    Origin_state();
    Origin_state(const std::string &);
/*  Origin_state(const std::filesystem::path &);
    Origin_state(const unsigned int &, const std::string &);
    Origin_state(const unsigned int[]);
*/
//destructor
    ~Origin_state() override;

// countries
    std::unique_ptr<State::Country> create_country(const std::string &, std::vector<std::string> &) override;
    void create_pops(const std::string &, const std::string &, const std::string &, const std::string &, const int &) override; 
    void create_buildings(const std::string &, const std::string &, const int &, const int &, const std::vector<std::string> &) override;

// prov manipulation
    void remove_prov ();
    void calculate_remaining_provs(Origin_state &);

};

/**** REMNANT STATE CLASS ****/

class Remnant_state : public State  {

public:
// constructors 
    Remnant_state();
    Remnant_state(State*);

// destructor
    ~Remnant_state() override;

    void remove_prov ();
    void calculate_remaining_provs(State &);

    // void create_country(const std::string &, std::vector<std::string> &);
    // Remnant_state::Country *const create_country(const std::string &, std::vector<std::string> &) override;
    void create_pops(const std::string &, const std::string &, const std::string &, const std::string &, const int &) override; 
    void create_buildings(const std::string &, const std::string &, const int &, const int &, const std::vector<std::string> &) override;
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
    // Transfer_state();
    // Transfer_state(Transfer_state const&);
    Transfer_state(Transfer_state const&);
    Transfer_state(const std::string &, const std::string &, const std::string &); // TODO replace this constructor with the one below
    Transfer_state(const std::string &, const std::string &, const std::vector<std::string> &);  

// destructor
    ~Transfer_state() override;

// countries
    // State::Country create_country(State::Country &, std::vector<std::string> &, const double &); // ? merge the two functions
    State::Country create_country(State::Country &, const std::vector<std::string> &, const double &);
    std::unique_ptr<State::Country> create_country(const std::string &, std::vector<std::string> &) override; // this function exist so the class is not abstract

    void create_pops(const std::string &, const std::string &, const std::string &, const std::string &, const int &) override; 
    void create_buildings(const std::string &, const std::string &, const int &, const int &, const std::vector<std::string> &) override;

// getters
    inline std::vector<std::string> getProvs() {return this->transfer_provs;}
    inline std::string getOrigin() {return this->origin;}

// transfer
    void find_origin_states(const std::vector<Origin_state> &, std::vector<Transfer_state> &);
    void calculate_resources(const std::vector<Origin_state> &);
    void create_target_states(std::vector<Transfer_state> &);
    void create_remaining_states(std::vector<State*> &, const std::vector<State*> &);
    void calculate_remaining_resources(std::vector<Origin_state> &);
    void copy_state_info(State &);

// debug functions
    void debug_print_provs();
};

#endif