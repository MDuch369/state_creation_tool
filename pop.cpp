#include <fstream>
#include <filesystem>

#include "pop.h"



Pop::~Pop()
{
}

 void Pop::setSize(const int &s) 
 {
    this->size = s;
 }

void Pop::print_pops(std::ofstream  &dst) 
{ 
    if(size != 0) {
        dst << "\t\t\tcreate_pop = {" << std::endl;
        if (type != "") {
            dst << "\t\t\t\tpop_type = " << type << std::endl;
        }
        dst << "\t\t\t\tculture = " << culture << std::endl;
        if (religion != "") {
            dst << "\t\t\t\treligion = " << religion << std::endl;
        }
        dst << "\t\t\t\tsize = " << size << std::endl
            << "\t\t\t}" << std::endl;
    }
}
