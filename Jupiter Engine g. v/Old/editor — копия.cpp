#pragma warning(disable : 4996)
#define EDITOR
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <glad/glad.h>
#include <iostream>
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

using namespace std; using namespace ImGui;

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
        core.global_res.tilesets[0].tiles[i].tile_texture.base_frame.resize(1),
        core.global_res.tilesets[0].tiles[i].tile_texture.base_frame[0].load_from_raw(tmp[i], {16, 16});
    
    core.global_res.prefabs.resize(1);
    core.global_res.prefabs[0].txt.base_frame.resize(1);
    core.global_res.prefabs[0].txt.base_frame[0].load(u8"caine_1.png");
    core.global_res.prefabs[0].txt.base_frame[0].original_size = {48, 128};
    core.global_res.prefabs[0].txt.animations.resize(1);
    core.global_res.prefabs[0].txt.animations[0].resize(1);
    core.global_res.prefabs[0].txt.animations[0][0].frames.resize(5);
    
    core.global_res.tmp_textures[1].load(u8"anim.png");
    vector<string> tmp_anim = texture_processing::crop_from_croplist(core.global_res.tmp_textures[1].get_texture(), texture_processing::generate_croplist(&core.global_res.tmp_textures[1], 5, {64, 64}), core.global_res.tmp_textures[1].original_size);
    for (int i = 0; i < 5; i++) {
        core.global_res.prefabs[0].txt.animations[0][0].frames[i].frame.load_from_raw(tmp_anim[i], { 64, 64 });
        core.global_res.prefabs[0].txt.animations[0][0].frames[i].duration = 0.07;
    }
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
    while (core.window.checkWindow()) {
        //core.update();
        core.on_frame_start();
        cam2d& camera = core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].cams[core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].selected_cam];
        tiles::t_tile& ctile = core.global_res.tilesets[selected_tile.tset_value].tiles[selected_tile.tile_value];

        mg.update();
        //core.scenes[0].rooms[0].get_obj_from_pointer(0)->rotation += 2;
        // Dockspace
        if (mg.io->ConfigFlags & ImGuiConfigFlags_DockingEnable){
            ImGuiID dockspace_id = ImGui::GetID("Root");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), mg.dockspace_flags);

            static bool first_time = true;
            if (first_time){
                first_time = false;

                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, mg.dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, mg.viewport->Size);
                ImGui::DockBuilderGetCentralNode(dockspace_id);
                auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.17f, nullptr, &dockspace_id);
                auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f, nullptr, &dockspace_id);

                ImGui::DockBuilderDockWindow(u8"Слои", dock_id_right);
                ImGui::DockBuilderDockWindow(u8"Менеджер ресурсов", dock_id_right);
                ImGui::DockBuilderDockWindow(u8"Выбор тайлов", dock_id_left);

                ImGui::DockBuilderDockWindow(u8"Главное окно", dockspace_id);
                
                ImGui::DockBuilderFinish(dockspace_id);
            }
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
            if (Button(u8"Переключить режим")) {
                mode = !mode;
            }
            End();
        }



        Begin(u8"Главное окно", NULL, ImGuiWindowFlags_NoCollapse);
        static fb_widget mainwin;
        mainwin.fb = &camera.fb;
        mainwin.start({ core.ms.x, core.ms.y });
        if (mainwin.winsize.x != mainwin.old_winsize.x || mainwin.winsize.y != mainwin.old_winsize.y)
            camera.fb.resize({ int(float(mainwin.winsize.x) * float(camera.fb.fb_size.y) / float(mainwin.winsize.y)), core.window.scrs.y}), mainwin.old_winsize = mainwin.winsize;
        core.process(&camera);
        core.render(&camera);

        camera.fb.add_use();
        if (mode == 0 && mainwin.is_hovered) {
            gameobjs::Layer& clayer = core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer];
            pos2ds c_tile_pos = core.mst(selected_layer, mainwin.mspos, mainwin.winsize);
            tiles::t_tile& cttile = core.global_res.tilesets[selected_tile.tset_value].tiles[selected_tile.tile_value];

            core.drawtexture(cttile.tile_texture.get_curr_frame(), {c_tile_pos.x * clayer.tmap.grid_size.x + clayer.tmap.grid_size.x / 2 - int(clayer.parallax_speed.x * camera.center.x), c_tile_pos.y * clayer.tmap.grid_size.y + clayer.tmap.grid_size.y / 2 - int(clayer.parallax_speed.y * camera.center.y) }, 0.5, cttile.tile_texture.base_frame[cttile.tile_texture.variant].rotation_origin, cttile.tile_texture.base_frame[cttile.tile_texture.variant].scale, cttile.tile_texture.base_frame[cttile.tile_texture.variant].rotation);
            core.drawrect({ c_tile_pos.x * clayer.tmap.grid_size.x - int(clayer.parallax_speed.x * camera.center.x), c_tile_pos.y * clayer.tmap.grid_size.y - int(clayer.parallax_speed.y * camera.center.y) }, { c_tile_pos.x * clayer.tmap.grid_size.x + clayer.tmap.grid_size.x - int(clayer.parallax_speed.x * camera.center.x), c_tile_pos.y * clayer.tmap.grid_size.y + (clayer.tmap.grid_size.y - 1) - int(clayer.parallax_speed.y * camera.center.y) });

            tiles::Tilemap& curr_map = core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer].tmap;

            tiles::tile picked_tile = curr_map.lookup(c_tile_pos);

            if (picker.check() && (picked_tile.tset_value || (!picked_tile.tset_value && picked_tile.tile_value))) {
                selected_tile = picked_tile;
                selected_tile.tset_value--;
            }
            if (core.ms.lb) curr_map.put_value_and_resize(c_tile_pos, selected_tile.tile_value, selected_tile.tset_value + 1, selected_tile.is_local);
            if (core.ms.rb) curr_map.put_value_and_resize(c_tile_pos, 0, 0);

        }

        if (mode == 1 && mainwin.is_hovered) {
            gameobjs::Layer& clayer = core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer];
            pos2ds ctile = core.mst(selected_layer, mainwin.mspos, mainwin.winsize);
            tiles::t_tile& cttile = core.global_res.tilesets[selected_tile.tset_value].tiles[selected_tile.tile_value];
            core.drawtexture(cttile.tile_texture.get_curr_frame(), { ctile.x * clayer.tmap.grid_size.x + clayer.tmap.grid_size.x / 2, ctile.y * clayer.tmap.grid_size.y + clayer.tmap.grid_size.y / 2 }, 0.5, cttile.tile_texture.base_frame[cttile.tile_texture.variant].rotation_origin, cttile.tile_texture.base_frame[cttile.tile_texture.variant].scale, cttile.tile_texture.base_frame[cttile.tile_texture.variant].rotation);
            core.drawrect({ ctile.x * clayer.tmap.grid_size.x, ctile.y * clayer.tmap.grid_size.y }, { ctile.x * clayer.tmap.grid_size.x + clayer.tmap.grid_size.x, ctile.y * clayer.tmap.grid_size.y + (clayer.tmap.grid_size.y - 1) });
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

        //Слои
        {
            Begin(u8"Слои", NULL, ImGuiWindowFlags_NoCollapse);
            gameobjs::Layer* clayer = &core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer];
            for (int i = 0; i < core.scenes[0].rooms[0].Layers.size(); i++) {
                RadioButton(string("##layer_select" + to_string(i)).c_str(), (int*)&selected_layer, i), SameLine();
                InputText(string(u8"##layer_name" + to_string(i)).c_str(), &core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[i].name);
                if (i == selected_layer) {
                    Text(string(u8"Индекс: " + to_string(i)).c_str());
                    if (i != 0) {
                        if (ArrowButton(u8"##swap_layers_u", ImGuiDir_Up)) {
                            gameobjs::Layer tmp = *clayer;
                            *clayer = core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer - 1];
                            core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer - 1] = tmp;
                            selected_layer--;
                        }
                    }
                    if (i < core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers.size() - 1) {
                        if (i > 0) SameLine();
                        if (ArrowButton(u8"##swap_layers_d", ImGuiDir_Down)) {
                            gameobjs::Layer tmp = *clayer;
                            *clayer = core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer + 1];
                            core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer + 1] = tmp;
                            selected_layer++;
                        }
                    }

                    SameLine();
                    Checkbox(string(u8"Скрыть").c_str(), &core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers[selected_layer].hidden);
                    if (TreeNode(u8"Тайловая карта")) {
                        if (Button(u8"Импортировать##import_tmap")) {

                        }
                        if (Button(u8"Экспортировать##export_tmap")) {

                        }
                        NewLine();
                        Text(u8"Размер сетки");
                        int gs[2] = { clayer->tmap.grid_size.x, clayer->tmap.grid_size.y };
                        InputInt2("##grid_size", gs);
                        clayer->tmap.grid_size.x = gs[0], clayer->tmap.grid_size.y = gs[1];
                        Text(u8"Смещение");
                        int os[2] = { clayer->tmap.offset.x, clayer->tmap.offset.y };
                        InputInt2("##offset", os);
                        clayer->tmap.offset.x = os[0], clayer->tmap.offset.y = os[1];
                        NewLine();
                        Text(string(u8"Размер: " + to_string(clayer->tmap.gnwidth() + clayer->tmap.gwidth()) + u8" на " + to_string(clayer->tmap.gnheigh() + clayer->tmap.gheigh()) + u8" тайлов").c_str());
                        TreePop();
                    }

                    float ps[2] = { clayer->parallax_speed.x, clayer->parallax_speed.y };
                    Text(u8"Параллакс-скроллинг");
                    InputFloat2("##parallax", ps);
                    clayer->parallax_speed.x = ps[0];
                    clayer->parallax_speed.y = ps[1];


                    if (i != 0) {
                        if (Button(u8"Удалить")) {
                            clayer->tmap.finish();
                            //core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers.pop_back();
                            core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers.erase(core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers.begin() + selected_layer);
                            if (selected_layer >= core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers.size())
                                selected_layer--;
                        }
                        SameLine();
                    }
                    if (Button(u8"Очистить"))
                        clayer->tmap.clear();
                    Separator();
                }
            }
            if (Button(string(u8"Добавить слой").c_str())) {
                core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers.resize(core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers.size() + 1);
                core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers.back().name = u8"Слой " + to_string(core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].Layers.size());
            }
            ImGui::End();
        }

        Begin(u8"Менеджер ресурсов", NULL, ImGuiWindowFlags_NoCollapse);
        ImGui::BeginTabBar(u8"Виды ресурсов");
        static bool is_local;

        if (BeginTabItem(u8"Тайлсеты", NULL, ImGuiTabItemFlags_NoCloseButton)) {

            EndTabItem();
        }
        ImGui::EndTabBar();
        ImGui::End();

        Begin(u8"Выбор тайлов", NULL, ImGuiWindowFlags_NoCollapse);
        BeginChild(u8"##tile_selection", ImVec2(0, 400));
        for (int a = 0; a < core.global_res.tilesets.size(); a++) {
            Text(u8"Имя тайлсета:");
            InputText(string(u8"##tileset_id" + to_string(a)).c_str(), &core.global_res.tilesets[a].in_editor_name);
            static int hovered_tile;
            bool was_hovered_in_last_frame = 0;
            //int old_hovered = hovered_tile;
            for (int i = 0; i < core.global_res.tilesets[a].tiles.size(); i++) {
                PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                if (ImageButton(ImTextureID(core.global_res.tilesets[a].tiles[i].tile_texture.base_frame[core.global_res.tilesets[a].tiles[i].tile_texture.variant].texture_pointer), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, a == selected_tile.tset_value && i == selected_tile.tile_value ? 0.5 : (1 - (0.3 * (hovered_tile == i)))))) selected_tile.tile_value = i, selected_tile.tset_value = a;
                if (IsItemHovered()) hovered_tile = i, was_hovered_in_last_frame = 1;
                if ((i + 1) % 3 != 0) ImGui::SameLine();
                if (i == core.global_res.tilesets[a].tiles.size() - 1) {
                    if (Button(u8"Добавить##add_tiles")) add_textures_window = 1, target_type = 1, target_id = a;//0 = texture, 1 = tileset, 2 = animation
                }
                PopStyleVar();
            }
            if (!was_hovered_in_last_frame) hovered_tile = -1;
            NewLine();
            Separator();
        }
        EndChild();
        Separator();

        texture::basic_texture& curr_txt = *ctile.tile_texture.get_curr_frame();

        Text(u8"Параметры текстуры:");
        Image(ImTextureID(curr_txt.texture_pointer), ImVec2(64, 64));
        Text(u8"Вращение: ");

        SliderFloat(string(u8"##tile_rotation" + to_string(selected_tile.tset_value) + to_string(selected_tile.tile_value)).c_str(), &core.global_res.tilesets[selected_tile.tset_value].tiles[selected_tile.tile_value].tile_texture.base_frame[core.global_res.tilesets[selected_tile.tset_value].tiles[selected_tile.tile_value].tile_texture.variant].rotation, 0, 360);

        Text(u8"Центр вращения: ");
        PushItemWidth(mg.get_window_size().x / 2);
        SliderInt(string(u8"##tileorg_x" + to_string(selected_tile.tset_value) + to_string(selected_tile.tile_value)).c_str(), &curr_txt.rotation_origin.x, -(curr_txt.original_size.x + curr_txt.scale.x) / 2, (curr_txt.original_size.x + curr_txt.scale.x) / 2);
        SameLine();
        SliderInt(string(u8"##tileorg_y" + to_string(selected_tile.tset_value) + to_string(selected_tile.tile_value)).c_str(), &curr_txt.rotation_origin.y, -(curr_txt.original_size.y + curr_txt.scale.y) / 2, (curr_txt.original_size.y + curr_txt.scale.y) / 2);
        Text(u8"Смещение");
        SliderInt(string(u8"##tileoff_x" + to_string(selected_tile.tset_value) + to_string(selected_tile.tile_value)).c_str(), &curr_txt.offset.x, -(curr_txt.original_size.x + curr_txt.scale.x), curr_txt.original_size.x + curr_txt.scale.x);
        SameLine();
        SliderInt(string(u8"##tileoff_y" + to_string(selected_tile.tset_value) + to_string(selected_tile.tile_value)).c_str(), &curr_txt.offset.y, -(curr_txt.original_size.y + curr_txt.scale.y), curr_txt.original_size.y + curr_txt.scale.y);
        Text(u8"Размер");
        SliderFloat(string(u8"##tilesize_x" + to_string(selected_tile.tset_value) + to_string(selected_tile.tile_value)).c_str(), &curr_txt.scale.x, 0, 2);
        SameLine();
        SliderFloat(string(u8"##tilesize_y" + to_string(selected_tile.tset_value) + to_string(selected_tile.tile_value)).c_str(), &curr_txt.scale.y, 0, 2);
        PopItemWidth();

        pos2ds c_tile_pos = core.mst(selected_layer, mainwin.mspos, mainwin.winsize);
        Text(u8"Отладочная информация: ");
        Text(u8"Позиция выбранного тайла: ");
        Text(string(to_string(c_tile_pos.x) + " " + to_string(c_tile_pos.y)).c_str());
        pos2d scr_crds = core.screen_coords(mainwin.mspos, mainwin.winsize);
        Text(u8"Позиция в экранных координатах: ");
        Text(string(to_string(scr_crds.x) + " " + to_string(scr_crds.y)).c_str());
        ImGui::End();

        

        if (d.check()) camera.center.x += int(4 / camera.zoom);
        if (a.check()) camera.center.x -= int(4 / camera.zoom);
        if (w.check()) camera.center.y += int(4 / camera.zoom);
        if (s.check()) camera.center.y -= int(4 / camera.zoom);
        if (obj_mode.check()) {
            if (mode == 0) {
                mode = 1;
            }
            else mode = 0;
        }

        if (save.check()) core.save_proj(u8"bruh.bruh");
        if (load.check()) core.load_proj(u8"bruh.bruh");
        if (core.ms.wheel && (mainwin.is_hovered))
            core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].cams[core.scenes[core.c_scene_id].rooms[core.scenes[core.c_scene_id].c_room_id].selected_cam].zoom -= core.ms.wheel * 0.1;// , camera.center = core.screen_coords(mspos, winsize);

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

