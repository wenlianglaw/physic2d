//
//  sdl_p2d_util.hpp
//  physics2d
//
//  Created by Jiuhua on 6/7/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef sdl_p2d_util_hpp
#define sdl_p2d_util_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include "P2D_World.hpp"
#include "sdl_utilldd.hpp"
#include "P2D_Flags.hpp"
#include "P2D_ReplayWorld.hpp"

void drawWorld(SDL_Renderer*, P2D_World*, int, int, int);
void drawReplayWorld(SDL_Renderer*, P2D_ReplayWorld*, int, int, int);


#endif /* sdl_p2d_util_hpp */
