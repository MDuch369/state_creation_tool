#ifndef _State_H_
#define _State_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem> 

class State {
//sublcasses
// protected:
public:
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
    protected:
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

// data printing
    virtual void print_state_region();
    virtual void print_pops();
    virtual void print_buildings();
    virtual void print_state();

public:
    virtual ~State();

    virtual std::shared_ptr<State::Country> create_country(const std::string &, std::vector<std::string> &); 
    virtual void create_pops(const std::string &, const std::string &, const std::string &, const std::string &, const int &) = 0; 
    virtual void create_buildings(const std::string &, const std::string &, const int &, const int &, const std::vector<std::string> &) = 0;

// data printing
    virtual void print_entry();

// getters // ? refactor
    inline std::string getName() {return name;}
    inline std::string getSub() {return sub;}
    inline std::string getId() {return id;}
    inline std::string getNavalExit() {return naval_exit;}
    inline const unsigned int* getRes() {return res;}
    inline std::vector<std::string> getTraits() {return traits;}
    inline std::vector<std::string> getResources() {return ar_res;}
    inline std::vector<std::string> getImProvs() {return im_provs;}            
    inline std::vector<std::string> getHomelands() {return homelands;}
    inline std::vector<std::string> getClaims() {return claims;}
    inline std::vector<State::Country>& getCountries() {return countries;}
    inline State::Country& getCo(const int &i) {return countries[i];}

// setters // ? refactor
   void setName(const std::string &);
   void setId(const std::string &);
   void setSub(const std::string &);
   void setTraits(const std::vector<std::string> &);
   void setHubs(const std::string []);
   void setLand(const int &);
   void setArRes(const std::vector<std::string> &);
   void setRes(const int []);
//    void setProvs(const std::vector<std::string> &);
//    void setProv(const int &, const std::string &);
   void setPopSize(const int &, const int &);
   void setHomeland(const std::string &);
   void setClaim(const std::string &);
   void setArable(const int &);
   void setNavEx(const std::string &);

};

#endif