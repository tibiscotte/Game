#ifndef VAR_HPP
#define VAR_HPP

#include <SDL2/SDL.h>
#include <vector>

//screen info

inline int blockSize = 32;
inline int screenX = 800;
inline int screenY = 576;

//pre-boucle

inline bool running = true;
inline Uint64 last_time = 0;
inline double delta_time = 0;

//player

inline float playerX = 15 * blockSize;
inline float playerY = 17.5 * blockSize;

inline int playerSpeed = 200;

//camera

inline float camPosX = playerX - 1 * blockSize;
inline float camPosY = playerY - 8.5 * blockSize;

#endif