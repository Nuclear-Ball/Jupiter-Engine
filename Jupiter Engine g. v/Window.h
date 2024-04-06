#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "pos2d.h"


void InitFunc() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		glfwTerminate();
		std::abort();
	}
}

namespace GlfwEnv {
	double tmp_scroll;

	inline pos2dus GetScreenSize() {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		return { static_cast<unsigned short>(mode->width), static_cast<unsigned short>(mode->height) };
	}

	int GlobalInit() {
		if (!glfwInit()) {
			std::cout << "GLFW initialization failed!\n";
			std::abort();
		}
		return 0;
	}

	unsigned short window_instances = GlobalInit();

	class JWindow {
	private:
		class JMouse {
			friend class JWindow;
		private:
			GLFWwindow* owner = nullptr;

			pos2d pos = { 0, 0 };
			bool state[3] = { 0, 0, 0 };

			bool on_p_f[3] = { 0, 0, 0 }; //При однократном нажатии
			bool on_r_f[3] = { 0, 0, 0 }; //При однократном отпускании

			double scroll = 0;

			void Update() {
				{
					double x_adapter = 0, y_adapter = 0;
					glfwGetCursorPos(owner, &x_adapter, &y_adapter);
					pos = { static_cast<int>(std::floor(x_adapter)), static_cast<int>(std::floor(y_adapter)) };
				}

				scroll = tmp_scroll;
				tmp_scroll = 0;

				for (unsigned char i = 0; i < 3; i++) {
					const bool old_state = state[i];
					state[i] = glfwGetMouseButton(owner, i);

					on_r_f[i] = (old_state != state[i]) && old_state;
					on_p_f[i] = (old_state != state[i]) && !old_state;
				}
			}

		public:
			inline JMouse(){}

			inline JMouse(GLFWwindow* own) 
				{ Create(own); }

			inline void Create(GLFWwindow* own) {
				owner = own;
				glfwSetScrollCallback(owner, [](GLFWwindow* window, double xoffset, double yoffset)->void { tmp_scroll = yoffset; });
			}

			inline pos2d GetPos() 
				{ return pos; }

			inline double GetWheel() 
				{ return scroll; }

			inline bool GetLB() 
				{ return state[0]; }
			inline bool GetOnP_LB() 
				{ return on_p_f[0]; }
			inline bool GetOnR_LB()
				{ return on_r_f[0]; }

			inline bool GetRB()
				{ return state[1]; }
			inline bool GetOnP_RB()
				{ return on_p_f[1]; }
			inline bool GetOnR_RB()
				{ return on_r_f[1]; }

			inline bool GetWB()
				{ return state[2]; }
			inline bool GetOnP_WB()
				{ return on_p_f[2]; }
			inline bool GetOnR_WB()
				{ return on_r_f[2]; }
		};
	public:
		JMouse mouse;

		JWindow() {}
		JWindow(pos2dus winsize, const std::string& name, bool fullscreen = false, const std::vector<int>& hints = {});
		void Create(pos2dus winsize, const std::string& name, bool fullscreen = false, const std::vector<int>& hints = {});
		void Focus();
		void Start();

		void Iconify();
		void Maximize();
		void Restore();

		void SetFullscreenMode(bool mode);
		void SetVsyncMode(bool mode);
		void SetIcon(const std::string& data, pos2dus icon_size);
		void SetTitle(const std::string& title);
		void SetSize(pos2dus new_size);
		void SetPos(pos2ds new_pos);

		double GetDeltaTime();
		double GetFPS();
		pos2dus GetSize();
		pos2ds GetPos();
		bool GetFullscreenMode();
		bool GetVsyncMode();
		GLFWwindow* GetId();
		void End();
		bool Check() const;
		void Close();
		void Destroy();
		~JWindow();

	protected:
		GLFWwindow* id = nullptr;
		pos2dus window_size = { 0, 0 };
		pos2ds window_pos = { 0, 0 };

		//Для восстановления размеров и позиции окна после возвращения из полноэкранного режима
		pos2dus tmp_window_size = { 0, 0 };
		pos2ds tmp_window_pos = { 0, 0 };

		bool is_fullscreened = 0;
		bool vsync = 1;
		double delta_time = 0.0;
		double last_frame = 0.0;
		double fps = 0.0;
	};

	inline JWindow::JWindow(pos2dus winsize, const std::string& name, bool fullscreen, const std::vector<int>& hints)
		{ Create(winsize, name, fullscreen, hints); }
	void JWindow::Create(pos2dus winsize, const std::string& name, bool fullscreen, const std::vector<int>& hints) {
		if (id != nullptr)
			glfwDestroyWindow(id);

		window_size = winsize;
		tmp_window_size = window_size;
		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		if (hints.size() % 2 == 0) {
			for (size_t i = 0; i < hints.size(); i += 2)
				glfwWindowHint(hints[i], hints[i + 1]);
		}

		is_fullscreened = fullscreen;

		GLFWmonitor* fullscreen_monitor = nullptr;

		if (is_fullscreened) {
			fullscreen_monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(fullscreen_monitor);
			window_size = { static_cast<unsigned short>(mode->width), static_cast<unsigned short>(mode->height) };
		}

		if (!(id = glfwCreateWindow(window_size.x, window_size.y, name.c_str(), fullscreen_monitor, NULL))) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			std::abort();
		}

