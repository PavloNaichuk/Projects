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

using SharedWindow = std::shared_ptr<SDL_Window>;
using SharedRenderer = std::shared_ptr<SDL_Renderer>;
using SharedSurface = std::shared_ptr<SDL_Surface>;
using SharedTexture = std::shared_ptr<SDL_Texture>;
using UniqueFont = std::unique_ptr<TTF_Font, std::function<void(TTF_Font*)>>;