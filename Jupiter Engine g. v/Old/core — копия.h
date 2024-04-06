#pragma once
#include <Tilemap.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>
#include <vector>
#include <shader.h>
//#include <filewrite.h>
#include <texture_generated.h>
#include <locale>
#include <codecvt>
#include <filesystem>
#include <camera.h>
#include <window.h>
#include <texture.h>
#include <objects.h>
#include <required_opengl_objects.h>
#include <pos2d.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <jserial.h>
#include <type_traits>

using namespace std;

namespace core {
	enum animated_texture_host {
		prefab,
		tile, 
		obj
	};

	struct loaded_animated_texture {
		animated_texture_host type;
		unsigned short id;
		unsigned short id2;
		bool is_local;
	};

	class core {
	public:
		mwindow window;
		Shader sh;
		vector<gameobjs::Scene> scenes;
		texture::basic_texture error_texture;
		gameobjs::resources global_res;
		mouse ms;
		float delta_time;
		float last_frame;
		float fps;
		unsigned short c_scene_id = 0;
		pos2d chunksize = { 512, 512 };
		bool show_tmap_border_lines = 1;
		vector<loaded_animated_texture> loaded_anims;
		string saved_data;

		void save_proj(string path) {
			/*
			JSerial::JsTemplate bt, frm, anim, anim2, txt;
			bt.add_basic_type<string>("raw source");
			bt.add_basic_type<pos2d>("original size");
			bt.add_basic_type<pos2d>("offset");
			bt.add_basic_type<float>("rotation");
			bt.add_basic_type<pos2df>("scale");
			bt.add_basic_type<pos2d>("rotation origin");
			bt.add_basic_type<float>("transparency");

			frm.add_template("frame", bt, false);
			frm.add_basic_type<float>("duration");

			anim.add_template("frames", frm, true);
			anim.add_basic_type<texture::animation_types>("animation type");
			anim.add_basic_type<texture::animation_side>("animation side");

			anim2.add_template("animation", anim, true);

			txt.add_template("base frame", bt, true);
			txt.add_template("animation", anim2, true);
			txt.add_basic_type<int>("active anim id");
			txt.add_basic_type<int>("current frame id");
			txt.add_basic_type<int>("variant");

			JSerial::JsTemplate quarter, quarter1, tmap, layer, scene;
			quarter.add_array<tiles::tile>("tiles");
			quarter1.add_template("tiles", quarter, true);

			tmap.add_template("xpyp", quarter, true);
			tmap.add_template("xmyp", quarter, true);
			tmap.add_template("xpym", quarter, true);
			tmap.add_template("xmym", quarter, true);
			tmap.add_basic_type<pos2d>("pos size");
			tmap.add_basic_type<pos2d>("neg size");

			layer.add_template("tmap", tmap, false);
			layer.add_basic_type<string>("name");
			layer.add_basic_type<pos2df>("parallax speed");

			scene.add_template("layer", layer, true);

			JSerial::JSerial scn;
			scn.main_template = scene;
			scn.init_write();

			//Scene
			//----layers(template array)
			//--------tilemap(template)
			//------------quarter(template array)
			//----------------quarter1(template)
			//--------------------tiles(vector)
			//------------positive size(pos2d)
			//------------negative size(pos2d)
			//--------name(string)
			//--------parallax speed(pos2df)
			
			scn.prepare_template_array_write(scenes[0].rooms[0].Layers.size());
			for (int i = 0; i < scenes[0].rooms[0].Layers.size(); i++) {
				gameobjs::Layer& curr_layer = scenes[0].rooms[0].Layers[i];
				scn.prepare_template_array_write(curr_layer.tmap.map_xpyp.size());
				for (int j = 0; j < curr_layer.tmap.map_xpyp.size(); j++) {
					scn.write_basic_array<tiles::tile>(curr_layer.tmap.map_xpyp[j]);
				}
				
				scn.prepare_template_array_write(curr_layer.tmap.map_xmyp.size());
				for (int j = 0; j < curr_layer.tmap.map_xmyp.size(); j++) {
					scn.write_basic_array<tiles::tile>(curr_layer.tmap.map_xmyp[j]);
				}

				scn.prepare_template_array_write(scenes[0].rooms[0].Layers[i].tmap.map_xpym.size());
				for (int j = 0; j < curr_layer.tmap.map_xpym.size(); j++) {
					scn.write_basic_array<tiles::tile>(curr_layer.tmap.map_xpym[j]);
				}

				scn.prepare_template_array_write(scenes[0].rooms[0].Layers[i].tmap.map_xmym.size());
				for (int j = 0; j < scenes[0].rooms[0].Layers[i].tmap.map_xmym.size(); j++) {
					scn.write_basic_array<tiles::tile>(curr_layer.tmap.map_xmym[j]);
				}

				scn.write_basic_data<pos2d>(curr_layer.tmap.pos_size);
				scn.write_basic_data<pos2d>(curr_layer.tmap.neg_size);
				scn.write_string(curr_layer.name);
				scn.write_basic_data<pos2df>(curr_layer.parallax_speed);
			}
			saved_data = scn.data;
			//fstream file(path, ios::out, ios::binary);*/
		}

