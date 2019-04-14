//
//  P2D_ReplayWorld.cpp
//  physics2d
//
//  Created by Jiuhua on 6/20/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "P2D_ReplayWorld.hpp"
using namespace std;
void P2D_ReplayWorld::read(std::ifstream& inf){
    inf.read((char*)&perspectiveX, sizeof(perspectiveX));
    inf.read((char*)&perspectiveY, sizeof(perspectiveY));
    inf.read((char*)&perspectiveZoom, sizeof(perspectiveZoom));
    size_t nameSize = 0;
    inf.read((char*)&nameSize, sizeof(nameSize));
    inf.read((char*)&name, nameSize);
    size_t objectsSize = 0;
    inf.read((char*)&objectsSize, sizeof(objectsSize));
    for(size_t i = 0; i < objectsSize; ++i){
        P2D_ReplayObject ro;
        ro.read(inf);
        objects.push_back(ro);
    }

}
void P2D_ReplayWorld::write(std::ofstream& of) const{
    of.write((char*)&perspectiveX, sizeof(perspectiveX));
    of.write((char*)&perspectiveY, sizeof(perspectiveY));
    of.write((char*)&perspectiveZoom, sizeof(perspectiveZoom));
    size_t nameSize = sizeof(name);
    of.write((char*)&nameSize, sizeof(nameSize));
    of.write((char*)&name, nameSize);
    size_t objectsSize = objects.size();
    of.write((char*)&objectsSize, sizeof(objectsSize));
    for(vector<P2D_ReplayObject>::const_iterator i = objects.begin(); i != objects.end(); i++){
        i->write(of);
    }
}
