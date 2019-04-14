//
//  main.cpp
//  physics2d
//
//  Created by Jiuhua on 4/16/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include <iostream>
#include "P2D_Object.hpp"
#include "P2D_Shape.hpp"
#include "P2D_World.hpp"
#include "utildd.hpp"
#include "sdl_utilldd.hpp"
#include "P2D_Flags.hpp"
#include "sdl_p2d_util.hpp"
#include "P2D_SDL_Event.hpp"
#include "P2D_SDL_Replay.hpp"
#include "P2D_ReplayWorld.hpp"
#include "P2D_ReplayObject.hpp"
#include <string>
#include <vector>
#include <math.h>
#include <SDL2/SDL.h>

using namespace std;
using namespace dd;
unsigned int numberOfWindows = 0;
bool initSDL();

const string windowName = "Physics Simulation (2D)";

#define MAIN_WINDOW_WIDTH 1000u
#define MAIN_WINDOW_HEIGHT 600u
#define FRAME_RATE 60


/*
 --------------------------------TODO:
 multiple world(in multiple window)
 drawing all objects function
 command open get velocity and stuff feature
 cancel window focus when get ENTER input (same apply to console)
 memory!!!
 object color
mass =  0 bug
 */

void force1(P2D_Object*);


SDL_Window *mainWindow;
SDL_Renderer *mainWindowRenderer;
SDL_Event event;
P2D_SDL_Event* p2d_sdl_Event;
P2D_World* world;
string relativePath;
string worldSaveFileName;
string replaysSaveFileName;
//-------------------------------Main function---------------------------------
int main(int argc, const char * argv[]) {
    relativePath = getPath(argv[0]);
    worldSaveFileName = relativePath + "worldsave.wldsv";
    replaysSaveFileName = relativePath + "replays.prep";
    
    //if save files does not exist, create them
    ofstream of(worldSaveFileName, ios::binary | ios::app | ios::out);
    of.close();
    of.open(replaysSaveFileName, ios::binary | ios::app | ios::out);
    of.close();
    
    //initialize SDL, mainWindow and mainWindowRenderer
    if(!initSDL())return EXIT_FAILURE;
    
    world = P2D_World::P2D_Create_World();
    
    p2d_sdl_Event = new P2D_SDL_Event(world, mainWindow, mainWindowRenderer, &event, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, MAIN_WINDOW_HEIGHT);
    p2d_sdl_Event->setWorldFileName(worldSaveFileName);
    p2d_sdl_Event->setReplayFileName(replaysSaveFileName);
    //world = *(savedWorld->worlds.begin());
    ifstream inf("worldsave.wldsv");
    
    /*
    //initialize physics world
    world->perspectiveWidth = MAIN_WINDOW_WIDTH;
    world->perspectiveHeight = MAIN_WINDOW_HEIGHT;
    
    
    world->upperWall = 500;
    world->lowerWall = 100;
    world->leftWall = 600;
    world->rightWall = 900;
    P2D_Shape *sp = new P2D_Shape;
    sp->setCircle(1);
    
    world->createObjects(1000, sp, P2D_ADD_OBJECT_RANDOM_WITHIN_WALL);
    world->setPause(true);
    world->enableGravitation(false);
    //world->enableWall(true, P2D_ALL_WALL);
    //world->accelerationY = -0.1;
    world->setCollision(false);
    for(vector<P2D_Object*>::iterator i = world->objects.begin(); i!=world->objects.end(); ++i){
        (*i)->id = 1;
        (*i)->vy = 5;
    }
    world->individualForces.push_back(force1);
    world->gravitationK = 10;
    */
    
    //ofstream inf("worldsave.wldsv", ios::out | ios::binary);
    //world->write(inf);
    //inf.close();
    /*
    int i =0;
    P2D_SDL_Replay pr;
    pr.setName("exp1");
    ifstream of("replay.prep", ios::in | ios::binary);
    pr.read(of);
    while (pr.draw(mainWindowRenderer)) {
        ;
        while(SDL_PollEvent(&event)){
        }
    }*/
    
    bool quit = false;
    while(!quit){
        
        world->go();
        /*
        i++;
        pr.addFrame(world);
        if(i == 1000){
            pr.write(of);
            quit = true;
            of.close();
        }
            */
        p2d_sdl_Event->eventGo();
        
        //-----------------------------event-----------------------------
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                quit = true;
            else
                p2d_sdl_Event->eventOccur();
                //P2D_Event_Occur(world, &event, mainWindow, MAIN_WINDOW_HEIGHT);
        }
        //-----------------------------event-----------------------------

    }
    
    
    //TODO: free P2D_World and objects memory
    SDL_DestroyRenderer(mainWindowRenderer);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
    world->removeAllObject();
    delete world;
    return EXIT_SUCCESS;
}
//-------------------------------Main function---------------------------------

bool initSDL(){
    //init sdl
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(cout, "SDL_Init");
        SDL_Quit();
        return false;
    }
    
    //init window
    mainWindow = SDL_CreateWindow(windowName.c_str(), 0, SDL_WINDOWPOS_UNDEFINED, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(mainWindow == nullptr){
        logSDLError(cout, "SDL_CreateWindow");
        SDL_Quit();
        return false;
    }
    numberOfWindows++;
    
    //init renderer
    mainWindowRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(mainWindowRenderer == nullptr){
        SDL_DestroyWindow(mainWindow);
        logSDLError(cout, "SDL_CreateRenderer");
        SDL_Quit();
        return false;
    }
    SDL_SetRenderDrawBlendMode(mainWindowRenderer, SDL_BLENDMODE_BLEND);
    return true;
}

//---------------------------------------------------------------------------
void force1(P2D_Object* o){
    double x = 500, y = 300, mass = 1000;
    if(o->id == 1){
        double a1, a2 , rs, r;
        double deltaX = x - o->x;
        double deltaY = y - o->y;
        rs = deltaY * deltaY + deltaX * deltaX;
        r = sqrt(rs);
        
        double aa = world->gravitationK / rs;
        
        a1 = aa * mass;
        a2 = aa * o->mass;
        
        o->ax += a1 * deltaX / r;
        o->ay += a1 * deltaY / r;
    }
    if(o->vx > 30 || o->vy > 30){
        o->id = 0;
        o->x = -1000000;
        o->vx = 0;
        o->vy = 0;
    }
}

