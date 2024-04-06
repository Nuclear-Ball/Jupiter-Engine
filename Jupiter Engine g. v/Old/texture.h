#pragma once
#include <iostream>
#include <fstream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>
#include <vector>
#include <pos2d.h>
#include <codecvt>



namespace texture {
	string err_frame_0 = { 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1,
	-1, -1, 0, -1, -1, 0, 0, 0, -1, -8, -8, -8, -1, -95, -95, -95, -1, 102, 102, 102, -1, 102, 102, 102, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -70, -1, -1, -1, 118, -1, -1,
	-1, 0, -1, -1, -1, 0, -1, -1, 122, 0, 121, -1, 0, 0, 0, -1, -95, -95, -95, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1,
	-1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, -70, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0,
	-1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 122, 0, 121, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, 118, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1,
	0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 122, 0, 121, -1, 0, 0, 0, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1,
	0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 122, 0, 121, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0,
	-1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 122, 0, 121, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, 122, 0, 121, -1, -1, 0, -1, -1, 122, 0, 121, -1, 122, 0,
	121, -1, 122, 0, 121, -1, 122, 0, 121, -1, 122, 0, 121, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0,
	-1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -70, -1, -1, -1, 118, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 122, 0, 121, -1, 0, 0, 0, -1, -8, -8, -8, -1, -95, -95, -95, -1,
	102, 102, 102, -1, 102, 102, 102, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -95, -95, -95,
	-1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, -70, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 122, 0, 121, -1, 0, 0, 0, -1, 102, 102,
	102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, 118, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 122, 0, 121, -1, 0, 0, 0, -1, 102,
	102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 122, 0, 121, -1, 0, 0, 0, -1, 42,
	42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, 122, 0, 121, -1, 0, 0, 0, -1, 0, 0,
	0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, 122, 0, 121, -1, -1, 0, -1, -1, 122, 0, 121, -1, 122, 0, 121, -1, 122, 0, 121, -1, 122, 0, 121, -1, 122, 0, 121, -1, 0, 0, 0, -1,
	0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1 };

	string err_frame_1 = { 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0,
	-76, -1, -1, 0, -76, -1, 0, 0, 0, -1, -1, -1, -1, -1, -95, -95, -95, -1, 102, 102, 102, -1, 102, 102, 102, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -70, -82, -1, -1, 118, -52,
	-1, -1, 0, -76, -1, -1, 0, -76, -1, 122, 0, 71, -1, 0, 0, 0, -1, -95, -95, -95, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, -1, -1, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0,
	-76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, -70, -82, -1, -1, 0, -76, -1, -1, 0, -76, -1,
	-1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, 122, 0, 71, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, 118, -52, -1, -1, 0, -76, -1, -1, 0,
	-76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, 122, 0, 71, -1, 0, 0, 0, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1,
	0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, 122, 0, 71, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1,
	0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, 122, 0, 71, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, 122, 0, 71, -1, -1, 0, -76, -1, 122,
	0, 71, -1, 122, 0, 71, -1, 122, 0, 71, -1, 122, 0, 71, -1, 122, 0, 71, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0,
	0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -70, -82, -1, -1, 118, -52, -1, -1, 0, -76, -1, -1, 0, -76, -1, 122, 0, 71, -1, 0, 0, 0, -1, -1, -1, -1,
	-1, -95, -95, -95, -1, 102, 102, 102, -1, 102, 102, 102, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, -1, -1, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, 0,
	0, 0, -1, -95, -95, -95, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, -70, -82, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, 122, 0, 71,
	-1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, 118, -52, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, 122,
	0, 71, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1,
	122, 0, 71, -1, 0, 0, 0, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1,
	122, 0, 71, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, 122, 0, 71, -1, -1, 0, -76, -1, 122, 0, 71, -1, 122, 0, 71, -1, 122, 0, 71, -1, 122, 0, 71, -1,
	122, 0, 71, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1 };

