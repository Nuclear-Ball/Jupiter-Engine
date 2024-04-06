#pragma once
#include <filesystem>
#include <experimental/filesystem>
using namespace std;// using namespace ImGui;

inline bool in_range(int start_val, int middle_val, int end_val) {
	return (start_val < middle_val) && (middle_val < end_val);
}

string remove_zeros_from_float(string src) {
	for (int i = src.size() - 1; i > 0; i--) {
		switch (src[i]) {
		case '0': src.pop_back(); break;
		case '.': src.pop_back(); return src;
		default: return src;
		}
	}
}

void hash_unique_names(unordered_map<string, unsigned short>* all_names, string* name, unsigned short id) { //Преобразует повторяющиеся имена(то есть добавляет в конец " (n)" )
	string orig_name = *name;
	for (int i = 0; all_names->operator[](*name); i++) {
		*name = string(orig_name + " (" + to_string(i + 1) + ")");
	}
	all_names->operator[](*name) = id + 1;
}

class Editor {
public:
	JupiterEngine::core_class core;
	key_combination save, 
					LoadFile;

	string project_save_path;

	key w, 
		a, 
		s, 
		d, 
		picker, 
		home_button,
		delete_;
	
	fb_widget mainwin;
	unsigned short selected_layer = 0;
	unsigned short mode = 0; 

	Tiles::Tile selected_tile = { 0, 0, 0 };
	unsigned short selected_prefab = 0;
	bool selected_prefab_local = 0;

	//bool enable_tooltips = 0;
	JTexture::BasicTexture icon;

	mimg mg;
	ImGuiID dockspace_id;
	cam2d editor_cam;
	std::string script_output;

	bool project_editor_opened = 1;
	bool main_menu = 1;

	//texture_params_popup
	bool call_texture_params_popup = 0,
		 texture_params_popup_is_target_local = 0,
		 texture_params_popup_texture_type = 0;     //0-tile, 1-prefab
	unsigned short texture_params_popup_texture_id = 0, 
				   texture_params_popup_texture_id2 = 0;



	void init() {
		core.init();

		editor_cam.fb.CreateFramebuffer({1, 1});

		save.init(core.window->id, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_S);
		LoadFile.init(core.window->id, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_L);

		w.init(core.window->id, GLFW_KEY_W, 0);
		a.init(core.window->id, GLFW_KEY_A, 0);
		s.init(core.window->id, GLFW_KEY_S, 0);
		d.init(core.window->id, GLFW_KEY_D, 0);
		picker.init(core.window->id, GLFW_KEY_P);
		home_button.init(core.window->id, GLFW_KEY_HOME);
		delete_.init(core.window->id, GLFW_KEY_DELETE);

		JSerial::Read dat({JSerial::Type<string>("font"), JSerial::Array<string>("icons and graphics")}, JSerial::Utilities::ReadFile(u8"Данные движка.дфю"));

		string icon_src = dat.ReadArrayElement<string>("icons and graphics", 1);

		core.window->SetIcon(icon_src, { 32, 32 });
		icon.LoadRaw(icon_src, { 32, 32 });

		mg.init(core.window->id, dat.ReadType<string>("font"));
		mg.io->ConfigWindowsMoveFromTitleBarOnly = 1;
		mg.io->IniFilename = NULL;
	}

	void demo_create() {
		core.proj.scenes.resize(1);
		core.proj.scenes[0].name = u8"Демо";
		core.proj.scene_hash[u8"Демо"] = 1;

		core.proj.scenes[0].rooms.resize(1);
		//core.get_curr_room()->cams.resize(1);
		//core.get_curr_room()->cams[0].fb.CreateFramebuffer(core.window.scrs);

		//загрузка ресурсов
		core.proj.tmp_textures.resize(1);
		core.proj.tmp_textures[0].LoadFile(u8"tiles.png");

		vector<string> tmp = texture_processing::crop_from_croplist(core.proj.tmp_textures[0].GetBinary(),
			texture_processing::generate_croplist(&core.proj.tmp_textures[0], 34), core.proj.tmp_textures[0].GetSize());



		core.proj.global_res.tilesets.resize(1);
		core.proj.global_res.tilesets[0].tiles.resize(34);
		core.proj.global_res.tilesets[0].name = u8"Демо тайлсет";
		core.proj.global_res.tileset_hash[u8"Демо тайлсет"] = 1;

		for (int i = 0; i < 34; i++)
			core.proj.global_res.tilesets[0].tiles[i].tile_texture.base_frame.frame.LoadRaw(tmp[i], { 16, 16 });

		core.proj.get_curr_room()->layers.resize(1);
		//core.proj.get_curr_room()->layers[0].chunks.xpyp.quarter.resize(1);
		//core.proj.get_curr_room()->layers[0].chunks.xpyp.quarter[0].resize(1);
		//core.proj.get_curr_room()->layers[0].chunks.xpyp.size = { 1, 1 };
		core.proj.get_curr_room()->layers.back().name = u8"Слой 1";
		core.proj.get_curr_room()->layer_hash[u8"Слой 1"] = 1;
	}

	inline void init_dockspace(ImGuiID* dockspace_id, mimg* mg) {
		ImGui::DockBuilderRemoveNode(*dockspace_id);
		ImGui::DockBuilderAddNode(*dockspace_id, mg->dockspace_flags | ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(*dockspace_id, mg->viewport->Size);
		ImGui::DockBuilderGetCentralNode(*dockspace_id);
		ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(*dockspace_id, ImGuiDir_Left, 0.17f, nullptr, dockspace_id);
		ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(*dockspace_id, ImGuiDir_Right, 0.3f, nullptr, dockspace_id);
		ImGui::DockBuilderDockWindow(u8"Слои", dock_id_right);
		ImGui::DockBuilderDockWindow(u8"Менеджер ресурсов", dock_id_right);
		ImGui::DockBuilderDockWindow(u8"Выбор тайлов", dock_id_left);
		ImGui::DockBuilderDockWindow(u8"Главное окно", *dockspace_id);

		ImGui::DockBuilderDockWindow(u8"Редактор проекта", *dockspace_id);
		ImGui::DockBuilderDockWindow(u8"Добро пожаловать!", *dockspace_id);
		ImGui::DockBuilderDockWindow(u8"Выбор сцены", dock_id_left);

		ImGui::DockBuilderFinish(*dockspace_id);
	}

	void update() {
		core.on_frame_start();
		mg.update();
		dockspace_id = ImGui::GetID("Root");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), mg.dockspace_flags);

