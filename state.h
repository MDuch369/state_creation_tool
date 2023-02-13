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
            int size;
            std::string culture, religion{}, type{};

        public:
        // constructor
            Pop(const std::string &, const std::string &, const std::string &, const int &);
        // getters
            inline std::string getCult() {return culture;}
            inline std::string getRel() {return religion;}
            inline std::string getType() {return type;}
            inline int getSize() {return size;}
        // setters
            void setSize(const int &);
        };

        class Building {
        // data
            int level, reserves;
            std::string type, dlc{}; 
            std::vector<std::string> prod;
        public:
        // constructor
            Building(const std::string &, const int &, const int &, const std::vector<std::string> &);
        // getters
            inline std::string getType() {return type;}
            inline std::string getDlc() {return dlc;}
            inline std::vector<std::string> getProd() {return prod;}
            inline int getLvl() {return level;}
            inline int getRes() {return reserves;}        
        // setters
            void setLvl(const int &);
        };

    // data
        std::string country, type{};
        std::vector<std::string> provs;
        std::vector<State::Country::Pop> pops{};
        std::vector<State::Country::Building> buildings{};

    public:
    // constructors
        Country(const std::string &);
        Country(const std::string &, const std::vector<std::string> &);
        Country(const std::string &, const std::string &, const std::vector<std::string> &);
    // getters
        std::string getName(){return this->country;}
        std::string getType(){return this->type;}
        std::vector<State::Country::Pop>& getPops(){return this->pops;}
        std::vector<State::Country::Building>& getBuilds(){return this->buildings;}
        std::vector<std::string> &getProvs(){return this->provs;}
    // setters
        void setCountryType(const std::string &);
    };

//data 
    std::vector<State::Country> countries{};
    std::vector<std::string> traits, ar_res, im_provs{}, homelands{}, claims{};
    std::string id, name, hubs[5]{}, file_name, sub, naval_exit{};
    unsigned int res[12]{};
    // hubs = 0-city, 1-port, 2-farm, 3-mine, 4-wood
    // res = 0-arable land, 1-coal, 2-iron, 3-lead, 4-sulfur, 5-logging, 6-fishing, 7-whaling, 8-gold, 9-disc. gold, 10-rubber, 11-oil
// Functions:

public:
// constructors 
    State();
    State(const std::filesystem::path &);
    State(const unsigned int &, const std::string &);
    State(const unsigned int[]);
    State(const std::string &);
    // State(State_transfer &);

// countries
    void create_country(const std::string &, std::vector<std::string> &);

// pops  
    void create_pops(const std::string &, const std::string &, const std::string &, const std::string &, const int & /*std::vector<State::Pop> &*/);
    void add_pop(const std::string &, const std::string &, const std::string &, const int &);

// buildings
    void create_buildings(const std::string &, const std::string &, const int &, const int &, const std::vector<std::string> &);
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
    /*inline std::string getCity() {return city;}
    inline std::string getPort() {return port;}
    inline std::string getFarm() {return farm;}
    inline std::string getMine() {return mine;}
    inline std::string getWood() {return wood;}*/
    inline std::string getId() {return id;}
    // inline unsigned int getLand() {return ar_land;}
    /*inline unsigned int getCoal() {return coal;}
    inline unsigned int getIron() {return iron;}
    inline unsigned int getLead() {return lead;}
    inline unsigned int getSulfur() {return sulfur;}
    inline unsigned int getLog() {return log;}
    inline unsigned int getFish() {return fish;}
    inline unsigned int getWhale() {return whale;}
    inline unsigned int getOil() {return oil;}
    inline unsigned int getRubber() {return rubber;}
    inline unsigned int getGold() {return gold;}
    inline unsigned int getDiscGold() {return disc_gold;} */
    inline std::string getNavalExit() {return naval_exit;}
    inline const unsigned int* getRes() {return res;}
    // inline std::vector<std::string> getProvs() {return provs;}
    inline std::vector<std::string> getTraits() {return traits;}
    inline std::vector<std::string> getResources() {return ar_res;}
    inline std::vector<std::string> getImProvs() {return im_provs;}            
    inline std::vector<std::string> getHomelands() {return homelands;}
    inline std::vector<std::string> getClaims() {return claims;}
    inline std::vector<State::Country>& getCountries() {return countries;}
    inline State::Country& getCo(const int &i) {return countries[i];}

// setters
   void setName(const std::string &);
   void setId(const std::string &);
   void setSub(const std::string &);
   void setTraits(const std::vector<std::string> &);
   void setHubs(const std::string [/*5*/]);
   void setLand(const int &);
   void setArRes(const std::vector<std::string> &);
   void setRes(const int [/*12*/]);
   void setProvs(const std::vector<std::string> &);
   void setProv(const int &, const std::string &);
   void setPopSize(const int &, const int &);
   // void setResources(const std::vector<std::string> &);
   void setHomeland(const std::string &);
   void setClaim(const std::string &);
   void setArable(const int &);
   void setNavEx(const std::string &);

// debug functions
    // void debug_print_provs();
};

/**** STATE TRANSFER CLASS ****/

class State_transfer : public State {
// data
    // bool origin_found{0};
    int origin_pos{};
    std::string origin{};
    std::vector<std::string> transfer_provs;

// Functions:
    // double calculate_ratio(const std::vector<std::string> &); 
    unsigned int find_states(const std::string &, std::vector<State_transfer> &);

public:
// constructors
    State_transfer(const std::string &, const std::string &, const std::string &); // TODO replace this constructor with the one below
    State_transfer(const std::string &, const std::string &, const std::vector<std::string> &);  
    // State_transfer(const std::string &, const int &, const std::vector<std::string> &);

// countries
    State::Country create_transfer_country(State::Country &, std::vector<std::string> &, const double &);

// getters
    inline std::vector<std::string> getProvs() {return this->transfer_provs;}
    inline std::string getOrigin() {return this->origin;}

// transfer
    void find_origin_states(const std::vector<State> &, std::vector<State_transfer> &);
    void calculate_resources(std::vector<State> &);
    void create_target_states(std::vector<State_transfer> &/*, std::vector<State> &*/);
    void create_remaining_states(std::vector<State> &, std::vector<State> &);
    void calculate_remaining_resources(std::vector<State> &/*, std::vector<State> &*/);

// debug functions
    void debug_print_provs();
};

#endif