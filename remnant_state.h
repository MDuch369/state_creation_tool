#ifndef _REMNANT_STATE_H_
#define _REMNANT_STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "state.h"
// #include "state_list.h"

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
    // void create_pops(const std::string &, const std::string &, const std::string &, const std::string &, const int &) override; 
    // void create_buildings(const std::string &, const std::string &, const int &, const int &, const std::vector<std::string> &) override;
};

#endif