		static bool first_time = 1;
		if (first_time) {
			first_time = !first_time;
			init_dockspace(&dockspace_id, &mg);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool raw_texture_params_popup(JTexture::BasicTexture* target, JTexture::Frame::Properties* props) {

		//Инициализация
		static bool first_time = 1;
		if (first_time) {
			ImGui::OpenPopup(u8"Параметры текстуры##raw_texture_params");
			first_time = 0;
		}

		//Главное окно
		bool is_opened = 1;
		if (ImGui::BeginPopupModal(u8"Параметры текстуры##raw_texture_params", &is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
			is_opened = 0;
			ImGui::Text(u8"Параметры текстуры:                                  ");
			gameobjs::resources& res = *core.proj.get_res(texture_params_popup_is_target_local);
			pos2d winsize = get_window_size();
			ImGui::Image(ImTextureID(target->GetHandle()), ImVec2(winsize.x, winsize.x));
			if (ImGui::TreeNode(u8"Параметры")) {

				ImGuiStyle st = ImGui::GetStyle();
				ImGui::PushItemWidth(winsize.x - st.IndentSpacing);
				ImGui::Text(string(u8"Размер: " + to_string(target->GetSize().x) + u8" на " + to_string(target->GetSize().y) + u8" пикселей").c_str());
				ImGui::Text(u8"Прозрачность: ");
				ImGui::SliderFloat(string(u8"##txt_transparency").c_str(), &props->transparency, 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::PushItemWidth((winsize.x - st.ItemSpacing.x - st.IndentSpacing) / 2);
				ImGui::Text(u8"Увеличение");
				ImGui::SliderFloat(string(u8"##txt_size_x").c_str(), &props->scale.x, 0, 2, u8"x: %.3f");
				ImGui::SameLine();
				ImGui::SliderFloat(string(u8"##txt_size_y").c_str(), &props->scale.y, 0, 2, u8"y: %.3f");
				ImGui::PopItemWidth();

				ImGui::Text(u8"Вращение: ");
				ImGui::SliderFloat(string(u8"##txt_rotation").c_str(), &props->rotation, -180, 180);
				ImGui::PopItemWidth();

				ImGui::PushItemWidth((winsize.x - st.ItemSpacing.x - st.IndentSpacing) / 2);
				ImGui::Text(u8"Центр вращения: ");
				ImGui::SliderInt(string(u8"##txt_org_x").c_str(), &props->rotation_origin.x, 0, (target->GetSize().x * props->scale.x), u8"x: %d");
				ImGui::SameLine();
				ImGui::SliderInt(string(u8"##txt_org_y").c_str(), &props->rotation_origin.y, 0, (target->GetSize().y * props->scale.y), u8"y: %d");

				ImGui::Text(u8"Смещение");
				ImGui::SliderInt(string(u8"##txt_off_x").c_str(), &props->offset.x, -(target->GetSize().x * props->scale.x), target->GetSize().x * props->scale.x, u8"x: %d");
				ImGui::SameLine();
				ImGui::SliderInt(string(u8"##txt_off_y").c_str(), &props->offset.y, -(target->GetSize().y * props->scale.y), target->GetSize().y * props->scale.y, u8"y: %d");
				
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}
			if (ImGui::Button(u8"Закрыть", ImVec2(winsize.x, 0))) {
				ImGui::CloseCurrentPopup();
				is_opened = 1;
			}

			ImGui::EndPopup();
		}

		//При закрытии
		if (is_opened) {
			first_time = 1;
		}

		return !is_opened;
	}

	bool animation_editor(JTexture::Animation* c_anim, JTexture::Texture::AnimPlayerProperties* c_props, int target_id) {
		ImGuiStyle st = ImGui::GetStyle();
		static bool was_dragged = 0;
		bool is_opened = 1;

		//Инициализация
		static bool first_time = 1;
		if (first_time) {
			first_time = 0;
			ImGui::OpenPopup(u8"Редактировать анимацию##anim_edit");
			c_props->current_frame = 0;
			c_props->paused = 1;
		}

		//Окно редактора
		if (ImGui::BeginPopupModal(u8"Редактировать анимацию##anim_edit", &is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
			is_opened = 0;

			//Предпросмотр анимации
			ImGui::Image(ImTextureID(c_anim->GetFrames().size() ? c_anim->GetFrames()[c_props->current_frame].frame.GetHandle() : 0), ImVec2(150, 150));

			//Настройки анимации
			{
				ImGui::SameLine();
				ImGui::BeginChild(u8"anim_params_types", ImVec2(200, 150));
				pos2d c_winsize = get_window_size();

				//Кнопка паузы
				{
					//Изменяет цвет, если нажата
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(st.Colors[ImGuiCol_Button].x, st.Colors[ImGuiCol_Button].y, st.Colors[ImGuiCol_Button].z, st.Colors[ImGuiCol_Button].w - (0.3 * c_props->paused)));

					//Выключена, если кадров нет
					if (!c_anim->GetFrames().size())
						ImGui::BeginDisabled();

					//Если анимация не запущена, то ее запускает
					if (ImGui::Button(u8"Пауза", ImVec2((c_winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 3, 0)))
						c_props->paused = !c_props->paused, c_props->active_animation = target_id + 1;

					if (!c_anim->GetFrames().size())
						ImGui::EndDisabled();

					ImGui::PopStyleColor();
				}

				//Кнопка возврата в начало
				{
					ImGui::SameLine();

					if (ImGui::Button(u8"В начало")) {
						c_props->Reset(*c_anim);
					}
				}

				//Параметры анимации(Типы)
				if (ImGui::TreeNode(u8"Параметры анимации: ")) {
					ImGui::PushItemWidth(c_winsize.x - st.IndentSpacing);
					//Выбор типа
					{
						ImGui::Text(u8"Тип анимации:");
						static const char* Type[] = { u8"Повторяющийся",
																 u8"С остановкой в конце",
																 u8"С Отскоком" };
						if (ImGui::BeginCombo("##anim_type", Type[static_cast<unsigned char>(c_anim->type)])) {
							for (unsigned char i = 0; i < 3; i++) {
								if (ImGui::Selectable(Type[i], i == static_cast<unsigned char>(c_anim->type)))
									*reinterpret_cast<int*>(&c_anim->type) = i;
							}
							ImGui::EndCombo();
						}
					}

					//Выбор направления
					{
						ImGui::Text(u8"Направление: ");
						static const char* Direction[] = { u8"Нормальное",
														   u8"Обратное" };
						ImGui::SliderInt("##slider_anim_direction", reinterpret_cast<int*>(&c_anim->direction), 0, 1, Direction[static_cast<unsigned char>(c_anim->direction)]);
					}

					ImGui::PopItemWidth();
					ImGui::TreePop();
				}

				//Параметры выбранного кадра
				if (c_anim->GetFrames().size()) {
					if (ImGui::TreeNode(u8"Кадр: ")) {

						//Отключить если не стоит на паузе 
						if (!c_props->paused)
							ImGui::BeginDisabled();

						//Ползунок
						ImGui::Text(u8"Длительность:");
						
						ImGui::DragFloat(u8"##framedur", &c_anim->GetFrames(true)[c_props->current_frame].duration, 0.005f, 0.01f, 65535, string(remove_zeros_from_float(to_string(c_anim->GetFrames()[c_props->current_frame].duration)) + u8"сек.").c_str(), ImGuiSliderFlags_AlwaysClamp);

						//Кнопка "Применить для всех"
						if (ImGui::Button(u8"Применить для всех")) {
							for (JTexture::Frame& i : c_anim->GetFrames(true))
								i.duration = c_anim->GetFrames()[c_props->current_frame].duration;
						}

						if (!c_props->paused || !c_anim->GetFrames().size())
							ImGui::EndDisabled();
						ImGui::TreePop();
					}
				}

				ImGui::EndChild();
			}

			//Кадры
			{
				ImGui::BeginChild("##frames", ImVec2(0, 250 + st.ScrollbarSize + st.WindowPadding.y * 2), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

				static int frame_texture_params_id = -1;

				//Проход по всем кадрам текущей анимации
				for (unsigned short frame_id = 0; frame_id < c_anim->GetFrames().size(); ++frame_id) {

					//Выбор кадра
					if (ImGui::Selectable(string("##frame" + to_string(c_anim->GetFrames()[frame_id].additional_editor_id)).c_str(), frame_id == c_props->current_frame, 0, ImVec2(0, 50)) && !was_dragged) {
						c_props->current_frame = frame_id;
						c_props->paused = 1;
					}

					//Перемещение
					if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
						int next = frame_id + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
						if (next >= 0 && next < c_anim->GetFrames().size()) {
							was_dragged = 1;

							c_props->paused = 1;
							std::swap(c_anim->GetFrames(true)[frame_id], c_anim->GetFrames(true)[next]);

							//Перемещение индекса выделения
							if (c_props->current_frame == next) 
								c_props->current_frame = frame_id;
							else if (c_props->current_frame == frame_id) 
								c_props->current_frame = next;

							ImGui::ResetMouseDragDelta();
						}
					}

					//Меню кадра
					if (ImGui::BeginPopupContextItem()) {

						//Открытие окна с параметрами текстуры кадра
						if (ImGui::MenuItem(u8"Параметры текстуры")) {
							frame_texture_params_id = frame_id;
						}

						//Перемещение во временные текстуры
						if (ImGui::MenuItem(u8"Переместить во временные текстуры")) {
							core.proj.tmp_textures.push_back(c_anim->GetFrames()[frame_id].frame);
							c_anim->GetFrames(true).erase(c_anim->GetFrames().begin() + frame_id);
						}

						//Удаление
						ImGui::Separator();
						if (ImGui::MenuItem(u8"Удалить")) {
							c_anim->GetFrames(true).erase(c_anim->GetFrames().begin() + frame_id);

							//Изменение индекса выделения
							if (c_props->current_frame >= frame_id && c_props->current_frame > 0)
								c_props->current_frame--;

							if (!c_anim->GetFrames().size()) {
								c_props->Stop();
							}
						}

						ImGui::EndPopup();
					}

					//Остаток текста внутри кнопки. Иф здесь нужен, чтобы при удалении элемента не было выхода за границы массива
					if (frame_id < c_anim->GetFrames().size()) {
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - st.ItemSpacing.x);
						ImGui::Image(ImTextureID(c_anim->GetFrames()[frame_id].frame.GetHandle()), ImVec2(50, 50));
						ImGui::SameLine();
						ImGui::Text(string(u8"Кадр #" + to_string(frame_id) + u8", Задержка: " + remove_zeros_from_float(to_string(c_anim->GetFrames()[frame_id].duration)) + u8"сек").c_str());
					}
				}

				//Параметры текстуры кадра
				if (frame_texture_params_id >= 0) {
					if (!raw_texture_params_popup(&c_anim->GetFrames(true)[frame_texture_params_id].frame, &c_anim->GetFrames(true)[frame_texture_params_id].properties)) {
						frame_texture_params_id = -1;
					}
				}

				ImGui::EndChild();
			}

			//Добавление кадров
			{
				static bool import_wait = 0;
				static bool crop_wait = 0;
				static JTexture::BasicTexture tmp;

				//Кнопка "Добавить"
				if (ImGui::Button(u8"Добавить")) {
					import_wait = 1;
				}

				//Ожидание импорта
				if (import_wait) {
					static string data;
					if (!Popups::Import(&data, { u8"Изображения", u8"*.png", u8"*.jpg" })) {
						import_wait = 0;

						//Если хоть что-то было загружено
						if (data.size()) {
							crop_wait = 1;
							tmp.LoadCompressed(data);
							data.clear();
						}
					}
				}

				//Ожидание окончания разрезки
				if (crop_wait) {
					static vector<JTexture::BasicTexture> vec_tmp;
					if (!texture_slicing_popup(&vec_tmp, &tmp)) {
						c_anim->GetFrames(true).resize(c_anim->GetFrames().size() + vec_tmp.size());

						//Создать новые кадры из разрезанных текстур
						for (int i = 0; i < vec_tmp.size(); i++) {
							c_anim->GetFrames(true)[c_anim->GetFrames().size() - vec_tmp.size() + i].frame = vec_tmp[i];
							c_anim->GetFrames(true)[c_anim->GetFrames().size() - vec_tmp.size() + i].duration = 0.1f;
							c_anim->GetFrames(true)[c_anim->GetFrames().size() - vec_tmp.size() + i].additional_editor_id = c_anim->in_editor_ever_created;
							c_anim->in_editor_ever_created++;
						}

						crop_wait = 0;
					}
				}
			}

			was_dragged = was_dragged && core.ms.lb;
			//Закрытие
			if (ImGui::Button(u8"Закрыть")) {
				ImGui::CloseCurrentPopup();
				is_opened = 1;
			}
			ImGui::EndPopup();
		}

		//При закрытии
		if (is_opened) {
			first_time = 1;
		}

		return !is_opened;
	}

	void texture_params_popup() {
		static bool is_opened = 0;
		ImGuiStyle st = ImGui::GetStyle();

		//Инициализация
		if (call_texture_params_popup) {
			ImGui::OpenPopup(u8"Параметры текстуры##texture_params");
			call_texture_params_popup = 0;
			is_opened = 1;
		}

		if (is_opened) {
			if (ImGui::BeginPopupModal(u8"Параметры текстуры##texture_params", &is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
				gameobjs::resources* res = core.proj.get_res(texture_params_popup_is_target_local);
				JTexture::Texture* curr_txt_t = /*texture_params_popup_texture_type ? &res->prefabs[texture_params_popup_texture_id].txt :*/ &res->tilesets[texture_params_popup_texture_id].tiles[texture_params_popup_texture_id2].tile_texture;
				JTexture::BasicTexture curr_txt = curr_txt_t->GetCurrentFrame().frame;
				pos2d winsize = get_window_size();

				//Сама текстура. Нужно будет сделать рабочий предпросмотр
				ImGui::Image(ImTextureID(curr_txt.GetHandle()), ImVec2(winsize.x, winsize.x));

				//Все что связано с главным кадром
				if (ImGui::TreeNode(u8"Главный кадр")) {
					static bool main_frame_properties_open = 0;
					
					if (ImGui::Button(u8"Параметры главного кадра", ImVec2(winsize.x - st.IndentSpacing, 0)))
						ImGui::OpenPopup(u8"Параметры текстуры##raw_texture_params"), main_frame_properties_open = 1;
					//Параметры главного кадра(нуждается в переработке)
					if (main_frame_properties_open) {
						if (!raw_texture_params_popup(&curr_txt_t->base_frame.frame, &curr_txt_t->base_frame.properties))
							main_frame_properties_open = 0;
					}

					//Кнопка перемещения во временные текстуры
					if (ImGui::Button(u8"Переместить во временные текстуры")) {

						//Скопировать главный кадр во врем. текстуры
						core.proj.tmp_textures.push_back(curr_txt);

						//Если текстура - тайл
						if (texture_params_popup_texture_type == 0) {
							Tiles::tileset* c_tset = &res->tilesets[texture_params_popup_texture_id];

							//Удалить из тайлсета
							c_tset->tiles.erase(c_tset->tiles.begin() + texture_params_popup_texture_id2);

							//Заменить во всех сценах на 0
							core.proj.replace_tile_in_all_scenes({ static_cast<unsigned short>(texture_params_popup_texture_id2), static_cast<unsigned short>(texture_params_popup_texture_id + 1), texture_params_popup_is_target_local }, { 0, 0, 0 });

							//Заменить все тайлы стоявшие за ним на их новые индексы
							for (unsigned short t_id = texture_params_popup_texture_id2; t_id < c_tset->tiles.size(); t_id++)
								core.proj.replace_tile_in_all_scenes({ static_cast<unsigned short>(t_id + 1), static_cast<unsigned short>(texture_params_popup_texture_id + 1), texture_params_popup_is_target_local },
																	 { static_cast<unsigned short>(t_id),	  static_cast<unsigned short>(texture_params_popup_texture_id + 1), texture_params_popup_is_target_local });
						}

						ImGui::CloseCurrentPopup();
					}

					ImGui::TreePop();
				}

				//Анимации
				static int selected_anim = 0;
				if (ImGui::TreeNode(u8"Анимации")) {

					//Панель с кнопками воспроизведения анимации
					{
						//Кнопка запуска анимации
						{
							//Выключить кнопку, если либо нет анимаций, либо в выбранной анимации нет кадров
							if (!curr_txt_t->animations.size() || !curr_txt_t->animations[selected_anim].GetFrames().size())
								ImGui::BeginDisabled();

							//Кнопка запуска анимации
							if (ImGui::Button(u8"Запустить", ImVec2((winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 3, 0))) {
								curr_txt_t->animation_props.active_animation = selected_anim + 1, curr_txt_t->animation_props.paused = 0;
							}


							if (!curr_txt_t->animations.size() || !curr_txt_t->animations[selected_anim].GetFrames().size())
								ImGui::EndDisabled();
						}
						ImGui::SameLine();

						//Выключить кнопки, если нет запущеных анимаций. Это вынесено в отдельную переменную т.к. значение может изменится между включением/выключением кнопок, и тогда будет вылет
						bool was_active = curr_txt_t->animation_props.active_animation;
						if (!was_active)
							ImGui::BeginDisabled();

						//Кнопка паузы
						{
							//Установить другой цвет, если кнопка была нажата
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(st.Colors[ImGuiCol_Button].x, st.Colors[ImGuiCol_Button].y, st.Colors[ImGuiCol_Button].z, st.Colors[ImGuiCol_Button].w - (0.3 * curr_txt_t->animation_props.paused)));

							//Сама кнопка
							if (ImGui::Button(u8"Пауза", ImVec2((winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 3, 0)))
								curr_txt_t->animation_props.paused = !curr_txt_t->animation_props.paused;
							ImGui::PopStyleColor();
						}

						ImGui::SameLine();

						//Кнопка остановки анимации
						if (ImGui::Button(u8"Стоп")) {
							curr_txt_t->animation_props.Stop();
							curr_txt_t->animation_props.paused = 0;
						}

						if (!was_active)
							ImGui::EndDisabled();
					}

					static int rename_id = -1;
					static bool open_anim_edit_after_exit = 0;
					static int anim_editor_target = -1;

					static int anim_export_id = -1;

					//Список анимаций
					{
						ImGui::BeginChild(u8"##anim_c", ImVec2(0, 80), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

						//Если существуют анимации, то анимировать текстуру
						if (curr_txt_t->animation_props.active_animation) {
							
							core.Animate(curr_txt_t->animation_props, curr_txt_t->animations[curr_txt_t->animation_props.active_animation - 1]);
							//curr_txt_t->animation_props.was_loaded_in_current_frame = 0;
						}

						static bool was_dragged = 0;

						//Пройтись по всем анимациям
						for (unsigned short c_anim_id = 0; c_anim_id < curr_txt_t->animations.size(); c_anim_id++) {

							//Выбор анимации
							if (ImGui::Selectable(string(curr_txt_t->animations[c_anim_id].name).c_str(), (c_anim_id == selected_anim)) && !was_dragged) {
								selected_anim = c_anim_id;
							}

							//Перетаскивание анимации
							if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
								int next = c_anim_id + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
								if (next >= 0 && next < curr_txt_t->animations.size()) {
									was_dragged = 1;
									curr_txt_t->animation_props.Stop();
									swap(curr_txt_t->animation_hash[curr_txt_t->animations[c_anim_id].name], curr_txt_t->animation_hash[curr_txt_t->animations[next].name]);
									swap(curr_txt_t->animations[c_anim_id], curr_txt_t->animations[next]);
									if (selected_anim == next) selected_anim = c_anim_id;
									else if (selected_anim == c_anim_id) selected_anim = next;

									ImGui::ResetMouseDragDelta();
								}
							}

							//Контекстное меню анимации
							if (ImGui::BeginPopupContextItem()) {

								//Открытие редактора
								if (ImGui::MenuItem(u8"Открыть редактор")) {
									anim_editor_target = c_anim_id;
								}

								//Переименование
								if (ImGui::MenuItem(u8"Переименовать")) {
									rename_id = c_anim_id;
									res->tileset_hash[curr_txt_t->animations[c_anim_id].name] = 0;
								}

								//Экспорт
								if (ImGui::MenuItem(u8"Экспортировать")) {
									anim_export_id = c_anim_id;
								}

								//Удаление
								ImGui::Separator();
								if (ImGui::MenuItem(u8"Удалить")) {

									//Удалить кадры анимации
									//curr_txt_t->animations[c_anim_id].destroy();

									//Изменить индекс активной анимации
									if (((curr_txt_t->animation_props.active_animation - 1) > c_anim_id) && (curr_txt_t->animation_props.active_animation > 0))
										curr_txt_t->animation_props.active_animation--;
									else if ((curr_txt_t->animation_props.active_animation - 1) == c_anim_id) {
										curr_txt_t->animation_props.Stop();
									}

									//Изменить индекс выбранной в окне анимации
									if ((selected_anim >= c_anim_id) && (selected_anim > 0))
										selected_anim--;

									//Обнулить запись в хеш-таблице
									curr_txt_t->animation_hash[curr_txt_t->animations[c_anim_id].name] = 0;

									//Удалить саму анимацию
									curr_txt_t->animations.erase(curr_txt_t->animations.begin() + c_anim_id);

									//Сдвинуть индексы в хеш-таблице
									for (int j = c_anim_id; j < curr_txt_t->animations.size(); j++) {
										curr_txt_t->animation_hash[curr_txt_t->animations[j].name]--;
									}
								}

								ImGui::EndPopup();
							}

							was_dragged = was_dragged && core.ms.lb;
						}

						ImGui::EndChild();
					}

					//Переименование анимации
					if (rename_id >= 0) {
						if (!Popups::Rename(&curr_txt_t->animations[rename_id].name)) {

							//Хеширование имени
							hash_unique_names(&curr_txt_t->animation_hash, &curr_txt_t->animations[rename_id].name, rename_id);

							if (open_anim_edit_after_exit){
								open_anim_edit_after_exit = 0; 
								anim_editor_target = rename_id; //открыть редактор анимаций
							}

							rename_id = -1;
						}
					}

					//Редактор анимаций
					if (anim_editor_target >= 0) {
						if (!animation_editor(&curr_txt_t->animations[anim_editor_target], &curr_txt_t->animation_props, anim_editor_target)) {
							anim_editor_target = -1;
						}
					}

					//Создание анимации
					if (ImGui::Button(u8"Создать новую анимацию")) {
						curr_txt_t->animations.resize(curr_txt_t->animations.size() + 1);

						//Открыть окно переименования
						rename_id = curr_txt_t->animations.size() - 1;

						//Открыть редактор анимаций по завершении
						open_anim_edit_after_exit = 1;
					}

					//Импорт/Экспорт
					{
						//Кнопка импорта
						static bool anim_import_wait = 0;
						if (ImGui::BeginPopupContextItem()) {
							if (ImGui::MenuItem(u8"Импортировать")) {
								anim_import_wait = 1;
							}
							ImGui::EndPopup();
						}

						//Сам импорт
						if (anim_import_wait) {
							static string imported_anim;
							if (!Popups::Import(&imported_anim, { u8"Анимации", u8"*.аним" })) {

								//Если хоть что-нибудь было импортировано
								if (imported_anim.size()) {
									curr_txt_t->animations.resize(curr_txt_t->animations.size() + 1);
									curr_txt_t->animations.back().Deserialize(imported_anim);
									hash_unique_names(&curr_txt_t->animation_hash, &curr_txt_t->animations.back().name, curr_txt_t->animations.size());
								}

								anim_import_wait = 0;
							}
						}

						//Экспорт
						if (anim_export_id >= 0) {
							static string serialized_anim;
							static bool first_time = 1;
							if (first_time) {
								serialized_anim = curr_txt_t->animations[anim_export_id].Serialize();
							}

							if (!Popups::Export(serialized_anim, { u8"Анимации", u8"*.аним" }, string(curr_txt_t->animations[anim_export_id].name + u8".аним"))) {
								anim_export_id = -1;
								first_time = 1;
							}
						}
					}

					ImGui::TreePop();
				}
				
				//Пустышка с невидимым для установки ширины окна
				ImGui::Text(u8"                                                                                ");

				//Закрыть окно параметров текстуры
				if (ImGui::Button(u8"Закрыть")) {
					ImGui::CloseCurrentPopup();
					is_opened = 0;
					selected_anim = 0;
				}
				ImGui::EndPopup();
			}
		}
	}
	
	bool texture_slicing_popup(vector<JTexture::BasicTexture>* target, JTexture::BasicTexture* tmp) {
		pos2dus texture_size = tmp->GetSize();
		static fb_widget slice_view;
		static cam2d texture_view_cam;

		//Список областей разрезки
		static vector<texture_processing::cropper> slice_list;

		//Индекс выбранной области разрезки
		static int selected_slicer = -1;

		//Параметры автогенерации разрезочного списка
		static int number_of_elements_to_generate = 0;
		static pos2dus elem_size = { 16, 16 };


		//Начальная инициализация
		{
			static bool init_flag = 1;
			if (init_flag) {
				texture_view_cam.fb.CreateFramebuffer({ texture_size.x, texture_size.y });
				slice_view.fb = &texture_view_cam.fb;
				init_flag = 0;
			}
		}

		//Инициализация
		static bool first_time = 1;
		if (first_time) {
			ImGui::OpenPopup(u8"Действия с текстурой");

			//Изменить размер экранного буфера под размеры текстуры
			if ((texture_view_cam.fb.fb_size.x != texture_size.x) || (texture_view_cam.fb.fb_size.y != texture_size.y)) {
				texture_view_cam.fb.resize({texture_size.x, texture_size.y});
			}

			first_time = 0;
		}

		//Главное окно
		bool is_opened = 1;
		if (ImGui::BeginPopupModal(u8"Действия с текстурой", &is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
			is_opened = 0;

			ImGuiStyle st = ImGui::GetStyle();

			static bool was_delete_hovered = 0;

			//Отрисовка в экранный буфер и действия с мышью
			{
				//Подготовка к отрисовке
				slice_view.start({ core.ms.x, core.ms.y });
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDisable(GL_DEPTH_TEST);
				texture_view_cam.fb.clear_use();

				//Установка координат камеры для шейдеров
				core.setvecs(&texture_view_cam, &core.flat_sh);
				core.setvecs(&texture_view_cam, &core.txt_sh);

				//Нарисовать текстуру в экранный буфер
				core.drawtexture_s_f(tmp, { -(static_cast<float>(texture_size.x) / 2.0f), -(static_cast<float>(texture_size.y) / 2.0f) });

				//Позиция мыши. Если она не наведена на окно, то -1, -1, иначе какое-либо другое число. Есть локальная, старая и абсольтная. Локальная - координаты окна, старая - координаты окна с прошлой итерации, абсолютная - координаты в области текстуры
				static pos2d converted_mousepos = { -1, -1 }, old_mousepos = converted_mousepos, abs_mousepos;

				//Позиция камеры в координатах текстуры
				pos2d converted_camerapos = { texture_size.x / 2.0 + texture_view_cam.center.x,
											  texture_size.y / 2.0 + texture_view_cam.center.y };

				//Координаты верхнего левого угла видимой области
				pos2d upper_left_corner = { converted_camerapos.x - (texture_size.x / 2.0) / texture_view_cam.zoom,
											converted_camerapos.y - (texture_size.y / 2.0) / texture_view_cam.zoom };

				//Координаты нижнего правого угла видимой области
				pos2d lower_right_corner = { converted_camerapos.x + (texture_size.x / 2.0) / texture_view_cam.zoom,
											 converted_camerapos.y + (texture_size.y / 2.0) / texture_view_cam.zoom };

				//Расчет позиции мыши
				converted_mousepos = { -1, -1 };
				if (slice_view.winsize.x && slice_view.is_hovered) {
					converted_mousepos = { static_cast<int>((slice_view.mspos.x / (static_cast<float>(slice_view.winsize.x) / static_cast<float>(texture_size.x))) / texture_view_cam.zoom),
										   static_cast<int>((slice_view.mspos.y / (static_cast<float>(slice_view.winsize.x) / static_cast<float>(texture_size.x))) / texture_view_cam.zoom) };

					abs_mousepos = { converted_mousepos.x + upper_left_corner.x ,
									 converted_mousepos.y + upper_left_corner.y };
				}

				//cout << "conv x: " << converted_mousepos.x << " conv y: " << converted_mousepos.y << " abs x: " << abs_mousepos.x << " abs y: " << abs_mousepos.y << "\n";
				//cout << "cam x: " << converted_camerapos.x << " cam y: " << converted_camerapos.y << "\n";

				//Дополнительная информация о действиях с выделенной областью
				static bool is_holding_item = 0;
				static bool is_resizing = 0;
				static bool resize_axis = 0;
				static bool resize_closer_further = 0; //имеется ввиду сторона которая ближе или дальше от нулей

				//Информация о том, были ли кнопки мыши нажаты один раз
				pos2d once_buttons = core.ms.once_ret();

				//Проход по всем областям выделения
				for (int i = 0; i < slice_list.size(); i++) {

					// Ближе/дальше для осей X/Y (Тут необходимы отдельные переменные, иначе не работает)
					bool further_x = 0, further_y = 0;

					//Ось (X/Y)
					bool axis = 0;

					//Если было нажатие в середине области, не касаясь ребер
					if (in_range(slice_list[i].pos.x, abs_mousepos.x, (slice_list[i].pos.x + slice_list[i].size.x) - 1) && in_range(slice_list[i].pos.y, abs_mousepos.y, (slice_list[i].pos.y + slice_list[i].size.y) - 1)) {
						if (once_buttons.x)
							selected_slicer = i, is_holding_item = 1;
					}

					//Если было нажатие на ребра
					else if ((axis = ((abs_mousepos.x == slice_list[i].pos.x || (further_x = (abs_mousepos.x == (slice_list[i].pos.x + slice_list[i].size.x) - 1))) && in_range(slice_list[i].pos.y, abs_mousepos.y, (slice_list[i].pos.y + slice_list[i].size.y) - 1))) !=
						((abs_mousepos.y == slice_list[i].pos.y || (further_y = (abs_mousepos.y == (slice_list[i].pos.y + slice_list[i].size.y) - 1))) && in_range(slice_list[i].pos.x, abs_mousepos.x, (slice_list[i].pos.x + slice_list[i].size.x) - 1))) {
						if (once_buttons.x) {
							selected_slicer = i, is_resizing = 1, is_holding_item = 0, resize_axis = axis, resize_closer_further = axis ? !further_x : !further_y;
						}

					}

					if (once_buttons.x && (!is_holding_item && !is_resizing) && !was_delete_hovered) selected_slicer = -1;

					//Если не выделено то белый, если выделено, то желтый
					rgb rectcol = { 255, 255, (i != selected_slicer) * 255 };

					//Отрисовка областей разрезки
					core.drawrect_f({ static_cast<float>(slice_list[i].pos.x) - static_cast<float>(texture_size.x) / 2.0f, (static_cast<float>(slice_list[i].pos.y) - static_cast<float>(texture_size.y) / 2.0f) },
									{ static_cast<float>(slice_list[i].pos.x + slice_list[i].size.x) - static_cast<float>(texture_size.x) / 2.0f, (static_cast<float>(slice_list[i].pos.y + slice_list[i].size.y) - static_cast<float>(texture_size.y) / 2.0f) }, rectcol);
				}

				//Изменение позиции мыши за кадр
				pos2d deltapos = { converted_mousepos.x >= 0 ? (old_mousepos.x - converted_mousepos.x) : 0, converted_mousepos.y >= 0 ? (old_mousepos.y - converted_mousepos.y) : 0};

				//cout << "deltapos x: " << deltapos.x << " deltapos y: " << deltapos.y << "\n";

				//Если в границах окна
				if (in_range(-1, abs_mousepos.x, texture_size.x) && in_range(-1, abs_mousepos.y, texture_size.y) && slice_view.is_hovered) {

					//Перемещение областей выделения. Возможно только, если будущая позиция будет такой, что границы области не выходят за границы текстуры
					if (core.ms.lb && !core.ms.rb && is_holding_item && ((slice_list[selected_slicer].pos.x - deltapos.x) > -1) &&
						((slice_list[selected_slicer].pos.x + slice_list[selected_slicer].size.x - deltapos.x) <= texture_size.x) &&
						((slice_list[selected_slicer].pos.y - deltapos.y) > -1) && ((slice_list[selected_slicer].pos.y + slice_list[selected_slicer].size.y - deltapos.y) <= texture_size.y)) {
						slice_list[selected_slicer].pos.x -= deltapos.x;
						slice_list[selected_slicer].pos.y -= deltapos.y;
					}

					//Изменение размера области выделения
					if (core.ms.lb && is_resizing && !core.ms.rb) {

						//Тут выбираются все данные, с которыми будет работа
						unsigned short* size_axis = resize_axis ? &slice_list[selected_slicer].size.x : &slice_list[selected_slicer].size.y;
						unsigned short mouse_axis = resize_axis ? abs_mousepos.x : abs_mousepos.y;
						unsigned short txtsize_axis = resize_axis ? texture_size.x : texture_size.y;
						unsigned short* cropperpos_axis = resize_axis ? &slice_list[selected_slicer].pos.x : &slice_list[selected_slicer].pos.y;

						//Если сторона выделения - блиняя от начала текущей оси координат
						if (resize_closer_further && in_range(-1, mouse_axis, *cropperpos_axis + *size_axis) && ((*size_axis + *cropperpos_axis - mouse_axis) >= 2)) {
							*size_axis = *size_axis + *cropperpos_axis - mouse_axis;
							*cropperpos_axis = mouse_axis;
						}

						//Если сторона выделения - дальняя от начала текущей оси координат
						else if (!resize_closer_further && in_range(0, mouse_axis - *cropperpos_axis, txtsize_axis))
							*size_axis = mouse_axis - *cropperpos_axis + 1;
					}

					//действия при изменении маштаба камеры
					if (core.ms.wheel && (texture_view_cam.zoom - (core.ms.wheel * 0.1f) >= 0.99999f)) {

						//Предотвращение выхода камеры за границы области текстуры. Выход возможен только при увеличении, соответственно нужно проверять только его
						if (core.ms.wheel >= 0.0f) {

							//Положение границы области видимости от начала координат
							float dist;

							if ((dist = (converted_camerapos.x - (texture_size.x / 2.0) / (texture_view_cam.zoom - (core.ms.wheel * 0.1f)))) < 0.0f)
								texture_view_cam.center.x -= dist;
							else if ((dist = (converted_camerapos.x + (texture_size.x / 2.0) / (texture_view_cam.zoom - (core.ms.wheel * 0.1f)))) >= texture_size.x)
								texture_view_cam.center.x += (texture_size.x - dist);

							if ((dist = (converted_camerapos.y - (texture_size.y / 2.0) / (texture_view_cam.zoom - (core.ms.wheel * 0.1f)))) < 0.0f)
								texture_view_cam.center.y -= dist;
							else if ((dist = (converted_camerapos.y + (texture_size.y / 2.0) / (texture_view_cam.zoom - (core.ms.wheel * 0.1f)))) >= texture_size.y)
								texture_view_cam.center.y += (texture_size.y - dist);
						}
						texture_view_cam.zoom -= core.ms.wheel * 0.1;
					}

					//Если была отпущена левая кнопка мыши или была нажата правая(перемещение области видимости)
					if (!core.ms.lb || core.ms.rb) {
						is_holding_item = 0;
						is_resizing = 0;
					}

					//Если была нажата правая кнопка мыши и курсор нахдится в области окна
					if (core.ms.rb && (upper_left_corner.x + deltapos.x >= 0) && (upper_left_corner.y + deltapos.y >= 0)
						&& (lower_right_corner.x + deltapos.x < texture_size.x) && (lower_right_corner.y + deltapos.y < texture_size.y)) {
						texture_view_cam.center.x += deltapos.x, texture_view_cam.center.y += deltapos.y;
					}
				}

				//Если не в границах окна
				else {
					is_holding_item = 0;
					is_resizing = 0;
				}

				old_mousepos = converted_mousepos;

				//ImGui::Text(to_string(deltapos.x).c_str());
				//ImGui::SameLine();
				//ImGui::Text(to_string(deltapos.y).c_str());
				texture_view_cam.fb.end_render(core.window->window_size);
			}

			//Интерфейс
			{
				ImGui::Image(ImTextureID(slice_view.fb->texture_id), ImVec2(slice_view.winsize.x, static_cast<float>(slice_view.winsize.x)* (static_cast<float>(texture_size.y) / static_cast<float>(texture_size.x))), ImVec2(0, 1), ImVec2(1, 0));
				slice_view.is_hovered = ImGui::IsItemHovered();

				

				if (ImGui::TreeNode(u8"Разрезка")) {
					ImGui::DragInt("##number_of_elements", &number_of_elements_to_generate, 0.1f, 0, 65535, u8"Число элементов: %d");
					ImGui::Text(u8"Размер элемента:");
					ImGui::PushItemWidth((slice_view.winsize.x - st.ItemSpacing.x - st.IndentSpacing) / 2);

					pos2d elem_size_adapter = { elem_size.x, elem_size.y };

					ImGui::DragInt("##elemsize_x", &elem_size_adapter.x, 0.1f, 0, 0, "x: %d");
					ImGui::SameLine();
					ImGui::DragInt("##elemsize_y", &elem_size_adapter.y, 0.1f, 0, 0, "y: %d");
					ImGui::PopItemWidth();
					elem_size = { static_cast<unsigned short>(elem_size_adapter.x), static_cast<unsigned short>(elem_size_adapter.y) };

					//Создать новый элемент. Всегда появляется в центре, функционал нуждается в доработке
					if (ImGui::Button(u8"Новый элемент", ImVec2((slice_view.winsize.x - st.ItemSpacing.x - st.IndentSpacing) / 2, 0.0f))) {
						if ((((texture_size.x - elem_size.x) / 2) >= 0 && ((texture_size.x + elem_size.x) / 2) <= texture_size.x) && (((texture_size.y - elem_size.y) / 2) >= 0 && ((texture_size.y + elem_size.y) / 2) <= texture_size.y))
							slice_list.push_back({ { static_cast<unsigned short>((texture_size.x - elem_size.x) / 2), static_cast<unsigned short>((texture_size.y - elem_size.y) / 2) }, elem_size }), selected_slicer = slice_list.size() - 1;
					}
					ImGui::SameLine();

					//Удаление выбранного элемента
					{
						//Выключить кнопку, если не выбрана область разрезки
						bool was_disabled = selected_slicer == -1;
						if (was_disabled)
							ImGui::BeginDisabled();

						//Сама кнопка
						if (ImGui::Button(u8"Удалить элемент", ImVec2((slice_view.winsize.x - st.ItemSpacing.x - st.IndentSpacing) / 2, 0.0f))) {
							slice_list.erase(slice_list.begin() + selected_slicer);
							selected_slicer = -1;
						}

						//Нужно для того, чтобы индекс выбранной области выделения не сбрасывался до нажатия кнопки
						was_delete_hovered = ImGui::IsItemHovered();

						if (was_disabled)
							ImGui::EndDisabled();
					}
					
					ImGui::NewLine();

					//Сгенерировать элементы областей разрезки. Заменяет предыдущий список
					if (ImGui::Button(u8"Сгенерировать список разрезки", ImVec2(slice_view.winsize.x - st.IndentSpacing, 0)))
						slice_list = texture_processing::generate_croplist(tmp, number_of_elements_to_generate, elem_size);

					ImGui::TreePop();
				}
				else was_delete_hovered = 0;

				//Пустышка с невидимым символом на конце, нужна чтобы выставить ширину окна
				ImGui::Text(u8"                                                                                           ");

				//Кнопка "Подтвердить"
				if (ImGui::Button(u8"Подтвердить", ImVec2((slice_view.winsize.x - st.ItemSpacing.x) / 2, 0))) {

					//Если нет областей разрезки
					if (!slice_list.size()) {
						target->resize(1);
						target->operator[](0) = *tmp;
					}

					//Если эти области есть
					else {
						vector<string> cropped_data = texture_processing::crop_from_croplist(tmp->GetBinary(), slice_list, texture_size);
						target->resize(slice_list.size());

						//Создание текстур из данных разрезки
						for (int id = 0; id < cropped_data.size(); id++) {
							target->operator[](id).LoadRaw(cropped_data[id], slice_list[id].size);
						}
					}

					is_opened = 1;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();

				//Кнопка "Отмена"
				if (ImGui::Button(u8"Отмена", ImVec2((slice_view.winsize.x - st.ItemSpacing.x) / 2, 0))) {
					is_opened = 1;
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}

		//При закрытии
		if (is_opened) {
			first_time = 1;

			//Возврат к параметрам по умолчанию
			slice_list.clear();
			number_of_elements_to_generate = 0;
			elem_size = { 16,16 };
			texture_view_cam.center = { 0,0 };
			texture_view_cam.zoom = 1;
			selected_slicer = -1;
		}

		return !is_opened;
	}

	bool script_editor(std::vector<std::string> &script) {
		//Инициализация
		static bool first_time = 1;
		if (first_time) {
			ImGui::OpenPopup(u8"Редактор скриптов");
			first_time = 0;
		}

		//Главное окно
		bool is_opened = 1;
		if (ImGui::BeginPopupModal(u8"Редактор скриптов", &is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
			is_opened = 0;
			for (int line_id = 0; line_id < script.size(); line_id++) {
				ImGui::InputText(std::string("##line" + std::to_string(line_id)).c_str(), &script[line_id]);
				if (ImGui::BeginPopupContextItem()) {
					if (ImGui::MenuItem(u8"Удалить"))
						script.erase(script.begin() + line_id);

					ImGui::EndPopup();
				}
			}
			if (ImGui::Button(u8"Новая строка"))
				script.push_back("");

			if (ImGui::Button(u8"Закрыть")) {
				ImGui::CloseCurrentPopup();
				is_opened = 1;
			}
			ImGui::EndPopup();
		}

		if (is_opened) {
			first_time = 1;
		}

		return !is_opened;
	}
	/*
	bool var_editor(std::vector<programming_lang::var_declaration>& vartypes, std::unordered_map<std::string, unsigned short>& var_hash, std::vector<gameobjs::ObjectSystem::ObjectPointer>& objs) {
		//Инициализация
		static bool first_time = 1;
		if (first_time) {
			ImGui::OpenPopup(u8"Редактор переменных                     ");
			first_time = 0;
		}

		//Главное окно
		bool is_opened = 1;
		if (ImGui::BeginPopupModal(u8"Редактор переменных                     ", &is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
			is_opened = 0;
			static int rename_id = -1;
			static int params_id = -1;
			//ImGui::Text(u8"Редактировать переменные:                     ");
			static bool was_dragged = 0;

			for (int var_id = 0; var_id < vartypes.size(); var_id++) {
				ImGui::Separator();
				ImGui::Selectable(string(vartypes[var_id].name + u8"##vardecl").c_str(), false, ImGuiSelectableFlags_DontClosePopups);

				if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
					int next = var_id + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
					if (next >= 0 && next < core.proj.scenes.size()) {
						was_dragged = 1;
						for (gameobjs::ObjectSystem::ObjectPointer& i : objs)
							swap(i.GetObj()->vars[var_id], i.GetObj()->vars[next]);
						swap(vartypes[var_id], vartypes[next]);
						swap(var_hash[vartypes[var_id].name], var_hash[vartypes[next].name]);
						ImGui::ResetMouseDragDelta();
					}
				}

				if (ImGui::BeginPopupContextItem()) {
					if (ImGui::MenuItem(u8"Переименовать")) {
						rename_id = var_id;
						var_hash[vartypes[var_id].name] = 0;
					}

					if (ImGui::MenuItem(u8"Параметры")) {
						params_id = var_id;
					}

					ImGui::Separator();
					if (ImGui::MenuItem(u8"Удалить")) {
						var_hash[vartypes[var_id].name] = 0;
						vartypes.erase(vartypes.begin() + var_id);
						for (gameobjs::ObjectSystem::ObjectPointer& i : objs)
							i.GetObj()->vars.erase(i.GetObj()->vars.begin() + var_id);
						for (int i = var_id; i < vartypes.size(); ++i)
							var_hash[vartypes[i].name] = i;
					}

					ImGui::EndPopup();
				}
			}
			if (ImGui::Button(u8"Добавить")) {
				vartypes.resize(vartypes.size() + 1);
				vartypes.back().name = u8"Без имени";
				for (gameobjs::ObjectSystem::ObjectPointer& i : objs)
					i.GetObj()->vars.resize(i.GetObj()->vars.size() + 1);
				rename_id = vartypes.size() - 1;
			}

			if (rename_id >= 0) {
				if (!Popups::Rename(&vartypes[rename_id].name)) {
					hash_unique_names(&var_hash, &vartypes[rename_id].name, rename_id);
					rename_id = -1;
				}
			}

			if (params_id >= 0) {
				static bool p_first_time = 1;
				static programming_lang::var_declaration c_var;
				if (p_first_time) {
					ImGui::OpenPopup(u8"Параметры переменной                             ");
					p_first_time = 0;
					c_var = vartypes[params_id];
				}

				bool is_win_opened = 1;
				if (ImGui::BeginPopupModal(u8"Параметры переменной                             ", &is_win_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
					is_win_opened = 0;
					const char* text_types[] = { u8"Целое", u8"Координаты", u8"Слово"};

					ImGui::SliderInt("##vartype", reinterpret_cast<int*>(&c_var.type), 0, 2, text_types[c_var.type]);

					if (ImGui::Button(u8"Ок")) {
						vartypes[params_id] = c_var;
						for (gameobjs::ObjectSystem::ObjectPointer& i : objs)
							i.GetObj()->vars[params_id].type = c_var.type;
						is_win_opened = 1;
					}
					ImGui::EndPopup();
				}

				if (is_win_opened) {
					p_first_time = 1;
					params_id = -1;
				}

			}

			if (ImGui::Button(u8"Закрыть")) {
				ImGui::CloseCurrentPopup();
				is_opened = 1;
			}
			ImGui::EndPopup();
		}

		if (is_opened) {
			first_time = 1;
		}

		return !is_opened;
	}*/
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void project_editor() {
		ImGui::Begin(u8"Выбор сцены", NULL, ImGuiWindowFlags_NoCollapse);
		pos2d proj_winsize = get_window_size();

		//Параметры проекта
		{
			if (ImGui::TreeNode(u8"Параметры проекта: ")) {
				{
					ImGui::Text(string(u8"Имя: " + string({ char(34) }) + core.proj.name + char(34)).c_str());
					static bool project_rename_wait = 0;
					if (ImGui::Button(u8"Переименовать##tileset_rename"))
						project_rename_wait = 1;
					if (project_rename_wait)
						project_rename_wait = Popups::Rename(&core.proj.name);
				}
				ImGui::Checkbox(u8"Полноэкранный режим", &core.proj.fullscreen);

				ImGui::SeparatorText(u8"Высота экрана");
				int scr_height_adapter = static_cast<int>(core.proj.screen_height);
				ImGui::DragInt(u8"##scr_height", &scr_height_adapter, 0.1, 1, 5000);
				core.proj.screen_height = scr_height_adapter;

				ImGui::SeparatorText(u8"Иконка: ");

				if (core.proj.icon.GetSize().x) {
					ImGui::Image(ImTextureID(core.proj.icon.GetHandle()), ImVec2(64, 64));
					if (ImGui::Button(u8"Удалить")) {
						
					}
				}
				else {
					static bool load_wait = 0;

					if (ImGui::Button(u8"Загрузить")) {
						load_wait = 1;
					}
					ImGui::Text(u8"Примечание:\nиконка ДОЛЖНА быть \n32 на 32 пикселя по размерам");

					if (load_wait) {
						static string loaded_ic;

						if (!Popups::Import(&loaded_ic, { u8"Изображения", "*.png", "*.jpg", "*.bmp" })) {
							if (loaded_ic.size()) {
								core.proj.icon.LoadCompressed(loaded_ic);

								if(core.proj.icon.GetSize().x != 32 || core.proj.icon.GetSize().y != 32) {
									Popups::Error("ОШИБКА: Иконка должна быть 32 на 32 пикселя!"); 
									core.proj.icon.GetSize() = {0, 0};
								}
							}
							load_wait = 0;
						}
					}
				}

				ImGui::TreePop();
			}
		}

		//Сцены
		{
			ImGui::Text(u8"Сцены: ");
			ImGui::Separator();
			static int rename_id = -1;

			static bool was_dragged = 0;
			//Пройтись по всем сценам
			for (int scene_id = 0; scene_id < core.proj.scenes.size(); scene_id++) {
				if (ImGui::Selectable(core.proj.scenes[scene_id].name.c_str(), (scene_id == core.proj.c_scene_id)) && !was_dragged) {
					core.proj.c_scene_id = scene_id;

					if(selected_tile.is_local && (scene_id != selected_tile.tset_value))
						selected_tile = { 0, 0, 0 };

					selected_layer = 0;
					selected_prefab = 0;
				}
				if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
					int next = scene_id + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
					if (next >= 0 && next < core.proj.scenes.size()) {
						was_dragged = 1;
						swap(core.proj.scene_hash[core.proj.scenes[scene_id].name], core.proj.scene_hash[core.proj.scenes[next].name]);
						swap(core.proj.scenes[scene_id], core.proj.scenes[next]);
						if (core.proj.c_scene_id == scene_id)
							core.proj.c_scene_id = next;
						else if (core.proj.c_scene_id == next)
							core.proj.c_scene_id = scene_id;
						ImGui::ResetMouseDragDelta();
					}
				}
				if (ImGui::BeginPopupContextItem()) {

					if (ImGui::MenuItem(u8"Переименовать")) {
						rename_id = scene_id;
						core.proj.scene_hash[core.proj.scenes[scene_id].name] = 0;
					}

					ImGui::Separator();
					if (ImGui::MenuItem(u8"Удалить")) {
						core.proj.scene_hash[core.proj.scenes[scene_id].name] = 0;
						core.proj.scenes.erase(core.proj.scenes.begin() + scene_id);
						if (core.proj.c_scene_id >= scene_id && core.proj.c_scene_id != 0) {
							core.proj.c_scene_id--;
						}
					}
					

					ImGui::EndPopup();
				}
			}
			was_dragged = was_dragged && core.ms.lb;

			if (ImGui::Button(u8"Создать сцену")) {
				core.proj.scenes.resize(core.proj.scenes.size() + 1);
				rename_id = core.proj.scenes.size() - 1;
			}

			//Переименование сцены
			if (rename_id >= 0) {
				if (!Popups::Rename(&core.proj.scenes[rename_id].name)) {
					hash_unique_names(&core.proj.scene_hash, &core.proj.scenes[rename_id].name, rename_id);
					rename_id = -1;
				}
			}
		}

		ImGui::End();


		ImGui::Begin(u8"Редактор проекта", NULL, ImGuiWindowFlags_NoCollapse);
		{
			if (core.proj.scenes.size()) {
				ImGui::Text(u8"Комнаты: ");
				ImGui::Separator();
				gameobjs::Scene* c_scene = core.proj.get_curr_scene();
				static int rename_id = -1;
				static bool was_dragged = 0;
				for (int room_id = 0; room_id < c_scene->rooms.size(); room_id++) {
					if (ImGui::Selectable(string(c_scene->rooms[room_id].name + u8"##room").c_str(), room_id == c_scene->c_room_id) && !was_dragged) {
						c_scene->c_room_id = room_id;
						selected_layer = 0;
					}

					if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
						int next = room_id + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
						if (next >= 0 && next < core.proj.scenes.size()) {
							was_dragged = 1;
							swap(c_scene->room_hash[c_scene->rooms[room_id].name], c_scene->room_hash[c_scene->rooms[next].name]);
							swap(c_scene->rooms[room_id], c_scene->rooms[next]);
							if (c_scene->c_room_id == room_id)
								core.proj.c_scene_id = next;
							else if (c_scene->c_room_id == next)
								core.proj.c_scene_id = room_id;
							ImGui::ResetMouseDragDelta();
						}
					}
					if (ImGui::BeginPopupContextItem()) {
						if (ImGui::MenuItem(u8"Открыть в редакторе")) {
							c_scene->c_room_id = room_id;
							project_editor_opened = 0;
						}

						if (ImGui::MenuItem(u8"Переименовать")) {
							rename_id = room_id;
							c_scene->room_hash[c_scene->rooms[room_id].name] = 0;
						}

						ImGui::Separator();
						if (ImGui::MenuItem(u8"Удалить")) {
							c_scene->room_hash[c_scene->rooms[room_id].name] = 0;
							c_scene->rooms.erase(c_scene->rooms.begin() + room_id);
							if (c_scene->c_room_id >= room_id && c_scene->c_room_id != 0) {
								c_scene->c_room_id--;
							}
						}


						ImGui::EndPopup();
					}
				}
				was_dragged = was_dragged && core.ms.lb;

				if (ImGui::Button(u8"Создать комнату")) {
					c_scene->rooms.resize(c_scene->rooms.size() + 1);
					rename_id = c_scene->rooms.size() - 1;
				}

				if (rename_id >= 0) {
					if (!Popups::Rename(&c_scene->rooms[rename_id].name)) {
						hash_unique_names(&c_scene->room_hash, &c_scene->rooms[rename_id].name, static_cast<unsigned short>(rename_id));
						rename_id = -1;
					}
				}
			}
		}
		ImGui::End();
	}

	void main_menu_window() {
		ImGui::Begin(u8"Добро пожаловать!");
		ImGuiStyle st = ImGui::GetStyle();
		pos2d main_menu_winsize = get_window_size();
		ImGui::SetCursorPosX((main_menu_winsize.x / 2.0f) - (main_menu_winsize.y / 3.0f));
		ImGui::Image(ImTextureID(icon.GetHandle()), ImVec2(main_menu_winsize.y / 3.0f * 2.0f, (main_menu_winsize.y / 3.0f * 2.0f)));

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();


		static bool new_proj_creation_wait = 0;
		static bool open_proj_wait = 0;

		ImGui::SetCursorPosX(((main_menu_winsize.x) / 12.0f));
		if (ImGui::Button(u8"Создать новый проект", ImVec2((main_menu_winsize.x - st.ItemSpacing.x) / 3.0f, (main_menu_winsize.y / 3.0f - st.ItemSpacing.y) / 2.0f))) {
			new_proj_creation_wait = 1;
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX((main_menu_winsize.x + st.ItemSpacing.x) / 2.0f + ((main_menu_winsize.x - st.ItemSpacing.x) / 12.0f));
		if (ImGui::Button(u8"Открыть существующий", ImVec2((main_menu_winsize.x - st.ItemSpacing.x) / 3.0f, (main_menu_winsize.y / 3.0f - st.ItemSpacing.y)/ 2.0f))) {
			open_proj_wait = 1;
		}

		if (new_proj_creation_wait) {
			if (!Popups::Export(" ", { u8"Файлы проекта", u8"*.фпю" }, u8"Без имени.фпю", &project_save_path)) {
				new_proj_creation_wait = 0;
				//core.proj.destroy();
				if (project_save_path.size()) {
					main_menu = 0;
					project_editor_opened = 1;
					selected_tile = { 0,0,0 };
					selected_prefab = 0;
					selected_layer = 0;
				}
			}
		}

		if (open_proj_wait) {
			static string load_data;
			if (!Popups::Import(&load_data, { u8"Файлы проекта", u8"*.фпю" }, &project_save_path)) {
				if (load_data.size()) {
					//core.proj.destroy();
					core.proj.Deserialize(load_data);
					main_menu = 0;
					project_editor_opened = 1;
				}
				open_proj_wait = 0;
			}
		}

		ImGui::End();
	}

	void resource_manager() {
		ImGui::Begin(u8"Менеджер ресурсов", NULL, ImGuiWindowFlags_NoCollapse);
		ImGuiStyle st = ImGui::GetStyle();
		pos2d winsize = get_window_size();

		static bool is_local = 0;
		//Режим (локальность)
		{
			ImGui::Text(u8"Режим:");
			int is_local_adapter = is_local;
			ImGui::PushItemWidth(winsize.x);
			ImGui::SliderInt("##localresslider", &is_local_adapter, 0, 1, string(is_local ? u8"Локальный" : u8"Глобальный").c_str());
			//if (enable_tooltips) {
			//	if (ImGui::BeginItemTooltip())
			//		ImGui::Text(u8"Имеется ввиду режим хранения ресурсов.\nЛокальные хранятся в текущей сцене,\nа глобальные в проекте. Именно поэтому\nлокальные ресурсы не получится использовать\nвне сцены, где они были определены.\n\nНе распространяется на временные текстуры."),
			//		ImGui::EndTooltip();
			//}

			ImGui::PopItemWidth();
			is_local = is_local_adapter;
		}

		gameobjs::resources* res = core.proj.get_res(is_local);

		ImGui::BeginTabBar(u8"Виды ресурсов");

		//Вкладка с тайлсетами
		if (ImGui::BeginTabItem(u8"Тайлсеты", NULL, ImGuiTabItemFlags_NoCloseButton)) {
			

			//Эта переменная нужна для того, чтобы при отпускании кнопки мыши перемещаемый элемент не нажимался
			static bool was_dragged = 0; 

			//Индекс обьекта для переименования.
			static int rename_id = -1; 

			//Проход по всем тайлсетам
			for (unsigned short c_tset_id = 0; c_tset_id < res->tilesets.size(); c_tset_id++) {
				Tiles::tileset* c_tileset = &res->tilesets[c_tset_id];

				//Выбор тайлсета
				if (ImGui::Selectable(string(c_tileset->name + u8"##tset").c_str(), (c_tset_id == selected_tile.tset_value) && (is_local == selected_tile.is_local)) && !was_dragged) {
					selected_tile.tset_value = c_tset_id, selected_tile.is_local = is_local; 
					selected_tile.tile_value = 0;
					mode = 0;
				}

				//Перестановка тайлсетов
				if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
					int next = c_tset_id + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
					if (next >= 0 && next < res->tilesets.size()) {
						was_dragged = 1;
						for (unsigned short t_id = 0; t_id < res->tilesets[c_tset_id].tiles.size(); t_id++) {
							core.proj.replace_tile_in_all_scenes({t_id, static_cast<unsigned short>(c_tset_id + 1), is_local}, {t_id, 65535, is_local});
						}
						for (unsigned short t_id = 0; t_id < res->tilesets[next].tiles.size(); t_id++) {
							core.proj.replace_tile_in_all_scenes({ t_id, static_cast<unsigned short>(c_tset_id + next + 1), is_local }, { t_id, static_cast<unsigned short>(c_tset_id + 1), is_local });
						}
						for (unsigned short t_id = 0; t_id < res->tilesets[c_tset_id].tiles.size(); t_id++) {
							core.proj.replace_tile_in_all_scenes({ t_id, 65535, is_local }, { t_id, static_cast<unsigned short>(next + 1), is_local });
						}

						swap(res->tileset_hash[res->tilesets[c_tset_id].name], res->tileset_hash[res->tilesets[next].name]);
						swap(res->tilesets[c_tset_id], res->tilesets[next]);

						if (selected_tile.is_local == is_local) {
							if (selected_tile.tset_value == next) selected_tile.tset_value = c_tset_id;
							else if (selected_tile.tset_value == c_tset_id) selected_tile.tset_value = next;
						}
						ImGui::ResetMouseDragDelta();
					}
				}

				//Перенос тайла в тайлсет
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Tile DND")) {
						int payload_n = *(int*)payload->Data;
						c_tileset->tiles.push_back(core.proj.get_res(selected_tile.is_local)->tilesets[selected_tile.tset_value].tiles[payload_n]);
						core.proj.get_res(selected_tile.is_local)->tilesets[selected_tile.tset_value].tiles.erase(core.proj.get_res(selected_tile.is_local)->tilesets[selected_tile.tset_value].tiles.begin() + payload_n);
						core.proj.swap_replace_tile_in_all_scenes({ static_cast<unsigned short>(payload_n), static_cast<unsigned short>(selected_tile.tset_value + 1), selected_tile.is_local }, { static_cast<unsigned short>(c_tileset->tiles.size() - 1), static_cast<unsigned short>(c_tset_id + 1), is_local });
					}
					ImGui::EndDragDropTarget();
				}		

				//Меню тайлсета
				if (ImGui::BeginPopupContextItem()) {
					ImGui::Text(string(u8"Количество тайлов: " + to_string(c_tileset->tiles.size())).c_str());
					ImGui::Separator();

					//Переименование
					if (ImGui::MenuItem(u8"Переименовать")) {
						rename_id = c_tset_id;

						//Отвязка от хеша
						res->tileset_hash[c_tileset->name] = 0;
					}

					//Сделать глобальным/локальным
					if (ImGui::MenuItem(string(u8"Сделать " + string(is_local ? u8"глобальным" : u8"локальным")).c_str())) {
						gameobjs::resources* new_res = core.proj.get_res(!is_local);

						//Замена тайлов тайлсета во всех слоях всех сцен
						for (unsigned short i = 0; i < c_tileset->tiles.size(); i++) {
							core.proj.replace_tile_in_all_scenes({ i, static_cast<unsigned short>(c_tset_id + 1), is_local }, { i, static_cast<unsigned short>(core.proj.get_res(!is_local)->tilesets.size() + 1), !is_local });

							//Обнулить тайлы в других сценах, если тайлсет делается локальным
							if (!is_local) {
								core.proj.replace_tile_in_all_scenes({ i, static_cast<unsigned short>(c_tset_id + 1), is_local }, { 0, 0, 0 });
							}
						}

						//Копирование в новое место
						new_res->tilesets.push_back(*c_tileset); 

						//Удаление старого
						res->tilesets.erase(res->tilesets.begin() + c_tset_id); 

						//Если тайлсет выбранного тайла находится в той же области расположения, что и перемещенный оттуда тайлсет. Тогда нужно уменьшить индекс тайлсета выбранного тайла если он стоит за перемещенным тайлсетом 
						if ((selected_tile.tset_value == c_tset_id) && (selected_tile.is_local == is_local)) {
							selected_tile.tset_value = new_res->tilesets.size() - 1;
							selected_tile.is_local = !selected_tile.is_local;
						}

						//Хеширование имени
						res->tileset_hash[new_res->tilesets.back().name] = 0;
						hash_unique_names(&new_res->tileset_hash, &new_res->tilesets.back().name, new_res->tilesets.size() - 1);
						
						ImGui::CloseCurrentPopup();
					}

					//Удаление
					ImGui::Separator();
					if (ImGui::MenuItem(u8"Удалить")) {
						for (unsigned short tile_id = 0; tile_id < c_tileset->tiles.size(); tile_id++) {
							core.proj.replace_tile_in_all_scenes({ tile_id, static_cast<unsigned short>(c_tset_id + 1), is_local }, { 0, 0, 0 });
						}

						//Если тайлсет выбранного тайла находится в той-же область расположения, что и удаленный тайлсет. Тогда нужно уменьшить индекс тайлсета выбранного тайла если он стоит за удаленным тайлсетом 
						if ((selected_tile.is_local == is_local)) {
							if (selected_tile.tset_value == c_tset_id) selected_tile.tile_value = 0;
							if (selected_tile.tset_value >= c_tset_id && selected_tile.tset_value != 0) selected_tile.tset_value--;
						}

						res->tileset_hash[res->tilesets[c_tset_id].name] = 0;
						res->tilesets.erase(res->tilesets.begin() + c_tset_id);
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			//Когда кнопка мыши отпущена, обнулить(т.е. элементы больше не перемещаются, и можно включить возможность нажатий на элементы)
			was_dragged = was_dragged && core.ms.lb; 

			if (ImGui::Button(u8"Добавить тайлсет", ImVec2(winsize.x, 0))) {
				res->tilesets.resize(core.proj.get_res(is_local)->tilesets.size() + 1);
				rename_id = res->tilesets.size() - 1;
				//Хеширование имени нового тайлсета только после переименования
			}

			//Переименование
			if (rename_id >= 0) {
				if (!Popups::Rename(&res->tilesets[rename_id].name)) {
					hash_unique_names(&res->tileset_hash, &res->tilesets[rename_id].name, rename_id);
					rename_id = -1;
				}
			}

			ImGui::EndTabItem();
		}
		/*
		//Временные текстуры💀💀💀 (Нуждается в ОЧЕНЬ СЕРЬЕЗНОЙ ПЕРЕРАБОТКЕ, можно даже сказать в полном переделывании)
		if (ImGui::BeginTabItem(u8"Врем. текстуры", NULL, ImGuiTabItemFlags_NoCloseButton)) {
			static int slice_id = 0;
			int dist = 4, items_per_row = 3;
			for (int i = 0; i < core.proj.tmp_textures.size(); i++) {
				texture::basic_texture* c_texture = &core.proj.tmp_textures[i];
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(dist, dist));

				if (ImGui::ImageButton(ImTextureID(c_texture->texture_pointer), ImVec2((winsize.x - dist * items_per_row) / items_per_row, (winsize.x - dist * items_per_row) / items_per_row)))
					;//selected_tile.tile_value = i;

				if (ImGui::BeginPopupContextItem()) {
					pos2d winsize_popup = get_window_size();

					if (ImGui::MenuItem(u8"Разрезать"))
						slice_id = i + 1;

					if (ImGui::MenuItem(u8"Создать элемент...")) {
						//заглушка
						core.proj.get_res(selected_tile.is_local)->tilesets[selected_tile.tset_value].tiles.resize(core.proj.get_res(selected_tile.is_local)->tilesets[selected_tile.tset_value].tiles.size() + 1);
						core.proj.get_res(selected_tile.is_local)->tilesets[selected_tile.tset_value].tiles.back().tile_texture.base_frame.frame = core.proj.tmp_textures[i];
						core.proj.tmp_textures.erase(core.proj.tmp_textures.begin() + i);
						ImGui::CloseCurrentPopup();
					}

					if (ImGui::MenuItem(u8"Удалить")) {
						core.proj.tmp_textures[i].delete_texture();
						core.proj.tmp_textures.erase(core.proj.tmp_textures.begin() + i);
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				if ((i + 1) % items_per_row != 0) ImGui::SameLine();
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
			}

			static bool was_waiting_for_new = 0;
			if (ImGui::Button(u8"Загрузить", ImVec2((winsize.x - dist * items_per_row) / items_per_row, (winsize.x - dist * items_per_row) / items_per_row))) {
				was_waiting_for_new = 1;
			}
			if (was_waiting_for_new) {
				static string data;
				if (!Popups::Import(&data, { u8"Изображения" ,"*.png", "*.jpg", "*.gif" })) {
					was_waiting_for_new = 0;
					if (data.size()) {
						core.proj.tmp_textures.resize(core.proj.tmp_textures.size() + 1);
						core.proj.tmp_textures.back().load_from_png_string(data);
					}
				}
			}

			if (slice_id) {
				static bool first_time = 0;
				if (!first_time)
					ImGui::OpenPopup(u8"Действия с текстурой"), first_time = 1;
				static vector<texture::basic_texture> tmp;
				if (texture_slicing_popup(&tmp, &core.proj.tmp_textures[slice_id - 1])) {	
					first_time = 0;
					core.proj.tmp_textures.erase(core.proj.tmp_textures.begin() + slice_id - 1);
					core.proj.tmp_textures.resize(core.proj.tmp_textures.size() + tmp.size());
					slice_id = 0;
					for (int i = 0; i < tmp.size(); i++) {
						core.proj.tmp_textures[core.proj.tmp_textures.size() - tmp.size() + i] = tmp[i];
					}
				}
			}
			ImGui::EndTabItem();
		}
		*/
		/*
		if (ImGui::BeginTabItem(u8"Префабы", NULL, ImGuiTabItemFlags_NoCloseButton)) {
			static bool was_dragged = 0;
			static int rename_id = -1;

			for (unsigned short c_pfab_id = 0; c_pfab_id < core.proj.get_res(is_local)->prefabs.size(); ++c_pfab_id) {
				gameobjs::prefab* c_pref = &core.proj.get_res(is_local)->prefabs[c_pfab_id];

				if (ImGui::Selectable(string("##" + c_pref->name).c_str(), (c_pfab_id == selected_prefab) && (is_local == selected_prefab_local), 0, ImVec2(0, 50)) && !was_dragged) {
					selected_prefab = c_pfab_id;
					selected_prefab_local = is_local;
					mode = 1;
				}

				//Перемещение
				if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
					int next = c_pfab_id + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
					if (next >= 0 && next < core.proj.get_res(is_local)->prefabs.size()) {
						was_dragged = 1;

						swap(*c_pref, core.proj.get_res(is_local)->prefabs[next]);

						//Замена индекса префаба внутри обьектов
						{
							vector<gameobjs::ObjectSystem::ObjectPointer> all_ptrs = core.proj.get_curr_room()->all_objs.GetAllObjectPointers();
							for (gameobjs::ObjectSystem::ObjectPointer& i : all_ptrs) {
								if (i.GetObj()->pfab_id == next && i.GetObj()->is_pfab_local == is_local)
									i.GetObj()->pfab_id = c_pfab_id;
								else if (i.GetObj()->pfab_id == c_pfab_id && i.GetObj()->is_pfab_local == is_local)
									i.GetObj()->pfab_id = next;
							}
						}

						//Перемещение индекса выделения
						if ((selected_prefab == next) && (selected_prefab_local == is_local))
							selected_prefab = c_pfab_id;
						else if ((selected_prefab == c_pfab_id) && (selected_prefab_local == is_local))
							selected_prefab = next;

						ImGui::ResetMouseDragDelta();
					}
				}

				//Меню префаба
				
				if (ImGui::BeginPopupContextItem()) {

					//Открытие окна с параметрами текстуры
					if (ImGui::MenuItem(u8"Параметры текстуры")) {
						call_texture_params_popup = 1;
						texture_params_popup_is_target_local = is_local;
						texture_params_popup_texture_type = 1;
						texture_params_popup_texture_id = c_pfab_id;
						texture_params_popup_texture_id2 = 0;
					}


					if (ImGui::MenuItem(u8"Переименовать")) {
						res->prefab_hash[c_pref->name] = 0;
						rename_id = c_pfab_id;
					}

					//Удаление
					ImGui::Separator();
					if (ImGui::MenuItem(u8"Удалить")) {
						res->prefab_hash[c_pref->name] = 0;

						c_pref->txt.destroy();

						vector<gameobjs::ObjectSystem::ObjectPointer> all_ptrs = core.proj.get_curr_room()->all_objs.GetAllObjectPointers();
						for (int i = 0; i < all_ptrs.size(); i++) {
							if (all_ptrs[i].GetObj()->pfab_id == c_pfab_id && all_ptrs[i].GetObj()->is_pfab_local == is_local)
								all_ptrs[i].DeleteObj();
							else if (all_ptrs[i].GetObj()->pfab_id > c_pfab_id && all_ptrs[i].GetObj()->is_pfab_local == is_local)
								all_ptrs[i].GetObj()->pfab_id--;
						}
						res->prefabs.erase(res->prefabs.begin() + c_pfab_id);
						 
						//Изменение индекса выделения
						if (selected_prefab >= c_pfab_id && selected_prefab > 0 && (selected_prefab_local == is_local))
							selected_prefab--;
					}
					ImGui::EndPopup();
				}
				

				//Остаток текста внутри кнопки. Иф здесь нужен, чтобы при удалении элемента не было выхода за границы массива
				if (c_pfab_id < core.proj.get_res(is_local)->prefabs.size()) {
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() - st.ItemSpacing.x);
					ImGui::Image(ImTextureID(c_pref->txt.GetCurrentFrame()->Frame.handle), ImVec2(50, 50));
					ImGui::SameLine();
					ImGui::Text(c_pref->name.c_str());
				}
			}
			was_dragged = was_dragged && core.ms.lb;

			static bool import_wait = 0;
			if (ImGui::Button(u8"Создать")) {
				import_wait = 1;
			}

			if (import_wait) {
				static string data;
				if (!Popups::Import(&data, { u8"Изображения" ,"*.png", "*.jpg", "*.gif" })) {
					import_wait = 0;
					if (data.size()) {
						res->prefabs.resize(res->prefabs.size() + 1);
						res->prefabs.back().txt.base_frame.Frame.LoadCompressed(data);
						rename_id = res->prefabs.size() - 1;
					}
				}
			}

			if (rename_id >= 0) {
				if (!Popups::Rename(&res->prefabs[rename_id].name)) {
					hash_unique_names(&res->prefab_hash, &res->prefabs[rename_id].name, rename_id);
					rename_id = -1;
				}
			}

			ImGui::EndTabItem();
		}*/

		if (ImGui::BeginTabItem(u8"Звуки", NULL, ImGuiTabItemFlags_NoCloseButton)) {
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
		ImGui::End();
	}

	void layer_select_edit_window() {
		ImGui::Begin(u8"Слои", NULL, ImGuiWindowFlags_NoCollapse);
		gameobjs::room* c_room = core.proj.get_curr_room();
		static bool import_tmap_wait = 0;
		static int import_tmap_id = 0;
		static int ever_existed = 0;
		pos2d winsize = get_window_size();
		ImGuiStyle st = ImGui::GetStyle();
		static bool was_dragged = 0;

		static int rename_id = -1;

		static int params_id = -1;

		for (int c_layer_id = 0; c_layer_id < c_room->layers.size(); c_layer_id++) {
			gameobjs::Layer* c_layer = &core.proj.get_curr_room()->layers[c_layer_id];

			//Выбрать слой
			if (ImGui::Selectable(string(c_room->layers[c_layer_id].name + (c_room->layers[c_layer_id].hidden ? u8"(Скрыт)" : "") + "##layer").c_str(), c_layer_id == selected_layer) && !was_dragged) {
				if (selected_layer == c_layer_id) {
					c_room->layers[c_layer_id].hidden = !c_room->layers[c_layer_id].hidden;
				}
				selected_layer = c_layer_id;
			}
			
			//Поменять слои местами
			if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
				int next = c_layer_id + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
				if (next >= 0 && next < c_room->layers.size()) {
					was_dragged = 1;
					swap(*c_layer, core.proj.get_curr_room()->layers[next]);
					if (selected_layer == next) selected_layer = c_layer_id;
					else if (selected_layer == c_layer_id) selected_layer = next;
					ImGui::ResetMouseDragDelta();
				}
			}


			//Контекстное меню слоя
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem(u8"Переименовать")) {
					//call_rename_popup = 1, rename_target = &c_room->Layers[c_layer_id].name;
					rename_id = c_layer_id;
					c_room->layer_hash[c_layer->name] = 0;
				}

				if (ImGui::MenuItem(u8"Параметры")) {
					params_id = c_layer_id;
				}

				ImGui::Separator();
				if (ImGui::MenuItem(u8"Удалить")) {
					c_room->layer_hash[c_layer->name] = 0;
					//c_layer->tmap.finish();
					c_room->layers.erase((c_room->layers.begin() + c_layer_id));
					ImGui::CloseCurrentPopup();
					if (selected_layer == c_room->layers.size() && selected_layer) selected_layer--;
				}
				ImGui::EndPopup();
			}
		}

		was_dragged = was_dragged && core.ms.lb;
		if (ImGui::Button(string(u8"Добавить слой").c_str(), ImVec2(winsize.x, 0))) {
			c_room->layers.resize(core.proj.get_curr_room()->layers.size() + 1);
			c_room = core.proj.get_curr_room();
			c_room->layers.back().name = u8"Слой " + to_string(c_room->layers.size());

			rename_id = c_room->layers.size() - 1;
		}

		//Параметры слоя
		if(params_id >= 0) {
			bool params_is_opened = 1;

			gameobjs::Layer* params_layer = &c_room->layers[params_id];
			static bool params_first_time = 1;
			if (params_first_time) {
				ImGui::OpenPopup(u8"Параметры слоя");
				params_first_time = 0;
			}

			if (ImGui::BeginPopupModal(u8"Параметры слоя", &params_is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
				params_is_opened = 0;
				pos2d popup_winsize = get_window_size();
				ImGui::Text(string(static_cast<char>(34) + params_layer->name + static_cast<char>(34)).c_str());
				ImGui::SameLine();

				//Переименование
				if (ImGui::Button(u8"Переименовать##rename_layer")) {
					rename_id = params_id;

					//Отвязать хеш
					c_room->layer_hash[params_layer->name] = 0;
				}

				//Общие параметры слоя
				{
					ImGui::Text(u8"Параллакс-скроллинг:");
					ImGui::PushItemWidth((popup_winsize.x - st.ItemSpacing.x) / 2);
					ImGui::DragFloat("##parallax_x", &params_layer->parallax_speed.x, 0.1f, 0.0, 0.0, "x: %.3f");
					ImGui::SameLine();
					ImGui::DragFloat("##parallax_y", &params_layer->parallax_speed.y, 0.1f, 0.0, 0.0, "y: %.3f");
					ImGui::PopItemWidth();
					ImGui::PushItemWidth(popup_winsize.x);

					ImGui::NewLine();
					ImGui::SliderFloat(u8"##layer_transparency", &params_layer->transparency, 0, 1, u8"Прозрачность: %.3f", ImGuiSliderFlags_AlwaysClamp);
					ImGui::PopItemWidth();
				}

				//Параметры и действия с тайловой картой
				if (ImGui::TreeNode(u8"Тайловая карта")) {


					/*if (Button(u8"Импортировать##import_tmap", ImVec2((popup_winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 2, 0))) {
						call_import_popup = 1, import_tmap_wait = 1, import_tmap_id = c_layer_id;
						import_popup_filters = { u8"Формат тайловых карт Юпитера (.фткю)" ,u8"*фткю" };
					}
					SameLine();
					if (Button(u8"Экспортировать##export_tmap", ImVec2((popup_winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 2, 0))) {
						call_export_popup = 1, export_popup_export_data = c_layer->tmap.serialize(),
						export_popup_filters = { u8"Формат тайловых карт Юпитера (.фткю)" ,u8"*фткю" },
						export_popup_default_filename = u8"Сохраненная тайловая карта.фткю";
					}*/

					ImGui::PushItemWidth((popup_winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 2);

					//Размер сетки
					{
						ImGui::Text(u8"Размер сетки");
						pos2d gridsize = { params_layer->tmap.grid_size.x, params_layer->tmap.grid_size.y };
						ImGui::DragInt("##gridsize_x", &gridsize.x, 0.1f, 0, 0, "x: %d");
						ImGui::SameLine();
						ImGui::DragInt("##gridsize_y", &gridsize.y, 0.1f, 0, 0, "y: %d");
						params_layer->tmap.grid_size = { static_cast<unsigned short>(gridsize.x),
													static_cast<unsigned short>(gridsize.y) };
					}

					//Смещение тайловой карты(нужно для НЕБОЛЬШОЙ коррекции координат тайловой карты)
					{
						pos2d offset = { params_layer->tmap.offset.x, params_layer->tmap.offset.y };
						ImGui::Text(u8"Смещение");
						ImGui::SliderInt("##offset_x", &offset.x, -params_layer->tmap.grid_size.x / 2, params_layer->tmap.grid_size.x / 2, "x: %d", ImGuiSliderFlags_AlwaysClamp);
						ImGui::SameLine();
						ImGui::SliderInt("##offset_y", &offset.y, -params_layer->tmap.grid_size.y / 2, params_layer->tmap.grid_size.y / 2, "y: %d", ImGuiSliderFlags_AlwaysClamp);
						params_layer->tmap.offset = { static_cast<short>(offset.x),
													  static_cast<short>(offset.y) };
					}

					ImGui::PopItemWidth();

					//Кнопки смещения содержимого тайловой карты
					{
						if (ImGui::Button(u8"Сместить вверх", ImVec2((popup_winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 2, 0)))
							params_layer->tmap.ShiftY(1);
						ImGui::SameLine();
						if (ImGui::Button(u8"Сместить вниз", ImVec2((popup_winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 2, 0)))
							params_layer->tmap.ShiftY(-1);

						if (ImGui::Button(u8"Сместить влево", ImVec2((popup_winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 2, 0)))
							params_layer->tmap.ShiftX(-1);
						ImGui::SameLine();
						if (ImGui::Button(u8"Сместить вправо", ImVec2((popup_winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 2, 0)))
							params_layer->tmap.ShiftX(1);
					}

					ImGui::Text(string(u8"Размер: " + to_string(params_layer->tmap.NegSize().x + params_layer->tmap.PosSize().x) + u8" на " + to_string(params_layer->tmap.NegSize().y + params_layer->tmap.PosSize().y) + u8" тайлов").c_str());
					ImGui::TreePop();
				}


				ImGui::Text(u8"                                                                                           ");

				//Очистить весь слой
				if (ImGui::Button(u8"Очистить", ImVec2((popup_winsize.x - st.ItemSpacing.x) / 2, 0)))
					params_layer->tmap.Clear();

				//Удалить слой
				ImGui::SameLine();
				if (ImGui::Button(u8"Удалить", ImVec2((popup_winsize.x - st.ItemSpacing.x) / 2, 0))) {
					c_room->layer_hash[params_layer->name] = 0;
					//params_layer->tmap.finish();
					c_room->layers.erase((c_room->layers.begin() + params_id));
					ImGui::CloseCurrentPopup();
					if (selected_layer == c_room->layers.size() && selected_layer) selected_layer--;
				}

				//Закрыть окно
				if (ImGui::Button(u8"Закрыть", ImVec2(popup_winsize.x, 0))) {
					params_is_opened = 1;
				}
				//ImGui::Separator();
				ImGui::EndPopup();
			}

			if (params_is_opened) {
				params_first_time = 1;
				ImGui::CloseCurrentPopup();
				params_id = -1;
			}
		}

		if (rename_id >= 0) {
			if (!Popups::Rename(&c_room->layers[rename_id].name)) {
				hash_unique_names(&c_room->layer_hash, &c_room->layers[rename_id].name, rename_id);
				rename_id = -1;
			}
		}

		ImGui::End();
	}

	void tile_select_edit_window() {
		ImGui::Begin(u8"Выбор тайлов", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysVerticalScrollbar);
		gameobjs::resources* res = core.proj.get_res(selected_tile.is_local);
		pos2d winsize = get_window_size();

		{
			ImGui::PushItemWidth(winsize.x);
			const char* modes[4] = {u8"Установка тайлов", u8"Установка обьектов", u8"Выбор обьектов", u8"Камера"};
			int mode_adaptor = mode;
			ImGui::Text(u8"Режим");
			ImGui::SliderInt("##mode_selector", &mode_adaptor, 0, 2, modes[mode_adaptor]);
			mode = mode_adaptor;
			ImGui::PopItemWidth();
		}

		if ((mode == 0) && res->tilesets.size()) {
			Tiles::tileset* c_tileset = &res->tilesets[selected_tile.tset_value];

			//Переименование тайлсета
			{
				ImGui::Text(string(u8"Имя: " + string({char(34)}) + c_tileset->name + char(34)).c_str());
				static bool rename_wait = 0;
				if (ImGui::Button(u8"Переименовать##tileset_rename")) {
					//Отвязка имени от хеша
					res->tileset_hash[c_tileset->name] = 0;

					rename_wait = 1;
				}

				//Окно переименования
				if (rename_wait) {
					if (!Popups::Rename(&c_tileset->name)) {
						hash_unique_names(&res->tileset_hash, &c_tileset->name, selected_tile.tset_value);
						rename_wait = 0;
					}
				}
			}

			int dist = 4, items_per_row = 3;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(dist, dist));

			static int hovered_tile;
			bool was_hovered_in_last_frame = 0;

			ImGuiStyle st = ImGui::GetStyle();

			for (int c_tile_id = 0; c_tile_id < c_tileset->tiles.size(); c_tile_id++) {
				Tiles::t_tile* c_tile = &c_tileset->tiles[c_tile_id];

				//Выбор тайла
				if (ImGui::ImageButton(to_string(c_tile_id).c_str(), ImTextureID(c_tile->tile_texture.GetCurrentFrame().frame.GetHandle()), ImVec2((winsize.x - dist * items_per_row) / items_per_row, (winsize.x - dist * items_per_row) / items_per_row), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, c_tile_id == selected_tile.tile_value ? 0.5 : (1 - (0.3 * (hovered_tile == c_tile_id))))))
					selected_tile.tile_value = c_tile_id;

				//Нужно для того, чтобы поменять тайлы местами или перенести их в другой тайлсет через менеджер ресурсов
				{
					//Источник
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
						ImGui::SetDragDropPayload("Tile DND", &c_tile_id, sizeof(int));
						ImGui::Text(u8"Переместить тайл");
						ImGui::EndDragDropSource();
					}
					//Приемник
					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Tile DND")) {
							int src = *reinterpret_cast<int*>(payload->Data);

							if (selected_tile.tile_value == src) selected_tile.tile_value = c_tile_id;
							else if (selected_tile.tile_value == c_tile_id) selected_tile.tile_value = src;
							swap(*c_tile, res->tilesets[selected_tile.tset_value].tiles[src]);
							core.proj.swap_replace_tile_in_all_scenes({ static_cast<unsigned short>(c_tile_id), static_cast<unsigned short>(selected_tile.tset_value + 1), selected_tile.is_local }, { static_cast<unsigned short>(src), static_cast<unsigned short>(selected_tile.tset_value + 1), selected_tile.is_local });
						}
						ImGui::EndDragDropTarget();
					}
				}

				//Контектсное меню тайла
				if (ImGui::BeginPopupContextItem()) {

					//Дубликат тайла. Нужен, если нужно иметь 2 одинаковые текстуры, но разные параметры
					if (ImGui::MenuItem(u8"Создать дубликат")) {
						c_tileset->tiles.push_back(c_tileset->tiles[c_tile_id]);
						Tiles::t_tile* c_tile = &c_tileset->tiles.back();

						//Копия главного кадра
						//c_tile->tile_texture.base_frame.frame.LoadRaw(c_tile->tile_texture.base_frame.Frame.GetBinary(), c_tile->tile_texture.base_frame.Frame.size);

						//Копии кадров всех анимаций
						//for (int duplicate_animation_id = 0; duplicate_animation_id < c_tileset->tiles.back().tile_texture.animations.size(); duplicate_animation_id++) {
						//	Texture::Animation* c_anim = &c_tileset->tiles.back().tile_texture.animations[duplicate_animation_id];
						//	for (int duplicate_frame_id = 0; duplicate_frame_id < c_anim->frames.size(); duplicate_animation_id++) {
						//		//Texture::Frame* c_frame = &c_anim->frames[duplicate_frame_id];
						//		//c_frame->Frame.LoadRaw(c_frame->Frame.GetBinary(), c_frame->Frame.size);
						//	}
						//}
					}

					//Открыть окно параметров текстур (нужно переделать)
					if (ImGui::MenuItem(u8"Параметры текстуры")) {
						call_texture_params_popup = 1;
						texture_params_popup_is_target_local = selected_tile.is_local;
						texture_params_popup_texture_type = 0;
						texture_params_popup_texture_id = selected_tile.tset_value;
						texture_params_popup_texture_id2 = c_tile_id;
					}

					ImGui::Separator();
					//Удалить
					if (ImGui::MenuItem(u8"Удалить")) {
						c_tileset->tiles.erase(c_tileset->tiles.begin() + c_tile_id);

						//Убрать удаленный тайл везде
						core.proj.replace_tile_in_all_scenes({ static_cast<unsigned short>(c_tile_id), static_cast<unsigned short>(selected_tile.tset_value + 1), selected_tile.is_local }, { 0, 0, 0 });

						//Сдвинуть индексы тайлов, которые были после удаленного
						for (unsigned short t_id = c_tile_id; t_id < c_tileset->tiles.size(); t_id++)
							core.proj.replace_tile_in_all_scenes({ static_cast<unsigned short>(t_id + 1), static_cast<unsigned short>(selected_tile.tset_value + 1), selected_tile.is_local },
																 { static_cast<unsigned short>(t_id),	  static_cast<unsigned short>(selected_tile.tset_value + 1), selected_tile.is_local });
					}
					ImGui::EndPopup();
				}

				//Определение подсвечивающегося элемента при наведении
				if (ImGui::IsItemHovered()) { 
					hovered_tile = c_tile_id;
					was_hovered_in_last_frame = 1; 
				}

				//Если тайл не последний в строке, то он должен быть вместе с остальными
				if ((c_tile_id + 1) % items_per_row != 0) 
					ImGui::SameLine();
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleVar();

			//Загрузка новых тайлов
			{
				static JTexture::BasicTexture load_txt;
				static bool waiting_load = 0;
				static bool waiting_slice = 0;



				//Кнопка загрузки
				if (ImGui::Button(u8"Загрузить", ImVec2((winsize.x - dist * items_per_row) / items_per_row, (winsize.x - dist * items_per_row) / items_per_row))) {
					waiting_load = 1;
				}

				//Импорт изображения
				if (waiting_load) {
					static string data;

					if (!Popups::Import(&data, { u8"Изображения" ,"*.png", "*.jpg", "*.gif" })) {

						//Если хоть что-нибудь было загружено
						if (data.size()) {
							ImGui::OpenPopup(u8"Действия с текстурой");
							waiting_slice = 1;
							load_txt.LoadCompressed(data);
						}

						waiting_load = 0;
					}
				}

				//Разрезка
				if (waiting_slice) {

					//Массив текстур, которые будут загружаться
					static vector<JTexture::BasicTexture> tmp_texture_array;
					if (!texture_slicing_popup(&tmp_texture_array, &load_txt)) {
						waiting_slice = 0;

						if (tmp_texture_array.size()) {

							//Копирование в тайлы
							c_tileset->tiles.resize(c_tileset->tiles.size() + tmp_texture_array.size());
							for (int i = 0; i < tmp_texture_array.size(); i++) {
								c_tileset->tiles[c_tileset->tiles.size() - tmp_texture_array.size() + i].tile_texture.base_frame.frame = tmp_texture_array[i];
							}
						}
					}
				}
			}

			if (!was_hovered_in_last_frame) hovered_tile = -1;
		}
		/*
		res = core.proj.get_res(selected_prefab_local);
		if ((mode == 1) && res->prefabs.size()) {
			static bool script_editor_open = 0;
			static bool variable_editor_open = 0;
			gameobjs::prefab* c_pfab = &res->prefabs[selected_prefab];
			ImGui::Image(ImTextureID(c_pfab->txt.GetCurrentFrame()->Frame.handle), ImVec2(winsize.x, winsize.x / (static_cast<float>(c_pfab->txt.GetCurrentFrame()->Frame.size.x) / static_cast<float>(c_pfab->txt.GetCurrentFrame()->Frame.size.y))));

			if (ImGui::Button(u8"Параметры текстуры")) {
				call_texture_params_popup = 1;
				texture_params_popup_is_target_local = selected_prefab_local;
				texture_params_popup_texture_type = 1;
				texture_params_popup_texture_id = selected_prefab;
				texture_params_popup_texture_id2 = 0;
			}

			if (ImGui::Button(u8"Скрипт", ImVec2(winsize.x, 0))) {
				script_editor_open = 1;
			}

			if (ImGui::Button(u8"Переменные")) {
				variable_editor_open = 1;
			}
			
			if (variable_editor_open) {
				static std::vector<gameobjs::ObjectSystem::ObjectPointer> objs;

				static bool first_time = 1;
				if (first_time) {
					objs.clear();
					std::vector<gameobjs::ObjectSystem::ObjectPointer> all_objs = core.proj.get_curr_room()->all_objs.GetAllObjectPointers();
					for (gameobjs::ObjectSystem::ObjectPointer& i : all_objs) {
						if (i.GetObj()->pfab_id == selected_prefab && i.GetObj()->is_pfab_local == selected_prefab_local)
							objs.push_back(i);
					}
					first_time = 0;
				}

				if (!var_editor(c_pfab->vartypes, c_pfab->var_hash, objs)) {
					variable_editor_open = 0;
				}
			}

			if (script_editor_open) {
				static std::vector<std::string> tmp_script;

				static bool first_time = 1;
				if (first_time) {
					tmp_script = c_pfab->script;
					first_time = 0;
				}

				if (!script_editor(tmp_script)) {
					first_time = 1;
					c_pfab->script = tmp_script;
					script_editor_open = 0;
				}
			}
			/*
			ImGui::Text(u8"Вывод");
			ImGui::BeginChild(u8"Вывод", ImVec2(winsize.x, 250), true);
			ImGui::Text(script_output.c_str());
			script_output.clear();
			ImGui::EndChild();

			if (ImGui::Button(u8"Очистить")) {
				script_output.clear();
			}
			
		}*/

		ImGui::End();
	}

	void settings_window() {
		static bool settings_closed = 0;
		static bool export_wait = 0;

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu(u8"Файл")) {
				if (ImGui::MenuItem(u8"Сохранить")) {
					JSerial::Utilities::SaveFile(project_save_path, core.proj.Serialize());
				}
				if (ImGui::MenuItem(u8"Сохранить как...")) {
					export_wait = 1;
				}

				if (ImGui::MenuItem(u8"Настройки")) settings_closed = 1;

				if (ImGui::MenuItem(u8"Возврат в главное меню")) {
					JSerial::Utilities::SaveFile(project_save_path, core.proj.Serialize());
					main_menu = 1;
				}
				ImGui::EndMenu();
			}
			
			static bool proj_params_open = 0;

			if (ImGui::BeginMenu(u8"Проект")) {
				if (!project_editor_opened) {
					if (ImGui::MenuItem(u8"Открыть редактор проекта")) {
						project_editor_opened = 1;
						ImGui::CloseCurrentPopup();
					}
				}

				if (ImGui::MenuItem(u8"Параметры проекта")) {
					proj_params_open = 1;
				}
				ImGui::EndMenu();
			}

			if (export_wait) {
				export_wait = Popups::Export(core.proj.Serialize(), { u8"Файлы проекта", u8"*.фпю" }, string(core.proj.name + u8".фпю"));
			}

			if (proj_params_open) {
				ImGui::Begin(u8"Параметры", &proj_params_open, ImGuiWindowFlags_AlwaysAutoResize);

				ImGui::SeparatorText(u8"Параметры проекта");
				ImGui::Text(u8"Высота экрана(в пикселях)");
				int scrh = core.proj.screen_height;
				ImGui::DragInt("##scrheight", &scrh, 0.1, 1, 0);
				core.proj.screen_height = scrh;

				if (!project_editor_opened) {
					ImGui::SeparatorText(u8"Параметры комнаты");
					gameobjs::room* c_room = core.proj.get_curr_room();
					ImVec4 color(c_room->background_color.r / 255.0f, c_room->background_color.g / 255.0f, c_room->background_color.b / 255.0f, 1.0f);
					ImGui::ColorEdit3(u8"Цвет фона", (float*)&color, ImGuiColorEditFlags_Uint8);
					c_room->background_color.r = color.x * 255.0f, c_room->background_color.g = color.y * 255.0f, c_room->background_color.b = color.z * 255.0f;
				}
					
				//core.window.scrs.y = 128;
				ImGui::End();
			}
			ImGui::EndMenuBar();
		}


		ImGui::End();

		if (settings_closed) {
			ImGui::Begin(u8"Настройки", &settings_closed);
			//static string path;
			//ImGui::Checkbox(u8"Показывать границы тайловой карты", &core.proj.show_tmap_border_lines);

			pos2d mspos = core.screen_coords(mainwin.mspos, mainwin.winsize, &editor_cam);

			ImGui::Text(u8"Экранные координаты: ");
			ImGui::Text(to_string(mspos.x).c_str());
			ImGui::Text(to_string(mspos.y).c_str());

			pos2ds tilepos = core.mst(selected_layer, mainwin.mspos, mainwin.winsize, &editor_cam);

			ImGui::Text(u8"Позиция тайла: ");
			ImGui::Text(to_string(tilepos.x).c_str());
			ImGui::Text(to_string(tilepos.y).c_str());

			ImGui::Text(u8"Камера: ");
			ImGui::Text(to_string(editor_cam.center.x).c_str());
			ImGui::Text(to_string(editor_cam.center.y).c_str());

			ImGui::Text(u8"ФПС:");
			ImGui::Text(to_string(core.fps).c_str());
			//Text(path.c_str());
			//if (Button(u8"Путь")) {
			//	path = pfd::select_folder(u8"Выберите папку", pfd::path::home()).result();
			//}
			ImGui::End();
		}
	}

	void render_mainwin() {
		gameobjs::room* c_room = core.proj.get_curr_room();
		gameobjs::Layer* c_layer = c_room->layers.size() ? &c_room->layers[selected_layer] : nullptr;
		ImGui::Begin(u8"Главное окно", NULL, ImGuiWindowFlags_NoCollapse);

		mainwin.fb = &editor_cam.fb;
		mainwin.start({ core.ms.x, core.ms.y });
		if (mainwin.winsize.x != mainwin.old_winsize.x || mainwin.winsize.y != mainwin.old_winsize.y || editor_cam.fb.fb_size.y != core.proj.screen_height) {
			editor_cam.fb.resize({ static_cast<int>(static_cast<float>(mainwin.winsize.x) * static_cast<float>(core.proj.screen_height) / static_cast<float>(mainwin.winsize.y)), core.proj.screen_height });
			mainwin.old_winsize = mainwin.winsize;
		}

		core.process(&editor_cam, &script_output);
		
		core.render(&editor_cam);

		pos2d c_layer_pos = { static_cast<int>(c_layer->parallax_speed.x * editor_cam.center.x),
							  static_cast<int>(c_layer->parallax_speed.y * editor_cam.center.y) };
		if (mainwin.is_hovered && (c_layer != nullptr)) {
			if (!mode && core.proj.get_res(selected_tile.is_local)->tilesets.size()) {
				pos2ds c_tile_pos = core.mst(selected_layer, mainwin.mspos, mainwin.winsize, &editor_cam);

				Tiles::Tilemap* c_map = &c_layer->tmap;
				Tiles::tileset* c_tset = &core.proj.get_res(selected_tile.is_local)->tilesets[selected_tile.tset_value];
				Tiles::Tile picked_tile = c_map->operator[](c_tile_pos);

				if (picker.check() && (picked_tile.tset_value || (!picked_tile.tset_value && picked_tile.tile_value))) {
					selected_tile = picked_tile;
					selected_tile.tset_value--;
				}

				if (selected_tile.tile_value < c_tset->tiles.size()) { //if tile value is valid
					Tiles::t_tile& c_ttile = c_tset->tiles[selected_tile.tile_value];
					JTexture::Frame::Properties props = c_ttile.tile_texture.GetCurrentFrame().properties;
					core.drawtexture(&c_ttile.tile_texture.GetCurrentFrame().frame, { c_tile_pos.x * c_layer->tmap.grid_size.x - (c_layer_pos.x + c_layer->tmap.offset.x),
																					  c_tile_pos.y * c_layer->tmap.grid_size.y - (c_layer_pos.y + c_layer->tmap.offset.y) }, 0.5, props.rotation_origin, props.scale, props.rotation);

					//core.draw_line({ 0,0 }, core.screen_coords(mainwin.mspos, mainwin.winsize, &editor_cam), 1);
					//core.draw_pixel({ 0, 0 });
					//core.draw_pixel(core.screen_coords(mainwin.mspos, mainwin.winsize, &editor_cam));

					//core.drawrect({ 0, 0 }, core.screen_coords(mainwin.mspos, mainwin.winsize, &editor_cam));

					core.drawrect({ c_tile_pos.x * c_layer->tmap.grid_size.x - (c_layer_pos.x + c_layer->tmap.offset.x),							//start point
									c_tile_pos.y * c_layer->tmap.grid_size.y - (c_layer_pos.y + c_layer->tmap.offset.y) },


						{ c_tile_pos.x * c_layer->tmap.grid_size.x + c_layer->tmap.grid_size.x - (c_layer_pos.x + c_layer->tmap.offset.x),			//end point
						  c_tile_pos.y * c_layer->tmap.grid_size.y + c_layer->tmap.grid_size.y - (c_layer_pos.y + c_layer->tmap.offset.y) });

					if (core.ms.lb) c_map->Set(c_tile_pos, { selected_tile.tile_value, static_cast<unsigned short>(selected_tile.tset_value + 1), selected_tile.is_local });
				}
				if (core.ms.rb) c_map->Set(c_tile_pos, { 0, 0, 0 });
			}
			/*
			else if (mode == 1 && core.proj.get_res(selected_prefab_local)->prefabs.size()) {
				gameobjs::prefab* c_pfab = &core.proj.get_res(selected_prefab_local)->prefabs[selected_prefab];

				pos2d mspos = core.screen_coords(mainwin.mspos, mainwin.winsize, &editor_cam);
				pos2d layer_mspos = { mspos.x - c_layer_pos.x, mspos.y - c_layer_pos.y };

				core.drawtexture_s(&c_pfab->txt, mspos, {0.5f});

				core.drawrect(mspos,

					{ mspos.x + c_pfab->txt.GetCurrentFrame()->Frame.size.x,			//end point
					  mspos.y + c_pfab->txt.GetCurrentFrame()->Frame.size.y });

				pos2d once_buttons = core.ms.once_ret();

				if (once_buttons.x) {
					gameobjs::object tmp;
					tmp.is_pfab_local = selected_prefab_local;
					tmp.pfab_id = selected_prefab;
					tmp.pos = layer_mspos;

					for (programming_lang::var_declaration& i : c_pfab->vartypes) {
						tmp.vars.push_back({ "\0\0\0\0", i.type });
					}

					pos2ds chunkpos = {layer_mspos.x / c_room->chunksize.x - (layer_mspos.x < 0), layer_mspos.y / c_room->chunksize.y - (layer_mspos.y < 0) };

					gameobjs::ObjectSystem::HeapChunk* c_chunk = c_room->layers[selected_layer].objs.AutoResize(chunkpos);
					//if (c_chunk == nullptr) {
					//	cout << "null chunk\n";
					//}
					c_chunk->objs.push_back(c_room->all_objs.NewObject(tmp));

					//cout << "pressed!\n";
				}
			}*/
		}

		//Начало координат
		core.drawtexture(&core.coords_start, { -16, -16 }, 1);
		
		editor_cam.fb.end_render(core.window->window_size);
		mainwin.end();
		ImGui::End();
	}

	void button_check() {
		//cam2d& camera = core.get_curr_room()->cams[core.get_curr_room()->selected_cam];

		//if (save.check()) layerdata = core.get_curr_room()->Layers[0].tmap.serialize();
		//if (load.check()) core.get_curr_room()->Layers[0].tmap.deserialize(layerdata);
		if (core.ms.wheel && (mainwin.is_hovered) && ((editor_cam.zoom -= core.ms.wheel * 0.1) > 0.0f))
			editor_cam.zoom -= core.ms.wheel * 0.1;// , camera.center = core.screen_coords(mspos, winsize);

		if (home_button.check() && mainwin.is_hovered) editor_cam.center = { 0, 0 }, editor_cam.zoom = 1.0f;

		if (d.check() && mainwin.is_hovered) editor_cam.center.x += int(4 / editor_cam.zoom);
		if (a.check() && mainwin.is_hovered) editor_cam.center.x -= int(4 / editor_cam.zoom);
		if (w.check() && mainwin.is_hovered) editor_cam.center.y += int(4 / editor_cam.zoom);
		if (s.check() && mainwin.is_hovered && !save.pressed) editor_cam.center.y -= int(4 / editor_cam.zoom);
	}

	void on_frame_end() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		core.on_frame_end();
	}

	void destroy() {
		JSerial::Utilities::SaveFile(project_save_path, core.proj.Serialize());
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}
};

/*
	void import_popup() {
		static bool is_opened = 0;
		static string path;
		static bool was_active_on_prev_frame = 0;

		if (call_import_popup)
			ImGui::OpenPopup(u8"Импортировать##import_window"), call_import_popup = 0, is_opened = 1, path.clear();

		if (ImGui::BeginPopupModal(u8"Импортировать##import_window", &is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
			was_active_on_prev_frame = 1;
			ImGuiStyle st = ImGui::GetStyle();
			pos2d winsize = mg.get_window_size();
			ImGui::InputText(string(u8"##edit_path").c_str(), &path);
			ImGui::SameLine();

			if (ImGui::Button(u8"Открыть")) {
				vector<string> pth = pfd::open_file(u8"Импортировать", pfd::path::home(), import_popup_filters, pfd::opt::none).result();
				path = pth.size() ? pth[0] : path;
			}

			if (ImGui::Button(u8"Ок", ImVec2((winsize.x - st.ItemSpacing.x) / 2, 0)) && path.size()) {
				ImGui::CloseCurrentPopup();
				is_opened = 0;
				ifstream file(experimental::filesystem::path(wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(path)), ios::binary);
				file.seekg(0, ios::end);
				size_t filesize = file.tellg();
				import_data.resize(filesize);
				file.seekg(0, ios::beg);
				char* buff = new char[filesize];
				file.read(buff, filesize);
				import_data = string(buff, filesize);
				delete[] buff;
				import_popup_import_ready = 1;
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"Отмена", ImVec2((winsize.x - st.ItemSpacing.x) / 2, 0)))
				ImGui::CloseCurrentPopup(), import_popup_import_canceled = 1, is_opened = 0;
			ImGui::EndPopup();
		}
	}
	*/


	/*
	void rename_popup() {
		static bool rename_opened = 0;
		if (call_rename_popup) {
			ImGui::OpenPopup(u8"Переименовать##rename_popup"), call_rename_popup = 0, rename_opened = 1;
		}
		if (rename_opened) {
			if (ImGui::BeginPopupModal(u8"Переименовать##rename_popup", &rename_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::InputText(string(u8"##rename_popup_data").c_str(), rename_target);

				if (ImGui::Button(u8"Закрыть", ImVec2(mg.get_window_size().x, 0))) { ImGui::CloseCurrentPopup(); rename_opened = 0; }

				ImGui::EndPopup();
			}
		}
	}*/


//if (import_tmap_wait) {
//	if (import_popup_import_ready && import_data.size())
//		core.proj.get_curr_room()->Layers[import_tmap_id].tmap.deserialize(import_data), import_popup_import_ready = 0, import_tmap_wait = 0;
//	else if (import_popup_import_canceled) import_tmap_wait = 0, import_popup_import_canceled = 0;
//	else if (import_popup_import_ready && !import_data.size()) import_popup_import_ready = 0, import_tmap_wait = 0;
//}

//core.draw_line({ 0, 0 }, core.screen_coords(mainwin.mspos, mainwin.winsize, &editor_cam), 1);
//core.drawtexture(core.proj.get_res(selected_tile.is_local)->tilesets[selected_tile.tset_value].tiles[15].tile_texture.get_curr_frame(), { 0, 0 }, 1, {0, 0}, { 1.0 / 16.0, 1.0 / 16.0 }, 0);
//core.drawtexture(core.proj.get_res(selected_tile.is_local)->tilesets[selected_tile.tset_value].tiles[15].tile_texture.get_curr_frame(), core.screen_coords(mainwin.mspos, mainwin.winsize, &editor_cam), 1, { 0,0 }, { 1.0 / 16.0, 1.0 / 16.0 }, 0);
						/*
						tiles::tileset& c_tset = res.tilesets[texture_params_popup_texture_id];
						c_tset.tiles.erase(c_tset.tiles.begin() + texture_params_popup_texture_id2);
						if (selected_tile.tile_value == c_tset.tiles.size()) selected_tile.tile_value--;
						for (int r_id = 0; r_id < core.proj.get_curr_scene()->rooms.size(); r_id++) {
							gameobjs::room& c_room = core.proj.get_curr_scene()->rooms[r_id];
							for (int l_id = 0; l_id < c_room.Layers.size(); l_id++) {
								gameobjs::Layer& c_layer = c_room.Layers[l_id];
								c_layer.tmap.replace({ static_cast<unsigned short>(texture_params_popup_texture_id2), static_cast<unsigned short>(texture_params_popup_texture_id + 1), texture_params_popup_is_target_local}, { 0, 0, 0 });
								for (int t_id = texture_params_popup_texture_id2; t_id < c_tset.tiles.size(); t_id++) {
									c_layer.tmap.replace({ static_cast<unsigned short>(t_id + 1), static_cast<unsigned short>(texture_params_popup_texture_id + 1), texture_params_popup_is_target_local },
														 { static_cast<unsigned short>(t_id), static_cast<unsigned short>(texture_params_popup_texture_id + 1),     texture_params_popup_is_target_local } );
								}
							}
						}*/



						/*if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
							ImGui::SetDragDropPayload("Layer DND", &c_layer_id, sizeof(int));
							Text(u8"Поменять местами слои");
							ImGui::EndDragDropSource();
						}*/

						/*
						if (ImGui::BeginDragDropTarget()) {
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Layer DND")) {
								int payload_n = *(int*)payload->Data;
								if (payload_n >= 0 && payload_n <= c_room->Layers.size() - 1) {
									gameobjs::Layer tmp = c_room->Layers[i];
									c_room->Layers[i] = c_room->Layers[payload_n];
									c_room->Layers[payload_n] = tmp;

								}
							}
						}
							//export popup
	bool call_export_popup = 0;
	string export_popup_export_data;
	vector<string> export_popup_filters;
	string export_popup_default_filename;

	//import_popup
	//bool call_import_popup = 0;
	//string import_data;
	//vector<string> import_popup_filters;
	//bool import_popup_import_ready = 0, 
	//	 import_popup_import_canceled = 0;
						*/
						/*
									//PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(650, 350));
									if (BeginPopupModal(u8"Действия с текстурой", 0, ImGuiWindowFlags_AlwaysAutoResize)) {
										static vector<texture_processing::cropper> croplist;
										cropp_view.start({ core.ms.x, core.ms.y });
										glEnable(GL_BLEND);
										glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
										glDisable(GL_DEPTH_TEST);
										texture_view_cam.fb.clear_use();
										core.setvecs(&texture_view_cam, &core.flat_sh);
										core.setvecs(&texture_view_cam, &core.txt_sh);

										core.drawtexture_s(&core.proj.tmp_textures.back(), { 0,0 });
										static int selected_cropper = 0;
										static pos2d converted_mousepos = { -1, -1 }, old_mousepos = converted_mousepos, abs_mousepos;

										pos2d converted_camerapos = { texture_size.x / 2.0 + texture_view_cam.center.x,
																		  texture_size.y / 2.0 - texture_view_cam.center.y };

										pos2d upper_left_corner = { converted_camerapos.x - (texture_size.x / 2.0) / texture_view_cam.zoom,
																	converted_camerapos.y - (texture_size.y / 2.0) / texture_view_cam.zoom };

										pos2d lower_right_corner = { converted_camerapos.x + (texture_size.x / 2.0) / texture_view_cam.zoom,
																	 converted_camerapos.y + (texture_size.y / 2.0) / texture_view_cam.zoom };
										converted_mousepos = { -1, -1 };
										if (cropp_view.winsize.x && cropp_view.is_hovered) {
											converted_mousepos = { static_cast<int>((cropp_view.mspos.x / (static_cast<float>(cropp_view.winsize.x) / static_cast<float>(texture_size.x))) / texture_view_cam.zoom),
																   static_cast<int>((cropp_view.mspos.y / (static_cast<float>(cropp_view.winsize.x) / static_cast<float>(texture_size.x))) / texture_view_cam.zoom) };



											abs_mousepos = { converted_mousepos.x + upper_left_corner.x ,
															 converted_mousepos.y + upper_left_corner.y };
											//converted_mousepos = core.screen_coords(cropp_view.mspos, { winsize.x, winsize.x / texture_size.x * texture_size.y }, &texture_view_cam);
											//converted_mousepos.x -= (texture_size.x / 2.0f);
											//converted_mousepos.y -= (texture_size.y / 2.0f);
										}

										static bool is_holding_item = 0;

										static bool is_resizing = 0;
										static bool resize_axis = 0;
										static bool resize_closer_further = 0; //имеется ввиду сторона которая ближе или дальше от нулей

										pos2d once_buttons = core.ms.once_ret();

										for (int i = 0; i < croplist.size(); i++) {
											bool furth = 0, furth1 = 0;
											bool axis = 0;
											if (in_range(croplist[i].pos.x, abs_mousepos.x, (croplist[i].pos.x + croplist[i].size.x) - 1) && in_range(croplist[i].pos.y, abs_mousepos.y, (croplist[i].pos.y + croplist[i].size.y) - 1)) {
												if (once_buttons.x)
													selected_cropper = i, is_holding_item = 1;
											}
											else if ((axis = ((abs_mousepos.x == croplist[i].pos.x || (furth = (abs_mousepos.x == (croplist[i].pos.x + croplist[i].size.x) - 1))) && in_range(croplist[i].pos.y, abs_mousepos.y, (croplist[i].pos.y + croplist[i].size.y) - 1))) !=
												((abs_mousepos.y == croplist[i].pos.y || (furth1 = (abs_mousepos.y == (croplist[i].pos.y + croplist[i].size.y) - 1))) && in_range(croplist[i].pos.x, abs_mousepos.x, (croplist[i].pos.x + croplist[i].size.x) - 1))) {
												if (once_buttons.x) {
													selected_cropper = i, is_resizing = 1, is_holding_item = 0, resize_axis = axis, resize_closer_further = axis ? !furth : !furth1;
												}

											}

											rgb rectcol = { 255, 255, (i != selected_cropper) * 255 };

											core.drawrect_f({ static_cast<float>(croplist[i].pos.x) - static_cast<float>(texture_size.x) / 2.0f, -(static_cast<float>(croplist[i].pos.y) - static_cast<float>(texture_size.y) / 2.0f + 1) },
												{ static_cast<float>(croplist[i].pos.x + croplist[i].size.x) - static_cast<float>(texture_size.x) / 2.0f, -(static_cast<float>(croplist[i].pos.y + croplist[i].size.y) - static_cast<float>(texture_size.y) / 2.0f) + 1 }, rectcol);

										}
										pos2d deltapos = { (old_mousepos.x - converted_mousepos.x), (old_mousepos.y - converted_mousepos.y) };

										if (in_range(-1, abs_mousepos.x, texture_size.x) && in_range(-1, abs_mousepos.y, texture_size.y) && cropp_view.is_hovered) {
											if (core.ms.lb && !core.ms.rb && is_holding_item && ((croplist[selected_cropper].pos.x - deltapos.x) > -1) && ((croplist[selected_cropper].pos.x + croplist[selected_cropper].size.x - deltapos.x) <= texture_size.x) &&
												((croplist[selected_cropper].pos.y - deltapos.y) > -1) && ((croplist[selected_cropper].pos.y + croplist[selected_cropper].size.y - deltapos.y) <= texture_size.y)) {
												croplist[selected_cropper].pos.x -= deltapos.x;
												croplist[selected_cropper].pos.y -= deltapos.y;
											}
											if (core.ms.lb && is_resizing && !core.ms.rb) {
												if (resize_closer_further) {
													int* size_axis = resize_axis ? &croplist[selected_cropper].size.x : &croplist[selected_cropper].size.y;
													int mouse_axis = resize_axis ? abs_mousepos.x : abs_mousepos.y;
													int txtsize_axis = resize_axis ? texture_size.x : texture_size.y;
													int* cropperpos_axis = resize_axis ? &croplist[selected_cropper].pos.x : &croplist[selected_cropper].pos.y;

													if (in_range(-1, mouse_axis, *cropperpos_axis + *size_axis - 1))
														*size_axis = *size_axis + *cropperpos_axis - mouse_axis, * cropperpos_axis = mouse_axis;
												}
												else {
													int* size_axis = resize_axis ? &croplist[selected_cropper].size.x : &croplist[selected_cropper].size.y;
													int mouse_axis = resize_axis ? abs_mousepos.x : abs_mousepos.y;
													int txtsize_axis = resize_axis ? texture_size.x : texture_size.y;
													int cropperpos_axis = resize_axis ? croplist[selected_cropper].pos.x : croplist[selected_cropper].pos.y;

													if (in_range(0, mouse_axis - cropperpos_axis, txtsize_axis))
														*size_axis = mouse_axis - cropperpos_axis + 1;
												}
											}
											if (core.ms.wheel && (texture_view_cam.zoom - (core.ms.wheel * 0.1f) >= 0.99999f)) {
												if (core.ms.wheel >= 0.0f) {
													float dist;
													if ((dist = (converted_camerapos.x - (texture_size.x / 2.0) / (texture_view_cam.zoom - (core.ms.wheel * 0.1f)))) < 0.0f){
														texture_view_cam.center.x -= dist;
													}
													else if ((dist = (converted_camerapos.x + (texture_size.x / 2.0) / (texture_view_cam.zoom - (core.ms.wheel * 0.1f)))) >= texture_size.x) {
														texture_view_cam.center.x += (texture_size.x - dist);
													}

													if ((dist = (converted_camerapos.y - (texture_size.y / 2.0) / (texture_view_cam.zoom - (core.ms.wheel * 0.1f)))) < 0.0f) {
														texture_view_cam.center.y += dist;
													}
													else if ((dist = (converted_camerapos.y + (texture_size.y / 2.0) / (texture_view_cam.zoom - (core.ms.wheel * 0.1f)))) >= texture_size.y) {
														texture_view_cam.center.y -= (texture_size.y - dist);
													}
												}
												texture_view_cam.zoom -= core.ms.wheel * 0.1;
											}
											if (!core.ms.lb)is_holding_item = 0, is_resizing = 0;
											if (core.ms.rb && (texture_view_cam.zoom >= 0.999999f) && (upper_left_corner.x + deltapos.x >= 0) && (upper_left_corner.y + deltapos.y >= 0)
														   && (lower_right_corner.x + deltapos.x < texture_size.x) && (lower_right_corner.y + deltapos.y < texture_size.y)
														   && (converted_mousepos.x != -1) && (converted_mousepos.y != -1)) {
												texture_view_cam.center.x += deltapos.x, texture_view_cam.center.y -= deltapos.y;
											}
										}
										if(!core.ms.lb)is_holding_item = 0, is_resizing = 0;

										old_mousepos = converted_mousepos;
										texture_view_cam.fb.end_render(core.window.wins);
										Image(ImTextureID(cropp_view.fb->texture_id), ImVec2(cropp_view.winsize.x, static_cast<float>(cropp_view.winsize.x) * (static_cast<float>(texture_size.y) / static_cast<float>(texture_size.x))), ImVec2(0, 1), ImVec2(1, 0));
										cropp_view.is_hovered = IsItemHovered();

										static int n;
										static pos2d elem_size = { 16, 16 };

										if (TreeNode(u8"Разрезка")) {
											DragInt("##number_of_elements", &n, 0.1f, 0, 65535, u8"Число элементов: %d");
											Text(u8"Размер элемента:");
											PushItemWidth((cropp_view.winsize.x - st.ItemSpacing.x - st.IndentSpacing) / 2);
											DragInt("##elemsize_x", &elem_size.x, 0.1f, 0, 0, "x: %d");
											SameLine();
											DragInt("##elemsize_y", &elem_size.y, 0.1f, 0, 0, "y: %d");
											PopItemWidth();

											if (Button(u8"Новый элемент", ImVec2((cropp_view.winsize.x - st.ItemSpacing.x - st.IndentSpacing) / 2, 0.0f))) {
												if((((texture_size.x - elem_size.x)/2) >= 0 && ((texture_size.x + elem_size.x) / 2) <= texture_size.x) && (((texture_size.y - elem_size.y) / 2) >= 0 && ((texture_size.y + elem_size.y) / 2) <= texture_size.y))
													croplist.push_back({ { (texture_size.x - elem_size.x) / 2 ,(texture_size.y - elem_size.y) / 2 }, elem_size}), selected_cropper = croplist.size() - 1;
											}
											SameLine();
											if (Button(u8"Удалить элемент", ImVec2((cropp_view.winsize.x - st.ItemSpacing.x - st.IndentSpacing) / 2, 0.0f))) {
												croplist.erase(croplist.begin() + selected_cropper);
												selected_cropper--;
											}
											NewLine();
											if (Button(u8"Сгенерировать список обрезки",ImVec2(cropp_view.winsize.x - st.IndentSpacing, 0))) {
												croplist = texture_processing::generate_croplist(&core.proj.tmp_textures.back(), n, elem_size);
											}

											TreePop();
										}
										Text(u8"                                                                                           ");
										if (Button(u8"Подтвердить", ImVec2((cropp_view.winsize.x - st.ItemSpacing.x) / 2, 0))) {
											if (!croplist.size()) {
												c_tileset->tiles.resize(c_tileset->tiles.size() + 1);
												c_tileset->tiles.back().tile_texture.base_frame = core.proj.tmp_textures.back();
											}
											else {
												vector<string> cropped_data = texture_processing::crop_from_croplist(core.proj.tmp_textures.back().get_texture(), croplist, texture_size);
												c_tileset->tiles.resize(c_tileset->tiles.size() + cropped_data.size());
												for (int t_id = 0; t_id < cropped_data.size(); t_id++) {
													c_tileset->tiles[c_tileset->tiles.size() - cropped_data.size() + t_id].tile_texture.base_frame.load_from_raw(cropped_data[t_id], croplist[t_id].size);
												}
												croplist.clear();
											}

											n = 0;
											elem_size = { 16,16 };
											texture_view_cam.center = { 0,0 };
											texture_view_cam.zoom = 1;
											selected_cropper = -1;

											core.proj.tmp_textures.pop_back();
											CloseCurrentPopup();
										}
										SameLine();
										if (Button(u8"Отмена", ImVec2((cropp_view.winsize.x - st.ItemSpacing.x) / 2, 0))) {
											CloseCurrentPopup();
											core.proj.tmp_textures.pop_back();
											croplist.clear();

											n = 0;
											elem_size = { 16,16 };
											texture_view_cam.center = { 0,0 };
											texture_view_cam.zoom = 1;
											selected_cropper = -1;
										}

										EndPopup();
									}*/

									//ImGui::Text(u8"Разрезать");
									//static int n;
									//static pos2d elem_size = { 16, 16 };
									//PushItemWidth(winsize.x);
									//ImGui::DragInt("##number_of_elements", &n, 0.1f, 0, 65535, u8"Число элементов: %d");
									//PopItemWidth();
									//ImGui::Text(u8"Размер элемента:");
									//ImGui::PushItemWidth((winsize_popup.x - st.ItemSpacing.x) / 2);
									//ImGui::DragInt("##gridsize_x", &elem_size.x, 0.1f, 0, 0, "x: %d");
									//ImGui::SameLine();
									//ImGui::DragInt("##gridsize_y", &elem_size.y, 0.1f, 0, 0, "y: %d");
									//ImGui::PopItemWidth();
									//if (ImGui::Button(u8"Подтвердить", ImVec2(winsize_popup.x, 0))) {
									//	vector<texture_processing::cropper> croplist = texture_processing::generate_croplist(&core.proj.tmp_textures[i], n, elem_size);
									//	vector<string> tmp = texture_processing::crop_from_croplist(core.proj.tmp_textures[i].get_texture(), croplist, core.proj.tmp_textures[i].original_size);
									//	core.proj.tmp_textures.resize(core.proj.tmp_textures.size() + n);
									//	for (int j = 0; j < n; j++)
									//		core.proj.tmp_textures[(core.proj.tmp_textures.size() - n) + j].load_from_raw(tmp[j], elem_size);
									//	ImGui::CloseCurrentPopup();
									//}