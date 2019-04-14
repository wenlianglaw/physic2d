//
//  World.cpp
//  physics2d
//
//  Created by Jiuhua on 6/5/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "P2D_World.hpp"
#include <vector>
#include "P2D_Object.hpp"
#include "mathdd.hpp"
#include <math.h>

using namespace std;
using namespace dd;

#define PI_OVER_TWO PI/2
#define THREE_PI_OVER_TWO 3 * PI / 2
#define SMALL_NUMBER 1E-30


//--------------------------------P2D_World class definitions--------------------------------
void P2D_World::go(){
    if(objects.size() == 0)
        return;
    for(unsigned int caltimes = 0;caltimes < calculationTimes;caltimes++){
        if(!pause){
            
            if(!isAccelerationProhibit){
                
                //TODO: detect occuring of invalid stats of objects(nan, inf...)
                //update position(x, y, angle) and various velocity based on various accelerations
                for(vector<P2D_Object*>::iterator i = objects.begin();i != objects.end();++i){
                    if((*i)->isStationary)
                        continue;
                    (*i)->move(calculationTimes);
                    (*i)->updateVelocity(calculationTimes);
                }
                
            }else{
                //update position(x, y, angle) based on various velocity
                for(vector<P2D_Object*>::iterator i = objects.begin();i != objects.end();++i){
                    if((*i)->isStationary)
                        continue;
                    (*i)->move(calculationTimes);
                }
            }
            
            //---------------------------------------collision------------------------------------
            //collision detection and calculations
            world_Collision();
            //---------------------------------------collision------------------------------------
            
            //accelerations
            if(!isAccelerationProhibit)
                updateAccelerations();
        }
    }
}


void P2D_World::setCalculationTimes(unsigned int n){
    if(n > 0){
        calculationTimes = n;
    }else{
        cout << "setCalculationTimes failed(invalid n)!" << endl;
    }
}
unsigned int P2D_World::getCalculationTimes()const{
    return calculationTimes;
}

void P2D_World::setName(const std::string& name){
    this->name = name;
    sizeOfName = sizeof(name);
}
std::string P2D_World::getName(){
    return name;
}
void P2D_World::setPause(bool p){
    pause = p;
}
bool P2D_World::getPause()const{
    return pause;
}
void P2D_World::enableAirResistance(bool b){
    isAirResistance = b;
}
bool P2D_World::getIsAirResistance()const{
    return isAirResistance;
}
void P2D_World::setAirResistanceCoefficient(double k){
    if(k > 0 || k < 0){
        airResistanceCoefficient = k;
        if(k < 0){
            cout << "Warning: setAirResistanceCoefficient (k < 0)!" << endl;
        }
    }else{
        cout << "setAirResistanceCoefficient failed (invalid k)!" << endl;
    }
}
double P2D_World::getAirResistanceCoefficient()const{
    return airResistanceCoefficient;
}
void P2D_World::enablePreciseAirResistance(double b){
    isPreciseAirResistance = b;
}
double P2D_World::getIsPreciseAirResistance()const{
    return isPreciseAirResistance;
}
void P2D_World::enableGravitation(bool b){
    isGravitation = b;
}
bool P2D_World::getIsGravitation()const{
    return isGravitation;
}
void P2D_World::enableElectroStaticForce(bool b){
    if(isElectroDynamicForce && b){
        logWarning("P2D_World::enableElectroStaticForce: you cannot enable the both, electro dynamic force is now disabled");
        isElectroDynamicForce = false;
    }
    isElectroStaticForce = b;
}
bool P2D_World::getIsElectroStaticForce()const{
    return isElectroStaticForce;
}
void P2D_World::enableElectroDynamicForce(bool b){
    if(isElectroStaticForce && b){
        logWarning("P2D_World::enableElectroDynamicForce: you cannot enable the both, electro static force is now disabled");
        isElectroStaticForce = false;
    }
    isElectroDynamicForce = b;
}
bool P2D_World::getIsElectroDynamicForce()const{
    return isElectroDynamicForce;
}

