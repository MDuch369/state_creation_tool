#ifndef _REMNANT_STATE_H_
#define _REMNANT_STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "state.h"


class Remnant_state : public State  {

public:
// constructors 
    Remnant_state();
    Remnant_state(State*);

// destructor
    ~Remnant_state() override;

    void remove_prov ();
    void calculate_remaining_provs(State &);
};

#endif