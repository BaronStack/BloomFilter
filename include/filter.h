#pragma once

#include <iostream>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <machine/endian.h>

#include <machine/endian.h>
#if defined(__DARWIN_LITTLE_ENDIAN) && defined(__DARWIN_BYTE_ORDER)
  #define PLATFORM_IS_LITTLE_ENDIAN \
      (__DARWIN_BYTE_ORDER == __DARWIN_LITTLE_ENDIAN)
#endif
//#include <endian.h>
//
//#ifndef PLATFORM_IS_LITTLE_ENDIAN
//#define PLATFORM_IS_LITTLE_ENDIAN (__BYTE_ORDER == __LITTLE_ENDIAN)
//#endif

using namespace std;

typedef struct BitObj {
  uint32_t bit_pos;
  bool is1;

  BitObj():
    bit_pos(0), is1(false){}
}BITTYPE;

// 构造hash 值时区分大小端,需要统一字节序
static const bool kLittleEndian = PLATFORM_IS_LITTLE_ENDIAN;
static uint32_t Hash(const char* data, size_t n, uint32_t seed);

static uint32_t BloomHash(const std::string& key) {
	return Hash(key.c_str(), key.size(), 0xbc9f1d34);
}

static uint32_t DecodeFixed32(const char* ptr) {
  if (kLittleEndian) {
    // Load the raw bytes
    uint32_t result;
    memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
    return result;
  } else { // 如果是大端，需要转换字节序
    return ((static_cast<uint32_t>(static_cast<unsigned char>(ptr[0])))
        | (static_cast<uint32_t>(static_cast<unsigned char>(ptr[1])) << 8)
        | (static_cast<uint32_t>(static_cast<unsigned char>(ptr[2])) << 16)
        | (static_cast<uint32_t>(static_cast<unsigned char>(ptr[3])) << 24));
  }
}

// 构造bloom filter的hash函数
uint32_t Hash(const char* data, size_t n, uint32_t seed) {
  const uint32_t m = 0xc6a4a793;
  const uint32_t r = 24;
  const char* limit = data + n;
  uint32_t h = static_cast<uint32_t>(seed ^ (n * m));

  // Pick up four bytes at a time
  while (data + 4 <= limit) {
    uint32_t w = DecodeFixed32(data);
    data += 4;
    h += w;
    h *= m;
    h ^= (h >> 16);
  }

  // Pick up remaining bytes
  switch (limit - data) {
    // Note: The original hash implementation used data[i] << shift, which
    // promotes the char to int and then performs the shift. If the char is
    // negative, the shift is undefined behavior in C++. The hash algorithm is
    // part of the format definition, so we cannot change it; to obtain the same
    // behavior in a legal way we just cast to uint32_t, which will do
    // sign-extension. To guarantee compatibility with architectures where chars
    // are unsigned we first cast the char to int8_t.
    case 3:
      h += static_cast<uint32_t>(static_cast<int8_t>(data[2])) << 16;
    case 2:
      h += static_cast<uint32_t>(static_cast<int8_t>(data[1])) << 8;
    case 1:
      h += static_cast<uint32_t>(static_cast<int8_t>(data[0]));
      h *= m;
      h ^= (h >> r);
      break;
  }
  return h;
}

