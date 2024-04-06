#pragma once
#include <iostream>
#include <fstream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>
#include <vector>
#include "pos2d.h"
#include <codecvt>
#include <unordered_map>
#include <jserial_modular.hpp>
#include "Common.h"

namespace JTexture {
	namespace ErrorFrames {
		std::string frame_0 = { 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1,
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
		std::string frame_1 = { 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0, -76, -1, -1, 0,
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
		std::string frame_2 = { 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1, -1, 0, 95, -1,
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
		std::string frame_3 = { 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0,
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
	}

	namespace SerializationData {
		namespace BT {
			const JSerial::Template _template = { JSerial::Type<pos2dus>("size"), JSerial::Type<std::string>("raw source") };
			JSerial::C_Write<unsigned int> writer(_template);
			JSerial::C_Read<unsigned int> reader(_template);
		}

		namespace legacyBT {
			const JSerial::Template _template = { JSerial::Type<pos2d>("size"), JSerial::Type<std::string>("raw source") };
			JSerial::Write writer(_template);
			JSerial::Read reader(_template);
		}

		namespace FRM {
			namespace PRT {
				const JSerial::Template _template{ JSerial::Type<float>("transparency"),
												   JSerial::Type<float>("rotation"),
												   JSerial::Type<pos2df>("scale"),
												   JSerial::Type<pos2d>("rotation origin"),
												   JSerial::Type<pos2d>("offset") };
				JSerial::C_Write<unsigned char> writer(_template);
				JSerial::C_Read<unsigned char> reader(_template);
			}

			const JSerial::Template _template{ JSerial::Type<std::string>("frame texture data"),
											   JSerial::Type<std::string>("frame texture properties"),
											   JSerial::Type<float>("duration") };
			JSerial::Write writer(_template);
			JSerial::Read reader(_template);
		}

		namespace ANM {
			const JSerial::Template _template = { JSerial::Array<std::string>("frames"),
												  JSerial::Type<unsigned char>("type"),
												  JSerial::Type<unsigned char>("direction"),
												  JSerial::Type<std::string>("name") };
			JSerial::Write writer(_template);
			JSerial::Read reader(_template);
		}

		namespace TXT {
			namespace APP {
				const JSerial::Template _template = { JSerial::Type<unsigned short>("active animation"),
													  JSerial::Type<unsigned short>("current frame"),
													  JSerial::Type<bool>("bounce flag"),
													  JSerial::Type<bool>("paused") };
				JSerial::C_Write<unsigned char> writer(_template);
				JSerial::C_Read<unsigned char> reader(_template);
			}

			const JSerial::Template _template = { JSerial::Type<std::string>("base frame"),
												  JSerial::Array<string>("animations"),
												  JSerial::Type<string>("animation props") };
			JSerial::Write writer(_template);
			JSerial::Read reader(_template);
		}
	}

	class BasicTexture {
	private:
		unsigned int handle = 0;
		pos2dus size = { 0, 0 };

		inline void Init() {
			glGenTextures(1, &handle);
			glBindTexture(GL_TEXTURE_2D, handle);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

	public:
		inline BasicTexture()
			{ Init(); }
		inline BasicTexture(const std::string& raw, pos2dus size) {
			Init();
			LoadRaw(raw, size);
		}
		inline BasicTexture(const std::string& compressed) {
			Init();
			LoadCompressed(compressed);
		}
		inline BasicTexture(const std::string& path, bool dummy) {
			Init();
			LoadFile(path);
		}

		inline BasicTexture(const BasicTexture& cpy) {
			Init();
			LoadRaw(cpy.GetBinary(), cpy.size);
		}
		inline BasicTexture& operator=(const BasicTexture& cpy) {
			LoadRaw(cpy.GetBinary(), cpy.size);
			return *this;
		}

		inline BasicTexture(BasicTexture&& cpy) noexcept {
			handle = cpy.handle;
			size = cpy.size;

			cpy.handle = 0;
			cpy.size = { 0, 0 };
		}
		inline BasicTexture& operator=(BasicTexture&& cpy) noexcept {
			std::swap(handle, cpy.handle);
			size = cpy.size;

			return *this;
		}

		inline ~BasicTexture() { 
			glDeleteTextures(1, &handle); 
			handle = 0;
			size = { 0, 0 };
		}


		inline std::string GetBinary() const {
			std::string res(size.x * size.y * 4, 0);
			glBindTexture(GL_TEXTURE_2D, handle);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<unsigned char*>(const_cast<char*>(res.data())));
			return res;
		}
		inline pos2dus GetSize() const
			{ return size; }
		inline unsigned int GetHandle() const
			{ return handle; }

		inline void LoadRaw(const std::string& raw, pos2dus size) {
			this->size = size;
			glBindTexture(GL_TEXTURE_2D, handle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw.c_str());
		}
		void LoadCompressed(const std::string& compr_src) {
			pos2d size_adapter = { 0, 0 };
			unsigned char* converted = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(compr_src.data()), compr_src.size(), &size_adapter.x, &size_adapter.y, nullptr, 4);
			size = { static_cast<unsigned short>(size_adapter.x), static_cast<unsigned short>(size_adapter.y) };

			glBindTexture(GL_TEXTURE_2D, handle);
			if (size.x)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted);
			else
				cout << "Failed to load texture" << endl;

			stbi_image_free(converted);
		}
		void LoadFile(const std::string& path) {
			pos2d size_adapter = { 0, 0 };
			unsigned char* converted = stbi_load(path.c_str(), &size_adapter.x, &size_adapter.y, nullptr, 4);
			size = { static_cast<unsigned short>(size_adapter.x), static_cast<unsigned short>(size_adapter.y) };

			glBindTexture(GL_TEXTURE_2D, handle);
			if (size.x)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted);
			else
				cout << "Failed to load texture" << endl;
			stbi_image_free(converted);
		}

		inline void Use() const 
			{ glBindTexture(GL_TEXTURE_2D, handle); }

		std::string Serialize() const {
			using SerializationData::BT::writer;

			writer.Reset();

			writer.WriteType(size);
			writer.WriteType(GetBinary());

			return writer.Result();
		}
		void Deserialize(const std::string& src) {
			using SerializationData::BT::reader;

			reader.Source(src);

			size = reader.ReadType<pos2dus>("size");
			LoadRaw(reader.ReadType<std::string>("raw source"), size);
		}
	};