void P2D_World::setCollision(bool set){
    isCollision = set;
}
bool P2D_World::getCollision()const{
    return isCollision;
}
void P2D_World::setCollisionCoefficientAll(double k){
    //TODO: finish
    addObjectCollisionCoefficient = k;
    setWallCollisionCoefficient(k);
    for(vector<P2D_Object*>::iterator i = objects.begin();i != objects.end();++i)
        (*i)->setCollisionCoefficient(k);
    if(0 > k || k > 1)
        logSetCollisionCoefficientOutOfRangeWarning("setCollisionCoefficientAll");
}
void P2D_World::setAccelerationProhibit(bool set){
    isAccelerationProhibit = set;
}
bool P2D_World::getAccelerationProhibit()const{
    return isAccelerationProhibit;
}
bool P2D_World::stickTogether(std::vector<size_t>){
    return 0;//TODO
}
bool P2D_World::splitObject(size_t){
    return 0;//TODO
}
void P2D_World::removeForces(unsigned int){
    //TODO
}
void P2D_World::removeObject(P2D_Object* o){
    if(o == nullptr)
        return;
    unsigned long uniqueID = o->getUniqueID();
    bool isFind = false;
    vector<P2D_Object*>::const_iterator de;
    for(vector<P2D_Object*>::const_iterator i = objects.begin();i != objects.end();++i){
        if((*i)->getUniqueID() == uniqueID){
            isFind = true;
            de = i;
        }
    }
    if(!isFind){
        cout << "removeObject: did not find the object in the world!" << endl;
    }
    else{
        delete o;
        objects.erase(de);
        numberOfObjects--;
    }
}
void P2D_World::removeAllObject(){
    //TODO: Thread safety
    for(vector<P2D_Object*>::const_iterator i = objects.begin();i != objects.end();++i){
        delete *i;
    }
    objects.clear();
    numberOfObjects = 0;
}
void P2D_World::setDefault(){
    isCollision = false;
    isAccelerationProhibit = false;
    pause = true;
    
    perspectiveX = 0;
    perspectiveY = 0;
    perspectiveZoom = 1;
    
    wallCollisionCoefficient = 1;
    upperWall = P2D_DEFAULT_UPPER_WALL;
    lowerWall = P2D_DEFAULT_LOWER_WALL;
    leftWall = P2D_DEFAULT_LEFT_WALL;
    rightWall = P2D_DEFAULT_RIGHT_WALL;
    isUpperWall = true;
    isLowerWall = true;
    isLeftWall = true;
    isRightWall = true;
    
    
    //TODO: finish?
}
void P2D_World::createObject(P2D_Object* object){
    //TODO
    
}
//TODO: more creat object options
P2D_Object* P2D_World::createObject(P2D_AddObjectFlag flag){
    //TODO memory
    P2D_Object* ret = new P2D_Object();
    ret->shape = addObjectShape;
    
    //flag
    if(flag == P2D_ADD_OBJECT_RANDOM_WITHIN_WALL){
        double randomX;
        double randomY;
        unsigned long deltaX = rightWall - leftWall;
        unsigned long deltaY = upperWall - lowerWall;
        randomX = dd::nrand(deltaX) + leftWall;
        randomY = dd::nrand(deltaY) + lowerWall;
        
        ret->x = randomX;
        ret->y = randomY;
    }else if(flag == P2D_ADD_OBJECT_BY_STORED_SETTING){
        ret->gravitationLevel = addObjectGravitationLevel;
        ret->x = addObjectX;
        ret->y = addObjectY;
        ret->angle = addObjectAngle;
        ret->vx = addObjectVX;
        ret->vy = addObjectVY;
        ret->angularVelocity = addObjectAngularVelocity;
        ret->mass = addObjectMass;
        ret->setCollisionCoefficient(addObjectCollisionCoefficient);
        ret->setFrictionCoefficient(addObjectFrictionCoefficient);
        ret->setStaticFrictionCoefficient(addObjectFrictionCoefficient);
        
        if(ret->shape.getShapeType() == P2D_SHAPE_CIRCLE){
            ret->shape.setCircle(addObjectCircleRadius);
        }else if(ret->shape.getShapeType() == P2D_SHAPE_RECT){
            ret->shape.setRect(addObjectRectWidth, addObjectRectHeight);
        }else if(ret->shape.getShapeType() == P2D_SHAPE_LINE){
            ret->shape.setLine(addObjectLineLength);
            ret->momentOfInertia = ret->mass * ret->shape.getWidth() * ret->shape.getWidth() / 12.0;
        }
    }else{
        cout << "WARNING: P2D_World::createObject: Did not find the flag!" << endl;
    }
    
    objects.push_back(ret);
    //objectsX.push_back(ret);
    //objectsY.push_back(ret);
    numberOfObjects++;
    updateGravitationGroups();
    return ret;
}


void P2D_World::createObjects(size_t n){
    createObjects(n, addObjectShape, addObjectFlag);
    numberOfObjects += n;
}

void P2D_World::createObjects(size_t n, const P2D_Shape& shape, P2D_AddObjectFlag flag){
    //TODO: check if shape is valid?
    //TODO: other flags
    if(!shape.getIsValid()){
        cout << "cannot create objects: invalid shape!" << endl;
        return;
    }
    if(flag == P2D_ADD_OBJECT_RANDOM_WITHIN_WALL){
        if(upperWall <= lowerWall){
            cout << "cannot create objects randomly within walls: upperWall <= lowerWall!" << endl;
            return;
        }else if(rightWall <= leftWall){
            cout << "cannot create objects randomly within walls: rightWall <= leftWall!" << endl;
            return;
        }
        
        double randomX;
        double randomY;
        unsigned long deltaX = rightWall - leftWall;
        unsigned long deltaY = upperWall - lowerWall;
        for(size_t i = 0;i != n;++i){
            randomX = dd::nrand(deltaX) + leftWall;
            randomY = dd::nrand(deltaY) + lowerWall;
            P2D_Object *newObject = new P2D_Object();
            newObject->gravitationLevel = addObjectGravitationLevel; //TODO random_within_wall or store_setting
            newObject->shape = shape;
            newObject->x = randomX;
            newObject->y = randomY;
            objects.push_back(newObject);
            //objectsX.push_back(newObject);
            //objectsY.push_back(newObject);
            updateGravitationGroups();
        }
    }else if(flag == P2D_ADD_OBJECT_BY_STORED_SETTING){
        //TODO
    }
    numberOfObjects += n;
}
P2D_Object* P2D_World::getObjectAtPosition(double x, double y){
    //TODO: better performance by divide area?
    for(vector<P2D_Object*>::iterator i = objects.begin();i != objects.end();++i)
        if((*i)->isAtPosition(x, y))
            return (*i);
    return nullptr;
}
P2D_Object* P2D_World::getObjectAtPositionArea(double x, double y, double r){
    //TODO: better performance by divide area?
    for(vector<P2D_Object*>::iterator i = objects.begin();i != objects.end();++i)
        if((*i)->isAtPositionArea(x, y, r))
            return (*i);
    return nullptr;
}

bool P2D_World::isObjectPtrValid(P2D_Object *o)const{
    if(o == nullptr)
        return false;
    bool isValid = false;
    for(vector<P2D_Object*>::const_iterator i = objects.begin();i != objects.end();++i){
        if((*i) == o)
            isValid = true;
    }
    return isValid;
}


