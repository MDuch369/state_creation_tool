#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem> 

class State {
//sublcasses
protected:
    class Country {
    // subclasses
        class Pop {
        // data
        std::string culture, religion{}, type{};
        int size;

        public:
        // constructor
        Pop(const std::string &, const std::string &, const std::string &, const int &);
        // getters
        inline std::string getCult() {
            return culture;
        }
        inline std::string getRel() {
            return religion;
        }
        inline std::string getType() {
            return type;
        }
        inline int getSize() {
            return size;
        }
        // setters
        void setSize(const int &);
        };

        class Building {
        // data
        std::string type, region, dlc{}, prod;
        int level, reserves;
        public:
        // constructor
        Building(const std::string &, const std::string &, const std::string &, const std::string &, const int &, const int &);
        // getters
        inline std::string getType() {
            return type;
        }
        inline std::string getRegion() {
            return region;
        }
        inline std::string getDlc() {
            return dlc;
        }
        inline std::string getProd() {
            return prod;
        }
        inline int getLvl() {
            return level;
        }
        inline int getRes() {
            return reserves;
        }        
        // setters
        };
    // data
        std::string country, type{};
        std::vector<std::string> provs;
        std::vector<State::Country::Pop> pops{};
        std::vector<State::Country::Building> buildings{};

    public:
    // constructors
        Country(const std::string &, const std::vector<std::string> &);
    // getters
        std::string getName(){return this->country;}
        std::vector<State::Country::Pop>& getPops(){return this->pops;}
    // setters
        void setCountryType(const std::string &);
    };

//data 
    std::vector<State::Country> countries{};
    std::vector<std::string> provs, traits, ar_res, im_provs{}, homelands{}, claims{};
    std::string id, name, hubs[5]{}, file_name, sub, city, port{}, farm, mine{}, wood{};
    unsigned int ar_land, res[10]{}, coal{}, iron{}, lead{}, sulfur{}, log{}, fish{}, whale{}, oil{}, rubber{}, gold{}, disc_gold{}, naval_exit{};
// Functions:

public:
// constructors 
    State(const std::filesystem::path &);
    State(const unsigned int &, const std::string &);
    State(const unsigned int[]);
    State(const std::string &);

// countries
    void create_country(const std::string &, std::vector<std::string> &);

// pops  
    void create_pops(const std::string &, const std::string &, const std::string &, const std::string &, const int & /*std::vector<State::Pop> &*/);
    void add_pop(const std::string &, const std::string &, const std::string &, const int &);

// buildings
    void create_buildings(const std::string &);
    void add_building(const std::string &, const std::string &, const std::string &, const std::string &, const int &, const int &);

// data manipulation
    void copy_state_info(State &);
    void remove_prov ();
    void calculate_remaining_provs(State &);
    void create_homelands(const std::string &);
    std::string data( std::string &);
    unsigned int data_int(std::string);
    void data_vector(std::vector<std::string> &, const std::string &, int);
    bool compare_string(const std::string &, std::string);
    void variable_string_vector(std::vector<std::string> &t, std::string &line);

// data printing
    void print_state_region();
    void print_pops();
    void print_buildings();
    void print_state();

// getters
    inline std::string getName() {return name;}
    inline std::string getSub() {return sub;}
    inline std::string getCity() {return city;}
    inline std::string getPort() {return port;}
    inline std::string getFarm() {return farm;}
    inline std::string getMine() {return mine;}
    inline std::string getWood() {return wood;}
    inline std::string getId() {return id;}
    inline unsigned int getLand() {return ar_land;}
    inline unsigned int getCoal() {return coal;}
    inline unsigned int getIron() {return iron;}
    inline unsigned int getLead() {return lead;}
    inline unsigned int getSulfur() {return sulfur;}
    inline unsigned int getLog() {return log;}
    inline unsigned int getFish() {return fish;}
    inline unsigned int getWhale() {return whale;}
    inline unsigned int getOil() {return oil;}
    inline unsigned int getRubber() {return rubber;}
    inline unsigned int getGold() {return gold;}
    inline unsigned int getDiscGold() {return disc_gold;}
    inline unsigned int getNavalExit() {return naval_exit;}
    inline std::vector<std::string> getProvs() {return provs;}
    inline std::vector<std::string> getTraits() {return traits;}
    inline std::vector<std::string> getResources() {return ar_res;}
    inline std::vector<std::string> getImProvs() {return im_provs;}            
    // inline std::vector<State::Pop> getPops() {return pops;}
    // inline std::vector<State::Building> getBuildingvoid save_states
    inline std::vector<std::string> getHomelands() {return homelands;}
    inline std::vector<State::Country>& getCountries() {return countries;}
    inline State::Country& getCo(const int &i) {return countries[i];}
    // inline std::string getCoName(const int &i) {return countries[i].country;} 

// setters
   void setName(const std::string &);
   void setId(const std::string &);
   void setSub(const std::string &);
   void setTraits(const std::vector<std::string> &);
   void setHubs(const std::string [5]);
   void setLand(const int &);
   void setArRes(const std::vector<std::string> &);
   void setRes(const int [10]);
   void setProvs(const std::vector<std::string> &);
   void setProv(const int &, const std::string &);
   void setPopSize(const int &, const int &);
   // void setResources(const std::vector<std::string> &);
   void setHomeland(const std::string &);
   void setClaim(const std::string &);
};

// class State_transfer : public State {
// // data
//     std::string origin{};
//     double ratio; 

// // Functions:
//     double calculate_ratio(const std::vector<std::string> &); 
//     unsigned int find_states(const std::string &, std::vector<State_transfer> &);
// public:
// // constructors
//     State_transfer(const std::string &, const int &, std::string &);
//     // State_transfer(const std::string &, const int &, const std::vector<std::string> &);
// };

#endif