	struct Frame {
		struct Properties {
			float transparency = 1;
			float rotation = 0.0f;
			pos2df scale = { 1.0, 1.0 };
			pos2d rotation_origin = { 0,0 };
			pos2d offset = { 0,0 };
			//vector<hitbox> hitboxes;

			std::string Serialize() {
				using SerializationData::FRM::PRT::writer;
				writer.Reset();

				writer.WriteType(transparency);
				writer.WriteType(rotation);
				writer.WriteType(scale);
				writer.WriteType(rotation_origin);
				writer.WriteType(offset);

				return writer.Result();
			}

			void Deserialize(const std::string& src) {
				using SerializationData::FRM::PRT::reader;
				reader.Source(src);

				transparency	= reader.ReadType<float>("transparency");
				rotation		= reader.ReadType<float>("rotation");
				scale			= reader.ReadType<pos2df>("scale");
				rotation_origin = reader.ReadType<pos2d>("rotation origin");
				offset			= reader.ReadType<pos2d>("offset");
			}
		};

		BasicTexture frame;
		Properties properties;
		unsigned short additional_editor_id = 0;
		float duration = 0.1;

		inline std::string Serialize() {
			using SerializationData::FRM::writer;
			writer.Reset();

			writer.WriteType(frame.Serialize());
			writer.WriteType(properties.Serialize());
			writer.WriteType(duration);

			return writer.Result();
		}
		inline void Deserialize(const std::string& src) {
			using SerializationData::FRM::reader;
			reader.Source(src);

			frame.Deserialize(	   reader.ReadType<std::string>("frame texture data"));
			properties.Deserialize(reader.ReadType<std::string>("frame texture properties"));
			duration =			   reader.ReadType<float>("duration");
		}
	};

	class Animation {
	private:
		friend class Texture;
		std::vector<Frame> frames;
		bool valid = 0;
		double cached_duration = 0;
	public:
		enum class Direction : unsigned char {
			normal,
			backwards
		};
		enum class Type : unsigned char {
			repeating,
			stop_when_end,
			bounce
		};
		
		Type type;
		Direction direction;
		std::string name = u8"Ѕез имени";
		unsigned short in_editor_ever_created = 0;

		double Duration() {
			if (!valid) {
				cached_duration = 0.0;
				for (const Frame& i : frames)
					cached_duration += i.duration;
				valid = true;
			}
			return cached_duration;
		}

		inline const std::vector<Frame>& GetFrames() const 
			{ return frames; }

		inline std::vector<Frame>& GetFrames(bool) {
			valid = false;
			return frames;
		}