void P2D_World::setWall(double w, P2D_WallFlag flag){
    if(flag == P2D_UPPER_WALL)
        upperWall = w;
    else if (flag == P2D_LOWER_WALL)
        lowerWall = w;
    else if (flag == P2D_LEFT_WALL)
        leftWall = w;
    else if(flag == P2D_RIGHT_WALL)
        rightWall = w;
    else if (flag == P2D_ALL_WALL)
        setWall(w, w, w, w);
}
void P2D_World::setWall(double u, double l, double le, double r){
    upperWall = u;
    lowerWall = l;
    leftWall = le;
    rightWall = r;
}
void P2D_World::enableWall(bool e, P2D_WallFlag flag){
    if(flag == P2D_UPPER_WALL)
        isUpperWall = e;
    else if (flag == P2D_LOWER_WALL)
        isLowerWall = e;
    else if (flag == P2D_LEFT_WALL)
        isLeftWall = e;
    else if(flag == P2D_RIGHT_WALL)
        isRightWall = e;
    else if (flag == P2D_ALL_WALL){
        isUpperWall = e;
        isLowerWall = e;
        isLeftWall = e;
        isRightWall = e;
    }
}
void P2D_World::setWallCollisionCoefficient(double k){
    wallCollisionCoefficient = k;
    if(0 > k || k > 1)
        logSetCollisionCoefficientOutOfRangeWarning("wall");
}
//--------------------------------get net stat definition--------------------------
double P2D_World::getNetCharge(){
    return 0;//TODO
}
double P2D_World::getNetKineticEnergy(){
    return 0;//TODO
}
double P2D_World::getNetMomentum(){
    return 0;//TODO
}
double P2D_World::getNetAngularMomentum(){
    return 0;//TODO
}
double P2D_World::getNetPotentialEnergyX(unsigned int zeroEnergyPoint){
    return 0;//TODO
}
double P2D_World::getNetPotentialEnergyY(unsigned int zeroEnergyPoint){
    return 0;//TODO
}
double P2D_World::getNetPotentialEnergyGravitation(){
    return 0;//TODO
}
double P2D_World::getNetPotentialEnergyElectric(){
    return 0;//TODO
}

void P2D_World::write(std::ofstream & of){
    of.write((char*)&isAccelerationProhibit, sizeof(isAccelerationProhibit));
    of.write((char*)&isCollision, sizeof(isCollision));
    of.write((char*)&pause, sizeof(pause));
    of.write((char*)&isAirResistance, sizeof(isAirResistance));
    of.write((char*)&isPreciseAirResistance, sizeof(isPreciseAirResistance));
    of.write((char*)&isGravitation, sizeof(isGravitation));
    of.write((char*)&isElectroStaticForce, sizeof(isElectroStaticForce));
    of.write((char*)&isElectroDynamicForce, sizeof(isElectroDynamicForce));
    of.write((char*)&calculationTimes, sizeof(calculationTimes));
    of.write((char*)&airResistanceCoefficient, sizeof(airResistanceCoefficient));
    of.write((char*)&sizeOfName, sizeof(sizeOfName));
    of.write((char*)&name, sizeof(name));
    of.write((char*)&id, sizeof(id));
    of.write((char*)&id2, sizeof(id2));
    of.write((char*)&id3, sizeof(id3));
    of.write((char*)&addObjectX, sizeof(addObjectX));
    of.write((char*)&addObjectY, sizeof(addObjectY));
    of.write((char*)&addObjectAngle, sizeof(addObjectAngle));
    of.write((char*)&addObjectVX, sizeof(addObjectVX));
    of.write((char*)&addObjectVY, sizeof(addObjectVY));
    of.write((char*)&addObjectAngularVelocity, sizeof(addObjectAngularVelocity));
    of.write((char*)&addObjectMass, sizeof(addObjectMass));
    of.write((char*)&addObjectCircleRadius, sizeof(addObjectCircleRadius));
    of.write((char*)&addObjectRectWidth, sizeof(addObjectRectWidth));
    of.write((char*)&addObjectRectHeight, sizeof(addObjectRectHeight));
    of.write((char*)&addObjectLineLength, sizeof(addObjectLineLength));
    of.write((char*)&addObjectCollisionCoefficient, sizeof(addObjectCollisionCoefficient));
    of.write((char*)&addObjectFrictionCoefficient, sizeof(addObjectFrictionCoefficient));
    of.write((char*)&addObjectStaticFrictionCoefficient, sizeof(addObjectStaticFrictionCoefficient));
    of.write((char*)&addObjectGravitationLevel, sizeof(addObjectGravitationLevel));
    of.write((char*)&addObjectFlag, sizeof(addObjectFlag));
    addObjectShape.write(of);
    of.write((char*)&perspectiveX, sizeof(perspectiveX));
    of.write((char*)&perspectiveY, sizeof(perspectiveY));
    of.write((char*)&perspectiveWidth, sizeof(perspectiveWidth));
    of.write((char*)&perspectiveHeight, sizeof(perspectiveHeight));
    of.write((char*)&perspectiveZoom, sizeof(perspectiveZoom));
    of.write((char*)&accelerationX, sizeof(accelerationX));
    of.write((char*)&accelerationY, sizeof(accelerationY));
    of.write((char*)&forceX, sizeof(forceX));
    of.write((char*)&forceY, sizeof(forceY));
    of.write((char*)&gravitationK, sizeof(gravitationK));
    of.write((char*)&electricForceK, sizeof(electricForceK));
    of.write((char*)&upperWall, sizeof(upperWall));
    of.write((char*)&lowerWall, sizeof(lowerWall));
    of.write((char*)&leftWall, sizeof(leftWall));
    of.write((char*)&rightWall, sizeof(rightWall));
    of.write((char*)&wallCollisionCoefficient, sizeof(wallCollisionCoefficient));
    of.write((char*)&wallFrictionCoefficient, sizeof(wallFrictionCoefficient));
    of.write((char*)&wallStaticFrictionCoefficient, sizeof(wallStaticFrictionCoefficient));
    of.write((char*)&isUpperWall, sizeof(isUpperWall));
    of.write((char*)&isLowerWall, sizeof(isLowerWall));
    of.write((char*)&isLeftWall, sizeof(isLeftWall));
    of.write((char*)&isRightWall, sizeof(isRightWall));
    of.write((char*)&upperWallV, sizeof(upperWallV));
    of.write((char*)&lowerWallV, sizeof(lowerWallV));
    of.write((char*)&leftWallV, sizeof(leftWallV));
    of.write((char*)&rightWallV, sizeof(rightWallV));
    numberOfObjects = objects.size();
    of.write((char*)&numberOfObjects, sizeof(numberOfObjects));
    for(vector<P2D_Object*>::const_iterator i = objects.begin(); i != objects.end(); ++i){
        (*i)->write(of);
    }
}

