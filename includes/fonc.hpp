#ifndef FONC_HPP
#define FONC_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

inline void drawMap(std::vector<std::vector<int>> map, int blockSize, std::vector<SDL_Texture*> textures, SDL_Renderer* renderer) {
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            float tileX = x * blockSize;
            float tileY = y * blockSize;

            if (tileX >= camPosX - blockSize && tileX <= camPosX + (25 * blockSize) &&
                tileY >= camPosY - blockSize && tileY <= camPosY + (18 * blockSize)) {
                SDL_Rect rect;
                rect.x = static_cast<int>(tileX - camPosX);
                rect.y = static_cast<int>(tileY - camPosY);
                rect.w = blockSize;
                rect.h = blockSize;
                SDL_RenderCopy(renderer, textures[map[y][x]], NULL, &rect);
            }
        }
    }
}

inline std::pair<bool, std::vector<bool>> move(SDL_FRect& player, float& playerX, float& playerY, std::vector<std::vector<int>>& collMap, double delta_time, int blockSize) {
    float dist = static_cast<float>(playerSpeed) * static_cast<float>(delta_time);
    
    const Uint8* state = SDL_GetKeyboardState(NULL);

    float moveX = 0.0f;
    float moveY = 0.0f;

    static bool w = false;
    static bool a = false;
    static bool s = true;
    static bool d = false;

    if (state[SDL_SCANCODE_W]) {moveY -= dist; w = true; a = s = d = false;}
    if (state[SDL_SCANCODE_A]) {moveX -= dist; a = true; w = s = d = false;}
    if (state[SDL_SCANCODE_S]) {moveY += dist; s = true; a = w = d = false;}
    if (state[SDL_SCANCODE_D]) {moveX += dist; d = true; a = s = w = false;}

    float hitBoxWidth = 32.0f;
    float hitBoxHeight = 64.0f; 

    float nextX = playerX + moveX;
    float hitBoxNextX = nextX + 16.0f; 
    float hitBoxCurrentY = playerY + 0.0f; 

    int left   = static_cast<int>(hitBoxNextX) / blockSize;
    int right  = static_cast<int>(hitBoxNextX + hitBoxWidth - 1) / blockSize;
    int top    = static_cast<int>(hitBoxCurrentY) / blockSize;
    int bottom = static_cast<int>(hitBoxCurrentY + hitBoxHeight - 1) / blockSize;

    if (collMap[top][right] != 1 && collMap[top][left] != 1 && collMap[bottom][right] != 1 && collMap[bottom][left] != 1) {
        playerX = nextX;
    }

    float nextY = playerY + moveY;
    float hitBoxCurrentX = playerX + 16.0f; 
    float hitBoxNextY = nextY + 0.0f;

    left   = static_cast<int>(hitBoxCurrentX) / blockSize;
    right  = static_cast<int>(hitBoxCurrentX + hitBoxWidth - 1) / blockSize;
    top    = static_cast<int>(hitBoxNextY) / blockSize;
    bottom = static_cast<int>(hitBoxNextY + hitBoxHeight - 1) / blockSize;

    int currentBottom = static_cast<int>(playerY + hitBoxHeight - 1) / blockSize;

    bool collision_solide = (collMap[top][right] == 1 || collMap[top][left] == 1 || collMap[bottom][right] == 1 || collMap[bottom][left] == 1);

    bool touchPlatform = (collMap[bottom][left] == 2 || collMap[bottom][right] == 2);

    bool stuckPlatform = (nextY < playerY) && touchPlatform && (currentBottom > bottom);

    if (!collision_solide && !stuckPlatform) {
        playerY = nextY;
    } //std::cout << "position = " << static_cast<int>(playerX + blockSize) / blockSize << ", " << static_cast<int>(playerY + blockSize) / blockSize << std::endl;

    std::vector<bool> direction = {w, a, s, d};

    if (moveX == 0 && moveY == 0) {
        return {false, direction};
    } else {
        return {true, direction};
    }
}

