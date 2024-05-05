#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "pos2d.h"


bool InitFunc() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		glfwTerminate();
		std::abort();
	}
	return true;
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

	class Key {
	public:
		int key_id;
		bool once;

		Key() : id(nullptr) {}
		Key(GLFWwindow* window) : id(window) {}
		Key(GLFWwindow* window, int key, bool once_flag = true) { id = window, key_id = key, once = once_flag; }

		void init(GLFWwindow* window, int key, bool once_flag = true) { id = window, key_id = key, once = once_flag; }

		bool Check() {
			bool keystate = glfwGetKey(id, key_id), cond = (keystate != key_flag);
			return ((key_flag = key_flag != cond) && cond) && once || keystate && !once;
		}
		bool key_flag; GLFWwindow* id;
	};
	class KeyCombination {
	public:
		int keys[2];

		KeyCombination() {

		}
		KeyCombination(GLFWwindow* window) {
			id = window;
		}
		KeyCombination(GLFWwindow* window, int first_key, int second_key) {
			id = window;
			keys[0] = first_key, keys[1] = second_key;
		}

		void init(GLFWwindow* window, int first_key, int second_key) {
			id = window;
			keys[0] = first_key, keys[1] = second_key;
		}

		bool check() {
			bool keystates[2] = { glfwGetKey(id, keys[0]), glfwGetKey(id, keys[1]) };
			pressed = pressed && !(!keystates[0] && keyflags[0]) && !(!keystates[1] && keyflags[1]);

			keyflags[0] = keyflags[0] != (keyflags[0] != keystates[0] != pressed);
			keyflags[1] = keyflags[1] != (keyflags[1] != keystates[1] != pressed);

			bool f_cond = (keystates[0] && keystates[1] && !pressed);
			pressed = pressed != f_cond;
			return f_cond;
		}
		bool keyflags[2], pressed = 0; GLFWwindow* id;
	};


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
			inline JMouse() {}
			inline JMouse(GLFWwindow* own)
				{ Create(own); }
			inline void Create(GLFWwindow* own) {
				owner = own;
				glfwSetScrollCallback(owner, [](GLFWwindow* window, double xoffset, double yoffset)->void { tmp_scroll = yoffset; });
			}

		public:
			inline pos2d GetPos() const
				{ return pos; }

			inline double GetWheel() const
				{ return scroll; }

			inline bool GetLB() const
				{ return state[0]; }
			inline bool GetOnP_LB() const
				{ return on_p_f[0]; }
			inline bool GetOnR_LB() const
				{ return on_r_f[0]; }

			inline bool GetRB() const
				{ return state[1]; }
			inline bool GetOnP_RB() const
				{ return on_p_f[1]; }
			inline bool GetOnR_RB() const
				{ return on_r_f[1]; }

			inline bool GetWB() const
				{ return state[2]; }
			inline bool GetOnP_WB() const
				{ return on_p_f[2]; }
			inline bool GetOnR_WB() const
				{ return on_r_f[2]; }
		};
	public:
		JMouse mouse;

		JWindow() {}
		JWindow(pos2dus winsize, const std::string& name, bool fullscreen = false, const std::vector<int>& hints = {}) 
			{ Create(winsize, name, fullscreen, hints); }
		void Create(pos2dus winsize, const std::string& name, bool fullscreen = false, const std::vector<int>& hints = {}) {
			if (handle != nullptr)
				glfwDestroyWindow(handle);

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

			if (!(handle = glfwCreateWindow(window_size.x, window_size.y, name.c_str(), fullscreen_monitor, NULL))) {
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				std::abort();
			}

			glfwMakeContextCurrent(handle);

			mouse.Create(handle);
		}
		void Focus() {
			glfwMakeContextCurrent(handle);
			glfwFocusWindow(handle);
		}

		void Start() {
			glfwMakeContextCurrent(handle);

			mouse.Update();
			pos2d size_adapter = { 0, 0 };
			pos2d pos_adapter = { 0, 0 };
			glfwGetWindowSize(handle, &size_adapter.x, &size_adapter.y);
			glfwGetWindowPos(handle, &pos_adapter.x, &pos_adapter.y);
			window_size = { static_cast<unsigned short>(size_adapter.x), static_cast<unsigned short>(size_adapter.y) };
			window_pos = { static_cast<short>(pos_adapter.x), static_cast<short>(pos_adapter.y) };
			if (!is_fullscreened) {
				tmp_window_size = window_size;
				tmp_window_pos = window_pos;
			}
		}
		void End() {
			glfwMakeContextCurrent(handle);
			//delta_time = glfwGetTime();

			double currentFrame = glfwGetTime();
			delta_time = currentFrame - last_frame;
			last_frame = currentFrame;
			fps = 1 / delta_time;

			//fps = 1.0f / delta_time;
			//glfwSetTime(0);
			glfwSwapBuffers(handle);
			glfwPollEvents();
		}

		void Iconify();
		void Maximize();
		void Restore();

		void SetFullscreenMode(bool mode) {
			is_fullscreened = mode;

			if (is_fullscreened) {
				GLFWmonitor* fullscreen_monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(fullscreen_monitor);
				glfwSetWindowMonitor(handle, fullscreen_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			}
			else
				glfwSetWindowMonitor(handle, NULL, tmp_window_pos.x, tmp_window_pos.y, tmp_window_size.x, tmp_window_size.y, 0);
		}
		inline void SetVsyncMode(bool mode)
			{ glfwSwapInterval(vsync = mode); }
		void SetIcon(const std::string& data, pos2dus icon_size);
		void SetTitle(const std::string& title);
		void SetSize(pos2dus new_size);
		void SetPos(pos2ds new_pos);

		inline void Bind() const {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, window_size.x, window_size.y);
		}

		inline double GetDeltaTime() const
			{ return delta_time; }
		inline double GetFPS() const
			{ return fps; }
		inline pos2dus GetSize() const 
			{ return window_size; }
		inline pos2ds GetPos() const 
			{ return window_pos; }
		inline bool GetFullscreenMode() const
			{ return is_fullscreened; }
		inline bool GetVsyncMode() const 
			{ return vsync; }
		inline GLFWwindow* GetHandle()
			{ return handle; }
		
		inline bool Check() const 
			{ return !glfwWindowShouldClose(handle); }
		void Close();
		void Destroy();
		~JWindow();

	protected:
		GLFWwindow* handle = nullptr;
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
	inline void JWindow::Close() 
		{ glfwSetWindowShouldClose(handle, 1); }
	inline void JWindow::Destroy() {
		glfwDestroyWindow(handle);
		--window_instances;
	}
	inline void JWindow::Iconify() 
		{ glfwIconifyWindow(handle); }
	inline void JWindow::Maximize()
		{ glfwMaximizeWindow(handle); }
	inline void JWindow::Restore()
		{ glfwRestoreWindow(handle); }
	inline JWindow::~JWindow() {
		glfwDestroyWindow(handle);
		if (!--window_instances)
			glfwTerminate();
	}

	inline void JWindow::SetIcon(const std::string& data, pos2dus size) {
		GLFWimage tmp = { size.x, size.y, reinterpret_cast<unsigned char*>(const_cast<char*>(data.c_str())) };
		glfwSetWindowIcon(handle, 1, &tmp);
	}
	inline void JWindow::SetSize(pos2dus new_size)
		{ glfwSetWindowSize(handle, new_size.x, new_size.y); }
	inline void JWindow::SetPos(pos2ds new_size)
		{ glfwSetWindowPos(handle, new_size.x, new_size.y); }

	inline void JWindow::SetTitle(const std::string& title)
		{ glfwSetWindowTitle(handle, title.c_str()); }
}