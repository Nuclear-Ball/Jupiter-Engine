#include <iostream>
#include <cmath>
#include <Bits.h>

using namespace std;

int main() {
	/*float a = 1.2345f;
	int count = 0;
	while (a - int(a) != 0.0f) {
		count += 1;
		a = a * 10;
	}
	cout << a << " " << count << " " << a * pow(10, count * -1);
	*/
	float a = -1.2345, b;
	wstring tmp = wSeparateF(a);
	b = wMergeF(tmp);
	cout << b;
}