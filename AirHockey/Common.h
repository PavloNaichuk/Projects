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
#include <array>
#include <cassert>

using UniqueWindowPointer = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>;
using SharedRendererPointer = std::shared_ptr<SDL_Renderer>;
using SharedSurfacePointer = std::shared_ptr<SDL_Surface>;
using SharedTexturePointer = std::shared_ptr<SDL_Texture>;
using UniqueFontPointer = std::unique_ptr<TTF_Font, std::function<void(TTF_Font*)>>;