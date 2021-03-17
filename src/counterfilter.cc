//
// Created by zhanghuigui on 2021/3/17.
//

#include "counterfilter.h"

// CreateCounterFilter: 创建一个计数功能的bloomfilter
// 参数1：keys ,输入多个string类型 的字符串
// 参数2： n, 输入的字符串串长度
// 参数3： 根据输入的字符串构造出来的计数bloom filter
//
// 原理如下：
// 维护一个uint32_t 的数组，其中每一个元素代表一个bit位
// 0 0 0 0 0 ... 0 0 0
//
// add string1 --> hash_func
//          |
//          |
// 0 1 0 1 1 ... 1 0 0
//
// add string2 --> hash_func
//          |
//          |
// 1 2 1 1 2 ... 1 0 1
//
// 如果想要从构造的bloom filter中删除string1,则只需要让hash_func
// 的对应 "bit" 位的数值-1 即可
// remove string1 --> hash_func
//          |
//          |
// 1 1 1 0 1 ... 0 0 1
void CounterFilter::CreateCounterFilter(std::string* keys, int n,
																				vector<uint32_t> *dst) {
	size_t bits = n* bits_per_key_;

	// 构造一个存放 uint32_t的bloomfilter数组 -- dst
	if(bits < 32) {
		bits = 32;
	}

	size_t bytes = (bits + 7) / 8;
	bits = bytes * 8;

	const size_t init_size = dst->size();
	dst->resize(bits, 0);
	uint32_t* array = &(*dst)[init_size];
	for(size_t i = 0;i < static_cast<size_t>(n); i++) {
		uint32_t h = hash_func_(keys[i]);
		vector<BITTYPE> bit_types;

		Bitcount(h, &bit_types);

		for(size_t j = 0; j < bit_types.size(); j++) {
			if(bit_types[j].is1) {
				array[bit_types[j].bit_pos] ++;
			}
		}
	}
}

// Bitcount: 检测输入的uint32_t 数据哪一个bit位为1，并保存下来
// 参数1：x, 输入一个uint32_t的元素
// 参数2：bit_types, 最终存放x中bit位为1以及其对应bit位的数组
//
// 通过 x & 1 的结果能判断当前x的最后一位是否是1
// 再x = x >> 1，来循环判断
void CounterFilter::Bitcount(uint32_t x, vector<BITTYPE> *bit_types) {
	int b;
	for (b = 0; x != 0; x >>= 1) {
		BITTYPE bit;
		bit.bit_pos = b;
		if (x & 1) {
			bit.is1 = true;
		}
		b++;
		bit_types->push_back(bit);
	}
}

void CounterFilter::PrintCounterFilter(vector<uint32_t> bloom_filter) {
	cout << "Print bloom filter " << endl;

	for (int i = 0;i < bloom_filter.size(); i++) {
		cout << bloom_filter[i] << " ";
	}
	cout << endl;
}

// KeyMayMatchWithCounter: 判断输入的string key是否存在于bloom filter之中
// 参数1： key,输入的元素
// 参数2： 之前构造好的bloom filter
bool CounterFilter::KeyMayMatchWithCounter(std::string key,
																				 vector<uint32_t> bloom_filter) {
	const size_t len = bloom_filter.size();
	if (len < 2) return false;


	uint32_t h = hash_func_(key);
	vector<BITTYPE> bit_types;

	Bitcount(h, &bit_types);

	cout << endl;
	for(int j = 0; j < bloom_filter.size(); j++) {
		uint32_t exists = bloom_filter[bit_types[j].bit_pos];
		bool is1 = bit_types[j].is1;
		if(is1 && exists == 0) {
			cout << endl;
			return false;
		}
	}

	return true;
}

// RemoveKey: 从已经构造好的bloom filter中移除一个元素
void CounterFilter::RemoveKey(std::string key,
														vector<uint32_t> &bloom_filter) {
	const size_t len = bloom_filter.size();
	if (len < 2) return ;

	vector<BITTYPE> bit_types;
	uint32_t h = hash_func_(key);

	Bitcount(h, &bit_types);

	for(int j = 0; j < bloom_filter.size(); j++) {
		uint32_t pos = bit_types[j].bit_pos;
		bool is1 = bit_types[j].is1;
		if(is1 && bloom_filter[pos] != 0) {
			bloom_filter[pos] --;
		}
	}
}
