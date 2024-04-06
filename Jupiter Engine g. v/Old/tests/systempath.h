#pragma once
#include <Windows.h>
#include <string>
#include <libloaderapi.h>
#include <String+.h>
#include <vector>
#include <iostream>

std::string GetPath()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");

	return std::string(buffer).substr(0, pos);
}

using namespace std;

wstring expath() {
	vector<wchar_t> pathBuf;
	DWORD copied = 0;
	do {
		pathBuf.resize(pathBuf.size() + MAX_PATH);
		copied = GetModuleFileName(0, &pathBuf.at(0), pathBuf.size());
	} while (copied >= pathBuf.size());
	pathBuf.resize(copied);
	wstring path(pathBuf.begin(), pathBuf.end());
	path = cut(path, 0, Find(path, 0, '\\', 1));
	path = replace(path, L'\\', L'/');
	return path;
}