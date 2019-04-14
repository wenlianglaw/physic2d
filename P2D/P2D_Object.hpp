//
//  Object.hpp
//  physics2d
//
//  Created by Jiuhua on 6/1/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <vector>
#include <math.h>
#include <fstream>
#include "P2D_Shape.hpp"
#include "utildd.hpp"
#include "P2D_Flags.hpp"

/*
 TODO
 error: mass is 0
 
*/

//-------------------------------------------------P2D_Object----------------------------------------

class P2D_Object{
public:
    //-------------------------------------public instance variables----------------------------------
    long id, id2, id3;
    unsigned int gravitationLevel = 0;
    double momentOfInertia, angularAcceleration = 0, angularVelocity = 0, angle = 0, x = 0, y = 0, vx = 0, vy = 0, ax = 0, ay = 0, extraAx = 0, extraAy = 0, mass = 1, charge = 0;
    P2D_Shape shape;
    
    bool isStationary = false, isProhibitAcceleration = false;
    bool isChangeAngularMomentum, isChangeMomentum, isChangeNetKineticEnergy;
    
    P2D_Object *nextLinkedObject = nullptr;
    
    //for calculation of collision
    bool isCollided = false;
    
    
    //------------------------------------------interfaces----------------------------------------
    
    P2D_Object():uniqueID(numberOfObjectsCreated+1){numberOfObjectsCreated++;};
    P2D_Object(const P2D_Object&);
    
    void move(unsigned int);
    void updateVelocity(unsigned int);
    bool isAtPosition(double, double);
    bool isAtPositionArea(double, double, double);
    
    //-------------------------------------getter and setters----------------------------------
    unsigned long getUniqueID();
    
    double getMomentumX();//TODO: define
    double getMomentumY();
    double getAngularMomentum();
    double getNetKineticEnergy();
    bool setMomentumX(double);
    bool setMomentumY(double);
    bool setAngularMomentum(double);
    bool setNetKineticEnergy(double);
    
    double getCollisionCoefficient();
    void setCollisionCoefficient(double);  //(1 is perfect elastic collision), the function makes sure 0 <= k <= 1
    //TODO: define
    double getFrictionCoefficient();
    bool setFrictionCoefficient(double);
    double getStaticFrictionCoefficient();
    bool setStaticFrictionCoefficient(double);
    
    void write(std::ofstream&);
    void read(std::ifstream&);
    P2D_Object* clone();
    
    P2D_Object& operator=(const P2D_Object&);
    ~P2D_Object(){delete nextLinkedObject;}
    
private:
    size_t numberOfLinkedObject = 0;
    
    static unsigned long numberOfObjectsCreated;
    const unsigned long uniqueID;
    
    double collisionCoefficient = 1, frictionCoefficient = 0, staticFrictionCoefficient = 0;
    double netKineticEnergy = 0, momentumX = 0, momentumY = 0, angularMomentum = 0;
};

//-----------------------------------------------------P2D_Object------------------------------------

#endif /* Object_hpp */
