#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <vector>

#include "../includes/var.hpp"
#include "../includes/fonc.hpp"

int main()
{
    // initialisation

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("Erreur SDL_Init : %s", SDL_GetError());
        return 1;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        SDL_Log("Erreur SDL_image : %s", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Erreur Mix_OpenAudio : %s", Mix_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    Mix_Music* musique = Mix_LoadMUS("../data/Music/Song.wav");
    if (musique == NULL) {
        SDL_Log("Erreur chargement musique : %s", Mix_GetError());
    } else {
        Mix_PlayMusic(musique, -1);
    }

    // creation fenetre et renderer

    SDL_Window *win = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenX, screenY, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Event event;
    Uint64 current_time = SDL_GetPerformanceCounter();

    Mix_VolumeMusic(60);

    SDL_FRect player = {playerX, playerY, static_cast<float>(blockSize) * 2, static_cast<float>(blockSize) * 2};

    auto [textures, Player] = createTextures(renderer);

    auto [map, collMap] = createMap("../data/map/map.txt", "../data/map/collMap.txt", 35);

    // boucle

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

    // fin

    for (size_t i = 0; i < textures.size(); i++)
        SDL_DestroyTexture(textures[i]);
    for (size_t i = 0; i < Player.size(); i++)
        SDL_DestroyTexture(Player[i]);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    
    if (musique) {
        Mix_FreeMusic(musique);
    }
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    return 0;
}