inline std::pair<std::vector<SDL_Texture*>, std::vector<SDL_Texture*>> createTextures(SDL_Renderer* renderer) {
    std::vector<SDL_Texture*> textures;
    std::vector<SDL_Texture *> Player;

    SDL_Texture *GrassLightUp = IMG_LoadTexture(renderer, "../data/blocks/GrassLightUp.png");
    textures.push_back(GrassLightUp);
    SDL_Texture *GrassFront = IMG_LoadTexture(renderer, "../data/blocks/GrassFront.png");
    textures.push_back(GrassFront);
    SDL_Texture *GrassDarkUp = IMG_LoadTexture(renderer, "../data/blocks/GrassDarkUp.png");
    textures.push_back(GrassDarkUp);


    SDL_Texture *StoneLightUp = IMG_LoadTexture(renderer, "../data/blocks/StoneLightUp.png");
    textures.push_back(StoneLightUp);
    SDL_Texture *StoneFront = IMG_LoadTexture(renderer, "../data/blocks/StoneFront.png");
    textures.push_back(StoneFront);
    SDL_Texture *StoneDarkUp = IMG_LoadTexture(renderer, "../data/blocks/StoneDarkUp.png");
    textures.push_back(StoneDarkUp);


    SDL_Texture *Hedge = IMG_LoadTexture(renderer, "../data/blocks/Hedge.png");
    textures.push_back(Hedge);


    SDL_Texture *Leaf = IMG_LoadTexture(renderer, "../data/blocks/Leaf.png");
    textures.push_back(Leaf);
    SDL_Texture *Trunk = IMG_LoadTexture(renderer, "../data/blocks/Trunk.png");
    textures.push_back(Trunk);


    SDL_Texture *Debug = IMG_LoadTexture(renderer, "../data/blocks/Debug.png");
    textures.push_back(Debug);



    SDL_Texture *player0 = IMG_LoadTexture(renderer, "../data/entity/Player/Player0.png");
    Player.push_back(player0);
    SDL_Texture *player1 = IMG_LoadTexture(renderer, "../data/entity/Player/Player1.png");
    Player.push_back(player1);
    SDL_Texture *player2 = IMG_LoadTexture(renderer, "../data/entity/Player/Player2.png");
    Player.push_back(player2);
    SDL_Texture *player3 = IMG_LoadTexture(renderer, "../data/entity/Player/Player3.png");
    Player.push_back(player3);


    SDL_Texture *player00 = IMG_LoadTexture(renderer, "../data/entity/Player/Player00.png");
    Player.push_back(player00);
    SDL_Texture *player01 = IMG_LoadTexture(renderer, "../data/entity/Player/Player01.png");
    Player.push_back(player01);
    SDL_Texture *player02 = IMG_LoadTexture(renderer, "../data/entity/Player/Player02.png");
    Player.push_back(player02);
    SDL_Texture *player03 = IMG_LoadTexture(renderer, "../data/entity/Player/Player03.png");
    Player.push_back(player03);


    SDL_Texture *player10 = IMG_LoadTexture(renderer, "../data/entity/Player/Player10.png");
    Player.push_back(player10);
    SDL_Texture *player11 = IMG_LoadTexture(renderer, "../data/entity/Player/Player11.png");
    Player.push_back(player11);
    SDL_Texture *player12 = IMG_LoadTexture(renderer, "../data/entity/Player/Player12.png");
    Player.push_back(player12);
    SDL_Texture *player13 = IMG_LoadTexture(renderer, "../data/entity/Player/Player13.png");
    Player.push_back(player13);


    SDL_Texture *player20 = IMG_LoadTexture(renderer, "../data/entity/Player/Player20.png");
    Player.push_back(player20);
    SDL_Texture *player21 = IMG_LoadTexture(renderer, "../data/entity/Player/Player21.png");
    Player.push_back(player21);
    SDL_Texture *player22 = IMG_LoadTexture(renderer, "../data/entity/Player/Player22.png");
    Player.push_back(player22);
    SDL_Texture *player23 = IMG_LoadTexture(renderer, "../data/entity/Player/Player23.png");
    Player.push_back(player23);


    return {textures, Player};
}

inline void drawPlayer(SDL_FRect& player, SDL_Renderer* renderer, const std::vector<SDL_Texture*>& Player, bool moved, const std::vector<bool>& movement) {
    Uint32 ticks = SDL_GetTicks();
    int index = 0;

    if (moved) {
        int frame = (ticks / 300) % 4;
        if (movement[2])      index = (frame == 3) ? 2 : (frame == 1 ? 1 : 0);
        else if (movement[3]) index = 4 + ((frame == 3) ? 2 : (frame == 1 ? 1 : 0));
        else if (movement[1]) index = 8 + ((frame == 3) ? 2 : (frame == 1 ? 1 : 0));
        else if (movement[0]) index = 12 + ((frame == 3) ? 2 : (frame == 1 ? 1 : 0));
    } else {
        int frame = (ticks / 600) % 2;
        if (movement[2])      index = (frame == 0) ? 0 : 3;
        else if (movement[3]) index = (frame == 0) ? 4 : 7;
        else if (movement[1]) index = (frame == 0) ? 8 : 11;
        else if (movement[0]) index = (frame == 0) ? 12 : 15;
    }

    if (index >= 0 && index < Player.size() && Player[index] != nullptr) {
        SDL_RenderCopyF(renderer, Player[index], NULL, &player);
    }
}

inline std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> createMap(const fs::path path, const fs::path path2, int rowSize) {
    auto loadByFixedSize = [rowSize](const fs::path filePath) {
        std::vector<std::vector<int>> grid;
        std::ifstream file(filePath);
        
        std::vector<int> currentRow;
        int value;

        while (file >> value) {
            currentRow.push_back(value);
            
            if (currentRow.size() == rowSize) {
                grid.push_back(currentRow);
                currentRow.clear();
            }
        }

        if (!currentRow.empty()) {
            grid.push_back(currentRow);
        }

        return grid;
    };

    return {loadByFixedSize(path), loadByFixedSize(path2)};
}

#endif