		glfwMakeContextCurrent(id);

		mouse.Create(id);
	}
	void JWindow::Focus() {
		glfwMakeContextCurrent(id);
		glfwFocusWindow(id);
	}
	void JWindow::Start() {
		glfwMakeContextCurrent(id);

		mouse.Update();
		pos2d size_adapter = { 0, 0 };
		pos2d pos_adapter = { 0, 0 };
		glfwGetWindowSize(id, &size_adapter.x, &size_adapter.y);
		glfwGetWindowPos(id, &pos_adapter.x, &pos_adapter.y);
		window_size = { static_cast<unsigned short>(size_adapter.x), static_cast<unsigned short>(size_adapter.y) };
		window_pos = { static_cast<short>(pos_adapter.x), static_cast<short>(pos_adapter.y) };
		if (!is_fullscreened) {
			tmp_window_size = window_size;
			tmp_window_pos = window_pos;
		}
	}
	void JWindow::SetFullscreenMode(bool mode) {
		is_fullscreened = mode;

		if (is_fullscreened) {
			GLFWmonitor* fullscreen_monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(fullscreen_monitor);
			glfwSetWindowMonitor(id, fullscreen_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
			glfwSetWindowMonitor(id, NULL, tmp_window_pos.x, tmp_window_pos.y, tmp_window_size.x, tmp_window_size.y, 0);
	}
	void JWindow::End() {
		glfwMakeContextCurrent(id);
		//delta_time = glfwGetTime();

		double currentFrame = glfwGetTime();
		delta_time = currentFrame - last_frame;
		last_frame = currentFrame;
		fps = 1 / delta_time;

		//fps = 1.0f / delta_time;
		//glfwSetTime(0);
		glfwSwapBuffers(id);
		glfwPollEvents();
	}
	inline bool JWindow::Check() const
		{ return !glfwWindowShouldClose(id); }
	inline void JWindow::Close() 
		{ glfwSetWindowShouldClose(id, 1); }
	inline void JWindow::Destroy() {
		glfwDestroyWindow(id);
		--window_instances;
	}
	inline void JWindow::Iconify() 
		{ glfwIconifyWindow(id); }
	inline void JWindow::Maximize()
		{ glfwMaximizeWindow(id); }
	inline void JWindow::Restore()
		{ glfwRestoreWindow(id); }
	inline JWindow::~JWindow() {
		glfwDestroyWindow(id);
		if (!--window_instances)
			glfwTerminate();
	}


	inline void JWindow::SetVsyncMode(bool mode)
		{ glfwSwapInterval(vsync = mode); }
	inline void JWindow::SetIcon(const std::string& data, pos2dus size) {
		GLFWimage tmp = { size.x, size.y, reinterpret_cast<unsigned char*>(const_cast<char*>(data.c_str())) };
		glfwSetWindowIcon(id, 1, &tmp);
	}
	inline void JWindow::SetSize(pos2dus new_size)
		{ glfwSetWindowSize(id, new_size.x, new_size.y); }
	inline void JWindow::SetPos(pos2ds new_size)
		{ glfwSetWindowPos(id, new_size.x, new_size.y); }

	inline void JWindow::SetTitle(const std::string& title)
		{ glfwSetWindowTitle(id, title.c_str()); }
	inline double JWindow::GetDeltaTime()
		{ return delta_time; }
	inline double JWindow::GetFPS()
		{ return fps; }
	inline pos2dus JWindow::GetSize() { 
		pos2d size_adapter = { 0, 0 };
		glfwGetWindowPos(id, &size_adapter.x, &size_adapter.y);
		window_size = { static_cast<unsigned short>(size_adapter.x), static_cast<unsigned short>(size_adapter.y) };

		if (!is_fullscreened)
			tmp_window_size = window_size;

		return window_size; 
	}
	inline pos2ds JWindow::GetPos() { 
		pos2d pos_adapter = { 0, 0 };
		glfwGetWindowPos(id, &pos_adapter.x, &pos_adapter.y);
		window_pos = { static_cast<short>(pos_adapter.x), static_cast<short>(pos_adapter.y) };

		if (!is_fullscreened)
			tmp_window_pos = window_pos;

		return window_pos; 
	}
	inline bool JWindow::GetFullscreenMode()
		{ return is_fullscreened; }
	inline bool JWindow::GetVsyncMode()
		{ return vsync; }
	inline GLFWwindow* JWindow::GetId()
		{ return id; }
}


