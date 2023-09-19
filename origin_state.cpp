#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>

#include "origin_state.h"

// constructors
Origin_state::Origin_state() {}
Origin_state::~Origin_state() {}

Origin_state::Origin_state(const std::string &n)
{
    name = n;
}
