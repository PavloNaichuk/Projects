#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <cassert>
#include <cstdio>
#include <cmath>
#include <memory>
#include <functional>
#include <algorithm>
#include <array>
#include <queue>
#include <map>

using SharedWindow = std::shared_ptr<SDL_Window>;
using SharedRenderer = std::shared_ptr<SDL_Renderer>;
using SharedSurface = std::shared_ptr<SDL_Surface>;
using SharedTexture = std::shared_ptr<SDL_Texture>;
using SharedFont = std::shared_ptr<TTF_Font>;