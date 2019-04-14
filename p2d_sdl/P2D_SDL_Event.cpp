//
//  P2D_SDL_Event.cpp
//  physics2d
//
//  Created by Jiuhua on 6/9/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "P2D_SDL_Event.hpp"
using namespace std;
using namespace dd;
P2D_SDL_Event::P2D_SDL_Event(P2D_World* world, SDL_Window* window, SDL_Renderer* ren, SDL_Event* sdlEvent, int windowWidth, int windowHeight, int y0){
    setAll(world, window, ren, sdlEvent, y0);
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    selectColor.r = DEFAULT_SELECT_COLOR_R;
    selectColor.g = DEFAULT_SELECT_COLOR_G;
    selectColor.b = DEFAULT_SELECT_COLOR_B;
    selectColor.a = DEFAULT_SELECT_COLOR_A;
}

//****************************************** Setter and Getter Definition ******************************************
void P2D_SDL_Event::setAll(P2D_World* world, SDL_Window* window,SDL_Renderer* ren, SDL_Event* event, int y0){
    setWorld(world);
    setWindow(window);
    setRenderer(ren);
    setSDL_Event(event);
    setY0(y0);
}
void P2D_SDL_Event::setWorld(P2D_World* world){
    this->world = world;
    if(world == nullptr)
        cout << "Warning: P2D_SDL_Event: setWorld(nullptr)!" << endl;
}
P2D_World* P2D_SDL_Event::getWorld(){
    return world;
}
void P2D_SDL_Event::setWindow(SDL_Window* window){
    this->window = window;
    if(window == nullptr)
        cout << "Warning: P2D_SDL_Event: setWindow(nullptr)!" << endl;
}
SDL_Window* P2D_SDL_Event::getWindow(){
    return window;
}
void P2D_SDL_Event::setRenderer(SDL_Renderer* ren){
    this->renderer = ren;
    if(ren == nullptr)
        cout << "Warning: P2D_SDL_Event: setWindow(nullptr)!" << endl;
}
SDL_Renderer* P2D_SDL_Event::getRenderer(){
    return renderer;
}
void P2D_SDL_Event::setSDL_Event(SDL_Event* event){
    this->sdlEvent = event;
    if(event == nullptr)
        cout << "Warning: P2D_SDL_Event: setSDL_Event(nullptr)!" << endl;
}
SDL_Event* P2D_SDL_Event::getSDL_Event(){
    return sdlEvent;
}
void P2D_SDL_Event::setY0(int y0){
    this->y0 = y0;
    if(y0 == NAN)
        cout << "Warning: P2D_SDL_Event: setY0(NAN)!" << endl;
}
int P2D_SDL_Event::getY0(){
    return y0;
}
void P2D_SDL_Event::setWorldFileName(const std::string& fileName){
    this->worldFileName = fileName;
}
void P2D_SDL_Event::setReplayFileName(const std::string& fileName){
    this->replaysFileName = fileName;
}
string P2D_SDL_Event::getWorldFileName(){
    return worldFileName;
}
string P2D_SDL_Event::getReplayFileName(){
    return replaysFileName;
}
int P2D_SDL_Event::updateRealMouseXY(){
    int windowHeightOver2 = windowHeight/2, windowWidthOver2 = windowWidth/2;
    realMouseX = (mouseX - windowWidthOver2)/world->perspectiveZoom + windowWidthOver2 + world->perspectiveX;
    realMouseY  = (y0 - mouseY - windowHeightOver2)/world->perspectiveZoom + windowHeightOver2 + world->perspectiveY;
    return realMouseY;
}

//****************************************** Setter and Getter Definition ******************************************

//****************************************** Public Function Definition ******************************************

