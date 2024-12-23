//Key-Value-Database.hpp

#ifndef KEY_VALUE_DATABASE_HPP
#define KEY_VALUE_DATABASE_HPP

#include<string>
#include<cstring>
#include<vector>
#include "MemoryRiver.hpp"

constexpr int MAX_STRING = 65;
constexpr int BLOCK_SIZE = 100;

template<typename T>
struct Key_Value {
  char key[MAX_STRING];
  T value;

  Key_Value(){
    value=T();
    memset(key, 0, sizeof(key));
  }

  Key_Value(const std::string &key, const T value): value(value) {
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

template<typename T>
struct Block {
  Key_Value<T> array[BLOCK_SIZE];
  size_t array_size;

  Block(): array_size(0) {
  };
};

template<typename T>
class File_Storage {
private:
  std::string filename;
  MemoryRiver<Index,2> index_file;
  MemoryRiver<Block<T>,0> block_file;

public:
  File_Storage(const std::string filename = "database"): filename(filename),index_file(filename + "_index.dat"),
                                                         block_file(filename + "_block.dat") {
    Index index_blank;
    Block<T> block_blank;
    if(!index_file.exist()) {
      index_file.initialise();
      block_file.initialise();
      index_blank.address=block_file.write(block_blank);
      index_file.write_info(index_file.write(index_blank),2);
      index_file.write_info(1,1);
    }
  };
  

  ~File_Storage()=default;

  void Insert(const std::string &key,const T &value);
  void Delete(const std::string &key,const T &value);
  void Update(const std::string &key,const T &value,const T &new_value);
  int Num();
  std::vector<T> Find(const std::string &key);
  std::vector<T> FindAll();
  std::vector<T> FindFirstN(const int &n,bool &flag);
  void Initialize();
};



#endif //KEY_VALUE_DATABASE_HPP


