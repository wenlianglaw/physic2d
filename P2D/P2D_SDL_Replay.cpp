//
//  P2D_SDL_Replay.cpp
//  physics2d
//
//  Created by Jiuhua on 6/20/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "P2D_SDL_Replay.hpp"
using namespace std;
using namespace dd;
P2D_SDL_Replay::P2D_SDL_Replay(){
    //TODO
}

bool P2D_SDL_Replay::calcrep(P2D_World* world, size_t n){
    P2D_World *w = world->clone();
    w->setPause(false);
    for(size_t i = 0; i < n; ++i){
        addFrame(w);
        w->go();
        showProcess(i, n);
    }
    //TODO: world memory
    delete w;
    cout << "Success!" << endl;
    return true;
}


bool P2D_SDL_Replay::draw(SDL_Renderer* ren){
    if(pause)
        return true;
    if(position == frames.size())
        return false;
    drawReplayWorld(ren, &*(frames.begin()+position), 1000, 600, 600);
    position++;
    return true;
}
void P2D_SDL_Replay::setPositionBegin(){
    position = 0;
}
void P2D_SDL_Replay::setPostionEnd(){
    position = frames.size() - 1;
    if(frames.size() == 0)
        position = 0;
}
void P2D_SDL_Replay::setPosition(size_t p){
    if(p < frames.size())
        position = p;
    else
        cout << "set position out of range!" << endl;
}
void P2D_SDL_Replay::movePosition(size_t r){
    if(position + r < frames.size())
        position += r;
    else
        cout << "move position out of range!" << endl;
}
size_t P2D_SDL_Replay::getPosition(){
    return position;
}

void P2D_SDL_Replay::addFrame(P2D_World* w){
    P2D_ReplayWorld rw;
    rw.name = w->getName();
    rw.perspectiveX = w->perspectiveX;
    rw.perspectiveY = w->perspectiveY;
    rw.perspectiveZoom = w->perspectiveZoom;
    for(vector<P2D_Object*>::const_iterator i = w->objects.begin(); i != w->objects.end(); ++i){
        P2D_ReplayObject ro;
        ro.x = (*i)->x;
        ro.y = (*i)->y;
        ro.angle = (*i)->angle;
        ro.shape = (*i)->shape;
        rw.objects.push_back(ro);
    }
    frames.push_back(rw);
}
void P2D_SDL_Replay::removeFrame(size_t i){
    if(i < frames.size()){
        frames.erase(frames.begin() + i);
    }else
        cout << "remove frame out of range!" << endl;
    if(position == frames.size()){
        position--;
    }
}
void P2D_SDL_Replay::removeAllFrame(){
    frames.clear();
    position = 0;
}

void P2D_SDL_Replay::read(std::ifstream& inf){
    inf.read((char*)&position, sizeof(position));
    size_t nameSize = 0;
    inf.read((char*)&nameSize, sizeof(nameSize));
    inf.read((char*)&name, nameSize);
    size_t framesSize = 0;
    inf.read((char*)&framesSize, sizeof(framesSize));
    for(size_t i = 0; i < framesSize; ++i){
        P2D_ReplayWorld rw;
        rw.read(inf);
        frames.push_back(rw);
    }
}
void P2D_SDL_Replay::write(std::ofstream& of) const{
    of.write((char*)&position, sizeof(position));
    size_t nameSize = sizeof(name);
    of.write((char*)&nameSize, sizeof(nameSize));
    of.write((char*)&name, nameSize);
    size_t framesSize = frames.size();
    of.write((char*)&framesSize, sizeof(framesSize));
    for(vector<P2D_ReplayWorld>::const_iterator i = frames.begin(); i != frames.end() ; ++i){
        i->write(of);
    }
}

void P2D_SDL_Replay::setName(const std::string& name){
    this->name = name;
}
std::string P2D_SDL_Replay::getName() const {
    return name;
}

P2D_SDL_Replay::~P2D_SDL_Replay(){
    //TODO: memory leak?
    frames.clear();
}

bool P2D_SDL_Replay::loadReplaysInFile(std::vector<P2D_SDL_Replay>& reps, const std::string& fileName){
    reps.clear();
    ifstream inf(fileName, ios::binary | ios::in);
    if(inf.fail()){
        logOpenFileError("loadReplaysInFile", fileName);
        inf.close();
        return false;
    }
    P2D_SDL_Replay* x;
    while(!inf.fail()){
        x = new P2D_SDL_Replay();
        x->read(inf);
        if(!inf.fail())
            reps.push_back(*x);
    }
    inf.close();
    return true;
}
P2D_SDL_Replay* P2D_SDL_Replay::findReplayInFile(const std::string& name, const std::string& fileName){
    //TODO: memory? vector reps
    vector<P2D_SDL_Replay> reps;
    loadReplaysInFile(reps, fileName);
    for(vector<P2D_SDL_Replay>::iterator i=reps.begin();i!=reps.end();++i){
        if(i->getName() == name){
            return (&*i);
        }
    }
    return nullptr;
}
bool P2D_SDL_Replay::saveReplaysToFile(const std::vector<P2D_SDL_Replay>& reps, const std::string& fileName){
    ofstream of(fileName, ios::binary | ios::out);
    if(of.fail()){
        logOpenFileError("saveReplaysToFile", fileName);
        of.close();
        return false;
    }
    for(vector<P2D_SDL_Replay>::const_iterator i=reps.begin();i!=reps.end();++i){
        i->write(of);
        if(of.fail()){
            cout << "saveReplaysToFile failed (while writing P2D_SDL_Replay)! " << endl;
            of.close();
            return false;
        }
    }
    of.close();
    return true;
}
bool P2D_SDL_Replay::appendReplayToFile(const P2D_SDL_Replay& rep, const std::string& fileName){
    ofstream of(fileName, ios::binary | ios::out | ios::app);
    if(of.fail()){
        logOpenFileError("appendReplayToFile", fileName);
        of.close();
        return false;
    }
    rep.write(of);
    if(of.fail()){
        cout << "appendReplayToFile failed (while writing P2D_SDL_Replay)! " << endl;
        of.close();
        return false;
    }
    
    of.close();
    return true;
}

void showProcess(size_t n1, size_t n2){
    if(n1 % ((n2-1)/10) == 0){
        size_t prep = cout.precision();
        cout.precision(3);
        cout << endl << (float)n1 / n2 * 100 << "%" << flush;
        cout.precision(prep);
    }
    if(n1 % (n2/100) == 0)
        cout << "-" << flush;
}
