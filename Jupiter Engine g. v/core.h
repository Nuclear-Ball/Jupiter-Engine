#pragma once
#include <iostream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "EngineStructs.h"
#include "camera.h"
#include "shader.h"
#include "pos2d.h"
#include "Texture.h"
#include "Quad.h"
#include "Tilemap.h"
#include "Common.h"


namespace JE {
	void NewFrame() {
		double currentFrame = glfwGetTime();
		Commons::delta_time = currentFrame - Commons::last_frame;
		Commons::last_frame = currentFrame;
		Commons::fps = 1 / Commons::delta_time;

		Commons::frame_counter++;
	}

	namespace Internals {
		class EngineRes {
		private:
			JE::Quad qd = JE::Quad(false);

			JShader txt_sh;
			JShader flat_sh;
			JShader framebuffer_sh;

			JE::JTexture::BasicTexture error_texture = JE::JTexture::BasicTexture(false);
			//JTexture::BasicTexture coords_start = JTexture::BasicTexture(false);
		public:
			void Init() {
				qd.Init();
				txt_sh.Load(common_vs, texture_shader_f);
				flat_sh.Load(common_vs, flatcolor_shader_f);
				framebuffer_sh.Load(fb_shader_v, fb_shader_f);

				error_texture.Init();
				error_texture.LoadRaw(JE::JTexture::ErrorFrames::frame_0, { 16, 16 });
			}

			inline const JE::Quad& Qd()
				{ return qd; }

			inline const JShader& TxtSh()
				{ return txt_sh; }
			inline const JShader& FlatSh()
				{ return flat_sh; }
			inline const JShader& FbSh()
				{ return framebuffer_sh; }

			inline const JE::JTexture::BasicTexture& ErrTxt()
				{ return error_texture; }
		};
	}

	Internals::EngineRes res;
	
	namespace Draw {
		inline glm::mat4 MakeMatrix(pos2df pos, pos2dus texture_size, const JTexture::Frame::TransfProps& props = {}) {
			glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(pos.x, pos.y, 0.f));
			model = props.MakeMatrix(model);
			model = glm::scale(model, glm::vec3(texture_size.x, texture_size.y, 1.f));

			return model;
		}

