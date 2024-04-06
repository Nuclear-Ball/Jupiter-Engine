#pragma once
#include <bitset>
#include <string>
#include <cmath>

using namespace std;

struct point3 {
	int x, y;
};

int Merge(u16string line, int pos = 0) {
	return (line[pos] << 16) + line[pos + 1];
}

long long MergeLL(u16string line, int pos = 0) {
	return (line[pos] << 48) + (line[pos + 1] << 32) + (line[pos + 2] << 16) + line[pos + 3];
}

float MergeF(u16string line, int pos = 0) {
	return Merge(line, pos + 1) * pow(10, line[pos] * -1);
}

double MergeD(u16string line, int pos = 0) {
	return MergeLL(line, pos + 1) * pow(10, line[pos] * -1);
}


u16string Separate(int num) {
	return u16string{ char16_t(num >> 16), char16_t(num) };
}

u16string SeparateF(float num) {
	int i = 0;
	float a = num;
	for (i; a - int(a) != 0; i++) a *= 10;
	return u16string{ char16_t(i) } + Separate(a);
}


/*
int u16Merge(u16string line, int pos = 0) {
	bitset<32> res;
	bitset<16> a, b;
	a = line[pos], b = line[pos + 1];
	for (int i = 0; i < 16; i++)
		res[i + 16] = a[i], res[i] = b[i];
	return res.to_ulong();
}

float u16MergeF(u16string line, int pos = 0) {
	int tmp = u16Merge(line, pos + 1);
	float res = tmp * pow(10, (line[pos] * -1));
	return res;
}

u16string u16Separate(int num) {
	short tmp = 0;
	int tmp1 = num;
	bitset<32> src = tmp1;
	bitset<16> a;
	u16string line = u"  ";
	for (int i = 16; i < 32; i++)
		a[i - 16] = src[i];
	//line[0] = (num < 0);
	tmp = short(a.to_ulong());
	line[0] = tmp;
	tmp = short(tmp1);
	line[1] = tmp;
	return line;
}

u16string u16SeparateF(float num) {
	u16string line;
	float tmp = num;
	short counter = 0;
	while (tmp - int(tmp) != 0) {
		counter++;
		tmp * 10;
	}
	line += char16_t(counter);
	line += u16Separate(tmp);
	return line;
}

unsigned int Merge(string line, int pos) {
	bitset<16> res;
	bitset<8> a, b;
	a = line.at(pos) - 32, b = line.at(pos + 1) - 32;
	for (int i = 0; i < 8; i++) {
		if (b.test(i))
			res.flip(i);
		if (a.test(i))
			res.flip(i + 8);
	}
	return res.to_ulong();
}
 
string Separate(unsigned short int num) {
	bitset<16> src = num;
	bitset<8> a;
	string line;
	for (int i = 8; i < 16; i++)
		if (src.test(i))
			a.flip(i - 8);
	line += char(a.to_ulong()) + 32;
	a.reset();
	a = num;
	line += char(a.to_ulong()) + 32;
	return line;
}

unsigned int wMergeU(wstring line, int pos = 0) {
	bitset<32> res;
	bitset<16> a, b;
	a = line[pos] - 32, b = line[pos + 1] - 32;
	for (int i = 0; i < 16; i++)
		res[i + 16] = a[i], res[i] = b[i];
	return res.to_ulong();
}

long long wMergeLL(wstring line, int pos = 0) {
	bitset<32> a, b;
	bitset<64> res;
	a = wMergeU(line, 1 + pos);
	b = wMergeU(line, 3 + pos);
	for (int i = 0; i < 32; i++)
		res[i + 32] = a[i], res[i] = b[i];
	return line[pos] - 32 ? res.to_ullong() * -1 : res.to_ullong();
}

wstring wSeparateU(unsigned int num) {
	short tmp = 0;
	bitset<32> src = num;
	bitset<16> a;
	wstring line = L"  ";
	for (int i = 16; i < 32; i++)
		a[i - 16] = src[i];
	tmp = short(a.to_ulong());
	line[0] = tmp + 32;
	tmp = short(num);
	line[1] = tmp + 32;
	return line;
}

wstring wSeparateLL(long long num) {
	int tmp = 0;
	long long tmp1 = fabs(num);
	bitset<64> src = tmp1;
	bitset<32> a;
	wstring line = L" ";
	for (int i = 32; i < 64; i++)
		a[i - 32] = src[i];
	line[0] = (num < 0) + 32;
	line += wSeparateU(int(a.to_ulong()));
	line += wSeparateU(int(tmp1));
	return line;
}

short int wDecode(wstring data, int pos = 0) {
	if (data[pos] == 32) return (data[pos + 1] - 32) * -1;
	return data[pos + 1] - 32;
}


wstring wCode(short int num) {
	wstring line = L"  ";
	if (0 > num >= -32) line[1] = wchar_t(fabs(num) + 32);
	else line[0] = 33, line[1] = wchar_t(num + 32);
	return line;
}
*/