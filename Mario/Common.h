#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <conio.h>
#include <iostream>
#include <memory>
#include <functional>

using SDLQuiter = std::unique_ptr<void, std::function<void(void*)>>;
using SDLWindowPointer = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>;
using SDLRendererPointer = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>>;
using SDLSurfacePointer = std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface*)>>;
using SDLTexturePointer = std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture*)>>;
using TTFFontPointer = std::unique_ptr<TTF_Font, std::function<void(TTF_Font*)>>;

using TTFQuiter = std::unique_ptr<void, std::function<void(void*)>>;
using TTFFontPointer = std::unique_ptr<TTF_Font, std::function<void(TTF_Font*)>>;
