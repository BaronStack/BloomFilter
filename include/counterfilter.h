#pragma once

#include <iostream>
#include <string.h>
#include <vector>
#include <sys/types.h>

#include "filter.h"

using namespace std;

class CounterFilter {
public:
  CounterFilter(size_t bits_per_key):
                  bits_per_key_(bits_per_key),
                  hash_func_(BloomHash){}

  ~CounterFilter() {}

//  void CreateFilter(std::string* keys,int n, std::string *dst ) override;
//  bool KeyMayMatch(std::string key, std::string& bloom_filter) override;

  // 下面的三个函数主要是为了构造counter bloom_filter，来让
  // bloom filter支持动态得增加/删除key,只是会消耗更多的内存空间
  void CreateCounterFilter(std::string* keys, int n,
                                        vector<uint32_t> *dst);
  bool KeyMayMatchWithCounter(std::string key, 
                              vector<uint32_t> bloom_filter);
  void RemoveKey(std::string key, vector<uint32_t> &bloom_filter);

  // 计算一个uint32_6 中哪个bit位为1, 将结果存放在bit_types中
  static void Bitcount(uint32_t x, vector<BITTYPE> *bit_types);

  static void PrintCounterFilter(vector<uint32_t> bloom_filter);

private:
  size_t bits_per_key_;
  uint32_t (*hash_func_)(const std::string& key);
};

