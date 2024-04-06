#pragma warning(disable : 4996)
#define EDITOR
//#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <shader.h>
#include <stb_image.h>
#include <objects.h>
#include <texture.h>
//#include <window.h>
#include <Tilemap.h>
#include <core.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <my_img.h>
#include <pfd.h>

//#define sc static_cast<short> 

using namespace std; using namespace ImGui;

inline void init_dockspace(ImGuiID* dockspace_id, mimg* mg ) {
    ImGui::DockBuilderRemoveNode(*dockspace_id);
    ImGui::DockBuilderAddNode(*dockspace_id, mg->dockspace_flags | ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(*dockspace_id, mg->viewport->Size);
    ImGui::DockBuilderGetCentralNode(*dockspace_id);
    auto dock_id_left = ImGui::DockBuilderSplitNode(*dockspace_id, ImGuiDir_Left, 0.17f, nullptr, dockspace_id);
    auto dock_id_right = ImGui::DockBuilderSplitNode(*dockspace_id, ImGuiDir_Right, 0.2f, nullptr, dockspace_id);

    ImGui::DockBuilderDockWindow(u8"Слои", dock_id_right);
    ImGui::DockBuilderDockWindow(u8"Менеджер ресурсов", dock_id_right);
    ImGui::DockBuilderDockWindow(u8"Выбор тайлов", dock_id_left);

    ImGui::DockBuilderDockWindow(u8"Главное окно", *dockspace_id);

    ImGui::DockBuilderFinish(*dockspace_id);
}

int main()
{
    core::core core;
    core.init({ 900, 900 }, 256, u8"Игровой движок Юпитер");
    core.scenes.resize(1);
    core.scenes[0].rooms.resize(1);
    core.scenes[0].rooms[0].cams.resize(1);
    core.scenes[0].rooms[0].cams[0].fb.CreateFramebuffer(core.window.scrs);

    //core.addscene();
    //core.loadres(u"gamedata.bin");
    //загрузка ресурсов
    core.global_res.tmp_textures.resize(2);
    core.global_res.tmp_textures[0].load(u8"tiles.png");
    
    
    vector<string> tmp = texture_processing::crop_from_croplist(core.global_res.tmp_textures[0].get_texture(), texture_processing::generate_croplist(&core.global_res.tmp_textures[0], 34), core.global_res.tmp_textures[0].original_size);
    

    core.global_res.tilesets.resize(1);
    core.global_res.tilesets[0].tiles.resize(34);
    
    for (int i = 0; i < 34; i++)
        core.global_res.tilesets[0].tiles[i].tile_texture.base_frame.load_from_raw(tmp[i], {16, 16});
    
    //core.global_res.prefabs.resize(1);
    //core.global_res.prefabs[0].txt.base_frame.load(u8"caine_1.png");
    //core.global_res.prefabs[0].txt.base_frame.original_size = {48, 128};
    //core.global_res.prefabs[0].txt.animations.resize(1);
    //core.global_res.prefabs[0].txt.animations[0].frames.resize(5);
    
    //core.global_res.tmp_textures[1].load(u8"anim.png");
    //vector<string> tmp_anim = texture_processing::crop_from_croplist(core.global_res.tmp_textures[1].get_texture(), texture_processing::generate_croplist(&core.global_res.tmp_textures[1], 5, {64, 64}), core.global_res.tmp_textures[1].original_size);
    //for (int i = 0; i < 5; i++) {
    //    core.global_res.prefabs[0].txt.animations[0].frames[i].frame.load_from_raw(tmp_anim[i], { 64, 64 });
    //    core.global_res.prefabs[0].txt.animations[0].frames[i].duration = 0.07;
    //}
    //core.global_res.prefabs[0].txt.base_frame.rotation_origin = { 24, 64 };
    key_combination save(core.window.id, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_S), load(core.window.id, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_L), obj_mode(core.window.id, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_O);
    key w(core.window.id, GLFW_KEY_W, 0), a(core.window.id, GLFW_KEY_A, 0), s(core.window.id, GLFW_KEY_S, 0), d(core.window.id, GLFW_KEY_D, 0);
    
    tiles::tile selected_tile = {0, 0, 0};

    unsigned short selected_layer = 0,
        mode = 0,
        selected_prefab = 0;

    key up(core.window.id, GLFW_KEY_UP), down(core.window.id, GLFW_KEY_DOWN), picker(core.window.id, GLFW_KEY_P);
    int on_init = 0;
    mimg mg;
    mg.init(core.window.id);
    mg.io->ConfigWindowsMoveFromTitleBarOnly = 1;
    mg.io->IniFilename = NULL;
    core.scenes[0].rooms[0].Layers.resize(1);
    core.scenes[0].rooms[0].Layers[0].chunks.xpyp.quarter.resize(1);
    core.scenes[0].rooms[0].Layers[0].chunks.xpyp.quarter[0].resize(1);
    core.scenes[0].rooms[0].Layers[0].chunks.xpyp.size = { 1, 1 };
    core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers.back().name = u8"Слой 1";
    //core.scenes[0].rooms[0].Layers[0].chunks.xpyp.quarter[0][0].objs.push_back({ 0, 0, {12, 5} });
    //core.create_obj({ 12, 5 }, 0, 0, 0);
    fb_widget imgselect;
    //FramebufferObject imgfb({100, 100});
    string layerdata;
    while (core.window.checkWindow()) {
        //core.update();
        core.on_frame_start();
        //cam2d& camera = core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].cams[core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].selected_cam];
        //tiles::t_tile& ctile = core.global_res.tilesets[selected_tile.tset_value].tiles[selected_tile.tile_value];

        mg.update();
        // Dockspace
        ImGuiID dockspace_id = ImGui::GetID("Root");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), mg.dockspace_flags);

        static bool first_time = true;
        if (first_time){
            first_time = !first_time;
            init_dockspace(&dockspace_id, &mg);
        }

        static bool settings_closed = 0;
        if (BeginMenuBar()) {
            if (BeginMenu(u8"Файл")) {
                if (MenuItem(u8"Настройки")) settings_closed = 1;
                ImGui::EndMenu();
            }
            EndMenuBar();
        }
        End();

        if(settings_closed){
            Begin(u8"Настройки", &settings_closed);
            static string path;
            Checkbox(u8"Показывать границы тайловой карты", &core.show_tmap_border_lines);
            Text(path.c_str());
            if (Button(u8"Путь")) {
                path = pfd::select_folder(u8"Выберите папку", pfd::path::home()).result();
            }
            End();
        }

        static bool call_rename_popup = 0;
        static string* rename_target = nullptr;
        static bool rename_opened = 0;
        if (call_rename_popup) {
            ImGui::OpenPopup(u8"Переименовать##rename_popup"), call_rename_popup = 0, rename_opened = 1;
        }
        if (rename_opened) {
            if (ImGui::BeginPopupModal(u8"Переименовать##rename_popup", &rename_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
                InputText(string(u8"##rename_popup_data").c_str(), rename_target);
                if (ImGui::Button(u8"Закрыть", ImVec2(140, 0))) { ImGui::CloseCurrentPopup(); rename_opened = 0; }
                ImGui::EndPopup();
            }
        }

        static bool call_texture_properties_popup = 0, texture_opened = 0;
        static bool is_target_local = 0;
        static bool texture_type = 0;//0-tile, 1-prefab
        static int popup_texture_id = 0, popup_texture_id2 = 0;
        if (call_texture_properties_popup) {
            ImGui::OpenPopup(u8"Параметры текстуры##texture_params"), call_texture_properties_popup = 0, texture_opened = 1;
        }
        if (texture_opened) {
            if (ImGui::BeginPopupModal(u8"Параметры текстуры##texture_params", &texture_opened, ImGuiWindowFlags_AlwaysAutoResize)) {

                Text(u8"Параметры текстуры:");
                gameobjs::resources& res = is_target_local ? core.scenes[core.c_scene_id].local_res : core.global_res;

                texture::basic_texture* curr_txt = texture_type ? res.prefabs[popup_texture_id].txt.get_curr_frame() : res.tilesets[popup_texture_id].tiles[popup_texture_id2].tile_texture.get_curr_frame();
                pos2d winsize = { int(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x), int(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y) };
                Image(ImTextureID(curr_txt->texture_pointer), ImVec2(winsize.x, winsize.x));

                Text(u8"Прозрачность: ");
                SliderFloat(string(u8"##txt_transparency").c_str(), &curr_txt->transparency, 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                Text(u8"Вращение: ");
                SliderFloat(string(u8"##txt_rotation").c_str(), &curr_txt->rotation, -360, 360);
                Text(u8"Центр вращения: ");
                ImGuiStyle st = GetStyle();
                PushItemWidth((mg.get_window_size().x - st.ItemSpacing.x) / 2);
                SliderInt(string(u8"##txtx_org_x").c_str(), &curr_txt->rotation_origin.x, -(curr_txt->original_size.x + curr_txt->scale.x) / 2, (curr_txt->original_size.x + curr_txt->scale.x) / 2, u8"x: %d");
                SameLine();
                SliderInt(string(u8"##txt_org_y").c_str(), &curr_txt->rotation_origin.y, -(curr_txt->original_size.y + curr_txt->scale.y) / 2, (curr_txt->original_size.y + curr_txt->scale.y) / 2, u8"y: %d");
                Text(u8"Смещение");
                SliderInt(string(u8"##txtx_off_x").c_str(), &curr_txt->offset.x, -(curr_txt->original_size.x + curr_txt->scale.x), curr_txt->original_size.x + curr_txt->scale.x, u8"x: %d");
                SameLine();
                SliderInt(string(u8"##txt_off_y").c_str(), &curr_txt->offset.y, -(curr_txt->original_size.y + curr_txt->scale.y), curr_txt->original_size.y + curr_txt->scale.y, u8"y: %d");
                Text(u8"Размер");
                SliderFloat(string(u8"##txt_size_x").c_str(), &curr_txt->scale.x, 0, 2, u8"x: %.3f");
                SameLine();
                SliderFloat(string(u8"##txt_size_y").c_str(), &curr_txt->scale.y, 0, 2, u8"y: %.3f");
                PopItemWidth();
                if (ImGui::Button(u8"Закрыть"))
                    ImGui::CloseCurrentPopup(), texture_opened = 0;
                ImGui::EndPopup();
            }
        }

        static fb_widget mainwin;
        {
            cam2d& camera = core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].cams[core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].selected_cam];
            Begin(u8"Главное окно", NULL, ImGuiWindowFlags_NoCollapse);
            mainwin.fb = &camera.fb;
            mainwin.start({ core.ms.x, core.ms.y });
            if (mainwin.winsize.x != mainwin.old_winsize.x || mainwin.winsize.y != mainwin.old_winsize.y)
                camera.fb.resize({ int(float(mainwin.winsize.x) * float(camera.fb.fb_size.y) / float(mainwin.winsize.y)), core.window.scrs.y }), mainwin.old_winsize = mainwin.winsize;
            core.process(&camera);
            core.render(&camera);

            camera.fb.add_use();
            if (mode == 0 && mainwin.is_hovered) {

                gameobjs::Layer& c_layer = core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer];

                pos2ds c_tile_pos = core.mst(selected_layer, mainwin.mspos, mainwin.winsize);
                tiles::t_tile& c_ttile = core.global_res.tilesets[selected_tile.tset_value].tiles[selected_tile.tile_value];
                texture::basic_texture& c_txt = *c_ttile.tile_texture.get_curr_frame();

                pos2d c_layer_pos = { int(-c_layer.parallax_speed.x * camera.center.x) + c_layer.tmap.offset.x, int(-c_layer.parallax_speed.y * camera.center.y) + c_layer.tmap.offset.y };

                core.drawtexture(c_ttile.tile_texture.get_curr_frame(), { c_tile_pos.x * c_layer.tmap.grid_size.x + c_layer.tmap.grid_size.x / 2 - c_layer_pos.x, c_tile_pos.y * c_layer.tmap.grid_size.y + c_layer.tmap.grid_size.y / 2 - c_layer_pos.y }, 0.5, c_txt.rotation_origin, c_txt.scale, c_txt.rotation);
                core.drawrect({ c_tile_pos.x * c_layer.tmap.grid_size.x - c_layer_pos.x, c_tile_pos.y * c_layer.tmap.grid_size.y - c_layer_pos.y }, 
                              { c_tile_pos.x * c_layer.tmap.grid_size.x + c_layer.tmap.grid_size.x - c_layer_pos.x, c_tile_pos.y * c_layer.tmap.grid_size.y + (c_layer.tmap.grid_size.y - 1) - c_layer_pos.y });

                tiles::Tilemap& curr_map = core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer].tmap;

                tiles::tile picked_tile = curr_map.lookup(c_tile_pos);

                if (picker.check() && (picked_tile.tset_value || (!picked_tile.tset_value && picked_tile.tile_value))) {
                    selected_tile = picked_tile;
                    selected_tile.tset_value--;
                }

                if (core.ms.lb) curr_map.put_value_and_resize(c_tile_pos, { selected_tile.tile_value, jus(selected_tile.tset_value + 1), selected_tile.is_local });
                if (core.ms.rb) curr_map.put_value_and_resize(c_tile_pos, { 0, 0, 0 });

            }

            else if (mode == 1 && mainwin.is_hovered) {
                gameobjs::Layer& c_layer = core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer];
                pos2ds ctile = core.mst(selected_layer, mainwin.mspos, mainwin.winsize);
                tiles::t_tile& cttile = core.global_res.tilesets[selected_tile.tset_value].tiles[selected_tile.tile_value];
                core.drawtexture(cttile.tile_texture.get_curr_frame(), { ctile.x * c_layer.tmap.grid_size.x + c_layer.tmap.grid_size.x / 2, ctile.y * c_layer.tmap.grid_size.y + c_layer.tmap.grid_size.y / 2 }, 0.5, cttile.tile_texture.base_frame.rotation_origin, cttile.tile_texture.base_frame.scale, cttile.tile_texture.base_frame.rotation);
                core.drawrect({ ctile.x * c_layer.tmap.grid_size.x, ctile.y * c_layer.tmap.grid_size.y }, { ctile.x * c_layer.tmap.grid_size.x + c_layer.tmap.grid_size.x, ctile.y * c_layer.tmap.grid_size.y + (c_layer.tmap.grid_size.y - 1) });
                //core.ms.once();
                //if (core.ms.lb) 
                    //core.scenes[0].rooms[0].get_obj_from_pointer(0)->pos.x += 2;
                //if (core.ms.rb)
                    //core.scenes[0].rooms[0].get_obj_from_pointer(0)->active_animation_id = 0, core.scenes[0].rooms[0].get_obj_from_pointer(0)->current_frame_id = 0;
                    //core.create_obj(core.screen_coords(mainwin.mspos, mainwin.winsize), 0, 0, 0);

            }

            camera.fb.end_render(core.window.wins);
            mainwin.end();
            End();
        }

        static bool add_textures_window = 0, is_image_loaded;
        static int target_type = 0, target_id = 0;
        static texture::texture tmp_txt;

        /*
        imgselect.fb = &imgfb;
        if (add_textures_window) {
            
            Begin(u8"Добавить текстуры", &add_textures_window);
            if (!is_image_loaded) {
                static string path;
                //Checkbox(u8"Показывать границы тайловой карты", &core.show_tmap_border_lines);
                //Text(path.c_str());
                InputText(u8"##add_textures", &path);
                SameLine();
                if (Button(u8"Путь")) {
                    path = pfd::open_file(u8"Выберите текстуру", pfd::path::home(), { u8"Изображения", u8"*.png *.bmp *.jpg *.jpeg *.gif" }).result()[0];
                }
                if (Button(u8"Подтвердить")) tmp_txt.loadtexture(path), is_image_loaded = 1;
            }
            else {
                //Image(ImTextureID(tmp_txt.id), ImVec2(100, 100));
                imgselect.start({ core.ms.x, core.ms.y });
                core.drawtexture(&tmp_txt, { 32, 32 }, 1);
                imgselect.end();
            }
            End();
        }
        */

        Begin(u8"Менеджер ресурсов", NULL, ImGuiWindowFlags_NoCollapse);
        ImGui::BeginTabBar(u8"Виды ресурсов");
        static bool is_local;

        if (BeginTabItem(u8"Тайлсеты", NULL, ImGuiTabItemFlags_NoCloseButton)) {
            Text(u8"Отладочная информация:");
            EndTabItem();
        }
        ImGui::EndTabBar();
        ImGui::End();

        //Слои
        {
            Begin(u8"Слои", NULL, ImGuiWindowFlags_NoCollapse);
            gameobjs::room* c_room = core.get_curr_room();


            for (int i = 0; i < c_room->Layers.size(); i++) {
                if (Selectable(string(c_room->Layers[i].name + (c_room->Layers[i].hidden ? u8"(Скрыт)" : "") + "##layer" + to_string(i)).c_str(), i == selected_layer)) {
                    if (selected_layer == i) {
                        c_room->Layers[i].hidden = !c_room->Layers[i].hidden;
                    }
                    selected_layer = i;
                    
                }

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)){
                    ImGui::SetDragDropPayload("Layer DND", &i, sizeof(int));
                    Text(u8"Поменять местами слои");
                    ImGui::EndDragDropSource();
                }

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
                if (ImGui::BeginPopupContextItem()){
                    gameobjs::Layer* popup_layer = &core.get_curr_room()->Layers[i];

                    Text(string(static_cast<char>(34) + popup_layer->name + static_cast<char>(34)).c_str());
                    SameLine();
                    if (ImGui::Button(u8"Переименовать##rename_layer"))
                        call_rename_popup = 1, rename_target = &c_room->Layers[i].name;

                    if (i) {
                        if (Button(u8"Удалить")) {
                            popup_layer->tmap.finish();
                            c_room->Layers.erase((c_room->Layers.begin() + i));
                            CloseCurrentPopup();
                            if (selected_layer == c_room->Layers.size()) selected_layer--;
                        }
                        SameLine();
                    }
                    if (Button(u8"Очистить"))
                        popup_layer->tmap.clear();

                    Text(u8"Параллакс-скроллинг:");
                    pos2d winsize = mg.get_window_size();
                    ImGuiStyle st = GetStyle();
                    PushItemWidth((winsize.x - st.ItemSpacing.x) / 2);
                    DragFloat("##parallax_x", &popup_layer->parallax_speed.x, 0.1f, 0.0, 0.0, "x: %.3f");
                    SameLine();
                    DragFloat("##parallax_y", &popup_layer->parallax_speed.y, 0.1f, 0.0, 0.0, "y: %.3f");
                    PopItemWidth();
                    PushItemWidth(winsize.x);
                    SliderFloat(u8"##layer_transparency", &popup_layer->transparency, 0, 1, u8"Прозрачность: %.3f", ImGuiSliderFlags_AlwaysClamp);
                    PopItemWidth();
                    if (TreeNode(u8"Тайловая карта")) {
                        //Text(to_string(st.IndentSpacing).c_str());
                        //PushItemWidth(winsize.x - st.IndentSpacing);
                        if (Button(u8"Импортировать##import_tmap", ImVec2((winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 2, 0))) {

                        }
                        SameLine();
                        if (Button(u8"Экспортировать##export_tmap", ImVec2((winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 2, 0))) {

                        }
                        NewLine();

                        PushItemWidth((winsize.x - st.IndentSpacing - st.ItemSpacing.x) / 2);

                        Text(u8"Размер сетки");
                        pos2d gridsize = { popup_layer->tmap.grid_size.x, popup_layer->tmap.grid_size.y };
                        DragInt("##gridsize_x", &gridsize.x, 0.1f, 0, 0, "x: %d");
                        SameLine();
                        DragInt("##gridsize_y", &gridsize.y, 0.1f, 0, 0, "y: %d");
                        popup_layer->tmap.grid_size = {static_cast<unsigned short>(gridsize.x), 
                                                       static_cast<unsigned short>(gridsize.y)};
                        
                        Text(u8"Смещение");
                        pos2d offset = { popup_layer->tmap.offset.x, popup_layer->tmap.offset.y };
                        SliderInt("##offset_x", &offset.x, 0, gridsize.x, "x: %d", ImGuiSliderFlags_AlwaysClamp);
                        SameLine();
                        SliderInt("##offset_y", &offset.y, 0, gridsize.y, "y: %d", ImGuiSliderFlags_AlwaysClamp);
                        popup_layer->tmap.offset = { static_cast<unsigned short>(offset.x),
                                                     static_cast<unsigned short>(offset.y) };
                        PopItemWidth();

                        NewLine();
                        Text(string(u8"Размер: " + to_string(popup_layer->tmap.gnwidth() + popup_layer->tmap.gwidth()) + u8" на " + to_string(popup_layer->tmap.gnheigh() + popup_layer->tmap.gheigh()) + u8" тайлов").c_str());
                        TreePop();
                    }
                    Separator();
                    EndPopup();
                }
            }
            if (Button(string(u8"Добавить слой").c_str())) {
                c_room->Layers.resize(core.get_curr_room()->Layers.size() + 1);
                c_room = core.get_curr_room();
                c_room->Layers.back().name = u8"Слой " + to_string(c_room->Layers.size());
            }
            ImGui::End();
        }


        {
            Begin(u8"Выбор тайлов", NULL, ImGuiWindowFlags_NoCollapse);
            //BeginChild(u8"##tile_selection", ImVec2(0, 400));
            //for (int a = 0; a < core.global_res.tilesets.size(); a++) {
            {

                Text(u8"Имя тайлсета:");
                tiles::tileset& c_tileset = core.global_res.tilesets[selected_tile.tset_value];
                InputText(string(u8"##tileset_id" + to_string(selected_tile.tset_value)).c_str(), &c_tileset.in_editor_name);
                static int hovered_tile;
                bool was_hovered_in_last_frame = 0;

                for (int i = 0; i < c_tileset.tiles.size(); i++) {
                    int dist = 4, items_per_row = 3;
                    tiles::t_tile& c_tile = c_tileset.tiles[i];
                    PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(dist, dist));
                    pos2d winsize = mg.get_window_size();
                    if (ImageButton(ImTextureID(c_tile.tile_texture.get_curr_frame()->texture_pointer), ImVec2((winsize.x - dist * items_per_row) / items_per_row, (winsize.x - dist * items_per_row) / items_per_row), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, i == selected_tile.tile_value ? 0.5 : (1 - (0.3 * (hovered_tile == i))))))
                        selected_tile.tile_value = i;

                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                        // Set payload to carry the index of our item (could be anything)
                        ImGui::SetDragDropPayload("Tile DND", &i, sizeof(int));
                        Text(u8"Поменять местами тайлы");
                        ImGui::EndDragDropSource();
                    }

                    if (ImGui::BeginDragDropTarget()) {

                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Tile DND")){
                            int payload_n = *(int*)payload->Data;
                            tiles::t_tile tmp = c_tile;
                            gameobjs::resources& res = selected_tile.is_local ? core.scenes[core.c_scene_id].local_res : core.global_res;
                            c_tile = res.tilesets[selected_tile.tset_value].tiles[payload_n];
                            res.tilesets[selected_tile.tset_value].tiles[payload_n] = tmp;
                            core.get_curr_room()->Layers[selected_layer].tmap.swap_replace({jus(i), jus(selected_tile.tset_value + 1), selected_tile.is_local}, {jus(payload_n), jus(selected_tile.tset_value + 1), selected_tile.is_local});
                        }
                        ImGui::EndDragDropTarget();
                    }

                    if (ImGui::BeginPopupContextItem()){
                        call_texture_properties_popup = 1;
                        popup_texture_id = selected_tile.tset_value;
                        popup_texture_id2 = i;
                        CloseCurrentPopup();
                        EndPopup();
                        /*
                        Text(u8"Параметры тайла:");
                        gameobjs::resources& res = selected_tile.is_local ? core.scenes[core.c_scene_id].local_res : core.global_res;
                        texture::basic_texture& curr_txt = *res.tilesets[selected_tile.tset_value].tiles[i].tile_texture.get_curr_frame();
                        
                        Image(ImTextureID(curr_txt.texture_pointer), ImVec2(64, 64));
                        Text(u8"Вращение: ");
                        ImGuiStyle st = GetStyle();
                        SliderFloat(string(u8"##tile_rotation").c_str(), &res.tilesets[selected_tile.tset_value].tiles[i].tile_texture.get_curr_frame()->rotation, -360, 360);

                        Text(u8"Центр вращения: ");
                        PushItemWidth((mg.get_window_size().x - st.ItemSpacing.x) / 2);
                        SliderInt(string(u8"##tileorg_x").c_str(), &curr_txt.rotation_origin.x, -(curr_txt.original_size.x + curr_txt.scale.x) / 2, (curr_txt.original_size.x + curr_txt.scale.x) / 2, u8"x: %d");
                        SameLine();
                        SliderInt(string(u8"##tileorg_y").c_str(), &curr_txt.rotation_origin.y, -(curr_txt.original_size.y + curr_txt.scale.y) / 2, (curr_txt.original_size.y + curr_txt.scale.y) / 2, u8"y: %d");
                        Text(u8"Смещение");
                        SliderInt(string(u8"##tileoff_x").c_str(), &curr_txt.offset.x, -(curr_txt.original_size.x + curr_txt.scale.x), curr_txt.original_size.x + curr_txt.scale.x, u8"x: %d");
                        SameLine();
                        SliderInt(string(u8"##tileoff_y").c_str(), &curr_txt.offset.y, -(curr_txt.original_size.y + curr_txt.scale.y), curr_txt.original_size.y + curr_txt.scale.y, u8"y: %d");
                        Text(u8"Размер");
                        SliderFloat(string(u8"##tilesize_x").c_str(), &curr_txt.scale.x, 0, 2, u8"x: %.3f");
                        SameLine();
                        SliderFloat(string(u8"##tilesize_y").c_str(), &curr_txt.scale.y, 0, 2, u8"y: %.3f");
                        PopItemWidth();
                        if (ImGui::Button(u8"Закрыть"))
                            ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();*/
                    }
                    if (IsItemHovered()) hovered_tile = i, was_hovered_in_last_frame = 1;
                    if ((i + 1) % items_per_row != 0) ImGui::SameLine();
                    if (i == c_tileset.tiles.size() - 1) {
                        //if (Button(u8"Добавить##add_tiles")) add_textures_window = 1, target_type = 1, target_id = a;//0 = texture, 1 = tileset, 2 = animation
                    }
                    PopStyleVar();
                    PopStyleVar();
                }
                if (!was_hovered_in_last_frame) hovered_tile = -1;
            }
        }

        ImGui::End();
        cam2d& camera = core.get_curr_room()->cams[core.get_curr_room()->selected_cam];

        if (save.check()) layerdata = core.scenes[0].rooms[0].Layers[0].tmap.serialize();
        if (load.check()) core.scenes[0].rooms[0].Layers[0].tmap.deserialize(layerdata);
        if (core.ms.wheel && (mainwin.is_hovered))
            camera.zoom -= core.ms.wheel * 0.1;// , camera.center = core.screen_coords(mspos, winsize);

        
        if (d.check() && mainwin.is_hovered) camera.center.x += int(4 / camera.zoom);
        if (a.check() && mainwin.is_hovered) camera.center.x -= int(4 / camera.zoom);
        if (w.check() && mainwin.is_hovered) camera.center.y += int(4 / camera.zoom);
        if (s.check() && mainwin.is_hovered && !save.pressed) camera.center.y -= int(4 / camera.zoom);
        if (obj_mode.check()) {
            if (mode == 0) {
                mode = 1;
            }
            else mode = 0;
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        core.on_frame_end();
        core.window.endRender();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}






















