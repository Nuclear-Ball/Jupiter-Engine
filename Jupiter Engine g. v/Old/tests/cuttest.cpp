#include <iostream>
#include <String+.h>

using namespace std;

int main() {
	wstring a = L"asdfghjkl";
	wstring b = cut(a, 3, 9);
	wcout << b;
}