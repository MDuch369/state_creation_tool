#ifndef _ORIGIN_STATE_H_
#define _ORIGIN_STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "state.h"

class Origin_state : public State  {

public:
// constructors 
    Origin_state();
    Origin_state(const std::string &);

//destructor
    ~Origin_state() override;

// prov manipulation
    void remove_prov ();
    void calculate_remaining_provs(Origin_state &);

};

#endif