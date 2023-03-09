#ifndef _PARSER_H_
#define _PARSER_H_

// #include <iostream>
// #include <vector>

    std::string data(std::string &);
    std::string data(std::string &, const char &);
    unsigned int data_int( std::string);
    void data_vector(std::vector<std::string> &, const std::string &, int);
    void variable_string_vector(std::vector<std::string> &, std::string &);
    std::string data_name(std::string &);
    bool find_string(std::string &, const std::string &, const int &pos = 0);
    void find_name(std::ifstream &, std::string &, const std::string &, std::string &);
    void find_data(std::ifstream &, std::string &, const std::string &, std::string &);
    void find_data(std::ifstream &, std::string &, const std::string &, const char &, std::string &);
    void find_data_int(std::ifstream &, std::string &, const std::string &, int &);

#endif