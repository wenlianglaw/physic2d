//
//  sdl_utilldd.cpp
//  sdldrawcircle
//
//  Created by Jiuhua on 6/5/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//
#include <iostream>
#include <vector>
#include "sdl_utilldd.hpp"
using namespace std;
using namespace dd;
#define DRAW_CIRCLE_STEPS 100

static unsigned int ticksFromLastCall_MouseSpeedFromLastCall = 0;
static int mouseXFromLastCall_MouseSpeedFromLastCall = 0, mouseYFromLastCall_MouseSpeedFromLastCall = 0;

void drawCircle(SDL_Renderer* ren, int x, int y, int r){
    float deltaTheta = 2 * PI / DRAW_CIRCLE_STEPS;
    int oldX = r + x, oldY = y;
    for(float theta = deltaTheta; theta < 2 * PI + deltaTheta; theta += deltaTheta){
        int newX = r * cos(theta) + x, newY = r * sin(theta) + y;
        SDL_RenderDrawLine(ren, oldX, oldY, newX, newY);
        oldX = newX;
        oldY = newY;
    }
}
void drawCircle(SDL_Renderer* ren, Circle circle){
    //int steps = 0;
    //vector<TwoPoints> vec;
    
    float deltaTheta = 2 * PI / DRAW_CIRCLE_STEPS;
    int oldX = circle.r + circle.x, oldY = circle.y;
    for(float theta = deltaTheta; theta < 2 * PI + deltaTheta; theta += deltaTheta){
        int newX = circle.r * cos(theta) + circle.x, newY = circle.r * sin(theta) + circle.y;
        SDL_RenderDrawLine(ren, oldX, oldY, newX, newY);
        
        /*
        bool contained = false;
        for(vector<TwoPoints>::const_iterator i = vec.begin();i!=vec.end();++i){
            if((*i).a.x == oldX && (*i).a.y == oldY && (*i).b.x == newX && (*i).b.y == newY){
                contained = true;
                break;
            }
        }
        if(!contained){
            steps++;
            TwoPoints newTP;
            newTP.a.x = oldX;
            newTP.a.y = oldY;
            newTP.b.x = newX;
            newTP.b.y = newY;
            vec.push_back(newTP);
        }*/
        
        oldX = newX;
        oldY = newY;

    }
    //cout << "steps = " << steps << endl;

}
void drawFilledCircle(SDL_Renderer* ren, int x, int y, int r){
    int rSquare = r * r;
    for(int yy = 0; yy <= r; yy++){
        int xx = sqrt(rSquare - yy * yy);
        SDL_RenderDrawLine(ren, x - xx, yy + y, x + xx, yy + y);
        SDL_RenderDrawLine(ren, x - xx, -yy + y, x + xx, -yy + y);
    }
}
void drawFilledCircle(SDL_Renderer* ren, Circle circle){
    int rSquare = circle.r * circle.r;
    for(int yy = 0; yy <= circle.r; yy++){
        int xx = sqrt(rSquare - yy * yy);
        SDL_RenderDrawLine(ren, circle.x - xx, yy + circle.y, circle.x + xx, yy + circle.y);
        SDL_RenderDrawLine(ren, circle.x - xx, -yy + circle.y, circle.x + xx, -yy + circle.y);
    }
}
void drawRect(SDL_Renderer* ren, int x, int y, int w, int h, double theta){
    //TODO
}
void drawFilledRect(SDL_Renderer* ren, int x, int y, int w, int h, double theta){
    //TODO
}



void mouseSpeedFromLastCall(double* vx, double* vy){
    int x, y;
    unsigned int t = SDL_GetTicks();
    double deltaT = t - ticksFromLastCall_MouseSpeedFromLastCall;
    SDL_GetMouseState(&x, &y);
    *vx = (x - mouseXFromLastCall_MouseSpeedFromLastCall)/deltaT;
    *vy = (y - mouseYFromLastCall_MouseSpeedFromLastCall)/deltaT;
    
    ticksFromLastCall_MouseSpeedFromLastCall = t;
    mouseXFromLastCall_MouseSpeedFromLastCall = x;
    mouseYFromLastCall_MouseSpeedFromLastCall = y;
}


static unsigned int lastTicksGetFrameRate = 0;
float getFrameRate(){
    unsigned int currentTicks = SDL_GetTicks();
    unsigned int deltaTicks = currentTicks - lastTicksGetFrameRate;
    float ret;
    if(deltaTicks != 0){
        ret = 1000.0/deltaTicks;
        lastTicksGetFrameRate = currentTicks;
        
        return ret;
    }else
        return -1;
}

static unsigned int lastTicksDelayToMeetFrameInterval = 0;
void delayToMeetFrameInterval(unsigned int frameInterval){
    //delay to satisfy frame rate
    unsigned int currentTicks = SDL_GetTicks();
    unsigned int deltaTicks = currentTicks - lastTicksDelayToMeetFrameInterval;
    if(deltaTicks < frameInterval){
        unsigned int delayTime = frameInterval - deltaTicks;
        SDL_Delay(delayTime);
    }
    lastTicksDelayToMeetFrameInterval = SDL_GetTicks();
}


void logSDLError(ostream& os, const string& msg){
    os << msg << "error: " << SDL_GetError() << endl;
}

SDL_Texture *loadTexture(const string& fileName, SDL_Renderer *renderer){
    SDL_Texture *texture = nullptr;
    
    SDL_Surface *loadedImage = SDL_LoadBMP(fileName.c_str());
    if(loadedImage != nullptr){
        texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
        SDL_FreeSurface(loadedImage);
        
        if(texture == nullptr){
            logSDLError(cout, "SDL_CreateTextureFromSurface");
        }
    }else{
        logSDLError(cout, "SDL_LoadBMP");
    }
    return texture;
}
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int w, int h){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}
