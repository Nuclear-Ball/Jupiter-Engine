#pragma once
#include <glad/glad.h>
#include "Texture.h"
namespace JE {
	const std::string fb_shader_v = "#version 330 core\n"
									"layout(location = 0) in vec2 aPos;\n"
									"out vec2 TexCoord;\n"
									"void main() {\n"
										"gl_Position = vec4((aPos.x - 0.5) * 2, (aPos.y - 0.5) * 2, 0.0, 1.0);\n"
										"TexCoord = aPos;\n"
									"}";

	const std::string fb_shader_f = "#version 330 core\n"

									"out vec4 FragColor;\n"
									"in vec2 TexCoord;\n"

									"uniform float transparency;\n"
									"uniform sampler2D ourTexture;\n"

									"void main(){\n"
										"vec4 color = texture(ourTexture, TexCoord);\n"
										"FragColor = vec4(color.rgb, color.a - transparency);\n"
									"}";

	class Framebuffer : public JTexture::BasicTexture {
	private:
		unsigned int fbo_handle = 0;
		unsigned int rbo_handle = 0;

		inline void ResizeRBO(pos2dus size) {
			glBindRenderbuffer(GL_RENDERBUFFER, rbo_handle);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		}

	public:
		void Init() {
			glGenFramebuffers(1, &fbo_handle);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);

			BasicTexture::Init();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);

			glGenRenderbuffers(1, &rbo_handle);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_handle);
		}

		inline Framebuffer() : BasicTexture(0) 
			{ Init(); }
		inline Framebuffer(bool) : BasicTexture(0) {}
		inline Framebuffer(pos2dus size) {
			Init();
			Resize(size);
		}
		inline Framebuffer(const std::string& raw, pos2dus size) : BasicTexture(0) {
			Init();
			LoadRaw(raw, size);
		}
		inline Framebuffer(const std::string& compr_src) : BasicTexture(0) {
			Init();
			LoadCompressed(compr_src);
		}
		inline Framebuffer(const std::string& path, bool) : BasicTexture(0) {
			Init();
			LoadFile(path);
		}

		inline Framebuffer(const Framebuffer& cpy) : BasicTexture(0) {
			Init();
			this->LoadRaw(cpy.GetBinary(), cpy.GetSize());
		}
		inline Framebuffer(const BasicTexture& cpy) : BasicTexture(0) {
			Init();
			this->LoadRaw(cpy.GetBinary(), cpy.GetSize());
		}
		Framebuffer& operator=(const Framebuffer& cpy) {
			this->LoadRaw(cpy.GetBinary(), cpy.GetSize());
			return *this;
		}
		Framebuffer& operator=(const BasicTexture& cpy) {
			this->LoadRaw(cpy.GetBinary(), cpy.GetSize());
			return *this;
		}

		Framebuffer(Framebuffer&& cpy) noexcept : BasicTexture(0) {
			handle = cpy.handle;
			size = cpy.size;

			rbo_handle = cpy.rbo_handle;
			fbo_handle = cpy.fbo_handle;
		}
		Framebuffer& operator=(Framebuffer&& cpy) noexcept {
			std::swap(handle, cpy.handle);
			std::swap(size, cpy.size);

			std::swap(rbo_handle, cpy.rbo_handle);
			std::swap(fbo_handle, cpy.fbo_handle);
		}

		operator BasicTexture() {
			return *dynamic_cast<BasicTexture*>(this);
		}

		~Framebuffer() {
			glDeleteRenderbuffers(1, &rbo_handle);
			glDeleteFramebuffers(1, &fbo_handle);
		}


		void LoadRaw(const std::string& raw, pos2dus size) {
			BasicTexture::LoadRaw(raw, size);
			ResizeRBO(this->size);
		} 
		void LoadCompressed(const std::string& compr_src) {
			BasicTexture::LoadCompressed(compr_src);
			ResizeRBO(this->size);
		} 
		void LoadFile(const std::string& path) {
			BasicTexture::LoadFile(path);
			ResizeRBO(this->size);
		}

		void Resize(pos2dus new_size) {
			size = new_size;
			ResizeRBO(new_size);

			glBindTexture(GL_TEXTURE_2D, handle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, new_size.x, new_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}
		void Bind() const {
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
			glViewport(0, 0, size.x, size.y);
		}

		inline unsigned int GetFboHandle() 
			{ return fbo_handle; }
		inline unsigned int GetRboHandle()
			{ return rbo_handle; }

		void Deserialize(const std::string src) {
			BasicTexture::Deserialize(src);
			ResizeRBO(size);
		}
	};

	class Camera {
	public:
		Framebuffer frm = Framebuffer(0);
		pos2df center_pos = { 0, 0 };
		JTexture::Frame::Properties props;

		inline Camera() 
			{ frm.Init(); }
		Camera(bool){}

		inline void Init()
			{ frm.Init(); }

		inline glm::mat4 MakeViewMatrix() const {
			glm::mat4 view = props.MakeMatrix(glm::translate(glm::mat4(1), glm::vec3(frm.GetSize().x / 2.f + props.origin.x, frm.GetSize().y / 2.f + props.origin.y, 0.f)));
			view = glm::translate(view, glm::vec3(-center_pos.x + props.origin.x, center_pos.y + props.origin.y, 0.f));
			return view;
		}
		inline glm::mat4 MakeProjectionMatrix() const 
			{ return glm::ortho<float>(0, frm.GetSize().x, 0, frm.GetSize().y); }
		inline glm::mat4 MakePV() const 
			{ return MakeProjectionMatrix() * MakeViewMatrix(); }

		inline pos2d ScreenCrdsToAbs(pos2d pos) {
			const glm::vec4 res = MakeViewMatrix() / glm::vec4(pos.x, frm.GetSize().y - pos.y, 1.f, 1.f);
			return { static_cast<int>(res.x), static_cast<int>(res.y) };
		}
	};
}
//#include <window.h>
/*class Camera {
public:
	int x = 0, y = 0;
	wstring name;
	void lock(int objx, int objy, int scrw, int scrh) {
		x = scrw / 2 + objx, y = scrh / 2 + objy;
	}
};*/

