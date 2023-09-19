#ifndef _POP_H_
#define _POP_H_

#include <iostream>

class Pop
{
private:
    
    int size;

    std::string culture, religion{}, type{};

public:
    Pop(const std::string &cult, const std::string &rel, const std::string &t, const int &s)
    : size{s}, culture{cult}, religion{rel}, type{t}
    {
    }
    
    ~Pop();

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

    void setSize(const int &);
    
    void print_pops(std::ofstream &);

};

#endif