	string err_frame_2 = { 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1,
	-1, 0, 95, -1, 0, 0, 0, -1, -8, -8, -8, -1, -95, -95, -95, -1, 102, 102, 102, -1, 102, 102, 102, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 95, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -70, -82, -1, -1, 118, -109, -1, -1, 0,
	95, -1, -1, 0, 95, -1, 122, 0, 0, -1, 0, 0, 0, -1, -95, -95, -95, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 95, -1, -1, -1, -1, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95,
	-1, -1, 0, 95, -1, -1, 0, 95, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 95, -1, -1, -70, -82, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95,
	-1, -1, 0, 95, -1, 122, 0, 0, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 95, -1, -1, 118, -109, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95,
	-1, -1, 0, 95, -1, 122, 0, 0, -1, 0, 0, 0, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1,
	0, 95, -1, 122, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1,
	122, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 95, -1, 122, 0, 0, -1, -1, 0, 95, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0,
	-1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0,
	95, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -70, -82, -1, -1, 118, -109, -1, -1, 0, 95, -1, -1, 0, 95, -1, 122, 0, 0, -1, 0, 0, 0, -1, -8, -8, -8, -1, -95, -95, -95, -1, 102, 102, 102, -1, 102, 102, 102, -1, 42, 42, 42, -1, 0, 0, 0,
	-1, 0, 0, 0, -1, -1, 0, 95, -1, -1, -1, -1, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, 0, 0, 0, -1, -95, -95, -95, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1,
	0, 0, 0, -1, -1, 0, 95, -1, -1, -70, -82, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, 122, 0, 0, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0,
	0, 0, -1, -1, 0, 95, -1, -1, 118, -109, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, 122, 0, 0, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0,
	0, 0, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, 122, 0, 0, -1, 0, 0, 0, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0,
	-1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, 122, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0,
	95, -1, 122, 0, 0, -1, -1, 0, 95, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1 };

	string err_frame_3 = { 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0,
	-1, 0, 0, 0, -1, -8, -8, -8, -1, -95, -95, -95, -1, 102, 102, 102, -1, 102, 102, 102, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -70, -103, -1, -1, 118, 0, -1, -1, 0, 0, -1, -1, 0,
	0, -1, 122, 0, 0, -1, 0, 0, 0, -1, -95, -95, -95, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1,
	0, 0, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, -70, -103, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, 122, 0, 0,
	-1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, 118, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, 122, 0, 0, -1, 0, 0,
	0, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, 122, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0,
	-1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, 122, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1,
	0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, 122, 0, 0, -1, -1, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0,
	-1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -70, -103, -1, -1,
	118, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, 122, 0, 0, -1, 0, 0, 0, -1, -8, -8, -8, -1, -95, -95, -95, -1, 102, 102, 102, -1, 102, 102, 102, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, 0, 0,
	-1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, 0, 0, 0, -1, -95, -95, -95, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, -70, -103, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1,
	0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, 122, 0, 0, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, 118, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1,
	-1, 0, 0, -1, -1, 0, 0, -1, 122, 0, 0, -1, 0, 0, 0, -1, 102, 102, 102, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1,
	-1, 0, 0, -1, 122, 0, 0, -1, 0, 0, 0, -1, 42, 42, 42, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1,
	122, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, 122, 0, 0, -1, -1, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1, 122, 0, 0, -1,
	0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1 };


	enum animation_side {
		normal,
		backwards
	};

	enum animation_types {
		repeating,
		stop_when_end,
		bounce
	};

	class basic_texture {
	public:
		unsigned int texture_pointer;
		pos2d original_size;
		pos2d offset;
		float rotation;
		pos2df scale={1.0, 1.0};
		pos2d rotation_origin;
		float transparency = 1;
		//vector<hitbox> hitboxes;

		string get_texture() {
			unsigned char* buff = (unsigned char*)malloc(original_size.x * original_size.y * 4);
			glBindTexture(GL_TEXTURE_2D, texture_pointer);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buff);
			glBindTexture(GL_TEXTURE_2D, 0);
			string tmp = string((char*)buff, original_size.x * original_size.y * 4);
			free(buff);
			return tmp;
		}

