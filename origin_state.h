#ifndef _ORIGIN_STATE_H_
#define _ORIGIN_STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "state.h"
// #include "country.h"
// #include "state_list.h"

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
    // std::shared_ptr<Country> create_country(const std::string &, std::vector<std::string> &) override;
    // void create_pops(const std::string &, const std::string &, const std::string &, const std::string &, const int &) override; 
    // void create_buildings(const std::string &, const std::string &, const int &, const int &, const std::vector<std::string> &) override;

// prov manipulation
    void remove_prov ();
    void calculate_remaining_provs(Origin_state &);

};

#endif