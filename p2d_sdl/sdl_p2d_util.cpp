//
//  sdl_p2d_util.cpp
//  physics2d
//
//  Created by Jiuhua on 6/7/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "sdl_p2d_util.hpp"
#include <vector>
using namespace std;
using namespace dd;

//TODO: increase performance by OpenGL?
void drawWorld(SDL_Renderer* ren, P2D_World* world, int windowWidth, int windowHeight, int y0){
    int windowWidthOver2 = windowWidth/2, windowHeightOver2 = windowHeight/2;
    
    SDL_RenderClear(ren);
    //draw objects
    for(vector<P2D_Object*>::const_iterator i = world->objects.begin();i != world->objects.end();++i){
        //draw the specific object in the world
        double x = ((*i)->x - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2;
        double y = ((*i)->y - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2;
        
        //set object drawing color
        if(SDL_SetRenderDrawColor(ren, (*i)->shape.color.r, (*i)->shape.color.g, (*i)->shape.color.b, (*i)->shape.color.a) == -1)
            SDL_SetRenderDrawColor(ren, DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE, DEFAULT_ALPHA);
        
        //draw object
        if((*i)->shape.getShapeType() == P2D_SHAPE_CIRCLE){
            //draw circle
            drawFilledCircle(ren, x , y0 - y, (*i)->shape.getMaxRadius() * world->perspectiveZoom);
        }else if((*i)->shape.getShapeType() == P2D_SHAPE_RECT){
            //TODO
        }else if((*i)->shape.getShapeType() == P2D_SHAPE_LINE){
            //draw line
            double sinTheta = sin((*i)->angle);
            double cosTheta = cos((*i)->angle);
            double r = (*i)->shape.getMaxRadius() * world->perspectiveZoom;
            SDL_RenderDrawLine(ren, cosTheta * r + x, y0 - (sinTheta * r + y), -cosTheta * r + x, y0 - (-sinTheta * r + y));
        }
    }
    
    //draw walls
    //double wx = (world->upperWall - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2
    //double wy = ((*i)->y - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2;
    SDL_SetRenderDrawColor(ren, P2D_DEFAULT_WALL_COLOR_R, P2D_DEFAULT_WALL_COLOR_G, P2D_DEFAULT_WALL_COLOR_B, P2D_DEFAULT_WALL_COLOR_A);
    if(world->isUpperWall)
        SDL_RenderDrawLine(ren, 0, y0 - ((world->upperWall - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2), windowWidth, y0 - ((world->upperWall - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2));
    if(world->isLowerWall)
        SDL_RenderDrawLine(ren, 0, y0 - ((world->lowerWall - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2), windowWidth, y0 - ((world->lowerWall - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2));
    if(world->isLeftWall)
        SDL_RenderDrawLine(ren, (world->leftWall - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2 - 1, 0, (world->leftWall - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2 - 1, windowHeight);
    if(world->isRightWall)
        SDL_RenderDrawLine(ren, (world->rightWall - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2, 0, (world->rightWall - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2, windowHeight);
    
    //present the renderer
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderPresent(ren);
}

void drawReplayWorld(SDL_Renderer* ren, P2D_ReplayWorld* world, int windowWidth, int windowHeight, int y0){
    int windowWidthOver2 = windowWidth/2, windowHeightOver2 = windowHeight/2;
    
    SDL_RenderClear(ren);
    //draw objects
    for(vector<P2D_ReplayObject>::iterator i = world->objects.begin();i != world->objects.end();++i){
        //draw the specific object in the world
        double x = (i->x - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2;
        double y = (i->y - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2;
        
        //set object drawing color
        if(SDL_SetRenderDrawColor(ren, i->shape.color.r, i->shape.color.g, i->shape.color.b, i->shape.color.a) == -1)
            SDL_SetRenderDrawColor(ren, DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE, DEFAULT_ALPHA);
        
        //draw object
        if(i->shape.getShapeType() == P2D_SHAPE_CIRCLE){
            //draw circle
            drawFilledCircle(ren, x , y0 - y, i->shape.getMaxRadius() * world->perspectiveZoom);
        }else if(i->shape.getShapeType() == P2D_SHAPE_RECT){
            //TODO
        }else if(i->shape.getShapeType() == P2D_SHAPE_LINE){
            //draw line
            double sinTheta = sin(i->angle);
            double cosTheta = cos(i->angle);
            double r = i->shape.getMaxRadius() * world->perspectiveZoom;
            SDL_RenderDrawLine(ren, cosTheta * r + x, y0 - (sinTheta * r + y), -cosTheta * r + x, y0 - (-sinTheta * r + y));
        }
    }
    /*
    //draw walls
    //double wx = (world->upperWall - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2
    //double wy = i->y - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2;
    SDL_SetRenderDrawColor(ren, P2D_DEFAULT_WALL_COLOR_R, P2D_DEFAULT_WALL_COLOR_G, P2D_DEFAULT_WALL_COLOR_B, P2D_DEFAULT_WALL_COLOR_A);
    if(world->isUpperWall)
        SDL_RenderDrawLine(ren, 0, y0 - ((world->upperWall - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2), windowWidth, y0 - ((world->upperWall - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2));
    if(world->isLowerWall)
        SDL_RenderDrawLine(ren, 0, y0 - ((world->lowerWall - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2), windowWidth, y0 - ((world->lowerWall - world->perspectiveY - windowHeightOver2) * world->perspectiveZoom + windowHeightOver2));
    if(world->isLeftWall)
        SDL_RenderDrawLine(ren, (world->leftWall - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2 - 1, 0, (world->leftWall - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2 - 1, windowHeight);
    if(world->isRightWall)
        SDL_RenderDrawLine(ren, (world->rightWall - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2, 0, (world->rightWall - world->perspectiveX - windowWidthOver2) * world->perspectiveZoom + windowWidthOver2, windowHeight);
    */
    //present the renderer
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderPresent(ren);
}

