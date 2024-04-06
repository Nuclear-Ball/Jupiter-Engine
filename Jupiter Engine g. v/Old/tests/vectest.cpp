#include <iostream>
#include <vector>

using namespace std;

int main() {
	vector<int> test = {1, 2, 3, 4, 5, 6, 7, 9, 10};
	int* a = &test[4];
	cout << *a << endl;
	test.erase(test.begin() + 2);
	cout << *a;
}