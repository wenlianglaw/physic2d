//
//  P2D_GravitationGroup.hpp
//  physics2d
//
//  Created by Jiuhua on 6/22/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef P2D_GravitationGroup_hpp
#define P2D_GravitationGroup_hpp

#include <stdio.h>
#include <vector>
#include "P2D_Object.hpp"

class P2D_GravitationGroup{
public:
    std::vector<P2D_Object*> objects;
    unsigned int gravitationLevel = 0;
    bool isSelfProhibit = false;
    
    static bool comparaGravitationLevel(P2D_GravitationGroup, P2D_GravitationGroup);
};

#endif /* P2D_GravitationGroup_hpp */