//float fb_quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
//    -1,  1,        0, 1,
//    -1, -1,        0, 0,
//     1, -1,        1, 0,

//    -1,  1,        0, 1,
//     1, -1,        1, 0,
//     1,  1,        1, 1
//};

//string default_fb_shader_v = "#version 330 core\nlayout(location = 0) in vec2 aPos;\nlayout(location = 1) in vec2 aTexCoords;\nout vec2 TexCoords;\nvoid main() {\ngl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\nTexCoords = aTexCoords;\n}";
//string default_fb_shader_f = "#version 330 core\nout vec4 FragColor;\nin vec2 TexCoords;\nuniform sampler2D screenTexture;\nvoid main() {\nFragColor = texture(screenTexture, TexCoords);\n};";
/*
class FramebufferObject {
public:
    pos2d fb_size;
    unsigned int id, rbo_id, texture_id;
    Shader FramebufferShader;

    FramebufferObject() {

    }

    FramebufferObject(pos2d size) {
        CreateFramebuffer(size);
    }
    void CreateFramebuffer(pos2d size) {
        fb_size = size;
        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
        glGenRenderbuffers(1, &rbo_id);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_id);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindVertexArray(0);
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(fb_quadVertices), &fb_quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
        FramebufferShader.vsrc = default_fb_shader_v, FramebufferShader.fsrc = default_fb_shader_f;
        FramebufferShader.from_string();
    }

    void clear_use(rgb color = {}) {
        glViewport(0, 0, fb_size.x, fb_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, id);
		glClearColor(float(0.00390625f * color.r), float(0.00390625f * color.g), float(0.00390625f * color.b), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

	void clear(rgb color = {}) {
		glClearColor(float(0.00390625f * color.r), float(0.00390625f * color.g), float(0.00390625f * color.b), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

    void add_use() {
        glViewport(0, 0, fb_size.x, fb_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, id);
    }

    void resize(pos2d size) {
        fb_size = size;
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void end_render(pos2d window_size) {
        glViewport(0, 0, window_size.x, window_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void render(pos2d window_size) {
        glViewport(0, 0, window_size.x, window_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        FramebufferShader.use();
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

private: unsigned int quadVAO, quadVBO;
};*/
/*
class cam2d{
public:
    //mwindow *w;
    bool relative_to_center = true;
    pos2d pos;
    pos2d center;
    float zoom = 1;
    FramebufferObject fb;

    cam2d(){}

    cam2d(pos2d size) {
        fb.CreateFramebuffer(size);
    }

    glm::mat4 set_vec(pos2d screen_size) {
        if (!relative_to_center) {
            center.x = pos.x + screen_size.x / old_zoom / 2;
            pos.x = center.x - screen_size.x / zoom / 2;
            center.x = pos.x + screen_size.x / zoom / 2;

            center.y = pos.y + screen_size.y / old_zoom / 2;
            pos.y = center.y - screen_size.y / zoom / 2;
            center.x = pos.x + screen_size.x / zoom / 2;
        }
        old_zoom = zoom;

        //cout << center.x << " " << center.y << endl;
        float deltazoom = old_zoom - zoom;

        if (deltazoom != 0) {
            cout << zoom << endl;
            cout << (screen_size.x / old_zoom) << endl;
            pos.x += (screen_size.x / old_zoom - screen_size.x / zoom) / 2;
            pos.y += (screen_size.y / old_zoom - screen_size.y / zoom) / 2;
            //cout << "changed " << zoom - old_zoom << endl;
            old_zoom = zoom;
            //.x += (screen_size.x - screen_size.x / zoom) / 2;
            //pos.y -= (screen_size.y - screen_size.y / zoom) / 2;
            //cout << pos.x << " " << pos.y << endl;
        }
        else {
            old_zoom = zoom;
        }
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::scale(view, glm::vec3(zoom, zoom, 1.0f));
        view = glm::translate(view, glm::vec3(-center.x, -center.y, 0.0f));
        view = glm::translate(view, glm::vec3(screen_size.x / 2 / zoom, screen_size.y / 2 / zoom, 0.0f));
        return view;
    }
private: float old_zoom = 1;
};*/


/*
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
//#include <point.h>
#include <vector>
//#include <window.h>
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif

*/