# include <iostream>
# include <vector>
# include <algorithm>
# include <sstream>

void save_provinces(std::vector<std::string> &provs) {
    std::string prov, tmp; 
    
    std::cout<<"Enter provinces to transfer: "<<std::endl;
    std::getline(std::cin, prov);
    prov.erase (std::remove(prov.begin(), prov.end(), ' '), prov.end());
    transform(prov.begin(), prov.end(), prov.begin(), toupper);

    std::stringstream ss(prov);

    while(getline(ss, tmp, 'X')){
        provs.push_back(tmp);

    }
}

void debug(const std::vector<std::string> &provs) {
    for(std::string s : provs) {
        std::cout<< s << std::endl;
    }
}


int main() {

    std::vector<std::string> provinces = {};

    save_provinces( provinces );

    debug( provinces );
    
    return 0;
}