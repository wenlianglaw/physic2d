//
//  Object.cpp
//  physics2d
//
//  Created by Jiuhua on 6/1/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//
#include "P2D_Util.hpp"
#include "P2D_Math.hpp"
#include "P2D_Object.hpp"
using namespace std;
using namespace dd;

#define TWO_PI 2 * 3.1415926535

unsigned long P2D_Object::numberOfObjectsCreated = 0;

unsigned long P2D_Object::getUniqueID(){
    return uniqueID;
}
P2D_Object::P2D_Object(const P2D_Object& o):uniqueID(numberOfObjectsCreated+1){
    *this = o;
}
void P2D_Object::move(unsigned int calculationTimes){
    angle += angularVelocity / calculationTimes;
    if(angle > TWO_PI){
        do{
            angle -= TWO_PI;
        }while(angle > TWO_PI);
    }
    else if(angle < 0){
        do{
            angle += TWO_PI;
        }while(angle < 0);
    }
    x += vx / calculationTimes;
    y += vy / calculationTimes;
    
}
void P2D_Object::updateVelocity(unsigned int calculationTimes){
    angularVelocity += angularAcceleration / calculationTimes;
    vx += ax / calculationTimes;
    vy += ay / calculationTimes;
}
bool P2D_Object::isAtPosition(double x, double y){
    if(shape.getShapeType() == P2D_SHAPE_CIRCLE){
        double deltaXAbs = abs(x - this->x);
        double deltaYAbs = abs(y - this->y);
        //first step exclusion check
        if(deltaXAbs > shape.getRadius() || deltaYAbs > shape.getRadius())
            return false;
        else{
            double distance = sqrt(deltaXAbs * deltaXAbs + deltaYAbs * deltaYAbs);
            if(distance > shape.getRadius())
                return false;
            else
                return true;
        }
    }
    //TODO: other shapes (delete return false; below)
    return false;
}
bool P2D_Object::isAtPositionArea(double x, double y, double r){
    if(shape.getShapeType() == P2D_SHAPE_LINE){
        
        double k1 = tan(angle);
        double k2;
        if(k1 == 0)
            k2 = INFINITY;
        else
            k2 = -1.0/k1;
        double xx = (y - this->y - k2 * x + k1 * this->x) / (k1 - k2);
        double yy = k2 * (xx - x) + y;
        if(abs(xx - this->x) > abs(cos(angle) * shape.getMaxRadius()))
            return false;
        double dxx = xx - x;
        double dyy = yy - y;
        double distanceS = dxx * dxx + dyy * dyy;
        if(distanceS < r * r){
            return true;
        }
        return false;
    }
    //TODO: other shapes (delete return false; below)
    return false;
}