		void load_proj(string path) {
			/*
			scenes[0].rooms[0].Layers.clear();
			JSerial::JsTemplate bt, frm, anim, anim2, txt;
			bt.add_basic_type<string>("raw source");
			bt.add_basic_type<pos2d>("original size");
			bt.add_basic_type<pos2d>("offset");
			bt.add_basic_type<float>("rotation");
			bt.add_basic_type<pos2df>("scale");
			bt.add_basic_type<pos2d>("rotation origin");
			bt.add_basic_type<float>("transparency");

			frm.add_template("frame", bt, false);
			frm.add_basic_type<float>("duration");

			anim.add_template("frames", frm, true);
			anim.add_basic_type<texture::animation_types>("animation type");
			anim.add_basic_type<texture::animation_side>("animation side");

			anim2.add_template("animation", anim, true);

			txt.add_template("base frame", bt, true);
			txt.add_template("animation", anim2, true);
			txt.add_basic_type<int>("active anim id");
			txt.add_basic_type<int>("current frame id");
			txt.add_basic_type<int>("variant");

			JSerial::JsTemplate quarter, quarter1, tmap, layer, scene;
			quarter.add_array<tiles::tile>("tiles");
			quarter1.add_template("tiles", quarter, true);

			tmap.add_template("xpyp", quarter, true);
			tmap.add_template("xmyp", quarter, true);
			tmap.add_template("xpym", quarter, true);
			tmap.add_template("xmym", quarter, true);
			tmap.add_basic_type<pos2d>("pos size");
			tmap.add_basic_type<pos2d>("neg size");

			layer.add_template("tmap", tmap, false);
			layer.add_basic_type<string>("name");
			layer.add_basic_type<pos2df>("parallax speed");

			scene.add_template("layer", layer, true);

			//Scene
			//----layers(template array)
			//--------tilemap(template)
			//------------quarter(template array)
			//----------------quarter1(template)
			//--------------------tiles(vector)
			//------------positive size(pos2d)
			//------------negative size(pos2d)
			//--------name(string)
			//--------parallax speed(pos2df)

			JSerial::JSerial scn;
			scn.main_template = scene;
			scn.data = saved_data;
			scn.init_read();
			scn.no_next_after_reading = 1;
			size_t num_of_layers = scn.enter_template_array();
			for (int i = 0; i < num_of_layers; i++) {
				scn.acess_element_of_template_array(i);
				gameobjs::Layer tmp;
				scn.enter_template();//enters tmap template
				scn.go_to("pos size");
				tmp.tmap.pos_size = scn.read_basic_data<pos2d>();
				scn.next();
				tmp.tmap.neg_size = scn.read_basic_data<pos2d>();
				scn.go_to("xpyp");
				size_t size_x = scn.enter_template_array();
				//tmp.tmap.map_xpyp.resize(size_x);
				for (size_t j = 0; j < size_x; j++) {
					scn.acess_element_of_template_array(j);
					tmp.tmap.map_xpyp.push_back(scn.read_basic_array<tiles::tile>());
				}
				scn.leave_template_arr();

				scn.go_to("xmyp");
				size_x = scn.enter_template_array();
				for (size_t j = 0; j < size_x; j++) {
					scn.acess_element_of_template_array(j);
					tmp.tmap.map_xmyp.push_back(scn.read_basic_array<tiles::tile>());
				}
				scn.leave_template_arr();

				scn.go_to("xpym");
				size_x = scn.enter_template_array();
				for (size_t j = 0; j < size_x; j++) {
					scn.acess_element_of_template_array(j);
					tmp.tmap.map_xpym.push_back(scn.read_basic_array<tiles::tile>());
				}
				scn.leave_template_arr();

				scn.go_to("xmym");
				size_x = scn.enter_template_array();
				for (size_t j = 0; j < size_x; j++) {
					scn.acess_element_of_template_array(j);
					tmp.tmap.map_xmym.push_back(scn.read_basic_array<tiles::tile>());
				}
				scn.leave_template_arr();


				scn.leave_template();

				scn.go_to("name");
				tmp.name = scn.read_string();
				scn.next();
				tmp.parallax_speed = scn.read_basic_data<pos2df>();
				tmp.hidden = 0;
				tmp.show_bl = 1;
				tmp.tmap.offset = { 0, 0 };
				scenes[0].rooms[0].Layers.push_back(tmp);
			}
			scn.leave_template_arr();
			cout << endl;
			//saved_data = scn.data;
			//fstream file(path, ios::out, ios::binary);
			*/
		}

