// Key-Value-Database.cpp

#include "Key-Value-Database.hpp"

#include <algorithm>
#include <vector>

#include "Data.hpp"
#include "MemoryRiver.hpp"
#include "Exception.hpp"

template <typename T>
void File_Storage<T>::Insert(const std::string &key, const T &value) {
  Key_Value<T> kv(key, value);
  int start = 0;
  index_file.get_info(start, 2);
  int index_pos = start;
  int block_pos = 0;
  Index index;
  Block<T> block;
  int count;
  index_file.get_info(count, 1);
  if (count == 0) {
    block.array[0] = kv;
    block.array_size++;
    index.address = block_file.write(block);
    index.array_size = block.array_size;
    strcpy(index.max_key, key.c_str());
    strcpy(index.min_key, key.c_str());
    index_file.write_info(1, 1);
    index_file.write_info(index_file.write(index), 2);
  }
  for (int i = 0; i < count; ++i) {
    index_file.read(index, index_pos);
    if (strcmp(index.max_key, key.c_str()) >= 0 || index.next == -1) {
      block_pos = index.address;
      block_file.read(block, block_pos);

      if (block.array_size == BLOCK_SIZE) {
        Block<T> new_block;
        Index new_index;
        int mid = BLOCK_SIZE / 2;
        for (int j = mid; j < block.array_size; ++j) {
          new_block.array[j - mid] = block.array[j];
          block.array[j] = (Key_Value<T>){};
          new_block.array_size++;
        }
        block.array_size = mid;
        if (kv > new_block.array[0]) {
          new_block.array[new_block.array_size] = kv;
          new_block.array_size++;
          for (int j = new_block.array_size - 1; j >= 1; --j) {
            if (new_block.array[j - 1] > new_block.array[j]) {
              std::swap(new_block.array[j], new_block.array[j - 1]);
            } else {
              break;
            }
          }
        } else {
          block.array[block.array_size] = kv;
          block.array_size++;
          for (int j = block.array_size - 1; j >= 1; --j) {
            if (block.array[j - 1] > block.array[j]) {
              std::swap(block.array[j], block.array[j - 1]);
            } else {
              break;
            }
          }
        }
        index.array_size = block.array_size;
        new_index.array_size = new_block.array_size;
        new_index.address = block_file.write(new_block);
        strcpy(new_index.min_key, new_block.array[0].key);
        strcpy(new_index.max_key,
               new_block.array[new_block.array_size - 1].key);
        new_index.next = index.next;
        block_file.update(block, block_pos);
        strcpy(index.max_key, block.array[block.array_size - 1].key);
        strcpy(index.min_key, block.array[0].key);
        index.next = index_file.write(new_index);
        index_file.update(index, index_pos);
        index_file.write_info(count + 1, 1);
        break;
      } else {
        block.array[block.array_size] = kv;
        block.array_size++;
        for (int j = block.array_size - 1; j >= 1; --j) {
          if (block.array[j - 1] > block.array[j]) {
            std::swap(block.array[j], block.array[j - 1]);
          } else {
            break;
          }
        }
        index.array_size = block.array_size;
        if (strcmp(block.array[block.array_size - 1].key, index.max_key) != 0 ||
            block.array_size == 1) {
          strcpy(index.max_key, block.array[block.array_size - 1].key);
        }
        if (strcmp(block.array[0].key, index.min_key) != 0 ||
            block.array_size == 1) {
          strcpy(index.min_key, block.array[0].key);
        }
        index_file.update(index, index_pos);
        block_file.update(block, block_pos);
        break;
      }
    } else {
      index_pos = index.next;
    }
  }
}

template <typename T>
void File_Storage<T>::Delete(const std::string &key, const T &value) {
  Key_Value<T> kv(key, value);
  int start = 0;
  index_file.get_info(start, 2);
  int index_pos = start;
  int prev_pos = -1;
  int prev_array_size = -1;
  Index index;
  Block<T> block;
  int count;
  index_file.get_info(count, 1);
  for (int i = 0; i < count; ++i) {
    index_file.read(index, index_pos);
    if (strcmp(index.min_key, key.c_str()) > 0) {
      return;
    } else if (strcmp(index.max_key, key.c_str()) >= 0) {
      block_file.read(block, index.address);
      for (int j = 0; j < block.array_size; ++j) {
        if (block.array[j] == kv) {
          for (int k = j; k < block.array_size - 1; ++k) {
            block.array[k] = block.array[k + 1];
          }
          block.array_size--;

          if (block.array_size == 0) {
            if (prev_pos != -1) {
              Index prev_index;
              index_file.read(prev_index, prev_pos);
              prev_index.next = index.next;
              index_file.update(prev_index, prev_pos);
            } else {
              index_file.write_info(index.next, 2);
            }
            index_file.write_info(count - 1, 1);
          } else if (prev_pos != -1 && block.array_size <= BLOCK_SIZE / 2 &&
                     prev_array_size <= BLOCK_SIZE / 2) {
            Index prev_index;
            Block<T> prev_block;
            index_file.read(prev_index, prev_pos);
            block_file.read(prev_block, prev_index.address);
            prev_index.next = index.next;
            Block<T> new_prev_block;

            int k = 0, l = 0, r = 0;
            while (true) {
              if (l == block.array_size) {
                while (r < prev_block.array_size) {
                  new_prev_block.array[k] = prev_block.array[r];
                  ++r;
                  ++k;
                }
                break;
              }
              if (r == prev_block.array_size) {
                while (l < block.array_size) {
                  new_prev_block.array[k] = block.array[l];
                  ++l;
                  ++k;
                }
                break;
              }
              if (block.array[l] > prev_block.array[r]) {
                new_prev_block.array[k] = prev_block.array[r];
                ++k;
                ++r;
              } else {
                new_prev_block.array[k] = block.array[l];
                ++k;
                ++l;
              }
            }
            new_prev_block.array_size = k;
            prev_index.array_size = k;
            strcpy(prev_index.min_key, new_prev_block.array[0].key);
            strcpy(prev_index.max_key, new_prev_block.array[k - 1].key);
            index_file.update(prev_index, prev_pos);
            block_file.update(new_prev_block, prev_index.address);
            index_file.write_info(count - 1, 1);
          } else {
            if (strcmp(block.array[0].key, index.min_key) != 0) {
              strcpy(index.min_key, block.array[0].key);
            }
            if (strcmp(block.array[block.array_size - 1].key, index.max_key) !=
                0) {
              strcpy(index.max_key, block.array[block.array_size - 1].key);
            }
            index.array_size = block.array_size;
            index_file.update(index, index_pos);
            block_file.update(block, index.address);
          }

          return;
        }
      }
    }
    prev_array_size = index.array_size;
    prev_pos = index_pos;
    index_pos = index.next;
  }
}

