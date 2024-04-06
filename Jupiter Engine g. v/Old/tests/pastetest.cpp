#include <iostream>
#include <String+.h>

using namespace std;

int main() {
	u16string a = u"        ", b = u"  ";//8 символов
	for(int i = 0; i < 8; i += 2)
		b[0] = i, a = paste(a, b, i);
	cout << "end";
}