void P2D_SDL_Event::eventGo(){
    SDL_GetMouseState(&mouseX, &mouseY);
    updateRealMouseXY();
    
    //remove object along the path of mouse (when R is pressed)
    if(isRemovingObject)
        world->removeObject(getObjectOnMouse());
    
    //change the direction of uniform acceleration (when G key is pressed and mouse is down)
    if(mouseDown && G_Down){
        double mouseDeltaX = mouseX - mouseDownX, mouseDeltaY = mouseY - mouseDownY;
        double mouseDistance = sqrt(mouseDeltaX * mouseDeltaX + mouseDeltaY * mouseDeltaY);
        if(mouseDistance != 0){
            double cosTheta = mouseDeltaX/mouseDistance, sinTheta = mouseDeltaY/mouseDistance;
            world->accelerationX = mouseDownAcceleration * cosTheta;
            world->accelerationY = -mouseDownAcceleration * sinTheta;
        }
    }
    
    //mouse drag the object
    if(mouseDown && Z_Statue && !CONTROL_Down){
        //update object's position and velocity according to mouse's position and velocity
        
        double mvx, mvy;
        mouseSpeedFromLastCall(&mvx, &mvy);
        if(world->isObjectPtrValid(mouseMovingObject)){
            
            //TODO: make it better? don't center at object's center (at mouse)? other shape
            
            mouseMovingObject->x = realMouseX;
            mouseMovingObject->y = realMouseY;
            
            mouseMovingObject->vx = (mvx) * DEFAULT_FRAME_INTERVAL;
            mouseMovingObject->vy = -(mvy) * DEFAULT_FRAME_INTERVAL;
        }else{
            if(mouseMovingLowerWall){
                world->lowerWall = realMouseY;
                world->lowerWallV = -(mvy) * DEFAULT_FRAME_INTERVAL;
            }
            if(mouseMovingUpperWall){
                world->upperWall = realMouseY;
                world->upperWallV = -(mvy) * DEFAULT_FRAME_INTERVAL;
            }
            if(mouseMovingLeftWall){
                world->leftWall = realMouseX;
                world->leftWallV = (mvx) * DEFAULT_FRAME_INTERVAL;
            }
            if(mouseMovingRightWall){
                world->rightWall = realMouseX;
                world->rightWallV = (mvx) * DEFAULT_FRAME_INTERVAL;
            }
        }
    }
    
    //change perspective actions
    if(!SHIFT_Down){
        if(W_Down){
            world->perspectiveY += PERSPECTIVE_CHANGE_RATE/world->perspectiveZoom;
        }
        if(S_Down){
            world->perspectiveY -= PERSPECTIVE_CHANGE_RATE/world->perspectiveZoom;
        }
        if(A_Down){
            world->perspectiveX -= PERSPECTIVE_CHANGE_RATE/world->perspectiveZoom;
        }
        if(D_Down){
            world->perspectiveX += PERSPECTIVE_CHANGE_RATE/world->perspectiveZoom;
        }
    }else{
        if(world->isObjectPtrValid(selectedObject)){
            if(W_Down){
                selectedObject->vy += selectedObjectAddV;
            }
            if(S_Down){
                selectedObject->vy -= selectedObjectAddV;
            }
            if(A_Down){
                selectedObject->vx -= selectedObjectAddV;
            }
            if(D_Down){
                selectedObject->vx += selectedObjectAddV;
                
            }
        }
    }
    
    
    //-----------------------------drawing-----------------------------
    drawWorld(renderer, world, windowWidth, windowHeight, windowHeight);
    //-----------------------------drawing-----------------------------
}
void P2D_SDL_Event::eventOccur(){
    SDL_KeyboardEvent keyEvent = sdlEvent->key;
    if(sdlEvent->type == SDL_KEYDOWN){
        //=============================================== Key Down ===============================================
        if(keyEvent.keysym.scancode == SDL_SCANCODE_SPACE){
            //---------------------------------------SPACE BAR----------------------------------------
            if(!SHIFT_Down){
                if(world->getPause()){
                    world->setPause(false);
                    cout << "P2D_World start!" << endl;
                }else{
                    world->setPause(true);
                    cout << "P2D_World pause!" << endl;
                }
            }else{
                world->perspectiveX = 0;
                world->perspectiveY = 0;
                world->perspectiveZoom = 1;
            }
            //---------------------------------------SPACE BAR----------------------------------------
        }else if(keyEvent.keysym.scancode == SDL_SCANCODE_RETURN){
            //---------------------------------------Return----------------------------------------
            string input;
            
            //indicate input statue
            if(!continuousCommandInput)
                cout << "World " << world->id << ": ";
            else
                cout << "-> ";
            
            //get a line of input then split it and execute
            if(getline(cin, input)){
                executeCommandLine(input);
            }else{
                cin.clear();
                cout << "failed cin" << endl;
            }
            
            if(continuousCommandInput)
                eventOccur();
            else{
                cout << "Done!" << endl;
            }
            
            //the code at here may be called many times due to the iteration calling
            //---------------------------------------Return----------------------------------------
        }
        else if(keyEvent.keysym.scancode == SDL_SCANCODE_LSHIFT || keyEvent.keysym.scancode == SDL_SCANCODE_RSHIFT){
            //---------------------------------------Shift / Control----------------------------------------
            SHIFT_Down = true;
        }
        else if(keyEvent.keysym.scancode == SDL_SCANCODE_LCTRL || keyEvent.keysym.scancode == SDL_SCANCODE_RCTRL){
            CONTROL_Down = true;
            if(SHIFT_Down){
                if(WSADQEMovePerspective)
                    WSADQEMovePerspective = false;
                else
                    WSADQEMovePerspective = true;
            }
        }
        //---------------------------------------number Key----------------------------------------
        else if(keyEvent.keysym.scancode == SDL_SCANCODE_0){
            if(lastWorld != nullptr && world != nullptr){
                *world = *lastWorld;
            }
        }
        //---------------------------------------WSADQE Key----------------------------------------
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_W){
            W_Down = true;
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_S){
            S_Down = true;
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_A){
            A_Down = true;
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_D){
            D_Down = true;
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_E){
            if(!SHIFT_Down){
                world->perspectiveZoom /= PERSPECTIVE_ZOOM_CHANGE_RATE;
            }else
                if(world->isObjectPtrValid(selectedObject))
                selectedObject->angularVelocity -= selectedObjectAddAngV;
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_Q){
            if(!SHIFT_Down){
                world->perspectiveZoom *= PERSPECTIVE_ZOOM_CHANGE_RATE;
            }else
                if(world->isObjectPtrValid(selectedObject))
                    selectedObject->angularVelocity += selectedObjectAddAngV;
        }
        //---------------------------------------WSADARQE Key----------------------------------------
        
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_P){
            //---------------------------------------P Key----------------------------------------
            //turn on or turn off acceleration (isAccelerationProhibit)
            if(world->getAccelerationProhibit()){
                world->setAccelerationProhibit(false);
                cout << "P2D_World acceleration turn on!" << endl;
            }
            else{
                world->setAccelerationProhibit(true);
                cout << "P2D_World acceleration turn off!" << endl;
            }
            //---------------------------------------P Key----------------------------------------
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_C){
            //---------------------------------------C Key----------------------------------------
            //create an object based on stored setting in world
            SDL_GetMouseState(&mouseX, &mouseY);
            updateRealMouseXY();
            world->addObjectX = realMouseX;
            world->addObjectY = realMouseY;
            world->createObject(P2D_ADD_OBJECT_BY_STORED_SETTING);
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_R){
            //---------------------------------------R Key----------------------------------------
            isRemovingObject = true;
            R_Down = true;
            if(SHIFT_Down){
                world->removeAllObject();
            }
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_G){
            //---------------------------------------G Key----------------------------------------
            G_Down = true;
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_K){
            //---------------------------------------K Key----------------------------------------
            if(world->getCollision())
                world->setCollision(false);
            else
                world->setCollision(true);
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_Z){
            //---------------------------------------Z Key----------------------------------------
            if(Z_Statue){
                mouseMovingObject = nullptr;
                Z_Statue = false;
            }
            else{
                if(mouseDown){
                    SDL_GetMouseState(&mouseX, &mouseY);
                    mouseMovingObject = world->getObjectAtPosition(realMouseX, realMouseY);
                }
                Z_Statue = true;
            }
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_X){
            //---------------------------------------X Key----------------------------------------
            //select or de-select object on mouse's position
            P2D_Object* x = getObjectOnMouse();
            
            if(world->isObjectPtrValid(x)){
                //if object is the selected object, de-selecte the selected(this) object
                if(x == selectedObject){
                    selectedObject->shape.color = selectedObjectColorBefore;
                    selectedObject = nullptr;
                }else{
                    if(world->isObjectPtrValid(selectedObject))
                        selectedObject->shape.color = selectedObjectColorBefore;
                    
                    selectedObject = x;
                    selectedObjectColorBefore = x->shape.color;
                    selectedObject->shape.color = selectColor;
                }
            }
        }
        //=============================================== Key Down ===============================================
    }else if(sdlEvent->type == SDL_KEYUP){
        //=============================================== Key Up ===============================================
        if(keyEvent.keysym.scancode == SDL_SCANCODE_LSHIFT || keyEvent.keysym.scancode == SDL_SCANCODE_RSHIFT){
            SHIFT_Down = false;
        }
        else if(keyEvent.keysym.scancode == SDL_SCANCODE_LCTRL || keyEvent.keysym.scancode == SDL_SCANCODE_RCTRL){
            CONTROL_Down = false;
        }
        //---------------------------------------WSAD Key----------------------------------------
        else if(keyEvent.keysym.scancode == SDL_SCANCODE_W){
            W_Down = false;
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_S){
            S_Down = false;
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_A){
            A_Down = false;
        }
        else if (keyEvent.keysym.scancode == SDL_SCANCODE_D){
            D_Down = false;
        }
        //---------------------------------------WSAD Key----------------------------------------
        else if(keyEvent.keysym.scancode == SDL_SCANCODE_G){
            G_Down = false;
            world->accelerationX = mouseDownAccelerationX;
            world->accelerationY = mouseDownAccelerationY;
        }
        else if(keyEvent.keysym.scancode == SDL_SCANCODE_R){
            isRemovingObject = false;
            R_Down = false;
        }
        
        //=============================================== Key Up ===============================================
    }else if(sdlEvent->type == SDL_MOUSEBUTTONDOWN){
        //=============================================== Mouse Down ===============================================
        //store when mouse down
        SDL_GetMouseState(&mouseX, &mouseY);
        updateRealMouseXY();
        mouseDown = true;
        mouseDownX = mouseX;
        mouseDownY = mouseY;
        mouseDownAccelerationX = world->accelerationX;
        mouseDownAccelerationY = world->accelerationY;
        mouseDownAcceleration = sqrt(mouseDownAccelerationX * mouseDownAccelerationX + mouseDownAccelerationY * mouseDownAccelerationY);
        
        //if dragging object (pressed Z), set the dragging object(pointer)
        if(Z_Statue){
            //get object that is at mouse's position
            mouseMovingObject = getObjectOnMouse();
            
            //if no object on mouse, check if wall is on mouse
            if(!world->isObjectPtrValid(mouseMovingObject)){
                if(world->isLeftWall && realMouseX > world->leftWall - CLICK_GET_OBJECT_RANGE && realMouseX < world->leftWall + CLICK_GET_OBJECT_RANGE)
                    mouseMovingLeftWall = true;
                if(world->isRightWall && realMouseX > world->rightWall - CLICK_GET_OBJECT_RANGE && realMouseX < world->rightWall + CLICK_GET_OBJECT_RANGE)
                    mouseMovingRightWall = true;
                if(world->isUpperWall && realMouseY > world->upperWall - CLICK_GET_OBJECT_RANGE && realMouseY < world->upperWall + CLICK_GET_OBJECT_RANGE)
                    mouseMovingUpperWall = true;
                if(world->isLowerWall && realMouseY > world->lowerWall - CLICK_GET_OBJECT_RANGE && realMouseY < world->lowerWall + CLICK_GET_OBJECT_RANGE)
                    mouseMovingLowerWall = true;
                if(mouseMovingUpperWall && mouseMovingLowerWall){
                    mouseMovingLowerWall = false;
                }
                if(mouseMovingLeftWall && mouseMovingRightWall){
                    mouseMovingRightWall = false;
                }
            }
        }
        
        //display object's property when mouse down w/ shift        TODO options/finish/
        if(SHIFT_Down){
            P2D_Object* mouse_DownObject = getObjectOnMouse();
            if(world->isObjectPtrValid(mouse_DownObject)){
                cout << "m = " << mouse_DownObject->mass << endl;
            }
        }
        
        //=============================================== Mouse Down ===============================================
    }else if(sdlEvent->type == SDL_MOUSEBUTTONUP){
        //=============================================== Mouse Up ===============================================
        
        mouseDown = false;
        mouseMovingObject = nullptr;
        isMovingObject = false;
        mouseMovingLowerWall = false;
        mouseMovingUpperWall = false;
        mouseMovingLeftWall = false;
        mouseMovingRightWall = false;
        world->leftWallV = 0;
        world->rightWallV = 0;
        world->upperWallV = 0;
        world->lowerWallV = 0;
        
        
        world->accelerationX = mouseDownAccelerationX;
        world->accelerationY = mouseDownAccelerationY;
        //=============================================== Mouse Up ===============================================
    }
    //TODO
}
//****************************************** Public Function Definition ******************************************
//****************************************** Private Function Definition ******************************************
P2D_Object* P2D_SDL_Event::getObjectOnMouse(){
    P2D_Object* ret = nullptr;
    ret = world->getObjectAtPosition(realMouseX, realMouseY);
    if(ret == nullptr){
        ret = world->getObjectAtPositionArea(realMouseX, realMouseY, CLICK_GET_OBJECT_RANGE);
    }
    return ret;
}
void P2D_SDL_Event::applyNumberInput(double* target, double ind){
    if(!isOutputCommand){
        *target = ind;
        coutAppliedCommand(numberInput, to_string(ind));
    }else{
        coutOutputCommand(numberInput, to_string(ind));
    }
}
void P2D_SDL_Event::coutAppliedCommand(const std::string& msg1, const std::string& msg2){
    cout << msg1 << " -> " << msg2 << endl;
}
void P2D_SDL_Event::coutOutputCommand(const std::string& msg1, const std::string& msg2){
    cout << msg1 << " = " << msg2 << endl;
}
void P2D_SDL_Event::executeCommandLine(const std::string& inputLine){
    vector<string> inputV = split(inputLine);
    
    //-----------------------------------command inputs-----------------------------------
    for(vector<string>::const_iterator i = inputV.begin();i != inputV.end();++i){
        if(isNumberInput){
            if((*i) == "o")
                isOutputCommand = true;
            
            double ind = atof((*i).c_str());
            //------------------------create number------------------------
            if(numberInput == "x"){
                applyNumberInput(&world->addObjectX, ind);
                //world->addObjectX = ind;
                //coutAppliedCommand("x", to_string(ind));
            }
            else if(numberInput == "y"){
                world->addObjectY = ind;
                coutAppliedCommand("y", to_string(ind));
            }
            else if(numberInput == "ang"){
                world->addObjectAngle = ind;
                coutAppliedCommand("ang", to_string(ind));
            }
            else if(numberInput == "vx"){
                world->addObjectVX = ind;
                coutAppliedCommand("vx", to_string(ind));
            }
            else if(numberInput == "vy"){
                world->addObjectVY = ind;
                coutAppliedCommand("vy", to_string(ind));
            }
            else if(numberInput == "angv"){
                world->addObjectAngularVelocity = ind;
                coutAppliedCommand("angv", to_string(ind));
            }
            else if(numberInput == "m"){
                world->addObjectMass = ind;
                coutAppliedCommand("m", to_string(ind));
            }
            else if(numberInput == "collisionk" || numberInput == "clsk"){
                world->addObjectCollisionCoefficient = ind;//TODO: bug potential
                coutAppliedCommand("collision coefficient", to_string(ind));
            }
            else if(numberInput == "r"){
                world->addObjectCircleRadius = ind;
                coutAppliedCommand("r", to_string(ind));
            }
            else if(numberInput == "ccircle" || numberInput == "cc"){
                P2D_Shape shape;
                shape.setCircle(world->addObjectCircleRadius);
                if(shape.getIsValid()){
                    coutAppliedCommand("create circles", to_string(ind));
                    world->createObjects(ind, shape, world->addObjectFlag);
                }else
                    cout << "failed!" << endl;
            }
            //------------------------create number------------------------
            //------------------------world number------------------------
            else if(numberInput == "ax"){
                world->accelerationX = ind;
                coutAppliedCommand("ax", to_string(ind));
            }
            else if (numberInput == "ay"){
                world->accelerationY = ind;
                coutAppliedCommand("ay", to_string(ind));
            }
            else if (numberInput == "fx"){
                world->forceX = ind;
                coutAppliedCommand("fx", to_string(ind));
            }
            else if (numberInput == "fy"){
                world->forceY = ind;
                coutAppliedCommand("fy", to_string(ind));
            }
            else if (numberInput == "clska" || numberInput == "collisionkall"){
                world->setCollisionCoefficientAll(ind);
                coutAppliedCommand("All collision coefficient", to_string(ind));
            }
            else if (numberInput == "upperwall" || numberInput == "uw"){
                world->setWall(ind, P2D_UPPER_WALL);
                coutAppliedCommand("upperwall", to_string(ind));
            }
            else if (numberInput == "lowerwall" || numberInput == "lww"){
                world->setWall(ind, P2D_LOWER_WALL);
                coutAppliedCommand("lowerwall", to_string(ind));
            }
            else if (numberInput == "leftwall" || numberInput == "lfw"){
                world->setWall(ind, P2D_LEFT_WALL);
                coutAppliedCommand("leftwall", to_string(ind));
            }
            else if (numberInput == "rightwall" || numberInput == "rw"){
                world->setWall(ind, P2D_RIGHT_WALL);
                coutAppliedCommand("rightwall", to_string(ind));
            }
            else if (numberInput == "calculationtimes" || numberInput == "ct"){
                world->setCalculationTimes(ind);
                coutAppliedCommand("calculation times", to_string(ind));
            }
            else if(numberInput == "gravik"){
                world->gravitationK = ind;
                coutAppliedCommand("gravitation coefficient", to_string(ind));
            }
            else if(numberInput == "elesk"){
                world->electricForceK = ind;
                coutAppliedCommand("electro static field coefficient", to_string(ind));
            }
            else if(numberInput == "gravilvl"){
                world->addObjectGravitationLevel = ind;
                coutAppliedCommand("gravitation level for subsequent created objects", to_string(ind));
            }
            else if(numberInput == "gravigsp"){
                typedef vector<P2D_GravitationGroup>::iterator itg;
                //for(itg i = world->gravitationGroups.begin(); i != world->gravitationGroups.end(); ++i){
                //    if(i->gravitationLevel == (unsigned int)ind)
                //        i->isSelfProhibit = true;
                //}//TODO finish
            }
            //------------------------world number------------------------
            else
                cout << "command input error" << endl;
            isNumberInput = false;
            isOutputCommand = false;
        }else{
            //------------------------create------------------------
            if(*i == "normalposition" || *i == "normp"){
                world->addObjectFlag = P2D_ADD_OBJECT_BY_STORED_SETTING;
                coutAppliedCommand("position", "normal");
            }
            else if(*i == "randomposition" || *i == "ranp"){
                world->addObjectFlag = P2D_ADD_OBJECT_RANDOM_WITHIN_WALL;
                coutAppliedCommand("position", "random");
            }
            else if(*i == "circle"){
                coutAppliedCommand("Shape", "Circle");
                world->addObjectShape.setCircle(world->addObjectCircleRadius);
            }
            else if(*i == "rect"){
                coutAppliedCommand("Shape", "Rectangle");
                world->addObjectShape.setRect(world->addObjectRectWidth, world->addObjectRectHeight);
            }
            else if(*i == "line"){
                coutAppliedCommand("Shape", "Line");
                world->addObjectShape.setLine(world->addObjectLineLength);
            }
            else if(*i == "x" || *i == "y" || *i == "ang" || *i == "vx" || *i == "vy" || *i == "angv" || *i == "m" || *i == "collisionk" || *i == "clsk" || *i == "r"){
                isNumberInput = true;
                numberInput = (*i);
            }
            //------------------------create------------------------
            //------------------------world------------------------
            else if(*i == "ax" || *i == "ay" || *i == "fx" || *i == "fy" || *i == "clska" || *i == "collisionkall" || *i == "upperwall" || *i == "uw" || *i == "lowerwall" || *i == "lww" || *i == "leftwall" || *i == "lfw" || *i == "rightwall" || *i == "rw" || *i == "ct" || *i == "calculationtimes" || *i == "ccircle" || *i == "cc" || *i == "gravik" || *i == "elesk" || *i == "gravilvl" || *i == "gravigsp"){
                isNumberInput = true;
                numberInput = (*i);
            }
            else if(*i == "gravigspall"){
                typedef vector<P2D_GravitationGroup>::iterator itg;
                //for(itg i = world->gravitationGroups.begin(); i != world->gravitationGroups.end(); ++i)
                 //   i->isSelfProhibit = true;
            }
            else if(*i == "calcrep"){
                string name;
                cout << "name for this replay -> ";
                cin >> name;
                P2D_SDL_Replay *rep = P2D_SDL_Replay::findReplayInFile(name, replaysFileName);
                if(rep != nullptr){
                    rep->~P2D_SDL_Replay();
                    cout << "This name already exist, try another one!" << endl;
                }else{
                    rep = new P2D_SDL_Replay();
                    rep->setName(name);
                    cout << "# of frames -> ";
                    size_t frames = 0;
                    cin >> frames;
                    if(rep->calcrep(world, frames)){
                        P2D_SDL_Replay::appendReplayToFile(*rep, replaysFileName);
                        replays.push_back(*rep);
                        cout << endl << "Success!" << endl;
                    }
                    else
                        cout << "Failed!" << endl;
                }
                replayFileLoaded = false;
            }
            else if(*i == "lsrep"){
                if(!replayFileLoaded)
                    P2D_SDL_Replay::loadReplaysInFile(replays, replaysFileName);
                replayFileLoaded = true;
                cout << "-----------------replays----------------" << endl;
                for(vector<P2D_SDL_Replay>::const_iterator i=replays.begin();i!=replays.end();++i)
                    cout << (*i).getName() << endl;
                cout << "----------------------------------------" << endl;
            }
            else if(*i == "rep"){
                if(!replayFileLoaded)
                    P2D_SDL_Replay::loadReplaysInFile(replays, replaysFileName);
                replayFileLoaded = true;
                cout << "name of the replay -> ";
                string name;
                cin >> name;
                bool found = false;
                for(vector<P2D_SDL_Replay>::iterator i=replays.begin();i!=replays.end();++i){
                    if(i->getName() == name){
                        found = true;
                        replay(&*i);
                    }
                }
                if(!found) cout << "Did not find this replay!" << endl;
            }
            else if(*i == "rmrep"){
                if(!replayFileLoaded)
                    P2D_SDL_Replay::loadReplaysInFile(replays, replaysFileName);
                replayFileLoaded = true;
                cout << "remove replay: name -> " << endl;
                string name;
                cin >> name;
                bool found = false;
                for(vector<P2D_SDL_Replay>::iterator i=replays.begin();i!=replays.end();++i){
                    if(i->getName() == name){
                        found = true;
                        replays.erase(i);
                        P2D_SDL_Replay::saveReplaysToFile(replays, replaysFileName);
                    }
                    if(found) break;
                }
                if(!found) cout << "Did not find this replay to remove!" << endl;
            }
            else if(*i == "saveworld" || *i == "svwld"){
                cout << "save world: name -> ";
                string name, prename = world->getName();
                cin >> name;
                if(findWorld(name) == nullptr){
                    world->setName(name);
                    saveWorld(world);
                }else
                    cout << "This name already exist, try another name!" << endl;
            }
            else if(*i == "loadworld" || *i == "ldwld"){
                loadWorlds();
            }
            else if(*i == "filename" || *i == "fn"){
                cout << "save/load world file name -> ";
                string name;
                cin >> name;
                if(!(name.size() > 0)){
                    cout << "set file name failed (invalid name)!" << endl;
                }else{
                    worldFileName = name;
                }
            }
            else if(*i == "lsworld" || *i == "lswld"){
                cout << "-----------------worlds-----------------" << endl;
                if(!isWorldsLoaded)
                    loadWorlds();
                for(vector<P2D_World*>::const_iterator i = worlds.begin(); i != worlds.end(); ++i){
                    cout << (*i)->getName() << endl;
                }
                cout << "----------------------------------------" << endl;
            }
            else if(*i == "world" || *i == "wld"){
                cout << "world name -> ";
                string name;
                cin >> name;
                P2D_World* w = findWorld(name);
                if(w == nullptr){
                    cout << "No such world in the file: " << worldFileName << "!" << endl;
                }else{
                    applyWorld(w);
                }
            }
            else if(*i == "rmworld" || *i == "rmwld"){
                if(!isWorldsLoaded)
                    loadWorlds();
                cout << "remove world: name -> ";
                string name;
                cin >> name;
                bool found = false;
                for(vector<P2D_World*>::iterator i =worlds.begin();i!=worlds.end();++i){
                    if((*i)->getName() == name){
                        found = true;
                        delete (*i);
                        worlds.erase(i);
                        saveWorlds();
                    }
                    if(found) break;
                }
                if(!found) cout << "Did not find this world to remove!" << endl;
            }
            else if(*i == "clear" || *i == "removeall"){
                world->removeAllObject();
                cout << "removed all objects!" << endl;
            }
            else if(*i == "setdefault" || *i == "sd"){
                world->setDefault();
                cout << "Set sefault!" << endl;
            }
            else if(*i == "enablegravitation" || *i == "engravi"){
                world->enableGravitation(true);
            }
            else if(*i == "disablegravitation" || *i == "disgravi"){
                world->enableGravitation(false);
            }
            else if(*i == "enableeles" || *i == "eneles"){
                world->enableElectroStaticForce(true);
            }
            else if(*i == "disableeles" || *i == "diseles"){
                world->enableElectroStaticForce(false);
            }
            else if(*i == "enableeled" || *i == "eneled"){
                world->enableElectroDynamicForce(true);
            }
            else if(*i == "disableeled" || *i == "diseled"){
                world->enableElectroDynamicForce(false);
            }
            else if(*i == "enablecollisionall" || *i == "enclsa"){
                world->setCollision(true);
                cout << "enable all collision!" << endl;
            }
            else if(*i == "disablecollisionall" || *i == "disclsa"){
                world->setCollision(false);
                cout << "disable all collision!" << endl;
            }
            else if(*i == "enableairresistance" || *i == "enairr"){
                world->enableAirResistance(true);
            }
            else if(*i == "disableairresistance" || *i == "disairr"){
                world->enableAirResistance(false);
            }
            else if(*i == "enupperwall" || *i == "enuw"){
                world->enableWall(true, P2D_UPPER_WALL);
            }else if(*i == "enlowerwall" || *i == "enlww"){
                world->enableWall(true, P2D_LOWER_WALL);
            }else if(*i == "enleftwall" || *i == "enlfw"){
                world->enableWall(true, P2D_LEFT_WALL);
            }else if(*i == "enrightwall" || *i == "enrw"){
                world->enableWall(true, P2D_RIGHT_WALL);
            }else if(*i == "disupperwall" || *i == "disuw"){
                world->enableWall(false, P2D_UPPER_WALL);
            }else if(*i == "dislowerwall" || *i == "dislww"){
                world->enableWall(false, P2D_LOWER_WALL);
            }else if(*i == "disleftwall" || *i == "dislfw"){
                world->enableWall(false, P2D_LEFT_WALL);
            }else if(*i == "disrightwall" || *i == "disrw"){
                world->enableWall(false, P2D_RIGHT_WALL);
            }else if(*i == "diswallall" || *i == "diswa"){
                world->enableWall(false, P2D_ALL_WALL);
            }else if(*i == "enwallall" || *i == "enwa"){
                world->enableWall(true, P2D_ALL_WALL);
            }
            else if(*i == "showa"){
                cout << "----------------------------------------" << endl;
                cout << "m    = " << world->addObjectMass << endl;
                cout << "x    = " << world->addObjectX << endl;
                cout << "y    = " << world->addObjectY << endl;
                cout << "vx   = " << world->addObjectVX << endl;
                cout << "vy   = " << world->addObjectVY << endl;
                cout << "ang  = " << world->addObjectAngle << endl;
                cout << "angv = " << world->addObjectAngularVelocity << endl << endl;
                cout << "circle radius = " << world->addObjectCircleRadius << endl;
                cout << "rect width    = " << world->addObjectRectWidth << endl;
                cout << "rect height   = " << world->addObjectRectHeight << endl;
                cout << "line length   = " << world->addObjectLineLength << endl;
                cout << "k: collision  = " << world->addObjectCollisionCoefficient << endl;
                cout << "k: friction   = " << world->addObjectFrictionCoefficient << endl;
                cout << "k: static fri = " << world->addObjectStaticFrictionCoefficient << endl;
                if(world->addObjectFlag == P2D_ADD_OBJECT_RANDOM_WITHIN_WALL)
                cout << "flag          = random postions within walls" << endl;
                else if(world->addObjectFlag == P2D_ADD_OBJECT_BY_STORED_SETTING)
                cout << "flag          = by settings" << endl;
                cout << "----------------------------------------" << endl;
            }
            else if(*i == "showw"){
                cout << "----------------------------------------" << endl;
                cout << "name  = " << world->getName() << endl;
                if(world->getPause())
                    cout << "paused" << endl;
                else
                    cout << "not paused" << endl;
                cout << "perspective x      = " << world->perspectiveX << endl;
                cout << "perspective y      = " << world->perspectiveY << endl;
                cout << "perspective zoom   = " << world->perspectiveZoom << endl;
                cout << "perspective width  = " << world->perspectiveWidth << endl;
                cout << "perspective height = " << world->perspectiveHeight << endl << endl;
                
                cout << "ax    = " << world->accelerationX << endl;
                cout << "ay    = " << world->accelerationY << endl;
                cout << "fx    = " << world->forceX << endl;
                cout << "fy    = " << world->forceY << endl << endl;
                
                cout << "gravitation      = " << (world->getIsGravitation()? "ON" : "OFF") << endl;
                cout << "electro static   = " << (world->getIsElectroStaticForce()? "ON" : "OFF") << endl;
                cout << "electro dynamic  = " << (world->getIsElectroDynamicForce()? "ON" : "OFF") << endl;
                cout << "air resistance   = " << (world->getIsAirResistance()? "ON" : "OFF") << endl;
                cout << "precise air resistance = " << (world->getIsPreciseAirResistance()? "ON" : "OFF") << endl;
                cout << "no accelerations = " << (world->getAccelerationProhibit()? "ON" : "OFF") << endl;
                cout << "allow collision  = " << (world->getCollision()? "ON" : "OFF") << endl;

                cout << "gravitation k     = " << world->gravitationK << endl;
                cout << "electric static k = " << world->electricForceK << endl;
                cout << "air resistance k  = " << world->getAirResistanceCoefficient() << endl<<endl;
                //TODO collsion k
                
                cout << "Upper wall:  " << (world->isUpperWall? "ON" : "OFF") << " " << world->upperWall << endl;
                cout << "Lower wall:  " << (world->isLowerWall? "ON" : "OFF") << " " << world->lowerWall << endl;
                cout << "Left wall:   " << (world->isLeftWall? "ON" : "OFF") << " " << world->leftWall << endl;
                cout << "Right wall:  " << (world->isRightWall? "ON" : "OFF") << " " << world->rightWall << endl;
                cout << "Wall collision k       = " << world->wallCollisionCoefficient << endl;
                cout << "Wall friction k        = " << world->wallFrictionCoefficient << endl;
                cout << "Wall static friction k = " << world->wallStaticFrictionCoefficient << endl << endl;
                
                cout << "number of objects      = " << world->objects.size() << endl;
                cout << "calculation times      = " << world->getCalculationTimes() << endl;
                cout << "----------------------------------------" << endl;
            }
            //------------------------world------------------------
            else if(*i == "v"){
                if(continuousCommandInput)
                    continuousCommandInput = false;
                else
                    continuousCommandInput = true;
            }
            else{
                cout << "command not found: " << (*i) << endl;
            }//TODO: input x y vx vy
        }
    }
    isNumberInput = false;
    //-----------------------------------command inputs-----------------------------------
}

