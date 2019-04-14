//
//  P2D_ReplayWorld.hpp
//  physics2d
//
//  Created by Jiuhua on 6/20/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef P2D_ReplayWorld_hpp
#define P2D_ReplayWorld_hpp

#include <stdio.h>
#include "P2D_ReplayObject.hpp"
#include <string>
#include <vector>
#include <fstream>

class P2D_ReplayWorld{
public:
    double perspectiveX, perspectiveY, perspectiveZoom;
    std::string name;
    std::vector<P2D_ReplayObject> objects;
    
    void read(std::ifstream&);
    void write(std::ofstream&) const;
};

#endif /* P2D_ReplayWorld_hpp */
