//
//  P2D_Replay.hpp
//  physics2d
//
//  Created by Jiuhua on 6/20/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef P2D_Replay_hpp
#define P2D_Replay_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include "P2D_Math.hpp"
#include "P2D_Util.hpp"
#include "P2D_Object.hpp"
#include "P2D_World.hpp"
#include "P2D_Shape.hpp"
#include "P2D_Flags.hpp"
#include "P2D_ReplayObject.hpp"
#include "P2D_ReplayWorld.hpp"
#include "sdl_p2d_util.hpp"
#include <SDL2/SDL.h>

class P2D_SDL_Replay{
public:
    bool pause;
    
    P2D_SDL_Replay();
    
    bool calcrep(P2D_World*, size_t);
    
    bool draw(SDL_Renderer*);
    void setPositionBegin();
    void setPostionEnd();
    void setPosition(size_t);
    void movePosition(size_t);
    size_t getPosition();
    
    void addFrame(P2D_World*);
    void removeFrame(size_t);
    void removeAllFrame();
    
    void read(std::ifstream&);
    void write(std::ofstream&) const;
    
    void setName(const std::string&);
    std::string getName() const;
    
    ~P2D_SDL_Replay();
    
    static bool loadReplaysInFile(std::vector<P2D_SDL_Replay>&, const std::string&);
    static P2D_SDL_Replay* findReplayInFile(const std::string&, const std::string&);
    static bool saveReplaysToFile(const std::vector<P2D_SDL_Replay>&, const std::string&);
    static bool appendReplayToFile(const P2D_SDL_Replay&, const std::string&);

private:
    size_t position = 0;
    
    std::string name;
    
    std::vector<P2D_ReplayWorld> frames;
};


void showProcess(size_t, size_t);



#endif /* P2D_Replay_hpp */

