#include <iostream>
#include <vector>

using namespace std;

int main() {
	vector<int> a = { 1, 2, 3, 4, 5, 6 };
	for (int i = 0; i < a.size(); i++) {
		cout << a[i] << " ";
	}
	cout << endl;

	int element = 3;
	a.erase(a.begin() + element, a.end() - (a.size() - element) + 1);
	cout << a.size() << endl;

	for (int i = 0; i < a.size(); i++) {
		cout << a[i] << " ";
	}
}