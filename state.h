#ifndef _State_H_
#define _State_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem> 

#include "country.h"

class State 
{
protected:
    std::vector<std::shared_ptr<Country>> countries{};

    std::vector<std::string> traits, ar_res, im_provs{}, homelands{}, claims{};
    
    std::string id, name, hubs[5]{}, file_name, sub, naval_exit{};

    unsigned int res[12]{};

    // hubs = 0-city, 1-port, 2-farm, 3-mine, 4-wood
    // res = 0-arable land, 1-coal, 2-iron, 3-lead, 4-sulfur, 5-logging, 6-fishing, 7-whaling, 8-gold, 9-disc. gold, 10-rubber, 11-oil

    /*virtual*/ void print_state_region();

    /*virtual*/ void print_pops();

    /*virtual*/ void print_buildings();

    /*virtual*/ void print_state();

public:
    virtual ~State();

    /*virtual*/ void print_entry();

    /*virtual*/ void add_country(const std::shared_ptr<Country> &);

    inline std::string getName() {
        return name;
    }

    inline std::string getSub() {
        return sub;
    }

    inline std::string getId() {
        return id;
    }

    inline std::string getNavalExit() {
        return naval_exit;
    }

    inline const unsigned int* getRes() {
        return res;
    }

    inline std::vector<std::string> getTraits() {
        return traits;
    }

    inline std::vector<std::string> getResources() {
        return ar_res;
    }

    inline std::vector<std::string> getImProvs() {
        return im_provs;
    } 

    inline std::vector<std::string> getHomelands() {
        return homelands;
    }

    inline std::vector<std::string> getClaims() {
        return claims;
    }

    inline std::vector<std::shared_ptr<Country>> getCountries() {
        return countries;
    }

    inline std::shared_ptr<Country> getCo(const int &i) {
        return countries[i];
    }

   void setName(const std::string &);

   void setId(const std::string &);

   void setSub(const std::string &);

   void setTraits(const std::vector<std::string> &);

   void setHubs(const std::string []);

   void setLand(const int &);

   void setArRes(const std::vector<std::string> &);

   void setRes(const int []);

   void setPopSize(const int &, const int &);

   void addHomeland(const std::string &);

   void addClaim(const std::string &);

   void setArable(const int &);

   void setNavExit(const std::string &);

};

#endif