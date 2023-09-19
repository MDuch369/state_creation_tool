#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>

#include "remnant_state.h"


Remnant_state::Remnant_state() {}
Remnant_state::Remnant_state(State* state) 
    : State{*state} {}
Remnant_state::~Remnant_state() {}

