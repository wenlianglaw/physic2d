//
//  P2D_ReplayObject.cpp
//  physics2d
//
//  Created by Jiuhua on 6/20/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "P2D_ReplayObject.hpp"
using namespace std;
void P2D_ReplayObject::read(ifstream& inf){
    inf.read((char*)&x, sizeof(x));
    inf.read((char*)&y, sizeof(y));
    inf.read((char*)&angle, sizeof(angle));
    shape.read(inf);
}
void P2D_ReplayObject::write(ofstream& of)const{
    of.write((char*)&x, sizeof(x));
    of.write((char*)&y, sizeof(y));
    of.write((char*)&angle, sizeof(angle));
    shape.write(of);
}