double P2D_Object::getCollisionCoefficient(){
    return collisionCoefficient;
}
void P2D_Object::setCollisionCoefficient(double k){
    collisionCoefficient = k;
    if(0 > k || k > 1)
        logSetCollisionCoefficientOutOfRangeWarning("P2D_Object(id = " + to_string(id) + ")");
}
double P2D_Object::getFrictionCoefficient(){
    return frictionCoefficient;
}
bool P2D_Object::setFrictionCoefficient(double k){
    if(k >= 0){
        frictionCoefficient = k;
        return true;
    }
    logWarning("P2D_Object::setFrictionCoefficient: k < 0 or invalid");
    return false;
    
}
double P2D_Object::getStaticFrictionCoefficient(){
    return staticFrictionCoefficient;
}
bool P2D_Object::setStaticFrictionCoefficient(double k){
    if(k >= 0){
        staticFrictionCoefficient = k;
        return true;
    }
    logWarning("P2D_Object::setStaticFrictionCoefficient: k < 0 or invalid");
    return false;
}
void P2D_Object::write(std::ofstream& of){
    of.write((char*)&collisionCoefficient , sizeof(collisionCoefficient));
    of.write((char*)&frictionCoefficient , sizeof(frictionCoefficient));
    of.write((char*)&netKineticEnergy , sizeof(netKineticEnergy));
    of.write((char*)&momentumX , sizeof(momentumX));
    of.write((char*)&momentumY , sizeof(momentumY));
    of.write((char*)&angularMomentum , sizeof(angularMomentum));
    of.write((char*)&id, sizeof(id));
    of.write((char*)&id2, sizeof(id2));
    of.write((char*)&id3, sizeof(id3));
    of.write((char*)&gravitationLevel, sizeof(gravitationLevel));
    of.write((char*)&momentOfInertia, sizeof(momentOfInertia));
    of.write((char*)&angularAcceleration, sizeof(angularAcceleration));
    of.write((char*)&angularVelocity, sizeof(angularVelocity));
    of.write((char*)&angle, sizeof(angle));
    of.write((char*)&x, sizeof(x));
    of.write((char*)&y, sizeof(y));
    of.write((char*)&vx, sizeof(vx));
    of.write((char*)&vy, sizeof(vy));
    of.write((char*)&ax, sizeof(ax));
    of.write((char*)&ay, sizeof(ay));
    of.write((char*)&extraAx, sizeof( extraAx));
    of.write((char*)&extraAy, sizeof(extraAy));
    of.write((char*)&mass, sizeof(mass));
    of.write((char*)&charge, sizeof(charge));
    shape.write(of);
    of.write((char*)&isStationary, sizeof( isStationary));
    of.write((char*)&isProhibitAcceleration, sizeof( isProhibitAcceleration));
    of.write((char*)&isChangeAngularMomentum, sizeof( isChangeAngularMomentum));
    of.write((char*)&isChangeMomentum, sizeof( isChangeMomentum));
    of.write((char*)&isChangeNetKineticEnergy , sizeof( isChangeNetKineticEnergy));
    
    of.write((char*)&numberOfLinkedObject , sizeof( numberOfLinkedObject));
    for(size_t i = 0; i < numberOfLinkedObject; ++i){
        //TODO: important! affect data safety!
    }
}
void P2D_Object::read(std::ifstream& of){
    of.read((char*)&collisionCoefficient , sizeof(collisionCoefficient));
    of.read((char*)&frictionCoefficient , sizeof(frictionCoefficient));
    of.read((char*)&netKineticEnergy , sizeof(netKineticEnergy));
    of.read((char*)&momentumX , sizeof(momentumX));
    of.read((char*)&momentumY , sizeof(momentumY));
    of.read((char*)&angularMomentum , sizeof(angularMomentum));
    of.read((char*)&id, sizeof(id));
    of.read((char*)&id2, sizeof(id2));
    of.read((char*)&id3, sizeof(id3));
    of.read((char*)&gravitationLevel, sizeof(gravitationLevel));
    of.read((char*)&momentOfInertia, sizeof(momentOfInertia));
    of.read((char*)&angularAcceleration, sizeof(angularAcceleration));
    of.read((char*)&angularVelocity, sizeof(angularVelocity));
    of.read((char*)&angle, sizeof(angle));
    of.read((char*)&x, sizeof(x));
    of.read((char*)&y, sizeof(y));
    of.read((char*)&vx, sizeof(vx));
    of.read((char*)&vy, sizeof(vy));
    of.read((char*)&ax, sizeof(ax));
    of.read((char*)&ay, sizeof(ay));
    of.read((char*)&extraAx, sizeof( extraAx));
    of.read((char*)&extraAy, sizeof(extraAy));
    of.read((char*)&mass, sizeof(mass));
    of.read((char*)&charge, sizeof(charge));
    shape.read(of);
    of.read((char*)&isStationary, sizeof( isStationary));
    of.read((char*)&isProhibitAcceleration, sizeof( isProhibitAcceleration));
    of.read((char*)&isChangeAngularMomentum, sizeof( isChangeAngularMomentum));
    of.read((char*)&isChangeMomentum, sizeof( isChangeMomentum));
    of.read((char*)&isChangeNetKineticEnergy , sizeof( isChangeNetKineticEnergy));
    
    of.read((char*)&numberOfLinkedObject , sizeof( numberOfLinkedObject));
    for(size_t i = 0; i < numberOfLinkedObject; ++i){
        //TODO: important! affect data safety!
    }
}
P2D_Object* P2D_Object::clone(){
    P2D_Object* ret = new P2D_Object;
    *ret = *this;
    return ret;
}
P2D_Object& P2D_Object::operator=(const P2D_Object& o){
    collisionCoefficient = o.collisionCoefficient;
    frictionCoefficient = o.frictionCoefficient;
    netKineticEnergy = o.netKineticEnergy;
    momentumX = o.momentumX;
    momentumY = o.momentumY;
    angularMomentum = o.angularMomentum;
    id = o.id;
    id2 = o.id2;
    id3 = o.id3;
    gravitationLevel = o.gravitationLevel;
    momentOfInertia = o.momentOfInertia;
    angularAcceleration = o.angularAcceleration;
    angularVelocity = o.angularVelocity;
    angle = o.angle;
    x = o.x;
    y = o.y;
    vx = o.vx;
    vy = o.vy;
    ax = o.ax;
    ay = o.ay;
    extraAx = o. extraAx;
    extraAy = o.extraAy;
    mass = o.mass;
    charge = o.charge;
    shape = o. shape;
    isStationary = o. isStationary;
    isProhibitAcceleration = o. isProhibitAcceleration;
    isChangeAngularMomentum = o. isChangeAngularMomentum;
    isChangeMomentum = o. isChangeMomentum;
    isChangeNetKineticEnergy = o. isChangeNetKineticEnergy;
    delete nextLinkedObject;
    if(o.nextLinkedObject == nullptr)
        nextLinkedObject = nullptr;
    else{
        nextLinkedObject = new P2D_Object;
        try{
            *nextLinkedObject = *o.nextLinkedObject;
        }catch(...){
            cerr << "Error: P2D_Object assaignment operator: argument object contains invalid nextLinkedObject ptr!" << endl;
            delete nextLinkedObject;
            nextLinkedObject = nullptr;
        }
    }
    return *this;
}