void P2D_World::read(std::ifstream & inf){
    inf.read((char*)&isAccelerationProhibit, sizeof(isAccelerationProhibit));
    inf.read((char*)&isCollision, sizeof(isCollision));
    inf.read((char*)&pause, sizeof(pause));
    inf.read((char*)&isAirResistance, sizeof(isAirResistance));
    inf.read((char*)&isPreciseAirResistance, sizeof(isPreciseAirResistance));
    inf.read((char*)&isGravitation, sizeof(isGravitation));
    inf.read((char*)&isElectroStaticForce, sizeof(isElectroStaticForce));
    inf.read((char*)&isElectroDynamicForce, sizeof(isElectroDynamicForce));
    inf.read((char*)&calculationTimes, sizeof(calculationTimes));
    inf.read((char*)&airResistanceCoefficient, sizeof(airResistanceCoefficient));
    inf.read((char*)&sizeOfName, sizeof(sizeOfName));
    inf.read((char*)&name, sizeof(name));
    inf.read((char*)&id, sizeof(id));
    inf.read((char*)&id2, sizeof(id2));
    inf.read((char*)&id3, sizeof(id3));
    inf.read((char*)&addObjectX, sizeof(addObjectX));
    inf.read((char*)&addObjectY, sizeof(addObjectY));
    inf.read((char*)&addObjectAngle, sizeof(addObjectAngle));
    inf.read((char*)&addObjectVX, sizeof(addObjectVX));
    inf.read((char*)&addObjectVY, sizeof(addObjectVY));
    inf.read((char*)&addObjectAngularVelocity, sizeof(addObjectAngularVelocity));
    inf.read((char*)&addObjectMass, sizeof(addObjectMass));
    inf.read((char*)&addObjectCircleRadius, sizeof(addObjectCircleRadius));
    inf.read((char*)&addObjectRectWidth, sizeof(addObjectRectWidth));
    inf.read((char*)&addObjectRectHeight, sizeof(addObjectRectHeight));
    inf.read((char*)&addObjectLineLength, sizeof(addObjectLineLength));
    inf.read((char*)&addObjectCollisionCoefficient, sizeof(addObjectCollisionCoefficient));
    inf.read((char*)&addObjectFrictionCoefficient, sizeof(addObjectFrictionCoefficient));
    inf.read((char*)&addObjectStaticFrictionCoefficient, sizeof(addObjectStaticFrictionCoefficient));
    inf.read((char*)&addObjectGravitationLevel, sizeof(addObjectGravitationLevel));
    inf.read((char*)&addObjectFlag, sizeof(addObjectFlag));
    addObjectShape.read(inf);
    inf.read((char*)&perspectiveX, sizeof(perspectiveX));
    inf.read((char*)&perspectiveY, sizeof(perspectiveY));
    inf.read((char*)&perspectiveWidth, sizeof(perspectiveWidth));
    inf.read((char*)&perspectiveHeight, sizeof(perspectiveHeight));
    inf.read((char*)&perspectiveZoom, sizeof(perspectiveZoom));
    inf.read((char*)&accelerationX, sizeof(accelerationX));
    inf.read((char*)&accelerationY, sizeof(accelerationY));
    inf.read((char*)&forceX, sizeof(forceX));
    inf.read((char*)&forceY, sizeof(forceY));
    inf.read((char*)&gravitationK, sizeof(gravitationK));
    inf.read((char*)&electricForceK, sizeof(electricForceK));
    inf.read((char*)&upperWall, sizeof(upperWall));
    inf.read((char*)&lowerWall, sizeof(lowerWall));
    inf.read((char*)&leftWall, sizeof(leftWall));
    inf.read((char*)&rightWall, sizeof(rightWall));
    inf.read((char*)&wallCollisionCoefficient, sizeof(wallCollisionCoefficient));
    inf.read((char*)&wallFrictionCoefficient, sizeof(wallFrictionCoefficient));
    inf.read((char*)&wallStaticFrictionCoefficient, sizeof(wallStaticFrictionCoefficient));
    inf.read((char*)&isUpperWall, sizeof(isUpperWall));
    inf.read((char*)&isLowerWall, sizeof(isLowerWall));
    inf.read((char*)&isLeftWall, sizeof(isLeftWall));
    inf.read((char*)&isRightWall, sizeof(isRightWall));
    inf.read((char*)&upperWallV, sizeof(upperWallV));
    inf.read((char*)&lowerWallV, sizeof(lowerWallV));
    inf.read((char*)&leftWallV, sizeof(leftWallV));
    inf.read((char*)&rightWallV, sizeof(rightWallV));
    inf.read((char*)&numberOfObjects, sizeof(numberOfObjects));
    for(size_t i = 0; i < numberOfObjects; ++i){
        P2D_Object* x = new P2D_Object();
        x->read(inf);
        objects.push_back(x);
        //objectsX.push_back(x);
        //objectsY.push_back(x);
    }
}