		void load(string path) {
			glGenTextures(1, &texture_pointer);
			glBindTexture(GL_TEXTURE_2D, texture_pointer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			int n_channels;
			unsigned char* tmp = stbi_load(path.c_str(), &original_size.x, &original_size.y, &n_channels, 0);
			string src = string((char*)tmp, original_size.x * original_size.y * n_channels);
			stbi_image_free(tmp);

			if (n_channels < 4) {
				string res;
				res.resize(src.size() / n_channels * 4);
				for (int i = 0; i < src.size() / n_channels; i++) {
					for (int j = 0; j < n_channels; j++)
						res[i * 4 + j] = src[i * n_channels + j];
					res[i * 4 + 3] = 255;
				}
				src = res;
			}
				//src = texture_processing::convert_nc_to_rgba(src, n_channels);

			if (original_size.x)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, original_size.x, original_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, src.c_str());
			else
				cout << "Failed to load texture" << endl;
			
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void load_from_raw(string raw, pos2d size) {
			//src_data = raw;
			original_size = size;
			glGenTextures(1, &texture_pointer);
			glBindTexture(GL_TEXTURE_2D, texture_pointer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, original_size.x, original_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw.c_str());
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void use() {
			glBindTexture(GL_TEXTURE_2D, texture_pointer);
		}

		void delete_texture() {
			glDeleteTextures(1, &texture_pointer);
		}
	};

	struct frame {
		basic_texture frame;
		float duration;
	};

	struct animation {
		vector<frame> frames;
		animation_types type;
		animation_side side;
	};

	class texture {
	public:
		basic_texture base_frame;
		vector<animation> animations;
		int active_animation = -1;
		int current_frame;
		bool was_loaded_in_current_frame;
		float time_from_last_frame;
		bool bounce_flag;

		inline basic_texture* get_curr_frame() {
			return (active_animation + 1) ? &animations[active_animation].frames[current_frame].frame : &base_frame;
		}

		inline basic_texture* get_curr_frame_from_pfab(int actv_anim, int frame) {
			return (actv_anim + 1) ? &animations[actv_anim].frames[frame].frame : &base_frame;
		}

		void destroy() {
			base_frame.delete_texture();
			for (animation& i : animations) {
				for (frame& j : i.frames) {
					j.frame.delete_texture();
				}
			}
		}
	};
}

namespace texture_processing {
	string convert_nc_to_rgba(string src, int n_channels) {
		string res;
		res.resize(src.size() / n_channels * 4);
		for (int i = 0; i < src.size() / n_channels; i++) {
			for (int j = 0; j < n_channels; j++) {
				res[i * 4 + j] = src[i * n_channels + j];
			}
			res[i * 4 + 3] = 255;
		}
		return res;
	}

	struct cropper {
		pos2d pos, size;
	};

	vector<cropper> generate_croplist(texture::basic_texture* target, int n, pos2d size = { 16, 16 }, pos2d start_pos = { 0, 0 }, int horisontal_spacing = 0, int vertical_spacing = 0) {
		pos2d offset = start_pos;
		vector<cropper> croplist;
		for (int i = 0; i < n; i++) {
			cropper tmpcrop;
			if (offset.x + size.x <= target->original_size.x && offset.y + size.y <= target->original_size.y)
				croplist.push_back({ offset, size });

			if ((offset.x + size.x + horisontal_spacing + size.x <= target->original_size.x) && (offset.y + size.y <= target->original_size.y))
				offset.x += (size.x + horisontal_spacing);
			else if ((offset.x + size.x + horisontal_spacing + size.x > target->original_size.x) && (offset.y + size.y + vertical_spacing + size.y <= target->original_size.y) && (start_pos.x + size.x <= target->original_size.x))
				offset.x = start_pos.x, offset.y += (size.y + vertical_spacing);
			else break;

		}
		return croplist;
	}

