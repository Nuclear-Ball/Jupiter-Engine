#include <iostream>
#include <fstream>
#include <stb_image.h>
#include "jserial_modular.h"

int main() {
	JSerial::JsTemplate engine_data{ JSerial::Type<std::string>("font"), JSerial::Array<std::string>("icons and graphics") };
	
	std::string fontdata = JsUtilities::ReadFile(u8"OpenSans-Regular.ttf");
	std::string axis_src = JsUtilities::ReadFile(u8"coords_start.png");
	std::string logo_src = JsUtilities::ReadFile(u8"logo.png");

	JSerial::JSerialWrite<> eng_data(engine_data);
	eng_data.WriteType(fontdata);

	int x, y, ch;
	char* buff = reinterpret_cast<char*>(stbi_load_from_memory(reinterpret_cast<const unsigned char*>(axis_src.c_str()), axis_src.size(), &x, &y, &ch, NULL));
	axis_src = std::string(buff, 32 * 32 * 4);
	free(buff);

	buff = reinterpret_cast<char*>(stbi_load_from_memory(reinterpret_cast<const unsigned char*>(logo_src.c_str()), logo_src.size(), &x, &y, &ch, NULL));
	logo_src = std::string(buff, 32 * 32 * 4);
	free(buff);

	eng_data.WriteArray<std::string>({ axis_src, logo_src });
	
	JsUtilities::SaveFile(u8"Данные движка.дфю", eng_data.data);
}