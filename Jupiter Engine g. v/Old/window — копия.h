#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <pos2d.h>
#include <shader.h>
#include <camera.h>

using namespace std;
//#include <Framebuffer.h>




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

double scroll;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scroll = yoffset;
}

struct rgb {
    int r, g, b;
};

struct nrgb {
    float r = 0.0f, g = 0.0f, b = 0.0f; //float r = 0.0f, g = 0.5f, b = 0.6f;
};

class mwindow {
public:
    float aspratio;
    nrgb ncolor;
	GLFWwindow* id;
    pos2d scrs, wins;
    cam2d main_cam;
    //FramebufferObject fb;

    void createwindow(pos2d winsize, int scr_height, string name) {
        scrs.y = scr_height, wins = winsize, aspratio = (float)scrs.y / wins.y, scrs.x = wins.x * aspratio;
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        id = glfwCreateWindow(wins.x, wins.y, name.c_str(), NULL, NULL);
        if (id == NULL)
        {
            cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(id);
        glfwSetFramebufferSizeCallback(id, framebuffer_size_callback);
        //glfwSwapInterval(0);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            cout << "Failed to initialize GLAD" << std::endl;
        }
        //fb.CreateFramebuffer(scrs);
    }

    void startRender() {
        pos2d t;
        glfwGetWindowSize(id, &t.x, &t.y);
        if (t.x != wins.x || t.y != wins.y)
            wins = t, aspratio = (float)scrs.y / wins.y, scrs.x = wins.x * aspratio;//, fb.resize(scrs);
        //fb.use(scrs);
        glClearColor(ncolor.r, ncolor.g, ncolor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void update() {
        aspratio = (float)scrs.y / wins.y, scrs.x = wins.x * aspratio; //fb.resize(scrs);
    }

    void endRender() {
        //fb.render(wins);
        glfwSwapBuffers(id);
        glfwPollEvents();
    }

    bool checkWindow() {
        if (glfwWindowShouldClose(id)) return false;
        return true;
    }

    void closeWindow() {
        glfwSetWindowShouldClose(id, 1);
    }

    void setcolor(rgb color) {
        ncolor.r = (float)1.0f / 255 * color.r;
        ncolor.g = (float)1.0f / 255 * color.g;
        ncolor.b = (float)1.0f / 255 * color.b;
    }
};

class key {
public:
    int key_id;
    bool once;

    key(GLFWwindow* window) {
        id = window;
    }

    key(GLFWwindow* window, int key, bool once_flag = true) {
        id = window;
        key_id = key, once = once_flag;
    }

    bool check() {
        bool keystate = glfwGetKey(id, key_id), cond = (keystate != key_flag);
        return ((key_flag = key_flag != cond) && cond) && once || keystate && !once;
    }
    bool key_flag; GLFWwindow* id;
};


class key_combination {
public:
    int key1, key2;

    key_combination(GLFWwindow* window) {
        id = window;
    }

    key_combination(GLFWwindow* window, int first_key, int second_key) {
        id = window;
        key1 = first_key, key2 = second_key;
    }

    bool check() {
        bool keystate_1 = glfwGetKey(id, key1), keystate_2 = glfwGetKey(id, key2);
        pressed = pressed && !(!keystate_1 && key1_flag) && !(!keystate_2 && key2_flag);

        key1_flag = key1_flag != (key1_flag != keystate_1 != pressed);
        key2_flag = key2_flag != (key2_flag != keystate_2 != pressed);

        bool f_cond = (keystate_1 && keystate_2 && !pressed);
        pressed = pressed != f_cond;
        return f_cond;
    }

    bool key1_flag = 0, key2_flag = 0, pressed = 0; GLFWwindow* id;
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
    /*
    void update(pos2d wins, float aspratio) {
        wheel = scroll;
        scroll = 0;
        lb = 0, rb = 0;
        glfwGetCursorPos(id, &x, &y);
        if (x > wins.x) x = wins.x;
        if (y > wins.y) y = wins.y;
        //x = x * aspratio, y = y * aspratio;
        if (glfwGetMouseButton(id, 0))
            lb = 1;

        if (glfwGetMouseButton(id, 1))
            rb = 1;
    }
    */
    void once() {
        lb = 0, rb = 0;
        bool lb_state = glfwGetMouseButton(id, 0);
        bool rb_state = glfwGetMouseButton(id, 1);

        lb = lb || (!lb_state && lb_flag);
        lb_flag = lb_flag != (lb_state != lb_flag);

        rb = rb || (!rb_state && rb_flag);
        rb_flag = rb_flag != (rb_state != rb_flag);
    }

private: GLFWwindow* id; bool lb_flag, rb_flag;
       
};