P2D_World* P2D_World::clone(){
    P2D_World* ret = P2D_Create_World();
    *ret = *this;
    return ret;
}
void P2D_World::operator=(const P2D_World& w){
    isAccelerationProhibit = w.isAccelerationProhibit;
    isCollision = w.isCollision;
    pause = w.pause;
    isAirResistance = w.isAirResistance;
    isPreciseAirResistance = w.isPreciseAirResistance;
    isGravitation = w.isGravitation;
    isElectroStaticForce = w.isElectroStaticForce;
    isElectroDynamicForce = w.isElectroDynamicForce;
    calculationTimes = w.calculationTimes;
    airResistanceCoefficient = w.airResistanceCoefficient;
    name = w.name;
    id = w.id;
    id2 = w.id2;
    id3 = w.id3;
    addObjectX = w.addObjectX;
    addObjectY = w.addObjectY;
    addObjectAngle = w.addObjectAngle;
    addObjectVX = w.addObjectVX;
    addObjectVY = w.addObjectVY;
    addObjectAngularVelocity = w.addObjectAngularVelocity;
    addObjectMass = w.addObjectMass;
    addObjectCircleRadius = w.addObjectCircleRadius;
    addObjectRectWidth = w.addObjectRectWidth;
    addObjectRectHeight = w.addObjectRectHeight;
    addObjectLineLength = w.addObjectLineLength;
    addObjectCollisionCoefficient = w.addObjectCollisionCoefficient;
    addObjectFrictionCoefficient = w.addObjectFrictionCoefficient;
    addObjectStaticFrictionCoefficient = w.addObjectStaticFrictionCoefficient;
    addObjectFlag = w.addObjectFlag;
    addObjectShape = w.addObjectShape;
    perspectiveX = w.perspectiveX;
    perspectiveY = w.perspectiveY;
    perspectiveWidth = w.perspectiveWidth;
    perspectiveHeight = w.perspectiveHeight;
    perspectiveZoom = w.perspectiveZoom;
    accelerationX = w.accelerationX;
    accelerationY = w.accelerationY;
    forceX = w.forceX;
    forceY = w.forceY;
    gravitationK = w.gravitationK;
    electricForceK = w.electricForceK;
    upperWall = w.upperWall;
    lowerWall = w.lowerWall;
    leftWall = w.leftWall;
    rightWall = w.rightWall;
    wallCollisionCoefficient = w.wallCollisionCoefficient;
    wallFrictionCoefficient = w.wallFrictionCoefficient;
    wallStaticFrictionCoefficient = w.wallStaticFrictionCoefficient;
    isUpperWall = w.isUpperWall;
    isLowerWall = w.isLowerWall;
    isLeftWall = w.isLeftWall;
    isRightWall = w.isRightWall;
    upperWallV = w.upperWallV;
    lowerWallV = w.lowerWallV;
    leftWallV = w.leftWallV;
    rightWallV = w.rightWallV;
    this->removeAllObject();
    for(vector<P2D_Object*>::const_iterator i = w.objects.begin(); i != w.objects.end(); ++i){
        objects.push_back((*i)->clone());
    }
    updateGravitationGroups();
}


void P2D_World::updateAccelerations(){
    typedef vector<P2D_Object*>::iterator it;
    typedef vector<P2D_GravitationGroup>::iterator itg;
    
    //assume that acceleration won't affect other objects' acceleration
    for(vector<P2D_Object*>::iterator i = objects.begin(); i != objects.end(); ++i){
        (*i)->angularAcceleration = 0;
        (*i)->ax = 0;
        (*i)->ay = 0;
        if(!(*i)->isProhibitAcceleration)
            updateSingleObjectAccelerations(*i);
    }
    
    //this part is very time consuming: O(n^2)
    //TODO: higher performance
    if(isGravitation){
        for(itg ig = gravitationGroups.begin(); ig != gravitationGroups.end(); ig++){
            if(!ig->isSelfProhibit){
                for (it i = (*ig).objects.begin(); i != (*ig).objects.end(); i++)
                    for (it j = i + 1; j != (*ig).objects.end(); j++)
                        if((!(*i)->isProhibitAcceleration || !(*j)->isProhibitAcceleration)) //TODO prohibitAcceleration?
                            updateUniversalAccelerations((*i), (*j));
            }
            for(itg ig2 = ig + 1; ig2 != gravitationGroups.end(); ig2++){
                updateLeveledGravitationAccelerations(*ig, *ig2);
            }
        }
    }
    //updateSingleObjectAccelerations(&(**(begin + objectsSize - 1)));
    
    //TODO: grativation, electric force, universal forces, precise airresistance
}
//--------------------------------get net stat definition--------------------------
//--------------------------------P2D_World class definitions--------------------------------

//--------------------------------other definitions--------------------------------
P2D_World* P2D_World::P2D_Create_World(){
    //---------------------TODO: remove? memory?
    P2D_World* ret = new P2D_World();
    ret->addObjectShape.setCircle(P2D_DEFAULT_CIRCLE_RADIUS);
    for(int i = 0;i<100;i++){
        vector<P2D_Object*> v;//TODO partition
        ret->partitioning.push_back(v);
    }
    return ret;
}
//--------------------------------other definitions--------------------------------

