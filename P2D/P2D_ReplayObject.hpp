//
//  P2D_ReplayObject.hpp
//  physics2d
//
//  Created by Jiuhua on 6/20/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef P2D_ReplayObject_hpp
#define P2D_ReplayObject_hpp

#include <stdio.h>
#include "P2D_Shape.hpp"
#include <fstream>

class P2D_ReplayObject{
public:
    double x, y, angle;
    P2D_Shape shape;
    
    void read(std::ifstream&);
    void write(std::ofstream&)const;
};

#endif /* P2D_ReplayObject_hpp */
