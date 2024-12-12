//Key-Value-Database.cpp

#include<iostream>
#include "Key-Value-Database.h"
#include "MemoryRiver.h"

void File_Storage::Insert(const std::string &key, const int &value) {
  Key_Value kv(key,value);
  int start;
  index_file.get_info(start,2);
  int index_pos=start;
  int block_pos=0;
  Index index;
  Block block;
  int count;
  index_file.get_info(count,1);
  if(count==0) {
    block.array[0]=kv;
    block.array_size++;
    index.address=block_file.write(block);
    strcpy(index.max_key,key.c_str());
    strcpy(index.min_key,key.c_str());
    index_file.write_info(1,1);
    index_file.write(index);
  }
  for(int i=0;i<count;++i) {
    index_file.read(index,index_pos);
    if(strcmp(index.max_key,key.c_str())>=0||index.next==-1) {

      block_pos=index.address;
      block_file.read(block,block_pos);

      if(block.array_size==BLOCK_SIZE) {
        Block new_block;
        Index new_index;
        int mid=BLOCK_SIZE/2;
        for(int j=mid;j<block.array_size;++j) {
          new_block.array[j-mid]=block.array[j];
          block.array[j+mid]=(Key_Value());
          block.array_size--;
          new_block.array_size++;
        }

        block.array[block.array_size]=kv;
        block.array_size++;
        for(int j=block.array_size-1;j>=1;--j) {
          if(block.array[j]>block.array[j-1]) {
            std::swap(block.array[j],block.array[j-1]);
          }else {
            break;
          }
        }

        new_index.address=block_file.write(new_block);
        strcpy(new_index.min_key,new_block.array[0].key);
        strcpy(new_index.max_key,new_block.array[new_block.array_size-1].key);
        new_index.next=index.next;
        block_file.update(block,block_pos);
        strcpy(index.max_key,block.array[block.array_size-1].key);
        strcpy(index.min_key,block.array[0].key);
        index.next=index_file.write(new_index);
        index_file.update(index,index_pos);
        index_file.write_info(count+1,1);
        break;
      }else {
        block.array[block.array_size]=kv;
        block.array_size++;
        for(int j=block.array_size-1;j>=1;--j) {
          if(block.array[j-1]>block.array[j]) {
            std::swap(block.array[j],block.array[j-1]);
          }else {
            break;
          }
        }
        if(strcmp(block.array[block.array_size-1].key,index.max_key)!=0||block.array_size==1) {
          strcpy(index.max_key,block.array[block.array_size-1].key);
          index_file.update(index,index_pos);
        }
        if(strcmp(block.array[0].key,index.min_key)!=0||block.array_size==1){
          strcpy(index.min_key,block.array[0].key);
          index_file.update(index,index_pos);
        }
        block_file.update(block,block_pos);
        break;
      }
    }else {
      index_pos=index.next;
    }
  }
}

void File_Storage::Delete(const std::string &key, const int &value) {
  Key_Value kv(key,value);
  int start=0;
  index_file.get_info(start,2);
  int index_pos=start;
  int prev_pos=-1;
  Index index;
  Block block;
  int count;
  index_file.get_info(count,1);
  for(int i=0;i<count;++i) {
    index_file.read(index,index_pos);
    if(strcmp(index.min_key,key.c_str())>0) {
      return;
    }else if(strcmp(index.max_key,key.c_str())>0) {
      block_file.read(block,index.address);
      for(int j=0;j<block.array_size;++j) {
        if(block.array[j]==kv) {
          for(int k=j;k<block.array_size-1;++k) {
            block.array[k]=block.array[k+1];
          }
          block.array_size--;

          if(block.array_size==0) {
            if(prev_pos!=-1) {
              Index prev_index;
              index_file.read(prev_index,prev_pos);
              prev_index.next=index.next;
              index_file.update(prev_index,prev_pos);
            }else {
              index_file.write_info(index.next,2);
            }
            index_file.write_info(count-1,1);
          }else {
            if(strcmp(block.array[0].key,index.min_key)!=0) {
              strcpy(index.min_key,block.array[0].key);
              index_file.update(index,index_pos);
            }
            if(strcmp(block.array[block.array_size-1].key,index.max_key)!=0) {
              strcpy(index.max_key,block.array[block.array_size-1].key);
              index_file.update(index,index_pos);
            }
            block_file.update(block,index.address);
          }

          break;
        }
        prev_pos=index_pos;
        index_pos=index.next;
      }
    }
  }
}

void File_Storage::Find(const std::string &key) {
  int start=0,count=0;
  index_file.get_info(start,2);
  index_file.get_info(count,1);
  int index_pos=start;
  Index index;
  Block block;
  bool found=false;
  for(int i=0;i<count;++i) {
    index_file.read(index,index_pos);
    if(strcmp(index.min_key,key.c_str())>0) {
      return;
    }else if(strcmp(index.max_key,key.c_str())>=0) {
      block_file.read(block,index.address);
      int l=0,r=block.array_size-1;
      while(l<r) {
        int mid=(l+r+1)/2;
        if(strcmp(block.array[mid].key,key.c_str())<0) {
          l=mid;
        }else {
          r=mid-1;
        }
      }
      if(strcmp(block.array[l].key,key.c_str())!=0) {
        l=l+1;
      }
      while(l<block.array_size&&(strcmp(block.array[l].key,key.c_str())==0)) {
        found=true;
        std::cout<<block.array[l].value<<' ';
        ++l;
      }
    }
    index_pos=index.next;
  }
  if(!found) {
    std::cout<<"null";
  }
  std::cout<<'\n';
}

void File_Storage::Initialize() {
  index_file.initialise();
  block_file.initialise();
}





