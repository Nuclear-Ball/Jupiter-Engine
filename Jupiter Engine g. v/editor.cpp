#define EDITOR
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include "shader.h"
#include "stb_image.h"
#include "objects.h"
#include "texture.h"
#include "Tilemap.h"
#include "core.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "my_img.h"
#include "pfd.h"
#include "editor.h"
#include "Window.h"

using namespace std;// using namespace ImGui;

int main() {
	JWindow window;	
	window.Create({ 900, 900 }, u8"Игровой движок Юпитер");

	Editor editor;
	editor.core.window = &window;

    editor.init();
    //editor.demo_create();

    //string layerdata;

    while (editor.core.window->Check()) {
		
		editor.update();
		editor.settings_window();

		if (!editor.main_menu) {
			editor.resource_manager();
			editor.tile_select_edit_window();
			//ImGui::ShowDemoWindow();
			//ShowDebugLogWindow();
			if (!editor.project_editor_opened) {
				editor.render_mainwin();
				editor.texture_params_popup();

				editor.layer_select_edit_window();

				editor.button_check();
			}
			else {
				editor.project_editor();
			}
		}

		else {
			editor.main_menu_window();
		}

		window.StartRender({});
		editor.on_frame_end();
		window.EndRender();
    }

    editor.destroy();
}