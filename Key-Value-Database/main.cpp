// main.cpp

#include <iostream>
#include <string>

#include "Key-Value-Database.h"

int main() {
  // freopen("/Users/apple/Desktop/Programming/ACMOJ/BookStore/Bookstore-2024/Key-Value-Database/Test/output/test_data.txt",
  // "r", stdin); freopen("output.txt", "w", stdout);
  int n;
  std::cin >> n;
  File_Storage file_storage;
  // file_storage.Initialize();
  for (int i = 0; i < n; ++i) {
    // if(i>=2458){
    //   int a=0;
    // }
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
      // std::cout<<"Line "<<i+2<<": ";
      file_storage.Find(index);
    }
  }

  return 0;
}
