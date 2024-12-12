//main.cpp

#include <iostream>
#include <string>
#include "Key-Value-Database.h"

int main() {
  int n;
  std::cin >> n;
  File_Storage file_storage;
  for (int i = 0; i < n; ++i) {
    std::string command;
    std::cin >> command;
    if (command == "insert") {
      std::string index;
      int value;
      std::cin >> index >> value;
      file_storage.Insert(index, value);
    } else if (command == "delete") {
      std::string index;
      int value;
      std::cin >> index >> value;
      file_storage.Delete(index, value);
    } else if (command == "find") {
      std::string index;
      std::cin >> index;
      file_storage.Find(index);
    }
  }

  return 0;
}