		void init(pos2d size, int scrheight, string title) {
			window.createwindow(size, scrheight, title);
			LnVAO.init(window.wins, window.scrs);
			sh.createShader(u"shaders/vert1.vs", u"shaders/frag1.fs");
			ms.initmouse(window.id);
			VAO.initvao();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);

			float currentFrame = glfwGetTime();
			delta_time = currentFrame - last_frame;
			last_frame = currentFrame;
			fps = 1 / delta_time;

			error_texture.load_from_raw(texture::err_frame_0, { 16, 16 });
		}
		/*
		pos2d screen_coords() {
			gameobjs::room& c_room = scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id];
			cam2d& cam = c_room.cams[c_room.selected_cam];
			pos2d pos, upper_left_corner;
			upper_left_corner.x = cam.center.x - window.scrs.x / 2 / cam.zoom;
			upper_left_corner.y = cam.center.y - window.scrs.y / 2 / cam.zoom;

			pos2df aspr = { float(cam.fb.fb_size.x) / float(window_size.x), float(cam.fb.fb_size.y) / float(window_size.y) };

			pos.x = ms.x / cam.zoom + upper_left_corner.x;
			pos.y = ms.y / cam.zoom + upper_left_corner.y;
			return { int(ms.x / cam.zoom * aspr.x + upper_left_corner.x), int(mouse_pos.y / cam.zoom * aspr.y + upper_left_corner.y) };
		}
		*/
		pos2d screen_coords(pos2d mouse_pos, pos2d window_size) {
			gameobjs::room& c_room = scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id];
			cam2d& cam = c_room.cams[c_room.selected_cam];
			pos2d upper_left_corner = { cam.center.x - cam.fb.fb_size.x / 2 / cam.zoom , cam.center.y - cam.fb.fb_size.y / 2 / cam.zoom };