/*

        if (load.check()) {
            wstring load_path, sh_be = L"spisok", shitfuck;
            wcin >> load_path;
            if (load_path == L"ekran") {
                int height;
                cin >> height;
                window.scrs.y = height;
                window.update();
            }
            if (load_path == L"spisok") {
                wcout << L"Список обьектов:" << endl;
                wcout << L"Слои:" << endl;
                for (int i = 0; i < sc.Layers.size(); i++) wcout << L"Индекс слоя: " << i << L", имя слоя: \42" << sc.Layers[i].name << L"\42, размер тайловой карты: " << sc.Layers[i].tmap.gwidth() << L"x" << sc.Layers[i].tmap.gheigh() << endl;
                wcout << L"Текстурные папки:" << endl;
                for (int i = 0; i < sc.tf.size(); i++) {
                    wcout << L"Индекс папки: " << i << L", имя: \42" << sc.tf[i].name << L"\42, количество текстур: " << sc.tf[i].textures.size() << endl;
                    wcout << L"Текстуры в папке:" << endl;
                    for (int b = 0; b < sc.tf[i].textures.size(); b++) {
                        wcout << L"    Индекс текстуры: " << b << L", имя: \42" << sc.tf[i].textures[b].name << L"\42, путь: \42" << stws(sc.tf[i].textures[b].pth) << L"\42, размер (в пикселях): " << sc.tf[i].textures[b].size.x << L"x" << sc.tf[i].textures[b].size.y << endl;
                    }
                }
                wcout << L"Тайлсеты:" << endl;
                for (int i = 0; i < sc.tsets.size(); i++) {
                    wcout << L"Индекс тайлсета: " << i << L", имя: \42" << sc.tsets[i].name << L"\42, количество видов тайлов: " << sc.tsets[i].tiles.size() << endl;
                    for (int b = 0; b < sc.tf[i].textures.size(); b++) {
                        wcout << L"    Индекс текстуры: " << b << L", имя: \42" << sc.tf[i].textures[b].name << L"\42, путь: \42" << stws(sc.tf[i].textures[b].pth) << L"\42, размер (в пикселях): " << sc.tf[i].textures[b].size.x << L"x" << sc.tf[i].textures[b].size.y << endl;
                    }
                }
            }

            if (load_path == L"perecluchit"){
                wcin >> load_path;
                if (load_path == L"sloy")
                    wcin >> selected_layer;
                if (load_path == L"tset") {
                    wcin >> selected_tile.tset_value; selected_tile = 0;
                }
                if (load_path == L"pf")
                    wcin >> selected_pf;
            }

            if (load_path == L"dobavit") {
                wcin >> load_path;
                if (load_path == L"sloy") {
                    sc.add(layer);
                    pos2d scale;
                    wcin >> scale.x >> scale.y;
                    sc.Layers[sc.Layers.size() - 1].tmap.cnew(scale.x, scale.y);
                }
                if (load_path == L"teksturu") {
                    int folder;
                    wcin >> folder;
                    if (folder > sc.tf.size() || folder < 0)
                        wcout << L"Введен неверный индекс папки: выход за границы доступных вариантов. Используйте команду \42spisok\42 чтобы посмотреть возможные варианты" << endl;
                    else {
                        string path;
                        cin >> path;
                        sc.tf[folder].add(path);
                    }
                }

                if (load_path == L"tset")
                    sc.add(tset);

                if (load_path == L"tayl") {
                    int tset_id;
                    wcin >> tset_id;
                    if (tset_id > sc.tsets.size() || tset_id < 0)
                        wcout << L"Введен неверный индекс тайлсета: выход за границы доступных вариантов. Используйте команду \42spisok\42 чтобы посмотреть возможные варианты" << endl;
                    else {
                        int folder, id;
                        wcin >> folder >> id;
                        if (folder > sc.tf.size() || folder < 0)
                            wcout << L"Введен неверный индекс папки: выход за границы доступных вариантов. Используйте команду \42spisok\42 чтобы посмотреть возможные варианты" << endl;
                        else {
                            if (id > sc.tf[folder].textures.size() || id < 0)
                                wcout << L"Введен неверный индекс папки: выход за границы доступных вариантов. Используйте команду \42spisok\42 чтобы посмотреть возможные варианты" << endl;
                            else
                                sc.tsets[tset_id].add(folder, id);
                        }
                    }
                }
                if (load_path == L"tpapku")
                    sc.add(tFolder);
                if (load_path == L"pf")
                    sc.add(pFolder);
                if (load_path == L"prefab") {
                    int pf_id;
                    cin >> pf_id;

                }

            }

            if (load_path == L"izmenit_razmer_taylovoy_karty") {
                int layer; pos2d size;
                wcin >> layer >> size.x >> size.y;
                if (layer > sc.Layers.size() || layer < 0)
                    wcout << L"Введен неверный индекс слоя: выход за границы доступных вариантов. Используйте команду \42spisok\42 чтобы посмотреть возможные варианты" << endl;
                else
                    sc.Layers[layer].tmap.resize(size.x, size.y);
            }

            if (load_path == L"privyazat") {
                int tset_id;
                wcin >> tset_id;
                if (tset_id > sc.tsets.size() || tset_id < 0)
                    wcout << L"Введен неверный индекс тайлсета: выход за границы доступных вариантов. Используйте команду \42spisok\42 чтобы посмотреть возможные варианты" << endl;
                else {
                    int folder, id, tile_id;
                    wcin >> tile_id >> folder >> id;
                    if (tile_id > sc.tsets[tset_id].tiles.size())
                        wcout << L"Введен неверный индекс тайла: выход за границы доступных вариантов. Используйте команду \42spisok\42 чтобы посмотреть возможные варианты" << endl;
                    else {
                        if (folder > sc.tf.size() - 1 || folder < 0)
                            wcout << L"Введен неверный индекс папки: выход за границы доступных вариантов. Используйте команду \42spisok\42 чтобы посмотреть возможные варианты" << endl;
                        else {
                            if (id > sc.tf[folder].textures.size() - 1 || id < 0)
                                wcout << L"Введен неверный индекс папки: выход за границы доступных вариантов. Используйте команду \42spisok\42 чтобы посмотреть возможные варианты" << endl;
                            else
                                sc.tsets[tset_id].tiles[tile_id].folder = folder, sc.tsets[tset_id].tiles[tile_id].id = id;
                        }
                    }
                }
            }
            //sc.tf[0].add(load_path); sc.tsets[0].add(0, sc.tf[0].textures.size() - 1);
        }*/