template <typename T>
std::vector<T> File_Storage<T>::Find(const std::string &key) {
  int start = 0, count = 0;
  std::vector<T> result;
  index_file.get_info(start, 2);
  index_file.get_info(count, 1);
  int index_pos = start;
  Index index;
  Block<T> block;
  bool found = false;
  for (int i = 0; i < count; ++i) {
    index_file.read(index, index_pos);
    if (strcmp(index.min_key, key.c_str()) > 0) {
      break;
    } else if (strcmp(index.max_key, key.c_str()) >= 0) {
      block_file.read(block, index.address);
      int l = 0, r = block.array_size - 1;
      while (l < r) {
        int mid = (l + r + 1) / 2;
        if (strcmp(block.array[mid].key, key.c_str()) < 0) {
          l = mid;
        } else {
          r = mid - 1;
        }
      }
      if (strcmp(block.array[l].key, key.c_str()) != 0) {
        l = l + 1;
      }
      while (l < block.array_size &&
             (strcmp(block.array[l].key, key.c_str()) == 0)) {
        found = true;
        // std::cout<<block.array[l].value<<' ';
        result.push_back(block.array[l].value);
        ++l;
      }
    }
    index_pos = index.next;
  }
  return result;
}

template <typename T>
void File_Storage<T>::Update(const std::string &key, const T &value,
                             const T &new_value) {
  Key_Value<T> kv(key, value);
  Key_Value<T> new_kv(key, new_value);
  int start = 0;
  index_file.get_info(start, 2);
  int index_pos = start;
  Index index;
  Block<T> block;
  int count;
  index_file.get_info(count, 1);
  for (int i = 0; i < count; ++i) {
    index_file.read(index, index_pos);
    if (strcmp(index.min_key, key.c_str()) > 0) {
      return;
    } else if (strcmp(index.max_key, key.c_str()) >= 0) {
      block_file.read(block, index.address);
      for (int j = 0; j < block.array_size; ++j) {
        if (block.array[j] == kv) {
          block.array[j] = new_kv;
          block_file.update(block, index.address);
          return;
        }
      }
    }
    index_pos = index.next;
  }
}

template <typename T>
std::vector<T> File_Storage<T>::FindAll() {
  int start = 0, count = 0;
  std::vector<T> result;
  index_file.get_info(start, 2);
  index_file.get_info(count, 1);
  int index_pos = start;
  for (int i = 0; i < count; ++i) {
    Index index;
    index_file.read(index, index_pos);
    Block<T> block;
    block_file.read(block, index.address);
    for (int j = 0; j < block.array_size; ++j) {
      result.push_back(block.array[j].value);
    }
    index_pos = index.next;
  }
  return result;
}

template <typename T>
std::vector<T> File_Storage<T>::FindFirstN(const int &n,bool &flag){
  int start=0,count=0;
  std::vector<T> result;
  index_file.get_info(start,2);
  index_file.get_info(count,1);
  int index_pos=start;
  for(int i=0;i<count;++i){
    Index index;
    index_file.read(index,index_pos);
    Block<T> block;
    block_file.read(block,index.address);
    for(int j=0;j<block.array_size;++j){
      result.push_back(block.array[j].value);
      if(result.size()==n){
        flag=true;
        return result;
      }
    }
    index_pos=index.next;
  }
  if(result.size()<n){
    flag=false;
    return result;
  }
  flag=true;
  return result;
}

template <typename T>
void File_Storage<T>::Initialize() {
  Index index_blank;
  Block<T> block_blank;
  index_file.initialise();
  block_file.initialise();
  index_blank.address = block_file.write(block_blank);
  index_file.write_info(index_file.write(index_blank), 2);
  index_file.write_info(1, 1);
}

template <typename T>
int File_Storage<T>::Num(){
  int count;
  index_file.get_info(count,1);
  return count;
}


template class File_Storage<User_Info>;
template class File_Storage<Book_Info>;
template class File_Storage<ISBN_String>;
template class File_Storage<Transaction_Info>;