	vector<string> crop_from_croplist(string src_texture, vector<cropper> croplist, pos2d original_size) {
		vector<string> res;
		for (int i = 0; i < croplist.size(); i++) {
			string tmp;
			cropper& curr_crop = croplist[i];
			for (int a = 0; a < curr_crop.size.y; a++) {
				for (int b = 0; b < curr_crop.size.x; b++) {
					for (int c = 0; c < 4; c++)
						tmp.push_back(src_texture[(curr_crop.pos.y + a) * (original_size.x * 4) + (curr_crop.pos.x + b) * 4 + c]);
				}
			}
			res.push_back(tmp);
		}
		return res;
	}

}



/*
 	struct animation_frame {
		unsigned int id;
		string dat;
		pos2d size;
		float duration = 0.1;
	};
	class old_animation {
	public:
		animation_types type = repeating;
		animation_side side = normal;
		string name = u8"Без имени";
		vector<animation_frame> frames;
		int curr_frame = 0;
		float time_since_last_frame = 0;
		bool bounce_flag;
		void reset() {
			curr_frame = side == backwards ? frames.size() - 1 : 0;
			time_since_last_frame = 0;
			bounce_flag = 0;
		}

		void load_frame_from_string(string str, pos2d size) {
			frames.resize(frames.size() + 1);
			glGenTextures(1, &frames.back().id);
			glBindTexture(GL_TEXTURE_2D, frames.back().id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			if (str.c_str())
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, str.c_str());
			else cout << "Failed to load texture" << endl;
			glBindTexture(GL_TEXTURE_2D, 0);
			frames.back().dat = str;
			frames.back().size = size;
		}*/
		/*~animation() {
			for (int i = 0; i < frames.size(); i++) {
				frames[i].dat.clear();
				frames[i].dat.shrink_to_fit();
				glDeleteTextures(1, &frames[i].id);
			}
			frames.clear();
			frames.shrink_to_fit();
			name.clear();
			name.shrink_to_fit();
		}*//*
	};

	struct active_animation;

	class old_texture {
	public:
		int usage_id;
		vector<old_animation> animations;
		string dat;
		string name = u8"Без имени";
		pos2d size;
		int active_animation = -1;
		unsigned int id, root_id;

		void play_animation(int anim_id) {
			root_id = id;
			if (anim_id < animations.size())
				active_animation = anim_id, animations[active_animation].curr_frame = 0;
		}

		void animation_controller(float deltat) {
			if (active_animation != -1) {
				animations[active_animation].time_since_last_frame += deltat;
				for (int i = 0; animations[active_animation].time_since_last_frame >= animations[active_animation].frames[animations[active_animation].curr_frame].duration; i++) {
					animations[active_animation].time_since_last_frame -= animations[active_animation].frames[animations[active_animation].curr_frame].duration;
					if (animations[active_animation].curr_frame >= 0 && animations[active_animation].curr_frame < animations[active_animation].frames.size() - 1) {
						animations[active_animation].curr_frame++;
						id = animations[active_animation].frames[animations[active_animation].curr_frame].id;
						size = animations[active_animation].frames[animations[active_animation].curr_frame].size;
					}
					else if (animations[active_animation].curr_frame == animations[active_animation].frames.size() - 1) {
						animations[active_animation].curr_frame = 0;
						id = animations[active_animation].frames[animations[active_animation].curr_frame].id;
						size = animations[active_animation].frames[animations[active_animation].curr_frame].size;
					}
				}
			}
		}

		void loadfromstr() {
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			if (dat.c_str())
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, dat.c_str());
			else cout << "Failed to load texture" << endl;
			glBindTexture(GL_TEXTURE_2D, 0);
			root_id = id;
		}

		void loadtexture(string path) {
			//u16string tmp1;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			int nrChannels;

			stbi_load(path.c_str(), &size.x, &size.y, &nrChannels, 0);
			int strlength = size.x * size.y * 4;
			unsigned char* tmp = stbi_load(path.c_str(), &size.x, &size.y, &nrChannels, 0);
			//cout << sizeof(tmp) << endl;
			dat = string((char*)tmp, strlength);
			stbi_image_free(tmp);
			//dat = tmp;
			//char* tmp = reinterpret_cast<char*>(data);
			//dat.resize(size.x * size.y * 4);

			//for (int i = 0; i < size.x * size.y * 4; i++)
				//dat[i] = *(tmp + i);
			//dat = tmpdat;
			if (dat.size())
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, dat.c_str());
			else
				cout << "Failed to load texture" << endl;

			//stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);
			root_id = id;
		}

		void use() {
			glBindTexture(GL_TEXTURE_2D, id);
		}
		/*
		~texture() {
			animations.clear();
			animations.shrink_to_fit();
			glDeleteTextures(1, &id);
			dat.clear();
			dat.shrink_to_fit();
			name.clear();
			name.shrink_to_fit();
		}*//*
	};

	struct active_animation {
		old_texture* target;
		active_animation* used_by;
	};
}*/