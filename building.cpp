#include "building.h"
#include <fstream>

void Building::print_buildings(std::ofstream &dst)
{
    if(level != 0) {
        dst << "\t\t\tcreate_building = {" << std::endl;
        dst << "\t\t\t\tbuilding = " << type << std::endl;
        dst << "\t\t\t\tlevel = " << level << std::endl;
        dst << "\t\t\t\treserves = " << reserves << std::endl;
        dst << "\t\t\t\tactivate_production_methods = { ";
        for(std::string prod : prod) {
            dst << "\"" << prod << "\" ";
        }
        dst << "} " << std::endl << "\t\t\t}" << std::endl;
    }
}
