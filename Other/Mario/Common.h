#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <sstream>
#include <algorithm>

using SDLWindowPointer = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>;
using SDLRendererPointer = std::shared_ptr<SDL_Renderer>;
using SDLSurfacePointer = std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface*)>>;
using SDLTexturePointer = std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture*)>>;
using TTFFontPointer = std::unique_ptr<TTF_Font, std::function<void(TTF_Font*)>>;