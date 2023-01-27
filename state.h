#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream> 

class State {
//sublcasses
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
//data 
    std::vector<State::Pop> pops{};
    std::vector<State::Building> buildings{};
    std::vector<std::string> provs, traits, resources, im_provs{}, homelands{};
    std::string name, file_name, sub, city, port{}, farm, mine{}, wood{};
    unsigned int id, land, coal{}, iron{}, lead{}, sulfur{}, log{}, fish{}, whale{}, oil{}, rubber{}, gold{}, disc_gold{}, naval_exit{};
    // double ratio; 
// Functions:
// private data manipulation 
    std::string data( std::string &);
    unsigned int data_int(std::string);
    void data_vector(std::vector<std::string> &, const std::string &, int);
    bool compare_string(const std::string &, std::string);
    void variable_string_vector(std::vector<std::string> &t, std::string &line);

public:
// constructors 
    State(const unsigned int &, const std::string &);
    State(const unsigned int[]);

// pops  
    void create_pops(const std::string &/*, const std::string &, std::vector<State::Pop> &*/);
    void add_pop(const std::string &, const std::string &, const std::string &, const int &);

// buildings
    void create_buildings(const std::string &);
    void add_building(const std::string &, const std::string &, const std::string &, const std::string &, const int &, const int &);

// data manipulation
    void copy_state_info(State &);
    void remove_prov ();
    void calculate_remaining_provs(State &);
    void create_homelands(const std::string &);

// data printing
    void print_state_region();
    void print_pops();
    void print_buildings();
    void print_state();

// getters
    inline std::string getName() {
    	return name;
	}
    inline std::string getSub() {
    	return sub;
	}
    inline std::string getCity() {
    	return city;
	}
    inline std::string getPort() {
    	return port;
	}
    inline std::string getFarm() {
    	return farm;
	}
    inline std::string getMine() {
    	return mine;
	}
    inline std::string getWood() {
    	return wood;
	}
    inline unsigned int getId() {
    	return id;
	}
    inline unsigned int getLand() {
    	return land;
	}
    inline unsigned int getCoal() {
    	return coal;
	}
    inline unsigned int getIron() {
    	return iron;
	}
    inline unsigned int getLead() {
    	return lead;
	}
    inline unsigned int getSulfur() {
    	return sulfur;
	}
    inline unsigned int getLog() {
    	return log;
	}
    inline unsigned int getFish() {
    	return fish;
	}
    inline unsigned int getWhale() {
    	return whale;
	}
    inline unsigned int getOil() {
    	return oil;
	}
    inline unsigned int getRubber() {
    	return rubber;
	}
    inline unsigned int getGold() {
    	return gold;
	}
    inline unsigned int getDiscGold() {
    	return disc_gold;
	}
    inline unsigned int getNavalExit() {
    	return naval_exit;
	}
    inline std::vector<std::string> getProvs() {
        return provs;
    }
    inline std::vector<std::string> getTraits() {
        return traits;
    }
    inline std::vector<std::string> getResources() {
        return resources;
    }
    inline std::vector<std::string> getImProvs() {
        return im_provs;
    }            
    inline std::vector<State::Pop> getPops() {
        return pops;
    }
    inline std::vector<State::Building> getBuildings() {
        return buildings;
    }
    inline std::vector<std::string> getHomelands() {
        return homelands;
    }

// setters
   void setSub(const std::string &);
   void setTraits(const std::vector<std::string> &);
   void setName(const std::string &);
   void setId(const int &);
   void setProvs(const std::vector<std::string> &);
   void setProv(const int &, const std::string &);
   void setPopSize(const int &, const int &);
   // void setResources(const std::vector<std::string> &);
};

#endif