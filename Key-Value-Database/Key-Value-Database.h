//
// Created by apple on 24-12-9.
//

#ifndef KEY_VALUE_DATABASE_H
#define KEY_VALUE_DATABASE_H

#include<string>
#include<cstring>
#include<fstream>
#include "MemoryRiver.h"

constexpr int MAX_STRING = 65;
constexpr int BLOCK_SIZE = 4;

struct Key_Value {
  char key[MAX_STRING];
  int value;

  Key_Value(): value(0) {
    memset(key, 0, sizeof(key));
  }

  Key_Value(const std::string &key, const int value): value(value) {
    strcpy(this->key, key.c_str());
  }

  bool operator>(const Key_Value& other) const{
    if(strcmp(key,other.key)==0) {
      return value>other.value;
    }else {
      return strcmp(key,other.key)>0;
    }
  }

  bool operator==(const Key_Value &other) const {
    return (strcmp(key,other.key)==0)&&(value==other.value);
  }

};

struct Index {
  //保存本块在文件中的地址和下一块Index在index文件中的位置
  int address;
  int next;

  //保存本块的key范围
  char min_key[MAX_STRING];
  char max_key[MAX_STRING];

  //保存数组大小
  size_t array_size;

  Index(): address(0), next(-1),array_size(0) {
    memset(min_key, 0, sizeof(min_key));
    memset(max_key, 0, sizeof(max_key));
  }
};

struct Block {
  Key_Value array[BLOCK_SIZE];
  size_t array_size;

  Block(): array_size(0) {
  };
};

class File_Storage {
private:
  std::string filename;
  MemoryRiver<Index,2> index_file;
  MemoryRiver<Block,0> block_file;

public:
  File_Storage(const std::string filename = "database"): filename(filename),index_file(filename + "_index.dat"),
                                                         block_file(filename + "_block.dat") {
    Index index_blank;
    Block block_blank;
    if(!index_file.exist()) {
      index_file.initialise();
      block_file.initialise();
      index_blank.address=block_file.write(block_blank);
      index_file.write_info(index_file.write(index_blank),2);
      index_file.write_info(1,1);
    }
  };
  ~File_Storage()=default;

  void Insert(const std::string &key,const int &value);
  void Delete(const std::string &key,const int &value);
  void Find(const std::string &key);

  void Initialize();
};


#endif //KEY_VALUE_DATABASE_H
