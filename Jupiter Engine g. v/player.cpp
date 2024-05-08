#include "Window.h"
#include "core.h"
#include <glm/glm.hpp>
#include "my_img.h"
#include "camera.h"

int main() {
	GlfwEnv::JWindow win({ 500, 600 }, "Test");
	InitFunc();
	glfwSetFramebufferSizeCallback(win.GetHandle(), [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });
	JE::res.Init();

	JSerial::Read data({ JSerial::Type<std::string>("player sprite") }, JSerial::Utilities::ReadFile("Gamedata.bin"));

	JE::JTexture::BasicTexture player;
	player.Deserialize(data.ReadType<std::string>("player sprite"));
	
	JE::Camera cam;

	unsigned short map[16 * 16];
	memset(map, 1, 16 * 16 * sizeof(unsigned short));

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (win.Check()) {
		win.Start();

		glClear(GL_COLOR_BUFFER_BIT);

		cam.frm.Resize({ static_cast<unsigned short>(static_cast<float>(win.GetSize().x) * (256.f / win.GetSize().y)), 256 });
		cam.frm.Bind();

		//glm::mat4 projection = glm::ortho<float>(0, static_cast<float>(win.GetSize().x) * (256.f / win.GetSize().y), 0, 256);
		JE::res.TxtSh().Use();
		JE::res.TxtSh().setMat4("pv", cam.MakePV());
		

		if (glfwGetKey(win.GetHandle(), GLFW_KEY_D))
			cam.props.rotation += 2;
		if (glfwGetKey(win.GetHandle(), GLFW_KEY_A))
			cam.props.rotation -= 2;

		//if (glfwGetKey(win.GetHandle(), GLFW_KEY_W))
		//	cam.props.scale.y += 0.2;
		//if (glfwGetKey(win.GetHandle(), GLFW_KEY_S))
		//	cam.props.scale.y -= 0.2;

		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				int c_elem = map[i + j * 16];
				if(c_elem)
					JE::Draw::BTexture(JE::res.ErrTxt(), { i * 8, j * 8 }, { 1, 0, {0.5, 0.5} });
			}
		}
		const float size_ratio = static_cast<float>(256) / win.GetSize().y;
		const pos2d conv_coords = { static_cast<int>(win.mouse.GetPos().x * size_ratio), static_cast<int>(win.mouse.GetPos().y * size_ratio) };

		pos2d player_pos = cam.ScreenCrdsToAbs(conv_coords);
		JE::Draw::BTexture(player, player_pos);

		std::cout << player_pos.x << " " << player_pos.y << "\n";

		JE::Draw::DisplayFramebuffer(cam.frm, win);
		win.End();
	}
}