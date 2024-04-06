#pragma warning(disable : 4996)
#include <glad/glad.h>
#include <iostream>
#include <point.h>
#include <shader.h>
//#include <Framebuffer.h>
#include <stb_image.h>
#include <objects.h>
#include <window.h>
#include <Tilemap.h>

using namespace std;




int main()
{
    float lastFrame = 0.0f, deltaTime;
    //pos2d winsize;
    //winsize.x = 800, winsize.y = 800;
    //mwindow window({800, 800}, 256, u8"Игровой движок Юпитер(редактор)");
    //Shader sh(u"shaders/vert1.vs", u"shaders/frag1.fs");
    je_core::core core;
    core.init({ 900, 900 }, 256, u"Юпитер");
    core.addscene();
    core.loadres(u"bruh.bruh");
    
    //je_core::Scene sc;
    //sc.init(window.wins, window.scrs);
    //sc.add(je_core::jroom);
    //sc.rooms[0].add_layers();
    //sc.rooms[0].Layers[0].init_tmap({163, 29});
    //sc.add(je_core::txtre, 23);
    //sc.load_scene(u"bruh.bruh");
    //sc.add(je_core::tset, 2);
    //for (int i = 0; i < 16; i++) {
        //sc.textures[i].loadtexture("Textures/default/" + to_string(i) + ".png");
        //sc.tsets[0].add(i);
    //}
    //sc.rooms[0].cams.resize(1);
    //sc.rooms[0].cams[0].center.x = 128, sc.rooms[0].cams[0].center.y = 128;
    //for (int i = 0; i < 7; i++) {
        //sc.textures[i + 16].loadtexture("Textures/tc/" + to_string(i) + ".png"); 
        //sc.tsets[1].add(i + 16);
    //}
    //sc.save_scene(u"bruh.bruh");
    //sc.sh = sh;
    key_combination save(core.window.id, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_S), load(core.window.id, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_L), obj_mode(core.window.id, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_O);
    //mouse mc(window.id);
    key w(core.window.id, GLFW_KEY_W, 0), a(core.window.id, GLFW_KEY_A, 0), s(core.window.id, GLFW_KEY_S, 0), d(core.window.id, GLFW_KEY_D, 0);
    int selected_tile = 0, selected_layer = 0, selected_tset = 0, mode = 0, selected_prefab = 0, selected_pf = 0;
    key up(core.window.id, GLFW_KEY_UP), down(core.window.id, GLFW_KEY_DOWN), picker(core.window.id, GLFW_KEY_P);
    //u16string saved;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    while (core.window.checkWindow())
    {
        //sc.setvecs(window.scrs);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float fps = 1 / deltaTime;
        //cout << fps << endl;
        //window.startRender();
        pos2d pos1, pos2;
        core.render();
        //sc.render(window.scrs);
        //mc.update(window.wins, window.aspratio);
        //cam2d &curr_cam = sc.rooms[sc.current_room].cams[sc.rooms[sc.current_room].selected_cam];

        if (mode == 0) {
            pos2d ctile = core.mst(selected_layer);
            pos1.x = ctile.x * 16 + 8, pos1.y = ctile.y * 16 + 8;
            pos2.x = ctile.x * 16 + 16 + 8, pos2.y = ctile.y * 16 + 15 + 8;
            core.drawtexture(pos1, core.tsets[selected_tset].tiles[selected_tile].id);
            pos1.x = ctile.x * 16, pos1.y = ctile.y * 16;
            pos2.x = ctile.x * 16 + 16, pos2.y = ctile.y * 16 + 15;
            core.drawrect(pos1, pos2);

            if (picker.check() && core.scenes[core.curr_scene].rooms[core.scenes[core.curr_scene].current_room].Layers[selected_layer].tmap.gTile(ctile.x, ctile.y).tile != -1) /*---*/ selected_tile = core.scenes[core.curr_scene].rooms[core.scenes[core.curr_scene].current_room].Layers[selected_layer].tmap.gTile(ctile.x, ctile.y).tile;
            if (core.ms.lb && ctile.x <= core.scenes[core.curr_scene].rooms[core.scenes[core.curr_scene].current_room].Layers[selected_layer].tmap.gwidth() && ctile.y <= core.scenes[core.curr_scene].rooms[core.scenes[core.curr_scene].current_room].Layers[selected_layer].tmap.gheigh())/*---*/ core.scenes[core.curr_scene].rooms[core.scenes[core.curr_scene].current_room].Layers[selected_layer].tmap.gTile(ctile.x, ctile.y, selected_tile, selected_tset);
            if (core.ms.rb && ctile.x <= core.scenes[core.curr_scene].rooms[core.scenes[core.curr_scene].current_room].Layers[selected_layer].tmap.gwidth() && ctile.y <= core.scenes[core.curr_scene].rooms[core.scenes[core.curr_scene].current_room].Layers[selected_layer].tmap.gheigh())/*---*/ core.scenes[core.curr_scene].rooms[core.scenes[core.curr_scene].current_room].Layers[selected_layer].tmap.gTile(ctile.x, ctile.y, -1);
            if (up.check() && selected_tile < core.tsets[selected_tset].tiles.size() - 1) selected_tile += 1;
            if (down.check() && selected_tile > 0) selected_tile -= 1;
            
        }

        if (d.check()) core.movecam({ 4, 0 });
        if (a.check()) core.movecam({-4, 0 });
        if (w.check()) core.movecam({ 0, 4 });
        if (s.check()) core.movecam({ 0,-4 });
        if (obj_mode.check()) {
            if (mode == 0) {
                mode = 1;
            }
            else mode = 0;
        }
        if (save.check()) core.saveres(u"bruh.bruh");
        if (load.check()) core.loadres(u"bruh.bruh");
        

        //curr_cam.zoom -= mc.wheel * 0.1;
        //window.endRender();
        core.window.endRender();
    }
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
                    wcin >> selected_tset; selected_tile = 0;
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