			pos2df aspr = { float(cam.fb.fb_size.x) / float(window_size.x), float(cam.fb.fb_size.y) / float(window_size.y) };
			return { int(mouse_pos.x / cam.zoom * aspr.x + upper_left_corner.x), -int(mouse_pos.y / cam.zoom * aspr.y + upper_left_corner.y) };
		}


		pos2ds mst(jus layer_id, pos2d mouse_pos, pos2d window_size) {
			pos2d scrcrds = screen_coords(mouse_pos, window_size);
			gameobjs::room& c_room = scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id];
			return { short((scrcrds.x + (c_room.Layers[layer_id].parallax_speed.x * float(c_room.cams[c_room.selected_cam].center.x)) + c_room.Layers[layer_id].tmap.offset.x) / c_room.Layers[layer_id].tmap.grid_size.x - (scrcrds.x - c_room.Layers[layer_id].parallax_speed.x * float(c_room.cams[c_room.selected_cam].center.x) + c_room.Layers[layer_id].tmap.offset.x < 0)), 
				short((scrcrds.y + (c_room.Layers[layer_id].parallax_speed.y * float(c_room.cams[c_room.selected_cam].center.y)) + c_room.Layers[layer_id].tmap.offset.y) / c_room.Layers[layer_id].tmap.grid_size.y - (scrcrds.y - c_room.Layers[layer_id].parallax_speed.y * float(c_room.cams[c_room.selected_cam].center.y) + c_room.Layers[layer_id].tmap.offset.y < 0)) };
		}

		void setvecs(cam2d *cam) {
			sh.use();
			sh.setMat4("view", scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id].cams[scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id].selected_cam].set_vec(cam->fb.fb_size));
			projection = glm::ortho(0.0f, (float)cam->fb.fb_size.x, (float)cam->fb.fb_size.y, 0.0f, -1.0f, 1.0f);
			sh.setMat4("projection", projection);
		}

		void stop_animation(texture::texture* txt) {
			txt->active_animation = -1;
			txt->current_frame = 0;
			txt->time_from_last_frame = 0;
		}

		void stop_animation(gameobjs::object* obj) {
			obj->active_animation_id = -1;
			obj->current_frame_id = 0;
			obj->time_from_last_frame = 0;
		}

		void update_pointer(int pointer_id, int new_id) {
			gameobjs::room& c_room = scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id];
			for (int i = 0; i < c_room.global_object_pointers[pointer_id].usage_list.size(); i++) {
				object_pointer::pointer_usage &c_usage = c_room.global_object_pointers[pointer_id].usage_list[i];
				switch (c_usage.type) {
				//case object_pointer::animated_texture_list: loaded_anims[c_usage.id].id = new_id;
				}
			}
		}

		void create_obj(pos2d pos, unsigned short pfab_id, bool is_local, unsigned short layer_id) {
			pos2d pos_by_chunk = { pos.x / chunksize.x, pos.y / chunksize.y };
			pos2d pos_in_chunk = { pos.x - pos_by_chunk.x * chunksize.x, pos.y - pos_by_chunk.y * chunksize.y };
			gameobjs::room& c_room = scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id];
			c_room.global_object_pointers.push_back({ object_pointer::_object, layer_id, {0, 0}, int(c_room.Layers[layer_id].chunks.get_chunk({0, 0})->objs.size())});
			c_room.Layers[layer_id].chunks.get_chunk({ 0, 0 })->objs.push_back({pfab_id, is_local, pos, unsigned short(c_room.global_object_pointers.size() - 1)});
			c_room.Layers[layer_id].chunks.check_object(chunksize, c_room.Layers[layer_id].chunks.get_chunk({0, 0})->objs.size() - 1, {0, 0}, &c_room.global_object_pointers);
		}

		void delete_obj(int pointer_id) {
			gameobjs::room& c_room = scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id];
			vector<object_pointer::object_pointer>& c_pointer_list = c_room.global_object_pointers;
			object_pointer::object_pointer& c_pointer = c_pointer_list[pointer_id];
			gameobjs::chunk& c_chunk = *c_room.Layers[c_pointer.layer_id].chunks.get_chunk(c_pointer.chunk_id);

			for (int i = 0; i < c_room.global_object_pointers[pointer_id].usage_list.size(); i++) {
				object_pointer::pointer_usage& c_usage = c_room.global_object_pointers[pointer_id].usage_list[i];
				switch (c_usage.type) {
				//case object_pointer::animated_texture_list: loaded_anims[c_usage.id].id = -1;
				}
			}

			c_chunk.objs[c_pointer.in_chunk_object_id] = c_chunk.objs.back();
			c_pointer_list[c_chunk.objs[c_pointer.in_chunk_object_id].global_object_pointer_id].in_chunk_object_id = c_pointer.in_chunk_object_id;
			c_chunk.objs.pop_back();
			c_pointer_list[pointer_id] = c_pointer_list.back();
			update_pointer(pointer_id, pointer_id);
			c_pointer_list.pop_back();
		}

		bool animate(texture::texture* txt) {//возвращает true ели анимирование произошло, false если нет
			if (txt->active_animation > -1 && !txt->was_loaded_in_current_frame) {
				txt->was_loaded_in_current_frame = true;
				texture::animation& c_anim = txt->animations[txt->variant][txt->active_animation];
				int step = 1 - 2 * (c_anim.side == texture::backwards != txt->bounce_flag); //шаг отрицательный если либо сторона анимации оратная, либо флаг отскока(флаг отскока возможен только при типе анимации "отскок")
				txt->time_from_last_frame += delta_time;

				while (txt->time_from_last_frame >= c_anim.frames[txt->current_frame].duration) {
					txt->time_from_last_frame -= c_anim.frames[txt->current_frame].duration;

					if (((txt->current_frame > 0 && txt->current_frame < c_anim.frames.size() - 1) //если текущий кадр в допустимом диапазоне или если с начала, или если с конца (всегда срабатывает ТОЛЬКО одно из условий)
						|| (step + 1 && !txt->current_frame)
						|| (step - 1 && txt->current_frame == c_anim.frames.size() - 1)))

						txt->current_frame += step;
					else if ((txt->current_frame == c_anim.frames.size() - 1 && step) || (step == -1 && txt->current_frame == 0)) { //если текущий кадр достиг границы диапазона. в следующих линиях описано то, как дальше будет вести себя анимация
						switch (c_anim.type) {
						case texture::stop_when_end: stop_animation(txt); return true;
						case texture::repeating: txt->current_frame = (c_anim.frames.size() - 1) * (c_anim.side == texture::backwards); break;
						case texture::bounce: txt->bounce_flag = !txt->bounce_flag, step *= -1, txt->current_frame += step; break;
						}
					}
					else break;
				}
			}
			else return false;
			return true;
		}

		bool animate(gameobjs::object* obj) {//возвращает true ели анимирование произошло, false если нет
			if (obj->active_animation_id > -1) {
				texture::texture& t_txt = obj->is_local ? scenes[c_scene_id].local_res.prefabs[obj->pfab_id].txt : global_res.prefabs[obj->pfab_id].txt;
				texture::animation& c_anim = t_txt.animations[obj->variant][obj->active_animation_id];

				int step = 1 - 2 * (c_anim.side == texture::backwards != obj->bounce_flag); //шаг отрицательный если либо сторона анимации оратная, либо флаг отскока(флаг отскока возможен только при типе анимации "отскок")
				obj->time_from_last_frame += delta_time;

				while (obj->time_from_last_frame >= c_anim.frames[obj->current_frame_id].duration) {

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

		void on_frame_start() {
			float currentFrame = glfwGetTime();
			delta_time = currentFrame - last_frame;
			last_frame = currentFrame;
			fps = 1 / delta_time;
			window.startRender();
			ms.update();
		}

		void on_frame_end() {
			for (int i = 0; i < loaded_anims.size(); i++) {
				loaded_animated_texture& c_anim = loaded_anims[i];
				gameobjs::resources& res = c_anim.is_local ? scenes[c_scene_id].local_res : global_res;
				
				switch (c_anim.type) {
				case tile: res.tilesets[c_anim.id].tiles[c_anim.id2].tile_texture.was_loaded_in_current_frame = 0;  break;
				case prefab: res.prefabs[c_anim.id].txt.was_loaded_in_current_frame = 0;  break;
				}
			}
			loaded_anims.clear();
		}

		void process(cam2d *camera){
			gameobjs::room& c_room = scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id];
			cam2d& cam = *camera;

			for (unsigned short c_layer_id = 0; c_layer_id < c_room.Layers.size(); c_layer_id++) {
				gameobjs::Layer& c_layer = c_room.Layers[c_layer_id];
				pos2d base_tmap_layer_pos = { c_layer.tmap.offset.x - int(c_layer.parallax_speed.x * camera->center.x),
											  c_layer.tmap.offset.y - int(c_layer.parallax_speed.y * camera->center.y) };

				pos2d loading_range_x = { (cam.center.x - cam.fb.fb_size.x / 2 / cam.zoom) - base_tmap_layer_pos.x,
										  (cam.center.x + cam.fb.fb_size.x / 2 / cam.zoom) - base_tmap_layer_pos.x };

				pos2d loading_range_y = { (cam.center.y - cam.fb.fb_size.y / 2 / cam.zoom) - base_tmap_layer_pos.y,
										  (cam.center.y + cam.fb.fb_size.y / 2 / cam.zoom) - base_tmap_layer_pos.y };

				if (!c_layer.hidden) {
					for (int chunk_pos_x = loading_range_x.x / chunksize.x - (loading_range_x.x < 0); chunk_pos_x < loading_range_x.y / chunksize.x + (loading_range_x.y >= 0); chunk_pos_x++) { //Этот цикл отвечает за обработку обьектов перед отрисовкой
						for (int chunk_pos_y = loading_range_y.x / chunksize.x - (loading_range_y.x < 0); chunk_pos_y < loading_range_y.y / chunksize.y + (loading_range_y.y >= 0); chunk_pos_y++) {
							gameobjs::chunk& c_chunk = *c_layer.chunks.get_chunk({ chunk_pos_x, chunk_pos_y });
							if (&c_chunk != nullptr) {
								for (int i = 0; i < c_chunk.objs.size(); i++) {
									c_layer.chunks.check_object(chunksize, &c_chunk.objs[i], i, { chunk_pos_x, chunk_pos_y }, &c_chunk, &c_room.global_object_pointers);

								}
							}
						}
					}
				}
			}
		}

		void render(cam2d *camera) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);
			camera->fb.clear_use();
			setvecs(camera);

			gameobjs::room& c_room = scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id];
			cam2d& cam = *camera;
			
			for (unsigned short c_layer_id = 0; c_layer_id < c_room.Layers.size(); c_layer_id++) {
				gameobjs::Layer& c_layer = c_room.Layers[c_layer_id];
				pos2d base_tmap_layer_pos = { c_layer.tmap.offset.x - int(c_layer.parallax_speed.x * camera->center.x),
											  c_layer.tmap.offset.y - int(c_layer.parallax_speed.y * camera->center.y) };

				pos2d loading_range_x = { (cam.center.x - cam.fb.fb_size.x / 2 / cam.zoom) - base_tmap_layer_pos.x, 
										  (cam.center.x + cam.fb.fb_size.x / 2 / cam.zoom) - base_tmap_layer_pos.x };

				pos2d loading_range_y = { (-cam.center.y - cam.fb.fb_size.y / 2 / cam.zoom) - base_tmap_layer_pos.y, 
										  (-cam.center.y + cam.fb.fb_size.y / 2 / cam.zoom) - base_tmap_layer_pos.y };

				if (!c_layer.hidden) {
					for (short tile_pos_x = loading_range_x.x / c_layer.tmap.grid_size.x - (loading_range_x.x < 0); tile_pos_x < loading_range_x.y / c_layer.tmap.grid_size.x + (loading_range_x.y >= 0); tile_pos_x++) {
						for (short tile_pos_y = loading_range_y.x / c_layer.tmap.grid_size.x - (loading_range_y.x < 0); tile_pos_y < loading_range_y.y / c_layer.tmap.grid_size.y + (loading_range_y.y >= 0); tile_pos_y++) {
							tiles::tile c_tile = c_layer.tmap.lookup({ tile_pos_x, tile_pos_y });
							if (c_tile.tset_value) {
								try {
									gameobjs::resources& res = c_tile.is_local ? scenes[c_scene_id].local_res : global_res;
									tiles::t_tile& ct_tile = res.tilesets.at(c_tile.tset_value - 1).tiles.at(c_tile.tile_value);
									if (animate(&ct_tile.tile_texture)) 
										loaded_anims.push_back({tile, jus(c_tile.tset_value - 1), c_tile.tile_value, c_tile.is_local});
									texture::basic_texture& c_texture = *ct_tile.tile_texture.get_curr_frame();
									pos2d tile_pos = { tile_pos_x * c_layer.tmap.grid_size.x + c_texture.original_size.x / 2 + c_layer.tmap.offset.x + c_texture.offset.x,
													   tile_pos_y * c_layer.tmap.grid_size.y + c_texture.original_size.y / 2 + c_layer.tmap.offset.y + c_texture.offset.y };
									
									drawtexture_s(&c_texture, { tile_pos.x + base_tmap_layer_pos.x, tile_pos.y + base_tmap_layer_pos.y });
								}
								catch (...) {
									pos2d tile_pos = { tile_pos_x * c_layer.tmap.grid_size.x + 16 / 2 + c_layer.tmap.offset.x,
													   tile_pos_y * c_layer.tmap.grid_size.y + 16 / 2 + c_layer.tmap.offset.y};
									drawtexture(&error_texture, {tile_pos.x + base_tmap_layer_pos.x, tile_pos.y + base_tmap_layer_pos.y}, 1, {0, 0}, {c_layer.tmap.grid_size.x / 16.0f, c_layer.tmap.grid_size.y / 16.0f});
									//drawtexture(&error_texture,
									//	{ tile_pos_x * clayer.tmap.grid_size.x + error_texture.size.x / 2 + clayer.tmap.offset.x, tile_pos_y * clayer.tmap.grid_size.y + error_texture.size.y / 2 + clayer.tmap.offset.y },
									//	1);
								}
							}
							else {

							}
						}
					}

					#ifdef EDITOR
					if(show_tmap_border_lines && c_layer.show_bl &&(c_layer.tmap.pos_size.x || c_layer.tmap.neg_size.x))
						drawrect({ base_tmap_layer_pos.x - c_layer.tmap.gnwidth() * c_layer.tmap.grid_size.x,base_tmap_layer_pos.y - c_layer.tmap.gnheigh() * c_layer.tmap.grid_size.y},
							{ c_layer.tmap.gwidth() * c_layer.tmap.grid_size.x + base_tmap_layer_pos.x, c_layer.tmap.gheigh() * c_layer.tmap.grid_size.y + base_tmap_layer_pos.y - 1 });
					#endif
					//drawrect({ c_layer.tmap.offset.x - c_layer.tmap.gnwidth() * c_layer.tmap.grid_size.x,c_layer.tmap.offset.y - c_layer.tmap.gnheigh() * c_layer.tmap.grid_size.y }, { c_layer.tmap.gwidth()* c_layer.tmap.grid_size.x + c_layer.tmap.offset.x, c_layer.tmap.gheigh()* c_layer.tmap.grid_size.y + c_layer.tmap.offset.y });
					for (int chunk_pos_x = loading_range_x.x / chunksize.x - (loading_range_x.x < 0); chunk_pos_x < loading_range_x.y / chunksize.x + (loading_range_x.y >= 0); chunk_pos_x++) {//А этот за отрисовку
						for (int chunk_pos_y = loading_range_y.x / chunksize.x - (loading_range_y.x < 0); chunk_pos_y < loading_range_y.y / chunksize.y + (loading_range_y.y >= 0); chunk_pos_y++) {
							gameobjs::chunk& c_chunk = *c_layer.chunks.get_chunk({ chunk_pos_x, chunk_pos_y });

							//drawtexture(&global_res.tilesets[0].tiles[((chunk_pos_x % 2 == 0) != (chunk_pos_y % 2 == 0)) ? 1 : 15].tile_texture.base_frame[], {chunk_pos_x * chunksize.x + chunksize.x / 2, chunk_pos_y * chunksize.y + chunksize.y / 2}, &c_chunk == nullptr ? 0.2 : 0.5, {0, 0}, {chunksize.x / 16.0f, chunksize.x / 16.0f});
							if (&c_chunk != nullptr) {
								
								for (int i = 0; i < c_chunk.objs.size(); i++) {
									gameobjs::object* a = &c_chunk.objs[i];
									gameobjs::prefab& c_pref = a->is_local ? scenes[c_scene_id].local_res.prefabs[a->pfab_id] : global_res.prefabs[a->pfab_id];
									texture::texture tmp_txt = c_pref.txt;
									if (!animate(a))
										if (animate(&c_pref.txt))
											loaded_anims.push_back({ prefab, a->pfab_id, 0, a->is_local });
									texture::basic_texture& txt = a->active_animation_id + 1 ? *c_pref.txt.get_curr_frame_from_pfab(a->active_animation_id, a->current_frame_id) : *c_pref.txt.get_curr_frame();
									drawtexture(&txt, { a->pos.x + txt.offset.x + chunk_pos_x * chunksize.x, a->pos.y + txt.offset.y + chunk_pos_y * chunksize.y }, txt.transparency - a->transparency, txt.rotation_origin, {txt.scale.x + a->scale.x, txt.scale.y + a->scale.y}, txt.rotation + a->rotation);
								}
							}
						}
					}
					
					/*
					for (int obj_c = 0; obj_c < curr_layer.objs.size(); obj_c++) {
						gameobjs::object& curr_obj = curr_layer.objs[obj_c];
						gameobjs::prefab& curr_pfab = prefabs[curr_obj.pfab_id];

						model = glm::translate(model, glm::vec3(curr_obj.pos.x, curr_obj.pos.y, 0.0f));
						model = glm::scale(model, glm::vec3(textures[curr_pfab.txt_id].size.x + curr_obj.scale.x / 2, (textures[curr_pfab.txt_id].size.y + curr_obj.scale.x) / 2, 1.0f));
						sh.setMat4("model", model);
						model = glm::mat4(1.0f);
						textures[curr_pfab.txt_id].use();
						glBindVertexArray(VAO.VAO);
						glDrawArrays(GL_TRIANGLES, 0, 6);
					}*/

				}
			}
			//cam.fb.end_render(window.wins);
		}
		
		void drawtexture_s(texture::basic_texture* txt, pos2d pos) {
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(pos.x, -pos.y, 0.0f));
			model = glm::translate(model, glm::vec3(txt->rotation_origin.x, -txt->rotation_origin.y, 0.0f));
			model = glm::rotate(model, glm::radians((float)txt->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-txt->rotation_origin.x, txt->rotation_origin.y, 0.0f));
			model = glm::scale(model, glm::vec3((txt->original_size.x * txt->scale.x) / 2, (txt->original_size.y * txt->scale.y) / 2, 1.0f));
			sh.setMat4("model", model);
			model = glm::mat4(1);
			sh.setFloat("transparency", 1.0f - txt->transparency);
			txt->use();
			glBindVertexArray(VAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			sh.setFloat("transparency", 0);
		}

		void drawtexture(texture::basic_texture* txt, pos2d pos, float transparency = 0.5f, pos2d origin = { 0, 0 }, pos2df scale = { 1, 1 }, float rotation = 0) {
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(pos.x, -pos.y, 0.0f));
			model = glm::translate(model, glm::vec3(origin.x, -origin.y, 0.0f));
			model = glm::rotate(model, glm::radians((float)rotation), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-origin.x, origin.y, 0.0f));
			model = glm::scale(model, glm::vec3((txt->original_size.x * scale.x) / 2, (txt->original_size.y * scale.y) / 2, 1.0f));
			sh.setMat4("model", model);
			model = glm::mat4(1);
			sh.setFloat("transparency", 1.0f - transparency);
			txt->use();
			glBindVertexArray(VAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			sh.setFloat("transparency", 0);
		}

		void drawrect(pos2d start, pos2d end) {
			pos2d pos1, pos2;
			sh.setInt("isLine", 1);
			sh.setMat4("model", model);

			pos1.x = start.x, pos1.y = start.y, pos2.x = start.x, pos2.y = end.y + 1;
			LnVAO.update(pos1, pos2);
			glBindVertexArray(LnVAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			pos1 = pos2, pos2.x = end.x, pos2.y = end.y + 1;
			LnVAO.update(pos1, pos2);
			glBindVertexArray(LnVAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			pos1 = pos2, pos2.x = end.x, pos2.y = start.y + 1;
			LnVAO.update(pos1, pos2);
			glBindVertexArray(LnVAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			pos1 = pos2, pos2.x = start.x, pos2.y = start.y + 1;
			LnVAO.update(pos1, pos2);
			glBindVertexArray(LnVAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			sh.setInt("isLine", 0);
		}
	private:
		vao VAO;
		rectlnvao LnVAO;
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
	};
}




/*
if (curr_anim.type == texture::stop_when_end) {//если анимация должна остановится, то произойдет просто выход из цикла и обнуление индекса активной анимации, что приведет к тому, что текстура перестанет анимироваться
	stop_animation(txt); break;
}
else if (curr_anim.type == texture::repeating)//если анимация повторяется то установить текущий кадр в противоположной стороне от того, что есть сейчас
	txt->current_frame = (curr_anim.frames.size() - 1) * (curr_anim.side == texture::backwards);
else if (curr_anim.type == texture::bounce)//если тип анимации отскок, то изменить знак шага и перевернуть флаг отскока
	txt->bounce_flag = !txt->bounce_flag,
	step *= -1,
	txt->current_frame += step;*/
