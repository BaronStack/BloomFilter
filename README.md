## Description
There is a bloom fiter implementation libary with C++.

Two kind of bloom filter:
- Traditional bloom filter with hash function
- Counter bloom filter 

Implementation is simple.

### Traditional BloomFilter
Maintain a bit stream and several hash functions.

Input some string and produce a hash value by several hash functions. Then let the hash value's bit 
position to be 1.

### Counter BloomFilter
When we want to change the bloom filter's map strings, something like delete some string's bit position.

So we need to maintain a counter to replace the bit position, for we could decrease the counter when we want to delete a
string from bloom filter.And also ,we could increase the counter when we add a string to bloom filter.

> The counter bloomfilter's disadvantage is that there is much more memory wasted by storage the counter.

## Compiler
> I implemented the bloom filter on mac, and i used `machine/endian.h` to judge the big endian and little endian


### MAC

- make all

### Centos
- Change the code `include/filter.h`
    ```c
    #include <machine/endian.h>
    #if defined(__DARWIN_LITTLE_ENDIAN) && defined(__DARWIN_BYTE_ORDER)
      #define PLATFORM_IS_LITTLE_ENDIAN \
          (__DARWIN_BYTE_ORDER == __DARWIN_LITTLE_ENDIAN)
    #endif
    ```
    to
    ```c
    #include <endian.h>

    #ifndef PLATFORM_IS_LITTLE_ENDIAN
    #define PLATFORM_IS_LITTLE_ENDIAN (__BYTE_ORDER == __LITTLE_ENDIAN)
    #endif
    ```
- make all

## API Use
```c
// traditional bloom filter

BloomFilter filter2(16); 
string bloomfilter;

vector<string> buf = {"aaa","nihao","nihao","test"};
filter2.CreateFilter(&buf[0], buf.size(), &bloomfilter);

filter2.KeyMayMatch("test", bloomfilter); // match
filter2.KeyMayMatch("xihao", bloomfilter); // not match
 
// Counter bloom filter
CounterFilter filter(4);
vector<uint32_t> result; // storage counter bloomfilter's bit

vector<string> buf = {"aaa","nihao","nihao","test"};
filter.CreateCounterFilter(&buf[0], buf.size(), &result);

filter.PrintCounterFilter(result);
filter.KeyMayMatchWithCounter("test", result); // match
filter.KeyMayMatchWithCounter("nihao", result); // match
filter.RemoveKey("nihao", result);
filter.PrintCounterFilter(result);
filter.KeyMayMatchWithCounter("nihao", result); // match
filter.RemoveKey("nihao", result);
filter.KeyMayMatchWithCounter("nihao", result); // not match
```




