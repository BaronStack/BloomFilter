//
// Created by zhanghuigui on 2021/3/17.
//

#include <iostream>
#include <vector>

#include "bloomfilter.h"
#include "counterfilter.h"
using namespace std;

int main() {
	CounterFilter filter(4);
	// BloomFilter filter2(16); // test traditional filter
	vector<uint32_t> result;
	string bloomfilter;

	vector<string> buf = {"aaa","nihao","nihao","test"};
	filter.CreateCounterFilter(&buf[0], buf.size(), &result);
	// filter2.CreateFilter(&buf[0], buf.size(), &bloomfilter);

	filter.PrintCounterFilter(result);
	if(filter.KeyMayMatchWithCounter("test", result)) { // match
		cout << "test is match" << endl;
	}

	// if(filter2.KeyMayMatch("test", bloomfilter)) { // test traditional filter
	// 	cout << "test is match" << endl;
	// }

	if(filter.KeyMayMatchWithCounter("nihao", result)) { // match
		cout << "nihao is match" << endl;
	}

	filter.RemoveKey("nihao", result);
	filter.PrintCounterFilter(result);

	if(filter.KeyMayMatchWithCounter("nihao", result)) { // match, because input twice
		cout << "nihao is match" << endl;
	}

	filter.RemoveKey("nihao", result);
	filter.PrintCounterFilter(result);

	if(!filter.KeyMayMatchWithCounter("nihao", result)) { // not match, has removed all 'nihao'
		cout << "nihao is not match" << endl;
	}

	return 0;
}