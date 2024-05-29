#include "Window.h"
#include "core.h"
#include <glm/glm.hpp>
#include "my_img.h"
#include "camera.h"

#undef max
#undef min

int main() {
	GlfwEnv::JWindow win({ 500, 600 }, "Test");

	InitFunc();
	glfwSetFramebufferSizeCallback(win.GetHandle(), [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });
	JE::res.Init();

	//JSerial::Read data({ JSerial::Type<std::string>("player sprite") }, JSerial::Utilities::ReadFile("Gamedata.bin"));

	//JE::JTexture::BasicTexture player;
	//player.Deserialize(data.ReadType<std::string>("player sprite"));
	

	JE::Camera cam;


	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	JE::Core cr;

	cr.gamedata.global_res.textures.resize(1);
	JE::JTexture::Texture& txt = cr.gamedata.global_res.textures[0];
	txt.base_frame.frame = JE::res.ErrTxt();

	cr.gamedata.global_res.tilesets.resize(1);
	cr.gamedata.global_res.tilesets[0].tiles.resize(1);
	cr.gamedata.global_res.tilesets[0].tiles[0] = {0, 0};
	
	//txt.animations.resize(1);
	//txt.animations[0].
	
	cr.gamedata.scenes.resize(1);
	cr.gamedata.scenes[0].rooms.resize(1);
	cr.gamedata.curr_room().layers.resize(1);
	cr.gamedata.curr_room().layers[0].tmap[{0, 0}] = { 0, 1, 0 };
	cr.gamedata.curr_room().layers[0].tmap[{1, 0}] = { 0, 1, 0 };
	cr.gamedata.curr_room().layers[0].tmap[{2, 0}] = { 0, 1, 0 };

	while (win.Check()) {
		win.Start();
		JE::NewFrame();

		cam.frm.Resize({ static_cast<unsigned short>(static_cast<float>(win.GetSize().x) * (256.f / win.GetSize().y)), 256 });


		cr.Render(cam);

		JE::Draw::DisplayFramebuffer(cam.frm, win);
		win.End();
	}
}

/*
		win.Start();

		glClear(GL_COLOR_BUFFER_BIT);

		cam.frm.Resize({ static_cast<unsigned short>(static_cast<float>(win.GetSize().x) * (256.f / win.GetSize().y)), 256 });
		cam.frm.Bind();

		//glm::mat4 projection = glm::ortho<float>(0, static_cast<float>(win.GetSize().x) * (256.f / win.GetSize().y), 0, 256);
		JE::res.TxtSh().Use();
		JE::res.TxtSh().setMat4("pv", cam.MakePV());

		static float rotation = 0;

		if (glfwGetKey(win.GetHandle(), GLFW_KEY_Q))
			cam.props.rotation += 2;
		if (glfwGetKey(win.GetHandle(), GLFW_KEY_E))
			cam.props.rotation -= 2;

		if (glfwGetKey(win.GetHandle(), GLFW_KEY_D))
			cam.center_pos.x += 2;
		if (glfwGetKey(win.GetHandle(), GLFW_KEY_A))
			cam.center_pos.x -= 2;

		if (glfwGetKey(win.GetHandle(), GLFW_KEY_W))
			cam.center_pos.y += 2;
		if (glfwGetKey(win.GetHandle(), GLFW_KEY_S))
			cam.center_pos.y -= 2;

		if (glfwGetKey(win.GetHandle(), GLFW_KEY_R))
			cam.props.scale.x += 0.2, cam.props.scale.y += 0.2;
		if (glfwGetKey(win.GetHandle(), GLFW_KEY_F))
			cam.props.scale.y -= 0.2, cam.props.scale.x -= 0.2;

		glClear(GL_COLOR_BUFFER_BIT);

		pos2d range_max;
		pos2d range_min;
		{
			pos2d upper_left = cam.ScreenCrdsToAbs({ 0, 0 });
			pos2d lower_left = cam.ScreenCrdsToAbs({ 0, cam.frm.GetSize().y });
			pos2d lower_right = cam.ScreenCrdsToAbs({ cam.frm.GetSize().x, cam.frm.GetSize().y });
			pos2d upper_right = cam.ScreenCrdsToAbs({ cam.frm.GetSize().x, 0 });

			range_max = { std::max({ upper_left.x, lower_left.x, lower_right.x, upper_right.x }),
						  std::max({ upper_left.y, lower_left.y, lower_right.y, upper_right.y }) };

			range_min = { std::min({ upper_left.x, lower_left.x, lower_right.x, upper_right.x }),
						  std::min({ upper_left.y, lower_left.y, lower_right.y, upper_right.y }) };
		}

		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				int c_elem = map[i + j * 16];
				if(c_elem)
					JE::Draw::BTexture(JE::res.ErrTxt(), { i * 8, j * 8 }, { 0, {0.5, 0.5} });
			}
		}
		const float size_ratio = static_cast<float>(256) / win.GetSize().y;
		const pos2d conv_coords = { static_cast<int>(win.mouse.GetPos().x * size_ratio), static_cast<int>(win.mouse.GetPos().y * size_ratio) };

		pos2d player_pos = cam.ScreenCrdsToAbs(conv_coords);
		JE::Draw::BTexture(player, player_pos, { 45, {1,1}, {10, 10} });

		std::cout << "Cam pos: " << cam.center_pos.x << " " << cam.center_pos.y << " ";
		std::cout << "Range max: " << range_max.x << " " << range_max.y << " ";
		std::cout << "Range min: " << range_min.x << " " << range_min.y << " ";
		std::cout << "Mouse pos: " << player_pos.x << " " << player_pos.y << "\n";

		JE::Draw::DisplayFramebuffer(cam.frm, win);
		win.End();*/