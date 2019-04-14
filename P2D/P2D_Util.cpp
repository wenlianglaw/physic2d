//
//  P2D_Util.cpp
//  physics2d
//
//  Created by Jiuhua on 6/7/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "P2D_Util.hpp"
using namespace std;
using namespace dd;

void logSetCollisionCoefficientOutOfRangeWarning(string msg){
    cout << msg << " set collision coefficient Warning: k is not within [0,1]!" << endl;
}

void logWarning(string msg){
    cout << "Warning: " << msg << "!" << endl;
}
