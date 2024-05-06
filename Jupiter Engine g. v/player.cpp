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

	//JE::Framebuffer a(JE::res.ErrTxt());
	
	JE::Camera cam;

	unsigned short map[16 * 16];
	memset(map, 1, 16 * 16 * sizeof(unsigned short));

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (win.Check()) {
		win.Start();

		cam.frm.Resize({ static_cast<unsigned short>(static_cast<float>(win.GetSize().x) * (256.f / win.GetSize().y)), 256 });
		cam.frm.Bind();
		//glViewport(0, 0, 256, 300);
		glm::mat4 projection = glm::ortho<float>(0, static_cast<float>(win.GetSize().x) * (256.f / win.GetSize().y), 0, 256);
		JE::res.TxtSh().Use();
		JE::res.TxtSh().setMat4("pv", projection);

		//glClear(GL_COLOR_BUFFER_BIT);

		

		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				int c_elem = map[i + j * 16];
				if(c_elem)
					JE::Draw::BTexture(JE::res.ErrTxt(), { i * 8, j * 8 }, { 1, 0, {0.5, 0.5} });
			}
		}

		static pos2d player_pos = { 16, 32 };
		JE::Draw::BTexture(player, player_pos);

		JE::Draw::DisplayFramebuffer(cam.frm, win);
		win.End();
	}
}