#pragma once

#include <iostream>
#include <string.h>
#include <vector>
#include <sys/types.h>

#include "filter.h"

using namespace std;

class BloomFilter {
public:
  BloomFilter(size_t bits_per_key):bits_per_key_(bits_per_key), 
                                  num_probe_(bits_per_key * 0.69),
                                  hash_func_(BloomHash){}

  ~BloomFilter() {}



  void CreateFilter(std::string* keys,int n, std::string *dst ) ;
  bool KeyMayMatch(std::string key, std::string& bloom_filter) ;

private:
  size_t bits_per_key_;
  size_t num_probe_;
  uint32_t (*hash_func_)(const std::string& dst);
};

