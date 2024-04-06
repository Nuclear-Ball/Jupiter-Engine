#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <pos2d.h>

double scroll;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	scroll = yoffset;
}

class key {
public:
	int key_id;
	bool once;

	key() {}
	key(GLFWwindow* window) { id = window; }
	key(GLFWwindow* window, int key, bool once_flag = true) { id = window, key_id = key, once = once_flag; }

	void init(GLFWwindow* window, int key, bool once_flag = true) { id = window, key_id = key, once = once_flag; }

	bool check() {
		bool keystate = glfwGetKey(id, key_id), cond = (keystate != key_flag);
		return ((key_flag = key_flag != cond) && cond) && once || keystate && !once;
	}
	bool key_flag; GLFWwindow* id;
};


class key_combination {
public:
	int keys[2];

	key_combination() {

	}
	key_combination(GLFWwindow* window) {
		id = window;
	}
	key_combination(GLFWwindow* window, int first_key, int second_key) {
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

class n_key_combination {
public:
	std::vector<int> keys;

	n_key_combination(GLFWwindow* window, std::vector<int> keys) {
		id = window;
		this->keys = keys;
		keyflags.resize(keys.size());
	}

	bool check() {
		bool* keystates = new bool[keys.size()];
		bool keycond = 1;
		for (int i = 0; i < keys.size(); i++) {
			keystates[i] = glfwGetKey(id, keys[i]);
			keycond = keycond && !(!keystates[i] && keyflags[i]);
		}
		pressed = pressed && keycond;
		keycond = 1;

		for (int i = 0; i < keys.size(); i++) {
			keyflags[i] = keyflags[i] != (keyflags[i] != keystates[i] != pressed);
			keycond = keycond && keystates[i];
		}
		keycond = keycond && !pressed;
		pressed = pressed != keycond;
		delete[] keystates;
		return keycond;
	}
	std::vector<bool> keyflags;
	bool pressed = 0; GLFWwindow* id;
};

class mouse {
public:
	int x, y;
	bool lb, rb;
	double wheel;
	mouse(GLFWwindow* window) {
		id = window;
		glfwSetScrollCallback(window, scroll_callback);
	}

	mouse() {
	}

	void initmouse(GLFWwindow* window) {
		id = window;
		glfwSetScrollCallback(window, scroll_callback);
	}

	void update() {
		wheel = scroll;
		scroll = 0;

		lb = glfwGetMouseButton(id, 0), rb = glfwGetMouseButton(id, 1);
		double dx, dy;
		glfwGetCursorPos(id, &dx, &dy);
		x = dx, y = dy;
	}

	void once() {
		lb = 0, rb = 0;
		bool lb_state = glfwGetMouseButton(id, 0), rb_state = glfwGetMouseButton(id, 1);

		lb = lb || (!lb_state && lb_flag);
		lb_flag = lb_flag != (lb_state != lb_flag);

		rb = rb || (!rb_state && rb_flag);
		rb_flag = rb_flag != (rb_state != rb_flag);
	}

	pos2d once_ret() {
		bool buttons[] = { 0,0 };
		bool lb_state = glfwGetMouseButton(id, 0), rb_state = glfwGetMouseButton(id, 1);

		bool cond = (lb_state != lb_flag);
		buttons[0] = (lb_flag = lb_flag != cond) && cond;
		//buttons[0] = buttons[0] || (!lb_state && lb_flag);
		//lb_flag = lb_flag != (lb_state != lb_flag);

		buttons[1] = buttons[1] || (!rb_state && rb_flag);
		rb_flag = rb_flag != (rb_state != rb_flag);

		//bool keystate = glfwGetKey(id, key_id), cond = (keystate != key_flag);
		//buttons[0] = (key_flag = key_flag != cond) && cond;

		return { buttons[0], buttons[1] };
	}

private: GLFWwindow* id; bool lb_flag, rb_flag;

};