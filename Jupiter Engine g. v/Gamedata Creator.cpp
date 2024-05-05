#include "jserial_modular.hpp"
#include "stb_image.h"
#include "pos2d.h"
#include "texture.h"

struct TxtStruct {
	pos2dus size = { 0,0 };
	std::string data;

	void Load(const std::string& path) {
		std::string player_data = JSerial::Utilities::ReadFile(path);

		pos2d size_adapter = { 0, 0 };
		unsigned char* converted = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(player_data.c_str()), player_data.size(), &size_adapter.x, &size_adapter.y, nullptr, 4);
		size = { static_cast<unsigned short>(size_adapter.x), static_cast<unsigned short>(size_adapter.y) };
		data = std::string(reinterpret_cast<char*>(converted), size.x * size.y * 4);
	}

	std::string Serialize() {
		using JE::JTexture::SerializationData::BT::writer;
		writer.Reset();
		writer.WriteType(size);
		writer.WriteType<std::string>(data);

		return writer.Result();
	}
};

int main() {
	JSerial::Template templ = { JSerial::Type<std::string>("player sprite") };// , JSerial::Array<std::string>("tileset") };
	JSerial::Write wr(templ);

	TxtStruct player;
	player.Load("Player.png");
	
	wr.WriteType(player.Serialize());

	JSerial::Utilities::SaveFile("Gamedata.bin", wr.Result());
}