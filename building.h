#ifndef _BUILDING_H_
#define _BUILDING_H_

#include <iostream>
#include <vector>

class Building
{
private:
    int level, reserves;

    std::string type, dlc{}; 

    std::vector<std::string> prod;

public:
    Building(const std::string &type, const int &lvl, const int &res, const std::vector<std::string> &pr)
    : level{lvl}, reserves{res}, type{type}/* , dlc{dlc} */, prod{pr}
    {
    }

    ~Building();

    inline std::string getType() {
        return type;
    }

    inline std::string getDlc() {
        return dlc;
    }

    inline std::vector<std::string> getProd() {
        return prod;
    }

    inline int getLvl() {
        return level;
    }

    inline int getRes() {
        return reserves;
    } 

    void setLvl(const int &);
    
    void print_buildings(std::ofstream &);

};

#endif