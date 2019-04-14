//
//  sdl_utilldd.hpp
//  sdldrawcircle
//
//  Created by Jiuhua on 6/5/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef sdl_utilldd_hpp
#define sdl_utilldd_hpp

#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <iostream>
#include "mathdd.hpp"

struct Circle{
    int r;
    int x;
    int y;
};
void drawCircle(SDL_Renderer*, int, int, int);
void drawCircle(SDL_Renderer*, Circle);
void drawFilledCircle(SDL_Renderer*, int, int, int);
void drawFilledCircle(SDL_Renderer*, Circle);
void drawRect(SDL_Renderer*, int, int, int, int, double);
void drawFilledRect(SDL_Renderer*, int, int, int, int, double);

void mouseSpeedFromLastCall(double*, double*);

float getFrameRate();
void delayToMeetFrameInterval(unsigned int);

void logSDLError(std::ostream&, const std::string&);
SDL_Texture *loadTexture(const std::string&, SDL_Renderer*);
void renderTexture(SDL_Texture* , SDL_Renderer*, int, int);
void renderTexture(SDL_Texture*, SDL_Renderer*, int, int, int, int);

#endif /* sdl_utilldd_hpp */