/*
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

using namespace std;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

position window_size, screen_size;
float asp_ratio;
FramebufferObject1 fb;

float Vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1,  1,        0, 1,
    -1, -1,        0, 0,
     1, -1,        1, 0,

    -1,  1,        0, 1,
     1, -1,        1, 0,
     1,  1,        1, 1
};

int main()
{
    string path = "Textures/ball.png";
    window_size.x = 800, window_size.y = 800;
    screen_size.x = 200, screen_size.y = 100;
    asp_ratio = window_size.y / screen_size.y;
    screen_size.x = window_size.x / asp_ratio;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(window_size.x, window_size.y, "OpenGL test", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Shader objShader(L"shaders/vert2.vs", L"shaders/frag1.fs");
    vertex_attribute* a = new vertex_attribute[2];
    a[0].amount_of_attr = 2, a[1].amount_of_attr = 2;

    unsigned int texture, VAO, VBO;
    float rotation = 30.0f, scalex = 0.0f, scaley = 0.0f;
    int width, height;
    float x = 10.0f, y = 10.0f;
    //unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    generate_vertattibs(2, a);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);


    fb.CreateFramebuffer(screen_size);
    //Sprite a;
    //a.init();
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   //a.loadtexture("Textures/ball.png");
    //glm::mat4 view = glm::mat4(1);


    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        glViewport(0, 0, screen_size.x, screen_size.y);
        fb.use();
        glClearColor(0, 0.5, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        /*projection = glm::ortho(0.0f, (float)screen_size.x, (float)screen_size.y, 0.0f, -1.0f, 1.0f);
        objShader.use();
        objShader.setMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, y, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

        //model = glm::translate(model, glm::vec3(0.5f * width, 0.5f * height, 0.0f)); // move origin of rotation to center of quad
        //model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
        //model = glm::translate(model, glm::vec3(-0.5f * width, -0.5f * height, 0.0f)); // move origin back

        //model = glm::scale(model, glm::vec3(50, 50, 0)); // last scale
        //objShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        //a.render(&objShader, screen_size);

        fb.render(window_size);
        glUseProgram(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    window_size.y = height, window_size.x = width;
    asp_ratio = window_size.y / screen_size.y;
    screen_size.x = window_size.x / asp_ratio;
    fb.resize(screen_size);
}*/