		std::string Serialize() {
			using SerializationData::ANM::writer;
			writer.Reset();

			std::vector<std::string> serialized_frames(frames.size());

			for (unsigned short i = 0; i < frames.size(); i++)
				serialized_frames[i] = frames[i].Serialize();

			writer.WriteArray(serialized_frames);
			writer.WriteType(static_cast<unsigned char>(type));
			writer.WriteType(static_cast<unsigned char>(direction));
			writer.WriteType(name);
			return writer.Result();
		}
		void Deserialize(const std::string& src) {
			using SerializationData::ANM::reader;
			reader.Source(src);

			std::vector<std::string> serialized_frames = reader.ReadArray<std::string>("frames");
			in_editor_ever_created = serialized_frames.size();
			frames.resize(serialized_frames.size());

			for (unsigned short i = 0; i < serialized_frames.size(); i++) {
				frames[i].Deserialize(serialized_frames[i]);
				frames[i].additional_editor_id = i;
			}

			type = static_cast<Type>(reader.ReadType<unsigned char>("type"));
			direction = static_cast<Direction>(reader.ReadType<unsigned char>("direction"));

			name = reader.ReadType<std::string>("name");
		}
	};

	class Texture {
	public:
		struct AnimPlayerProperties {
			unsigned short active_animation = 0;
			unsigned short current_frame = 0;
			float time_from_the_last_frame = 0.0f;
			bool bounce_flag = 0;
			bool paused = 0;

			void Stop() {
				active_animation = 0;
				current_frame = 0;
				time_from_the_last_frame = 0;
				bounce_flag = 0;
				paused = 0;
			}

			void Reset(const Animation& c_anim) {
				current_frame = (c_anim.frames.size() - 1) * (c_anim.direction == Animation::Direction::normal);
				time_from_the_last_frame = 0;
				bounce_flag = 0;
			}

			std::string Serialize() {
				using SerializationData::TXT::APP::writer;
				writer.Reset();

				writer.WriteType(active_animation);
				writer.WriteType(current_frame);
				writer.WriteType(bounce_flag);
				writer.WriteType(paused);

				return writer.Result();
			}

			void Deserialize(const std::string& src) {
				using SerializationData::TXT::APP::reader;
				reader.Source(src);

				active_animation = reader.ReadType<unsigned short>("active animation");
				current_frame	 = reader.ReadType<unsigned short>("current frame");
				bounce_flag		 = reader.ReadType<bool>("bounce flag");
				paused			 = reader.ReadType<bool>("paused");
			}
		};
		Frame base_frame;
		std::unordered_map<std::string, unsigned short> animation_hash;
		std::vector<Animation> animations;
		AnimPlayerProperties animation_props;

		unsigned int load_frame_id = 0;

		inline const Frame& GetCurrentFrame() const
			{ return (animation_props.active_animation != 0) ? animations[animation_props.active_animation - 1].frames[animation_props.current_frame] : base_frame; }
		inline const Frame& GetCurrentFrame(const AnimPlayerProperties& props)
			{ return (props.active_animation != 0) ? animations[props.active_animation - 1].frames[props.current_frame] : base_frame; }

		inline Animation* GetCurrentAnimation()
			{ return (animation_props.active_animation != 0) ? &animations[animation_props.active_animation - 1] : nullptr; }

		std::string Serialize() {
			using SerializationData::TXT::writer;
			writer.Reset();

			writer.WriteType(base_frame.Serialize());
			vector<string> serialized_anims(animations.size());
			for (int i = 0; i < animations.size(); i++)
				serialized_anims[i] = animations[i].Serialize();
			writer.WriteArray(serialized_anims);
			writer.WriteType(animation_props.Serialize());

			return writer.Result();
		}

		void Deserialize(const std::string& src) {
			using SerializationData::TXT::reader;
			reader.Source(src);

			base_frame.Deserialize(reader.ReadType<std::string>("base frame"));

			std::vector<std::string> serialized_anims = reader.ReadArray<std::string>("animations");
			animations.resize(serialized_anims.size());
			for (unsigned short i = 0; i < serialized_anims.size(); i++) {
				animations[i].Deserialize(serialized_anims[i]);
				animation_hash[animations[i].name] = i + 1;
			}

			animation_props.Deserialize(reader.ReadType<std::string>("animation props"));
		}

