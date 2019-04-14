//
//  P2D_GravitationGroup.cpp
//  physics2d
//
//  Created by Jiuhua on 6/22/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "P2D_GravitationGroup.hpp"
bool P2D_GravitationGroup::comparaGravitationLevel(P2D_GravitationGroup g1, P2D_GravitationGroup g2){
    return g1.gravitationLevel > g2.gravitationLevel;
}