void P2D_SDL_Event::loadWorlds(){
    worlds.clear();
    ifstream inf(worldFileName, ios::binary | ios::in);
    if(inf.fail()){
        logOpenFileError("loadWorlds", worldFileName);
        inf.close();
        return;
    }
    bool b = true;
    while(b){
        P2D_World* w = P2D_World::P2D_Create_World();
        w->read(inf);
        if (inf.fail()) {
            delete w;
            b = false;
            continue;
        }
        worlds.push_back(w);
    }
    inf.close();
    isWorldsLoaded = true;
}
P2D_World* P2D_SDL_Event::findWorld(const std::string& name){
    if(!isWorldsLoaded)
        loadWorlds();
    for(vector<P2D_World*>::iterator i = worlds.begin(); i != worlds.end(); ++i){
        if((*i)->getName() == name)
            return (*i);
    }
    return nullptr;
}
void P2D_SDL_Event::applyWorld(P2D_World* w){
    *world = *w;
}
void P2D_SDL_Event::saveWorld(P2D_World * w){
    if(findWorld(w->getName()) == nullptr){
        ofstream of(worldFileName, ios::out | ios::binary | ios::app);
        if(of.fail()){
            logOpenFileError("saveWorld", worldFileName);
            of.close();
            return;
        }
        world->write(of);
        if(of.fail())
            cout << "SaveWorld failed (unkonwn reason, file may be not working anymore)!" << endl;
        of.close();
    }else{
        cout << "There is already a world named " << w->getName() << " in the file (" << worldFileName << ")!" << endl;
    }
    isWorldsLoaded = false;
}
void P2D_SDL_Event::saveWorlds(){
    ofstream of(worldFileName, ios::out | ios::binary);
    if(of.fail()){
        logOpenFileError("saveWorlds", worldFileName);
        of.close();
        return;
    }
    for(vector<P2D_World*>::iterator i = worlds.begin(); i != worlds.end(); ++i){
        (*i)->write(of);
        if(of.fail()){
            cout << "SaveWorlds failed, failed world name: " << (*i)->getName() << " (unkonwn reason, file may be not working anymore)!" << endl;
            return;
        }
    }
}
void P2D_SDL_Event::removeWorld(const string& name){
    P2D_World* w = findWorld(name);
    if(w == nullptr){
        cout << "RemoveWorld: could not find the world named " << name  << "!" << endl;
        return;
    }
    for(vector<P2D_World*>::iterator i = worlds.begin(); i != worlds.end(); ++i){
        if((*i)->getName() == name)
            worlds.erase(i);
    }
    saveWorlds();
}
//****************************************** Private Function Definition ******************************************



void replay(P2D_SDL_Replay* rep){
    SDL_Window* w = SDL_CreateWindow("replay", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(w, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Event e;
    rep->pause = true;
    bool quit = false;
    while (!quit) {
        rep->draw(ren);

        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT)
                quit = true;
            else if(e.type == SDL_KEYDOWN){
                SDL_KeyboardEvent keyEvent = e.key;
                if(keyEvent.keysym.scancode == SDL_SCANCODE_SPACE)
                    rep->pause = false;
                else if(keyEvent.keysym.scancode == SDL_SCANCODE_0)
                    rep->setPositionBegin();
                else if(keyEvent.keysym.scancode == SDL_SCANCODE_Q)
                    quit = true;
            }
        }
    }
    rep->setPositionBegin();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(w);
}