		bool Animate(AnimPlayerProperties* props = nullptr) {//возвращает true ели анимирование произошло, false если нет
			const bool using_local = props ? (props->active_animation ? 1 : 0) : 0;
			AnimPlayerProperties& c_props = using_local ? *props : animation_props;

			if (using_local) {
				if (load_frame_id == JupiterEngine::frame_counter)
					return false;
				load_frame_id = JupiterEngine::frame_counter;
			}

			if (c_props.active_animation && !c_props.paused) {
				const Animation& c_anim = animations[c_props.active_animation - 1];

				char step = 1 - 2 * (c_anim.direction == Animation::Direction::backwards != c_props.bounce_flag); //шаг отрицательный если либо сторона анимации оратна€, либо флаг отскока(флаг отскока возможен только при типе анимации "отскок")

				c_props.time_from_the_last_frame += JupiterEngine::delta_time;
				c_props.time_from_the_last_frame = std::remainder(c_props.time_from_the_last_frame, const_cast<Animation&>(c_anim).Duration());

				while (c_props.time_from_the_last_frame >= c_anim.frames[c_props.current_frame].duration) {
					c_props.time_from_the_last_frame -= c_anim.frames[c_props.current_frame].duration;


					if ((c_props.current_frame > 0 && c_props.current_frame < c_anim.frames.size() - 1) //если текущий кадр в допустимом диапазоне или если с начала, или если с конца (всегда срабатывает “ќЋ№ ќ одно из условий)
						|| (step + 1 && !c_props.current_frame)
						|| (step - 1 && c_props.current_frame == (c_anim.frames.size() - 1)))

						c_props.current_frame += step;

					else if ((c_props.current_frame == c_anim.frames.size() - 1 && step) || (step == -1 && c_props.current_frame == 0)) { //если текущий кадр достиг границы диапазона. в следующих лини€х описано то, как дальше будет вести себ€ анимаци€
						switch (c_anim.type) {
						case JTexture::Animation::Type::stop_when_end:
							c_props.Stop();
							return true;
						case JTexture::Animation::Type::repeating:
							c_props.current_frame = (c_anim.frames.size() - 1) * (c_anim.direction == JTexture::Animation::Direction::backwards); break;
						case JTexture::Animation::Type::bounce:
							c_props.bounce_flag = !c_props.bounce_flag;
							step *= -1;
							c_props.current_frame += step; break;
						}
					}
					else break;
				}
				return true;
			}
			else return false;
		}
	};
}


namespace texture_processing {
	struct cropper {
		pos2dus pos, size;
	};

	std::vector<cropper> generate_croplist(JTexture::BasicTexture* target, int n, pos2dus size = { 16, 16 }, pos2dus start_pos = { 0, 0 }, int horisontal_spacing = 0, int vertical_spacing = 0) {
		pos2dus offset = start_pos;
		vector<cropper> croplist;
		for (int i = 0; i < n; i++) {
			//cropper tmpcrop;
			if (offset.x + size.x <= target->GetSize().x && offset.y + size.y <= target->GetSize().y)
				croplist.push_back({ offset, size });

			if ((offset.x + size.x + horisontal_spacing + size.x <= target->GetSize().x) && (offset.y + size.y <= target->GetSize().y))
				offset.x += (size.x + horisontal_spacing);
			else if ((offset.x + size.x + horisontal_spacing + size.x > target->GetSize().x) && (offset.y + size.y + vertical_spacing + size.y <= target->GetSize().y) && (start_pos.x + size.x <= target->GetSize().x))
				offset.x = start_pos.x, offset.y += (size.y + vertical_spacing);
			else break;

		}
		return croplist;
	}

	std::vector<std::string> crop_from_croplist(std::string src_texture, vector<cropper> croplist, pos2dus size) {
		vector<string> res;
		for (int i = 0; i < croplist.size(); i++) {
			string tmp;
			cropper& curr_crop = croplist[i];
			for (int a = 0; a < curr_crop.size.y; a++) {
				for (int b = 0; b < curr_crop.size.x; b++) {
					for (int c = 0; c < 4; c++)
						tmp.push_back(src_texture[(curr_crop.pos.y + a) * (size.x * 4) + (curr_crop.pos.x + b) * 4 + c]);
				}
			}
			res.push_back(tmp);
		}
		return res;
	}
}

namespace Tiles {
	struct t_tile {
		JTexture::Texture tile_texture;
	};

	struct tileset {
		std::string name = u8"Ѕез имени";
		std::vector<t_tile> tiles;

		std::string Serialize() {
			JSerial::Template tset_templ = { JSerial::Array<std::string>("tiles"), JSerial::Type<std::string>("name") };

			std::vector<std::string> serialized_tiles(tiles.size());

			for (int i = 0; i < tiles.size(); i++) {
				serialized_tiles[i] = tiles[i].tile_texture.Serialize();
			}

			JSerial::Write tset_write(tset_templ);

			tset_write.WriteArray(serialized_tiles);
			tset_write.WriteType(name);

			return tset_write.Result();
		}

		void Deserialize(std::string src) {
			JSerial::Template tset_templ = { JSerial::Array<std::string>("tiles"), JSerial::Type<std::string>("name") };

			JSerial::Read tset_read(tset_templ, src);

			std::vector<std::string> serialized_tiles = tset_read.ReadArray<std::string>("tiles");

			tiles.resize(serialized_tiles.size());

			for (int i = 0; i < serialized_tiles.size(); i++) {
				tiles[i].tile_texture.Deserialize(serialized_tiles[i]);
			}

			name = tset_read.ReadType<std::string>("name");
		}
	};
}