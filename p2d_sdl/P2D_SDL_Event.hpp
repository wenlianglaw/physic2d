//
//  P2D_SDL_Event.hpp
//  physics2d
//
//  Created by Jiuhua on 6/9/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef P2D_SDL_Event_hpp
#define P2D_SDL_Event_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include "P2D_World.hpp"
#include "P2D_Object.hpp"
#include "P2D_Shape.hpp"
#include "P2D_Flags.hpp"
#include "sdl_p2d_util.hpp"
#include "sdl_utilldd.hpp"
#include "mathdd.hpp"
#include "P2D_Shape.hpp"
#include "P2D_SDL_Replay.hpp"
#include "P2D_ReplayObject.hpp"
#include "P2D_ReplayWorld.hpp"
#include "utildd.hpp"

#define DEFAULT_FRAME_INTERVAL 15
#define PERSPECTIVE_CHANGE_RATE 5
#define PERSPECTIVE_ZOOM_CHANGE_RATE 1.2
#define CLICK_GET_OBJECT_RANGE 20

#define DEFAULT_SELECT_COLOR_R 220
#define DEFAULT_SELECT_COLOR_G 200
#define DEFAULT_SELECT_COLOR_B 255
#define DEFAULT_SELECT_COLOR_A 255

#define DEFAULT_SELECTED_OBJECT_ADD_ANGV 0.01
#define DEFAULT_SELECTED_OBJECT_ADD_V 0.15

class P2D_SDL_Event{
    P2D_World* world = nullptr;
    P2D_World* lastWorld = nullptr;
    SDL_Window* window = nullptr;
    SDL_Event* sdlEvent = nullptr;
    SDL_Renderer* renderer = nullptr;
    int y0 = 0;
    
    std::vector<P2D_SDL_Replay> replays;
    std::vector<P2D_World*> worlds;
    bool isWorldsLoaded = false;
    std::string worldFileName, replaysFileName = "replays.prep";
    bool replayFileLoaded = false;
    int windowWidth;
    int windowHeight;
    P2D_Object* selectedObject = nullptr;
    P2D_Color selectedObjectColorBefore;
    P2D_Color selectColor;
    double selectedObjectAddAngV = DEFAULT_SELECTED_OBJECT_ADD_ANGV;
    double selectedObjectAddV = DEFAULT_SELECTED_OBJECT_ADD_V;
    
    
    int realMouseX = 0, realMouseY = 0;
    
    
    bool continuousCommandInput = false, isRemovingObject = false, isMovingObject = false, isNumberInput = false, isOutputCommand = false;
    std::string numberInput;
    
    
    bool W_Down = false, S_Down = false, A_Down = false, D_Down = false;
    bool G_Down = false, Z_Statue = true, R_Down = false, SHIFT_Down = false, CONTROL_Down = false;
    
    bool mouseDown = false;
    int mouseX, mouseY, mouseDownX, mouseDownY;
    
    bool WSADQEMovePerspective = false;
    //world's ax, ay and a when mouse down
    double mouseDownAccelerationX, mouseDownAccelerationY, mouseDownAcceleration;
    P2D_Object* mouseMovingObject = nullptr;
    bool mouseMovingLeftWall = false, mouseMovingRightWall = false, mouseMovingLowerWall = false, mouseMovingUpperWall = false;
    
    P2D_Object* getObjectOnMouse();
    void applyNumberInput(double*, double);
    void coutAppliedCommand(const std::string&, const std::string&);
    void coutOutputCommand(const std::string&, const std::string&);
    void executeCommandLine(const std::string&);
    void loadWorlds();
    P2D_World* findWorld(const std::string&);
    void applyWorld(P2D_World*);
    void saveWorld(P2D_World*);
    void saveWorlds();
    void removeWorld(const std::string&);
    
public:
    P2D_SDL_Event(P2D_World*, SDL_Window*, SDL_Renderer*, SDL_Event*, int, int, int);
    
    void setAll(P2D_World*, SDL_Window*, SDL_Renderer*, SDL_Event*, int);
    void setWorld(P2D_World*);
    P2D_World* getWorld();
    void setWindow(SDL_Window*);
    SDL_Window* getWindow();
    void setRenderer(SDL_Renderer*);
    SDL_Renderer* getRenderer();
    void setSDL_Event(SDL_Event*);
    SDL_Event* getSDL_Event();
    void setY0(int);
    int getY0();
    void setWorldFileName(const std::string&);
    void setReplayFileName(const std::string&);
    std::string getWorldFileName();
    std::string getReplayFileName();
    int updateRealMouseXY();
    
    void eventGo();
    void eventOccur();

};

void replay(P2D_SDL_Replay*);

#endif /* P2D_SDL_Event_hpp */