void P2D_World::updateSingleObjectAccelerations(P2D_Object *o1){
    //accelerations for single object
    for(vector<individualForce>::const_iterator i = individualForces.begin();i != individualForces.end();++i){
        (*i)(o1);
    }
    o1->ax += o1->extraAx;
    o1->ay += o1->extraAy;
    o1->ax += accelerationX + forceX/o1->mass;
    o1->ay += accelerationY + forceY/o1->mass;
    
    //air resistance
    if(isAirResistance){
        if(!isPreciseAirResistance){
            double airFrictionAX = o1->vx * o1->vx / o1->mass * airResistanceCoefficient * o1->shape.getMaxRadius() * 0.0001;
            if(o1->vx > 0)
                airFrictionAX *= -1;
            double airFrictionAY = o1->vy * o1->vy / o1->mass * airResistanceCoefficient * o1->shape.getMaxRadius() * 0.0001;
            if(o1->vy > 0)
                airFrictionAY *= -1;
            o1->ax += airFrictionAX;
            o1->ay += airFrictionAY;
        }
    }
}
//only called if !isAccelerationProhibit && isGravitation
void P2D_World::updateUniversalAccelerations(P2D_Object *o1, P2D_Object *o2){
    //TODO: array: universalForces
    double a1, a2 , rs, r;
    double deltaX = o2->x - o1->x;
    double deltaY = o2->y - o1->y;
    rs = deltaY * deltaY + deltaX * deltaX;
    
    if(rs == 0)
        rs = SMALL_NUMBER;
    
    r = sqrt(rs);
    
    if(r == 0)
        r = SMALL_NUMBER;
    
    double aa = gravitationK / rs;
    
    a1 = aa * o2->mass;
    a2 = aa * o1->mass;
    
    if(!o1->isProhibitAcceleration){
        o1->ax += a1 * deltaX / r;
        o1->ay += a1 * deltaY / r;
    }
    if(!o2->isProhibitAcceleration){
        o2->ax += a2 * -deltaX / r;
        o2->ay += a2 * -deltaY / r;
    }
    
}

void P2D_World::updateLeveledGravitationAccelerations(P2D_GravitationGroup& g1, P2D_GravitationGroup& g2){
    typedef vector<P2D_Object*>::iterator it;
    if(g1.gravitationLevel > g2.gravitationLevel){
        for(it i1 = g1.objects.begin(); i1 != g1.objects.end(); ++i1){
            for(it i2 = g2.objects.begin(); i2 != g2.objects.end(); ++i2){
                double a2, rs, r;
                double deltaX = (*i1)->x - (*i2)->x;
                double deltaY = (*i1)->y - (*i2)->y;
                rs = deltaY * deltaY + deltaX * deltaX;
                r = sqrt(rs);
                
                checkZeroDenominator(rs);
                double aa = gravitationK / rs;
                
                a2 = aa * (*i2)->mass;
                
                checkZeroDenominator(r);
                (*i2)->ax += a2 * deltaX / r;
                (*i2)->ay += a2 * deltaY / r;
            }
        }
    }else if(g1.gravitationLevel < g2.gravitationLevel){
        for(it i1 = g1.objects.begin(); i1 != g1.objects.end(); ++i1){
            for(it i2 = g2.objects.begin(); i2 != g2.objects.end(); ++i2){
                double a1, rs, r;
                double deltaX = (*i1)->x - (*i2)->x;
                double deltaY = (*i1)->y - (*i2)->y;
                rs = deltaY * deltaY + deltaX * deltaX;
                r = sqrt(rs);
                
                checkZeroDenominator(rs);
                double aa = gravitationK / rs;
                
                a1 = aa * (*i1)->mass;
                
                checkZeroDenominator(r);
                (*i1)->ax += a1 * deltaX / r;
                (*i1)->ay += a1 * deltaY / r;
            }
        }
    }else
        throw logic_error("Cannot update: g1 and g2 have the same gravitation level");
}

void P2D_World::world_Collision(){
    size_t objectsSize = objects.size();
    vector<P2D_Object*>::iterator begin = objects.begin();
    if(isCollision && objectsSize > 1){
       // broadObjectCollisionTest();//TODO partition
        for (int i = 0; i < objectsSize - 1; i++)
            for (int j = i + 1; j < objectsSize; j++)
                collision(*(begin + i), *(begin + j));

    }
    if(isUpperWall || isLowerWall || isLeftWall || isRightWall){
        for (int i = 0; i < objectsSize; i++) {
            objectWallCollision(i);
        }
    }
}

