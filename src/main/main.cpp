#include <iostream>
#include <SDL2/SDL.h>
#include "../raytracing_engine/raytracing_engine.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

bool DEBUG_MODE = false;

int main(int argc, char* argv[]) {
    std::string output_file = "output.png";

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--debug") {
            DEBUG_MODE = true;
        }
        if (arg == "--output_file") {
            if (i+1 >= argc) {
                std::cerr << "Expected file name for --output_file" << std::endl;
                return -1;
            }
            output_file = argv[i+1];
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error initializing video" << std::endl;
        return -2;
    }

    SDL_Window* window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    if (window == nullptr) {
        std::cerr << "Error creating window" << std::endl;
        return -3;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr) {
        return -4;
    }

    std::cout << "Sloth: It's ray.. tracing.. time... :D" << std::endl;
    Image canvas = Image(SCREEN_WIDTH, SCREEN_HEIGHT, Channel::RGBA);

    std::cout << "Reading test scene file" << std::endl;
//    std::ifstream f("test_scene.json");
//    json data = json::parse(f);
    Scene scene(1, 1, 1);
    scene.spheres.emplace_back(Vec3f(0, -1, 3), 1, Color(255, 0, 0), 500, 0.2);
    scene.spheres.emplace_back(Vec3f(2, 0 , 4), 1, Color(0, 0, 255), 500, 0.3);
    scene.spheres.emplace_back(Vec3f(-2, 0 , 4), 1, Color(0, 255, 0), 10, 0.4);
    scene.spheres.emplace_back(Vec3f(0, -5001, 0), 5000, Color(255, 255, 0), 1000, 0.5);

    scene.lights.emplace_back(LightType::AMBIENT, Vec3f(), 0.2);
    scene.lights.emplace_back(LightType::POINT, Vec3f(2, 1, 0), 0.6);
    scene.lights.emplace_back(LightType::DIRECTIONAL, Vec3f(1, 4, 4), 0.2);

    RayTracingEngine engine = RayTracingEngine(scene);

    // Create an SDL_Texture.
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Event event;
    bool running = true;
    int bounces = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        engine.RenderScene(canvas, bounces);

        // Update the texture with the image data.
        SDL_UpdateTexture(texture, nullptr, canvas.getData(), SCREEN_WIDTH*4);

        // Copy the texture to the rendering context.
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        // Update the window to display the new rendering.
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);  // To avoid maxing out the CPU
    }

    //std::cout << "Writing to file: output.png" << std::endl;
    printf("Writing to file: %s\n", output_file.c_str());
    canvas.writeToDiskAsPNG(output_file.c_str());

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
