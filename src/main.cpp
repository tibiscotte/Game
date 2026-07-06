#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <filesystem>

#include "../includes/var.hpp"
#include "../includes/fonc.hpp"

namespace fs = std::filesystem;

int main() {
    fs::path currentPath = fs::current_path();
    std::cout << "The terminal is here : " << currentPath << std::endl;

    // init

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("Error SDL_Init : %s", SDL_GetError());
        return 1;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        SDL_Log("Error SDL_image : %s", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Error Mix_OpenAudio : %s", Mix_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    fs::path musicPath = currentPath / "../data/Music/Song.wav";

    Mix_Music* music = Mix_LoadMUS(musicPath.string().c_str());
    if (music == NULL) {
        SDL_Log("Error charging music : %s", Mix_GetError());
    } else {
        Mix_PlayMusic(music, -1);
    }

    // create window and renderer

    SDL_Window *win = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenX, screenY, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Event event;
    Uint64 current_time = SDL_GetPerformanceCounter();

    Mix_VolumeMusic(60);

    SDL_FRect player = {playerX, playerY, static_cast<float>(blockSize) * 2, static_cast<float>(blockSize) * 2};

    auto [textures, Player] = createTextures(renderer);

    fs::path mapPath = currentPath / "../data/map/map.txt";
    fs::path collMapPath = currentPath / "../data/map/collMap.txt";

    auto [map, collMap] = createMap(mapPath.string(), collMapPath.string(), 35);

    // loop

    while (running)
    {
        last_time = current_time;
        current_time = SDL_GetPerformanceCounter();
        delta_time = (double)(current_time - last_time) / (double)SDL_GetPerformanceFrequency();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        auto [moved, direction] = move(player, playerX, playerY, collMap, delta_time, blockSize);

        camPosX = playerX - 11 * blockSize;
        camPosY = playerY - 8.5 * blockSize;

        SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
        SDL_RenderClear(renderer);

        drawMap(map, blockSize, textures, renderer);

        player.x = 12 * blockSize;
        player.y = 8.5 * blockSize;

        drawPlayer(player, renderer, Player, moved, direction);

        SDL_RenderPresent(renderer);
    }

    // end

    for (size_t i = 0; i < textures.size(); i++)
        SDL_DestroyTexture(textures[i]);
    for (size_t i = 0; i < Player.size(); i++)
        SDL_DestroyTexture(Player[i]);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    
    if (music) {
        Mix_FreeMusic(music);
    }
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    return 0;
}