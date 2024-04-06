#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <pos2d.h>
#include <camera.h>
#include <pfd.h>

class mimg {
public:
	ImGuiIO* io;
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoCentralNode;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport;

	void init(GLFWwindow* window, string fontdata) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = &ImGui::GetIO();
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
        //io.Fonts->AddFontDefault();

		void* data = malloc(fontdata.size());
		memcpy(data, fontdata.c_str(), fontdata.size());

		io->Fonts->AddFontFromMemoryTTF(data, (int)fontdata.size(), 20, NULL, io->Fonts->GetGlyphRangesCyrillic());

        //io->Fonts->AddFontFromFileTTF("OpenSans-Regular.ttf", 20, (const ImFontConfig*)0, io->Fonts->GetGlyphRangesCyrillic());


        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();
	}

    void update() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 0.0f));
        ImGui::Begin("Root", nullptr, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);
        
    }
};

pos2d get_mouse_pos_inside_widget(pos2d global_mouse_pos, pos2d winsize) {
    pos2d mspos = { global_mouse_pos.x - int(ImGui::GetWindowPos().x) - int(ImGui::GetContentRegionMax().x - winsize.x), 
					global_mouse_pos.y - int(ImGui::GetWindowPos().y) - int(ImGui::GetContentRegionMax().y - winsize.y) };
    if (mspos.x < 0 || mspos.y < 0) return { -1, -1 };
    return mspos;
}

pos2d get_window_size() {
	return { int(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x), int(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y) };
}

class fb_widget {
public:
    FramebufferObject *fb;
    pos2d winsize, mspos, old_winsize;
    bool is_hovered;

    void start(pos2d mouse_pos) {
        old_winsize = winsize;
        winsize = { int(ImGui::GetContentRegionAvail().x), int(ImGui::GetContentRegionAvail().y) };
		//mspos = { -1, -1 };
		mspos = get_mouse_pos_inside_widget(mouse_pos, winsize);
    }

    void end() {
        ImGui::Image(ImTextureID(fb->texture_id), ImVec2(winsize.x, winsize.y), ImVec2(0, 1), ImVec2(1, 0)); //главное окно
        is_hovered = ImGui::IsItemHovered();
    }
};

namespace Popups {

	//Экспорт
	bool Export(string export_data, vector<string> filters, string default_filename, string* return_path = nullptr) {
		static string path;

		static bool first_time = 1;
		if (first_time) {
			ImGui::OpenPopup(u8"Экспортировать##export_window"), first_time = 0;
		}

		bool is_opened = 1;
		if (ImGui::BeginPopupModal(u8"Экспортировать##export_window", &is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
			is_opened = 0;
			ImGuiStyle st = ImGui::GetStyle();
			pos2d winsize = get_window_size();
			ImGui::InputText(string(u8"##edit_path").c_str(), &path);
			ImGui::SameLine();

			//Выбор пути файла
			if (ImGui::Button(u8"Открыть")) {
				string pth = pfd::save_file(u8"Сохранить", pfd::path::home() + char(92) + default_filename, filters).result();
				path = pth.size() ? pth : path;
			}

			//Кнопка "Ок"
			if (ImGui::Button(u8"Ок", ImVec2((winsize.x - st.ItemSpacing.x) / 2, 0)) && path.size()) {
				ImGui::CloseCurrentPopup();
				is_opened = 1;

				//Сохранить файл
				JSerial::Utilities::SaveFile(path, export_data);

				if (return_path != nullptr)
					*return_path = path;
			}

			ImGui::SameLine();
			if (ImGui::Button(u8"Отмена", ImVec2((winsize.x - st.ItemSpacing.x) / 2, 0)))
				ImGui::CloseCurrentPopup(), is_opened = 1;

			ImGui::EndPopup();
		}

		if (is_opened) {
			first_time = 1;
			path.clear();
		}

		return !is_opened;
	}

	//Импорт
	bool Import(string* target, vector<string> filters, string* return_path = nullptr) {
		bool is_opened = 1; //должно быть равно 1, чтобы окно открылось. Используется как индикатор того, было ли окно открыто
		static string path;

		//Инициализация
		static bool first_time = 1;
		if (first_time) {
			ImGui::OpenPopup(u8"Импортировать##import_window"), first_time = 0;
		}

		//Главное окно
		if (ImGui::BeginPopupModal(u8"Импортировать##import_window", &is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
			is_opened = 0;
			ImGuiStyle st = ImGui::GetStyle();
			pos2d winsize = get_window_size();
			ImGui::InputText(string(u8"##edit_path").c_str(), &path);
			ImGui::SameLine();

			//Выбор пути файла
			if (ImGui::Button(u8"Открыть")) {
				vector<string> pth = pfd::open_file(u8"Импортировать", pfd::path::home(), filters, pfd::opt::none).result();
				path = pth.size() ? pth[0] : path;
			}

			//Кнопка "Ок"
			if (ImGui::Button(u8"Ок", ImVec2((winsize.x - st.ItemSpacing.x) / 2, 0)) && path.size()) {
				ImGui::CloseCurrentPopup();
				is_opened = 1;

				//Прочитать файл
				*target = JSerial::Utilities::ReadFile(path);

				if (return_path != nullptr)
					*return_path = path;

			}

			ImGui::SameLine();
			if (ImGui::Button(u8"Отмена", ImVec2((winsize.x - st.ItemSpacing.x) / 2, 0)))
				ImGui::CloseCurrentPopup(), is_opened = 1;
			ImGui::EndPopup();
		}

		//При закрытии
		if (is_opened) {
			first_time = 1;
			path.clear();
		}

		return !is_opened;
	}

	//Переименование
	bool Rename(std::string* target) {
		//Инициализация
		static bool first_time = 1;
		if (first_time) {
			ImGui::OpenPopup(u8"Переименовать##rename_popup");
			first_time = 0;
		}

		//Главное окно
		bool is_opened = 1;
		if (ImGui::BeginPopupModal(u8"Переименовать##rename_popup", &is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
			is_opened = 0;
			ImGui::InputText(u8"##rename_popup_data", target);

			//Закрытие
			if (ImGui::Button(u8"Закрыть", ImVec2(get_window_size().x, 0))) {
				ImGui::CloseCurrentPopup();
				is_opened = 0;
			}

			ImGui::EndPopup();
		}

		//При закрытии
		if (is_opened)
			first_time = 1;

		return !is_opened;
	}

	bool Error(std::string text) {
		//Инициализация
		static bool first_time = 1;
		if (first_time) {
			ImGui::OpenPopup(u8"Ошибка!##error_popup");
			first_time = 0;
		}

		//Главное окно
		bool is_opened = 1;
		if (ImGui::BeginPopupModal(u8"Переименовать##rename_popup", &is_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
			is_opened = 0;
			ImGui::Text(text.c_str());

			//Закрыть
			if (ImGui::Button(u8"Ок", ImVec2(get_window_size().x, 0))) {
				ImGui::CloseCurrentPopup();
				is_opened = 0;
			}

			ImGui::EndPopup();
		}

		//При закрытии
		if (is_opened)
			first_time = 1;

		return !is_opened;
	}
}