void P2D_World::broadObjectCollisionTest(){//TODO: partitioning
    //init
    typedef vector<P2D_Object*>::iterator it;
    typedef std::vector<std::vector<P2D_Object*> >::iterator ptnIt;
    it begin = objects.begin();
    it end = objects.end();
    
    //clear
    for(ptnIt ptni = partitioning.begin(); ptni!=partitioning.end();++ptni)
        (*ptni).clear();
    
    //assaign
    for(it oi = begin; oi != end; ++oi){
        double maxR = (*oi)->shape.getMaxRadius();
        double lowerLeftX = (*oi)->x - maxR;
        double lowerLeftY = (*oi)->y - maxR;
        double upperRightX = (*oi)->x + maxR;
        double upperRightY = (*oi)->y + maxR;
        
        //check if the object exceeds the main partitioning area bound, if so, adjust the area(add more cells)
        if(lowerLeftX < mainPartitionAreaX){
            size_t plus = (unsigned int)(mainPartitionAreaX - lowerLeftX) / partitionCellW + 1;
            partitionNumOfCellsX += plus;
            mainPartitionAreaX -= partitionCellW * plus;
            for(size_t i=0;i<partitionNumOfCellsY * plus;++i){
                ObjectContainer newCtn;
                partitioning.push_back(newCtn);
            }
        }
        if(lowerLeftY < mainPartitionAreaY){
            size_t plus = (unsigned int)(mainPartitionAreaY - lowerLeftY) / partitionCellH + 1;
            partitionNumOfCellsY += plus;
            mainPartitionAreaY -= partitionCellH * plus;
            for(size_t i=0;i<partitionNumOfCellsX * plus;++i){
                ObjectContainer newCtn;
                partitioning.push_back(newCtn);
            }
        }
        if(upperRightX > mainPartitionAreaX + mainPartitionAreaW){
            size_t plus = (unsigned int)(upperRightX - (mainPartitionAreaX + mainPartitionAreaW)) / partitionCellW + 1;
            partitionNumOfCellsX += plus;
            mainPartitionAreaW += partitionCellW * plus;
            for(size_t i=0;i<partitionNumOfCellsY * plus;++i){
                ObjectContainer newCtn;
                partitioning.push_back(newCtn);
            }
        }
        if(upperRightY > mainPartitionAreaY + mainPartitionAreaH){
            size_t plus = (unsigned int)(upperRightY - (mainPartitionAreaY + mainPartitionAreaH)) / partitionCellH + 1;
            partitionNumOfCellsY += plus;
            mainPartitionAreaH += partitionCellW * plus;
            for(size_t i=0;i<partitionNumOfCellsX * plus;++i){
                ObjectContainer newCtn;
                partitioning.push_back(newCtn);
            }
        }
        
        
        
        int i = (int)((*oi)->x - mainPartitionAreaX) / 100;
        int j = (int)((*oi)->y - mainPartitionAreaY) / 60;
        
        partitioning.at(j * 10 + i).push_back((*oi));
    }
    
    //go
    for(ptnIt i = partitioning.begin(); i!=partitioning.end();++i){
        it beg = (*i).begin();
        size_t sz = (*i).size();
        if(sz > 1){
        for (int i = 0; i < sz - 1; i++)
            for (int j = i + 1; j < sz; j++)
                collision(*(beg + i), *(beg + j));
        }
    }
    
    
    /*  collision test by sort
    typedef vector<P2D_Object*>::iterator it;
    size_t objectsSize = objects.size();
    it begin = objects.begin();
    it end = objects.end();
    it objectsXBeg = objectsX.begin();
    it objectsXEnd = objectsX.end();
    it objectsYBeg = objectsY.begin();
    it objectsYEnd = objectsY.end();
    
    
    //sort objects along x and y axis
    sort(objectsXBeg, objectsXEnd, compareObjectsXLeftBound);
    //sort(objectsYBeg, objectsYEnd, compareObjectsYLowerBound);
    //out_ctn(objectsX);/////
    //out_ctn(objectsY);////
    
    it ix = objectsXBeg;
    it iy = objectsYBeg;
    
    for(; ix != objectsXEnd - 1; ++ix){
        for(it iix = ix + 1; iix != objectsXEnd; ++iix){
            if(((*ix)->x + (*ix)->shape.getMaxRadius()) < ((*iix)->x - (*iix)->shape.getMaxRadius())){
                continue;
            }
            else{
                collision((*ix), (*iix));
            }
        }
        (*ix)->isCollided = true;
    }
    
    for(it i = begin; i != end; ++i)    (*i)->isCollided = false;
    */
}
void P2D_World::objectWallCollision(size_t n){//TODO: performance (line?) TODO: finish
    vector<P2D_Object*>::iterator begin = objects.begin();
    P2D_Object* o = (*(begin + n));
    double x = o->x, y = o->y, vx = o->vx, vy = o->vy;
    P2D_ShapeType shapeType = o->shape.getShapeType();
    double clsk = getCollisionk(wallCollisionCoefficient, o->getCollisionCoefficient());
    double frick = getFrictionk(o->getFrictionCoefficient(), wallFrictionCoefficient);
    double staticfrick = getFrictionk(o->getStaticFrictionCoefficient(), wallStaticFrictionCoefficient);
    
    if(shapeType == P2D_SHAPE_CIRCLE){
        if(isUpperWall && y > upperWall - o->shape.getMaxRadius()){
            o->y = upperWall - o->shape.getMaxRadius();
            if(o->vy > 0) o->vy = -(vy - upperWallV) * clsk;
        }
        if(isLowerWall && y < lowerWall + o->shape.getMaxRadius()){
            o->y = lowerWall + o->shape.getMaxRadius();
            if(o->vy < 0) o->vy = -(vy - lowerWallV) * clsk;
        }
        if(isLeftWall && x < leftWall + o->shape.getMaxRadius()){
            o->x = leftWall + o->shape.getMaxRadius();
            if(o->vx < 0) o->vx = -(vx - leftWallV) * clsk;
        }
        if(isRightWall && x > rightWall - o->shape.getMaxRadius()){
            o->x = rightWall - o->shape.getMaxRadius();
            if(o->vx > 0) o->vx = -(vx - rightWallV) * clsk;
        }
    }
    else if(shapeType == P2D_SHAPE_RECT){
        //TODO
    }
    else if(shapeType == P2D_SHAPE_LINE){
        if(isUpperWall && y > upperWall - o->shape.getMaxRadius()){
            //second test
            double sinTheta = sin(o->angle);
            double halfHeight = abs(sinTheta * o->shape.getMaxRadius());
            if(y + halfHeight > upperWall){
                //collision happens
                double px, py;
                if(o->angle < dd::PI && o->angle > 0)
                    px = cos(o->angle) * o->shape.getMaxRadius() + x;
                else
                    px = -cos(o->angle) * o->shape.getMaxRadius() + x;
                py = y + halfHeight;
                
                double v[3] = {vx, vy - upperWallV, o->angularVelocity};
                
                collision2dWall(P2D_UPPER_WALL, o->mass, o->momentOfInertia, x, y, px, py, v, clsk, frick, staticfrick);
                o->vx = v[0];
                o->vy = v[1];
                o->angularVelocity = v[2];
                
                //offset position TODO: plus angle offset(more accurate?)
                o->y = -halfHeight + upperWall;
            }
        }
        if(isLowerWall && y < lowerWall + o->shape.getMaxRadius()){
            //second test
            double sinTheta = sin(o->angle);
            double halfHeight = abs(sinTheta * o->shape.getMaxRadius());
            if(y - halfHeight < lowerWall){
                //collision happens
                double px, py;
                if(o->angle < dd::PI && o->angle > 0)
                    px = -cos(o->angle) * o->shape.getMaxRadius() + x;
                else
                    px = cos(o->angle) * o->shape.getMaxRadius() + x;
            
                py = y - halfHeight;
                
                double v[3] = {vx , vy - lowerWallV, o->angularVelocity};
                
                collision2dWall(P2D_LOWER_WALL, o->mass, o->momentOfInertia, x, y, px, py, v, clsk, frick, staticfrick);
                o->vx = v[0];
                o->vy = v[1];
                o->angularVelocity = v[2];
                
                //offset position TODO: plus angle offset(more accurate?)
                o->y = halfHeight + lowerWall;
            }
        }
        if(isLeftWall && x < leftWall + o->shape.getMaxRadius()){
            //second test
            double cosTheta = cos(o->angle);
            double halfWidth = abs(cosTheta * o->shape.getMaxRadius());
            if(x - halfWidth < leftWall){
                //collision happens
                double px, py;
                if(o->angle < THREE_PI_OVER_TWO && o->angle > PI_OVER_TWO)
                    py = sin(o->angle) * o->shape.getMaxRadius() + y;
                else
                    py = -sin(o->angle) * o->shape.getMaxRadius() + y;
                
                px = x - halfWidth;
                
                double v[3] = {vx - leftWallV, vy, o->angularVelocity};
                
                collision2dWall(P2D_LEFT_WALL, o->mass, o->momentOfInertia, x, y, px, py, v, clsk, frick, staticfrick);
                o->vx = v[0];
                o->vy = v[1];
                o->angularVelocity = v[2];
                
                //offset position TODO: plus angle offset(more accurate?)
                o->x = halfWidth + leftWall;
            }
        }
        if(isRightWall && x > rightWall - o->shape.getMaxRadius()){
            //second test
            double cosTheta = cos(o->angle);
            double halfWidth = abs(cosTheta * o->shape.getMaxRadius());
            if(x + halfWidth > rightWall){
                //collision happens
                double px, py;
                if(o->angle < THREE_PI_OVER_TWO && o->angle > PI_OVER_TWO)
                    py = -sin(o->angle) * o->shape.getMaxRadius() + y;
                else
                    py = sin(o->angle) * o->shape.getMaxRadius() + y;
                
                px = x + halfWidth;
                
                double v[3] = {vx - rightWallV, vy, o->angularVelocity};
                
                collision2dWall(P2D_RIGHT_WALL, o->mass, o->momentOfInertia, x, y, px, py, v, clsk, frick, staticfrick);
                o->vx = v[0];
                o->vy = v[1];
                o->angularVelocity = v[2];
                
                //offset position TODO: plus angle offset(more accurate?)
                o->x = -halfWidth + rightWall;
            }
        }
    }
    else if(shapeType == P2D_SHAPE_MIXED){
        //TODO
    }
    else{
        cout << "Warning: P2D_World::objectWallCollision: object's shape does not belong to any P2D_ShapeType!" << endl;
    }
    
}


