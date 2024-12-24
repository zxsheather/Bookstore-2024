//Utils.hpp
//Include some useful tools.

#ifndef UTILS_HPP
#define UTILS_HPP
#include<vector>
#include<string>

bool is_number(const std::string &s);

bool is_integer(const std::string &s);

bool is_positive_number(const std::string &s);

bool is_positive_integer(const std::string &s);

bool is_privilege(const std::string &s);

bool is_ISBN(const std::string &s);

bool is_name_or_author_or_keyword(const std::string &s);

bool is_price(const std::string &s);

bool is_quantity(const std::string &s);

bool is_userID_or_password(const std::string &s);

bool is_username(const std::string &s);

std::vector<std::string> CommandParser(std::string command,char delimiter);

std::vector<std::pair<std::string,std::string>> Modification_Parser(const std::vector<std::string> &input,const std::string &ISBN, int &state,std::vector<std::string> &keyword);

std::string remove_quote(const std::string &s);

std::string intToFixedString(int value,int length=6);


#endif