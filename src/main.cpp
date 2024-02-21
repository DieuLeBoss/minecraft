#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <future>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <condition_variable>

#include "opengl/renderer.hpp"
#include "opengl/shaders.hpp"
#include "opengl/texture.hpp"
#include "camera.hpp"

#include "world/structure/chunk.hpp"
#include "world/world.hpp"

#include "settings.hpp"

#include "nlohmann/json.hpp"


std::mutex mtx_init_renderer;
std::condition_variable cv_init_renderer;
bool is_init_renderer = false;


World world;

Camera camera;
std::mutex camera_mutex;

bool run_multithread;
std::mutex run_mutex;

void renderingThreadManager(World& world) {
    Renderer renderer;

    {
        std::lock_guard<std::mutex> lock(mtx_init_renderer);
        renderer.init("Minecraft");
        camera = Camera(renderer.getWidth(), renderer.getHeight(), glm::vec3(0, 256, 0), renderer.getWindow());
        is_init_renderer = true;
    }
    cv_init_renderer.notify_one();

    bool run;

    {
        std::lock_guard<std::mutex> lock(run_mutex);
        run_multithread = !glfwWindowShouldClose(renderer.getWindow()) && glfwGetKey(renderer.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS;
        run = run_multithread;
    }

    while(run) {
        renderer.render(camera, world);

        {
            std::lock_guard<std::mutex> lock(run_mutex);
            run_multithread = !glfwWindowShouldClose(renderer.getWindow()) && glfwGetKey(renderer.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS;
            run = run_multithread;
        }
    }

    renderer.terminate();
}

int minecraft()
{

    std::thread rendererThread(renderingThreadManager, std::ref(world));

    if(!loadSetting())
        return -1;

    {
        std::unique_lock<std::mutex> lock(mtx_init_renderer);
        cv_init_renderer.wait(lock, []{ return is_init_renderer; });
    }

    world = World(camera.getPointerPosition());

    bool run;

    {
        std::lock_guard<std::mutex> lock(run_mutex);
        run = run_multithread;
    }

    while (run)
    {        
        world.update(camera.getPosition());
        
        {
            std::lock_guard<std::mutex> lock(run_mutex);
            run = run_multithread;
        }
    }

    rendererThread.join();

    return 0;
}


int main()
{
    // nullptr = false
    // std::thread threadChunkGeneration(&World::chunkGeneration, &world, &run);

    minecraft();

    return 0;
}