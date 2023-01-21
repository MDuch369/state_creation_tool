#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream> 

class State {

    class Pop {
        std::string culture, religion{}, type{};
        int size;

    public:
        Pop(const std::string &, const std::string &, const std::string &, const int &);
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
    };
    std::vector<State::Pop> pops{};
    std::vector<std::string> provs, traits, resources, im_provs{};
    std::string name, file_name, sub, city, port{}, farm, mine{}, wood{};
    unsigned int id, land, coal{}, iron{}, lead{}, sulfur{}, log{}, fish{}, whale{}, oil{}, rubber{}, gold{}, disc_gold{}, naval_exit{};    

    std::string data(const std::string &);
    unsigned int data_int(std::string);
    void data_vector(std::vector<std::string> &, const std::string &, int);
    bool compare_string(const std::string &, std::string);
    void variable_string_vector(std::vector<std::string> &t, std::string &line);

public:
    void copy_pops(const std::string &, const std::string &/*, std::vector<State::Pop> &*/);
    State(const unsigned int &, const std::string &);
    void add_pop(const std::string &, const std::string &, const std::string &, const int &,  const double &);
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
    // const std::vector<State::Pop> *getPopsRef() {
    //     const std::vector<State::Pop> *p = &pops;
    //     return p;
    // }
};


#endif