bool P2D_World::compareObjectsXLeftBound(P2D_Object* o1, P2D_Object* o2){
    return (o1->x - o1->shape.getMaxRadius()) < (o2->x - o2->shape.getMaxRadius());
}
bool P2D_World::compareObjectsYLowerBound(P2D_Object* o1, P2D_Object* o2){
    return (o1->y - o1->shape.getMaxRadius()) < (o2->y - o2->shape.getMaxRadius());
}

void P2D_World::updateGravitationGroups(){
    typedef vector<P2D_Object*>::iterator it;
    typedef vector<P2D_GravitationGroup>::iterator itg;
    for(itg i = gravitationGroups.begin(); i != gravitationGroups.end(); ++i){
        i->objects.clear();
    }
    for(it i = objects.begin(); i != objects.end(); ++i){
        bool found = false;
        P2D_GravitationGroup* group = nullptr;
        for(itg ig = gravitationGroups.begin(); ig != gravitationGroups.end(); ig++){
            if(ig->gravitationLevel == (*i)->gravitationLevel){
                found = true;
                group = &(*ig);
            }
        }
        if(found){
            group->objects.push_back(*i);
        }
        else{
            P2D_GravitationGroup newGroup;
            newGroup.gravitationLevel = (*i)->gravitationLevel;
            newGroup.objects.push_back(*i);
            gravitationGroups.push_back(newGroup);
        }
    }/*
    itg endg = gravitationGroups.end();
    for(itg i = gravitationGroups.begin(); i != endg; ++i){
        if(i->objects.size() == 0){
            gravitationGroups.erase(i);
            endg = gravitationGroups.end();
            i = gravitationGroups.begin();
        }
    }*/
    sort(gravitationGroups.begin(), gravitationGroups.end(), P2D_GravitationGroup::comparaGravitationLevel);
}
