// Utils.cpp
// The implementation of the functions in Utils.hpp.

#include "Utils.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "Exception.hpp"

std::vector<std::string> CommandParser(std::string command, char delimiter) {
  std::vector<std::string> result;
  std::stringstream ss(command);
  std::string temp;
  while (getline(ss, temp, delimiter)) {
    result.push_back(temp);
  }
  return result;
}

bool is_integer(const std::string &s) {
  for (int i = 0; i < s.size(); i++) {
    if (i == 0 && s[i] == '-') {
      continue;
    }
    if (s[i] < '0' || s[i] > '9') {
      return false;
    }
  }
  return true;
}

bool is_number(const std::string &s) {
  bool point = false;
  for (int i = 0; i < s.size(); i++) {
    if (i == 0 && s[i] == '-') {
      continue;
    }
    if (s[i] == '.' && !point) {
      point = true;
      continue;
    }
    if (s[i] < '0' || s[i] > '9') {
      return false;
    }
  }
  return true;
}

bool is_positive_integer(const std::string &s) {
  if (s[0] == '0') {
    return false;
  }
  for (int i = 0; i < s.size(); i++) {
    if (s[i] < '0' || s[i] > '9') {
      return false;
    }
  }

  return true;
}

bool is_positive_number(const std::string &s) {
  bool point = false;
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == '.' && !point && i != 0) {
      point = true;
      continue;
    }
    if (s[i] < '0' || s[i] > '9') {
      return false;
    }
  }
  return true;
}

bool is_privilege(const std::string &s) {
  if (s.size() != 1) {
    return false;
  }
  if (s[0] < '0' || s[0] > '9') {
    return false;
  }
  int privilege = std::stoi(s);
  if (privilege == 1 || privilege == 3 || privilege == 7) {
    return true;
  }
  return false;
}

bool is_ISBN(const std::string &s) {
  if (s.size() > 20) {
    return false;
  }
  return true;
}

bool is_name_or_author_or_keyword(const std::string &s) {
  if (s.size() > 60) {
    return false;
  }
  for(auto &i:s){
    if(i=='"'){
      return false;
    }
  }
  return true;
}

bool is_price(const std::string &s) {
  if (s.size() > 13) {
    return false;
  }
  if(!is_positive_number(s)){
    return false;
  }
  return true;
}

bool is_quantity(const std::string &s){
  if(s.size()>10){
    return false;
  }
  if(!is_positive_integer(s)){
    return false;
  }
  return true;
}

std::vector<std::pair<std::string, std::string>> Modification_Parser(
    const std::vector<std::string> &input, const std::string &ISBN_, int &state,
    std::vector<std::string> &keyword) {
  bool visit[5] = {false, false, false, false, false};
  state = 0;
  std::vector<std::pair<std::string, std::string>> result;

  for (const auto &item : input) {
    std::stringstream ss(item);
    std::string order_;
    std::string value;

    if (!std::getline(ss, order_, '=')) {
      throw InvalidOperationException();
    }
    if (!std::getline(ss, value)) {
      throw InvalidOperationException();
    }

    if (order_ == "-ISBN") {
      if (visit[0] || value == ISBN_) {
        throw InvalidOperationException();
      }
      if (!is_ISBN(value)) {
        throw InvalidOperationException();
      }
      visit[0] = true;
      state += 1;
      result.emplace_back(order_, value);
    } else if (order_ == "-name") {
      if (visit[1]) {
        throw InvalidOperationException();
      }
      value = remove_quote(value);
      if(!is_name_or_author_or_keyword(value)){
        throw InvalidOperationException();
      }
      visit[1] = true;
      state += 2;
      result.emplace_back(order_, value);
    } else if (order_ == "-author") {
      if (visit[2]) {
        throw InvalidOperationException();
      }
      value = remove_quote(value);
      if(!is_name_or_author_or_keyword(value)){
        throw InvalidOperationException();
      }
      visit[2] = true;
      state += 4;
      result.emplace_back(order_, value);
    } else if (order_ == "-keyword") {
      if (visit[3]) {
        throw InvalidOperationException();
      }
      value = remove_quote(value);
      if(!is_name_or_author_or_keyword(value)){
        throw InvalidOperationException();
      }
      visit[3] = true;
      state += 8;
      std::stringstream ss_keyword(value);
      std::string word;
      while (std::getline(ss_keyword, word, '|')) {
        if(word.empty()){
          throw InvalidOperationException();
        }
        keyword.emplace_back(word);
      }
      std::sort(keyword.begin(), keyword.end());
      for (int j = 1; j < keyword.size(); j++) {
        if (keyword[j] == keyword[j - 1]) {
          throw InvalidOperationException();
        }
      }
      result.emplace_back(order_, value);
    } else if (order_ == "-price") {
      if (visit[4]) {
        throw InvalidOperationException();
      }
      if (!is_price(value)) {
        throw InvalidOperationException();
      }
      visit[4] = true;
      state += 16;
      result.emplace_back(order_, value);
    } else {
      throw InvalidOperationException();
    }
  }

  return result;
}

std::string remove_quote(const std::string &s) {
  if (s.size() < 2) {
    throw InvalidOperationException();
  }
  if (s[0] == '"' && s[s.size() - 1] == '"') {
    return s.substr(1, s.size() - 2);
  } else {
    throw InvalidOperationException();
  }
}

std::string intToFixedString(int value, int length) {
  std::string result = std::to_string(value);
  while (result.size() < length) {
    result = "0" + result;
  }
  return result;
}