		void BTexture(const JTexture::BasicTexture& txt, pos2d pos, const JTexture::Frame::TransfProps& props = {}, float transparency = 1.f) {
			glm::mat4 model = MakeMatrix({ static_cast<float>(pos.x), static_cast<float>(pos.y) }, txt.GetSize(), props);
			glm::vec4 test = glm::vec4(1.f, 1.f, 0.f, 0.f) * model;
			res.TxtSh().Use();
			res.TxtSh().setMat4("model", model);
			res.TxtSh().setFloat("transparency", 1.0f - transparency);

			txt.Use();
			glBindVertexArray(res.Qd().Handle());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		void Texture(const JTexture::Texture& txt, pos2d pos, const JTexture::Frame::TransfProps& props = {}, float transparency = 1.f) {
			const JTexture::Frame& c_frame = txt.GetCurrentFrame();
			const JTexture::Frame::TransfProps& frame_props = c_frame.properties;

			glm::mat4 model = MakeMatrix({ static_cast<float>(pos.x), static_cast<float>(pos.y) }, c_frame.frame.GetSize(), 
			{ frame_props.rotation + props.rotation, {frame_props.scale.x + props.scale.x, frame_props.scale.y + props.scale.y}, 
			{frame_props.origin.x + props.origin.x, frame_props.origin.y + props.origin.y} });

			res.TxtSh().Use();
			res.TxtSh().setMat4("model", model);
			res.TxtSh().setFloat("transparency", 1.0f - (c_frame.transparency - (1.0f - transparency)));

			c_frame.frame.Use();
			glBindVertexArray(res.Qd().Handle());
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		void Line(pos2d start, pos2d end, int thickness = 1, rgb color = { 255, 255, 255, 255 }) {
			const int ct_x = (end.x - start.x), ct_y = (end.y - start.y);  //Катеты прямоугольного треугольника с вершинами в данных 2х точках 
			const float l = std::sqrt(std::pow(ct_x, 2) + std::pow(ct_y, 2)); //Гипотенуза
			const float ct_sin = static_cast<float>(ct_x) / l;  //Синус угла при начальной вершине
			constexpr double pi = 3.14159265358979323846;
			const float rotation = std::asinf(ct_sin) * (ct_y >= 0 ? 1 : -1) - (pi / 2) + (ct_y >= 0 ? 0 : pi);  //Угол вращения прямоугольника. Фактически тут мы находим угол направления гипотенузы в пространстве.

			const float offset = static_cast<float>(thickness) / 2;

			glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(static_cast<float>(start.x), static_cast<float>(start.y), 0.0f)); //Перенос к началу

			//model = glm::translate(model, glm::vec3(offset, offset, 0.0));  //Тут задается центр вращения
			model = glm::rotate(model, -rotation, glm::vec3(0.0f, 0.0f, 1.0f));	 //Само вращение
			model = glm::translate(model, glm::vec3(-offset, -offset, 0.0));  //Конец центра вращения
			model = glm::scale(model, glm::vec3(l + thickness, static_cast<float>(thickness), 1.0f)); //Тот прямоугольник растягивается под длину линии

			//Рендер
			res.FlatSh().Use();
			res.FlatSh().setMat4("model", model);
			res.FlatSh().setVec4("flat_color", glm::vec4(0.00390625f * static_cast<float>(color.r), 0.00390625f * static_cast<float>(color.g), 0.00390625f * static_cast<float>(color.b), 1.0));

			glBindVertexArray(res.Qd().Handle());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		void Line(pos2df start, pos2df end, int thickness = 1, rgb color = { 255, 255, 255 }) {
			const float ct_x = (end.x - start.x), ct_y = (end.y - start.y);  //Катеты прямоугольного треугольника с вершинами в данных 2х точках 
			const float l = std::sqrt(std::pow(ct_x, 2) + std::pow(ct_y, 2)); //Гипотенуза
			const float ct_sin = ct_x / l;  //Синус угла при начальной вершине
			constexpr double pi = 3.14159265358979323846;
			const float rotation = std::asinf(ct_sin) * (ct_y >= 0 ? 1 : -1) - (pi / 2) + (ct_y >= 0 ? 0 : pi);  //Угол вращения прямоугольника. Фактически тут мы находим угол направления гипотенузы в пространстве.

			const float offset = static_cast<float>(thickness) / 2;

			glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(start.x, start.y, 0.0f)); //Перенос к началу

			//model = glm::translate(model, glm::vec3(offset, offset, 0.0));  //Тут задается центр вращения
			model = glm::rotate(model, -rotation, glm::vec3(0.0f, 0.0f, 1.0f));	 //Само вращение
			model = glm::translate(model, glm::vec3(-offset, -offset, 0.0));  //Конец центра вращения
			model = glm::scale(model, glm::vec3(l + thickness, thickness, 1.0f)); //Тот прямоугольник растягивается под длину линии

			//Рендер
			res.FlatSh().Use();
			res.FlatSh().setMat4("model", model);
			res.FlatSh().setVec4("flat_color", glm::vec4(0.00390625f * static_cast<float>(color.r), 0.00390625f * static_cast<float>(color.g), 0.00390625f * static_cast<float>(color.b), 1.0));

			glBindVertexArray(res.Qd().Handle());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		void Rect(pos2d start, pos2d end) {
			end.x--;
			end.y--;

			pos2d pos1, pos2;

			pos1 = { start.x, start.y }, pos2 = { start.x, end.y };
			Line(pos1, pos2, 1);

			pos1 = pos2, pos2 = { end.x, end.y };
			Line(pos1, pos2, 1);

			pos1 = pos2, pos2 = { end.x, start.y };
			Line(pos1, pos2, 1);

			pos1 = pos2, pos2 = { start.x, start.y };
			Line(pos1, pos2, 1);
		}
		void Rect(pos2df start, pos2df end) {
			end.x--;
			end.y--;

			pos2df pos1, pos2;

			pos1 = start, pos2 = { start.x, end.y };
			Line(pos1, pos2);

			pos1 = pos2, pos2 = { end.x, end.y };
			Line(pos1, pos2);

			pos1 = pos2, pos2 = { end.x, start.y };
			Line(pos1, pos2);

			pos1 = pos2, pos2 = { start.x, start.y };
			Line(pos1, pos2);
		}

		void DisplayFramebuffer(const JTexture::BasicTexture& frm, const GlfwEnv::JWindow& win) {
			win.Bind();

			frm.Use();
			res.FbSh().Use();
			glBindVertexArray(res.Qd().Handle());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
	}

	class Core {
	public:
		WorldTypes::Project gamedata;

		void Render(Camera& cam) {
			cam.frm.Bind();
			res.TxtSh().Use();
			res.TxtSh().setMat4("pv", cam.MakePV());



			for (unsigned short c_layer_id = 0; c_layer_id < gamedata.CurrRoom().layers.size(); c_layer_id++) {

			}
		}
	};
}

//pos2d c_tile_pos = { tile_pos.x * c_layer.tmap.grid_size.x + (c_texture.original_size.x * c_texture.scale.x) / 2 + c_texture.offset.x,
//					 tile_pos.y * c_layer.tmap.grid_size.y + (c_texture.original_size.y * c_texture.scale.y) / 2 + c_texture.offset.y };

/*
pos1 = { start.x + 1, start.y + 1 }, pos2 = { start.x + 1, end.y + 1 };
			draw_line(pos1, pos2, 1);

			pos1 = pos2, pos2 = { end.x, end.y + 1 };
			draw_line(pos1, pos2, 1);

			pos1 = { pos2.x, pos2.y }, pos2 = { end.x, start.y + 1 };
			draw_line(pos1, pos2, 1);

			pos1 = { pos2.x, pos2.y }, pos2 = { start.x + 1, start.y + 1 };
			draw_line(pos1, pos2, 1);



*/

/*
		bool animate(gameobjs::object* obj) {//возвращает true ели анимирование произошло, false если нет
			if (obj->animation_props.active_animation > -1) {
				texture::texture& t_txt = proj.get_res(obj->is_local)->prefabs[obj->pfab_id].txt;
				texture::animation& c_anim = t_txt.animations[obj->animation_props.active_animation];

				int step = 1 - 2 * (c_anim.side == texture::backwards != obj->animation_props.bounce_flag); //шаг отрицательный если либо сторона анимации оратная, либо флаг отскока(флаг отскока возможен только при типе анимации "отскок")
				obj->animation_props.time_from_last_frame += delta_time;

				while (obj->animation_props.time_from_last_frame >= c_anim.frames[obj->animation_props.current_frame].duration) {

					obj->time_from_last_frame -= c_anim.frames[obj->current_frame_id].duration;

					if (((obj->current_frame_id > 0 && obj->current_frame_id < c_anim.frames.size() - 1) //если текущий кадр в допустимом диапазоне или если с начала, или если с конца (всегда срабатывает ТОЛЬКО одно из условий)
						|| (step + 1 && !obj->current_frame_id)
						|| (step - 1 && obj->current_frame_id == c_anim.frames.size() - 1)))

						obj->current_frame_id += step;

					else if ((obj->current_frame_id == c_anim.frames.size() - 1 && step) || (step == -1 && obj->current_frame_id == 0)) { //если текущий кадр достиг границы диапазона. в следующих линиях описано то, как дальше будет вести себя анимация
						switch (c_anim.type) {
						case texture::stop_when_end: stop_animation(obj); return true;
						case texture::repeating: obj->current_frame_id = (c_anim.frames.size() - 1) * (c_anim.side == texture::backwards); break;
						case texture::bounce: obj->bounce_flag = !obj->bounce_flag, step *= -1, obj->current_frame_id += step; break;
						}
					}
					else break;
				}
			}
			else return false;
			return true;
		}
		*/

/*
cam2d& cam = *camera;

	pos2d upper_left_corner = { cam.center.x - cam.fb.fb_size.x / 2.0f / cam.zoom ,
								cam.center.y - cam.fb.fb_size.y / 2.0f / cam.zoom };

	pos2df aspr = { static_cast<float>(cam.fb.fb_size.x) / static_cast<float>(window_size.x),
					static_cast<float>(cam.fb.fb_size.y) / static_cast<float>(window_size.y) };

	return { static_cast<int>(mouse_pos.x / cam.zoom * aspr.x + upper_left_corner.x) - 1,
			 static_cast<int>(mouse_pos.y / cam.zoom * aspr.y + upper_left_corner.y) - 1 };
*/

/*

		void draw_line_f(pos2df start, pos2df end, int thickness, rgb color = { 255, 255, 255 }) {
			flat_sh.use();
			const float ct_x = (end.x - start.x), ct_y = (end.y - start.y);
			const float l = sqrt(pow(ct_x, 2) + pow(ct_y, 2));
			const int q_ids[] = { 0, 1, 3, 2 };
			const int quarter = q_ids[(((ct_y < 0) << 1) + (ct_x < 0))];
			const double pi = 3.14159265358979323846;
			const float rotation = (pi / 2) + (asinf(static_cast<float>(ct_x) / l) * (-1.0f * (ct_y >= 0) + 1 * (ct_y < 0)));

			model = glm::translate(model, glm::vec3(start.x + ct_x / 2.0f - 0.5f, -(start.y + ct_y / 2.0f + 0.5f), 0.0f));
			model = glm::rotate(model, -rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3((l + thickness) / 2.0f, static_cast<float>(thickness) / 2.0f, 1.0f));
			flat_sh.setMat4("model", model);
			model = glm::mat4(1);
			flat_sh.setVec4("flat_color", glm::vec4(0.00390625f * static_cast<float>(color.r), 0.00390625f * static_cast<float>(color.g), 0.00390625f * static_cast<float>(color.b), 1.0));
			glBindVertexArray(VAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			txt_sh.use();
		}

if (curr_anim.type == texture::stop_when_end) {//если анимация должна остановится, то произойдет просто выход из цикла и обнуление индекса активной анимации, что приведет к тому, что текстура перестанет анимироваться
	stop_animation(txt); break;
}
else if (curr_anim.type == texture::repeating)//если анимация повторяется то установить текущий кадр в противоположной стороне от того, что есть сейчас
	txt->current_frame = (curr_anim.frames.size() - 1) * (curr_anim.side == texture::backwards);
else if (curr_anim.type == texture::bounce)//если тип анимации отскок, то изменить знак шага и перевернуть флаг отскока
	txt->bounce_flag = !txt->bounce_flag,
	step *= -1,
	txt->current_frame += step;
	
		void drawrect(pos2d start, pos2d end) {
			pos2d pos1, pos2;
			//sh.setInt("isLine", 1);
			//sh.setMat4("model", model);

			pos1.x = start.x + 1, pos1.y = start.y + 1, pos2.x = start.x + 1, pos2.y = end.y + 1;
			//draw_line(pos1, pos2, 1);
			//LnVAO.update(pos1, pos2);
			//glBindVertexArray(LnVAO.VAO);
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			pos1 = pos2, pos2.x = end.x, pos2.y = end.y + 1;
			//draw_line(pos1, pos2, 1);
			//LnVAO.update(pos1, pos2);
			//glBindVertexArray(LnVAO.VAO);
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			pos1 = { pos2.x, pos2.y }, pos2.x = end.x, pos2.y = start.y + 1;
			//draw_line(pos1, pos2, 1);
			//LnVAO.update(pos1, pos2);
			//glBindVertexArray(LnVAO.VAO);
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			pos1 = { pos2.x, pos2.y }, pos2.x = start.x + 1, pos2.y = start.y + 1;
			//draw_line(pos1, pos2, 1);
			//LnVAO.update(pos1, pos2);
			//glBindVertexArray(LnVAO.VAO);
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			//sh.setInt("isLine", 0);
		}
	*/
