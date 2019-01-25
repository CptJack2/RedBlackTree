#include"RBTree.h"
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main() {
	RBTreeMap<int, int> tree;
	test(tree);
}
void test(RBTreeMap<int,int>& m) {
	/*for (int i = 1; i <= 4; ++i)
		m.insert(i, i);
	m.insert(6,6);
	m.insert(5,5);
	m.remove(3);*/
	vector<int>arr;
	for (int i = 1; i <= 10; ++i)
		arr.push_back(i);
	do {
		for (int i = 0; i < 10; ++i)
			m.insert(arr[i], arr[i]);
		for (int i = 0; i < 10; ++i)
			m.remove(arr[i]);
	}while (next_permutation(arr.begin(),arr.end()));
	
	int c = 1;
}