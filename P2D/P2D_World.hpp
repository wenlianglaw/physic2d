//
//  World.hpp
//  physics2d
//
//  Created by Jiuhua on 6/5/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include "P2D_Object.hpp"
#include "P2D_Shape.hpp"
#include "P2D_Math.hpp"
#include "P2D_Util.hpp"
#include "P2D_Flags.hpp"
#include "P2D_GravitationGroup.hpp"

#define P2D_DEFAULT_UPPER_WALL 600
#define P2D_DEFAULT_LOWER_WALL 0
#define P2D_DEFAULT_LEFT_WALL 0
#define P2D_DEFAULT_RIGHT_WALL 1000

#define P2D_DEFAULT_CIRCLE_RADIUS 20
#define P2D_DEFAULT_RECT_WIDTH 50
#define P2D_DEFAULT_RECT_HEIGHT 50
#define P2D_DEFAULT_LINE_LENGTH 100
#define P2D_DEFAULT_CALCULATION_TIMES 10

#define P2D_DEFAULT_FRICTION_COEFFICIENT 0.15
#define P2D_DEFAULT_STATIC_FRICTION_COEFFICIENT 1
#define P2D_DEFAULT_WALL_FRICTION_COEFFICIENT P2D_DEFAULT_FRICTION_COEFFICIENT
#define P2D_DEFAULT_WALL_STATIC_FRICTION_COEFFICIENT P2D_DEFAULT_STATIC_FRICTION_COEFFICIENT

#define P2D_DEFAULT_WALL_COLOR_R 255
#define P2D_DEFAULT_WALL_COLOR_G 255
#define P2D_DEFAULT_WALL_COLOR_B 255
#define P2D_DEFAULT_WALL_COLOR_A 255
/*
 //---------------------------------TODO
 object state: unaffected by everything (for until clicking)
 object id when generate and stuff
 world id
 continuous create objects by mouse
 collision and other computation divided by zero bug
 encapsulation
 two threads, one command one world
 precise air resistance
 config file and
 performace: store data that will be calculated for manytimes to save calculations(gravitation, collision)?
 replays needs lesser memory (int)
 objects color in different gravitation group
 gravitation group performance
 partitioning collision test finishing
 
 add to P2D_SavedWorld:
 wall color
 */

typedef void (*individualForce)(P2D_Object*);
typedef void (*universalForce)(P2D_Object*, P2D_Object*);
//------------------------------class P2D_World----------------------------
class P2D_World{
public:
    typedef std::vector<P2D_Object*> ObjectContainer;
    
    //reserved variables
    long id, id2, id3;
    
    //-------------------------------------add object settings-------------------------------------
    double addObjectX = 0, addObjectY = 0, addObjectAngle = 0, addObjectVX = 0, addObjectVY = 0, addObjectAngularVelocity = 0, addObjectMass = 1, addObjectCircleRadius = P2D_DEFAULT_CIRCLE_RADIUS, addObjectRectWidth = P2D_DEFAULT_RECT_WIDTH, addObjectRectHeight = P2D_DEFAULT_RECT_HEIGHT, addObjectLineLength = P2D_DEFAULT_LINE_LENGTH, addObjectCollisionCoefficient = 1, addObjectFrictionCoefficient = P2D_DEFAULT_FRICTION_COEFFICIENT, addObjectStaticFrictionCoefficient = P2D_DEFAULT_STATIC_FRICTION_COEFFICIENT;
    unsigned int addObjectGravitationLevel = 0;
    P2D_AddObjectFlag addObjectFlag = P2D_ADD_OBJECT_RANDOM_WITHIN_WALL;
    P2D_Shape addObjectShape;
    
    int perspectiveX = 0, perspectiveY = 0, perspectiveWidth, perspectiveHeight;
    double perspectiveZoom = 1;
    //-------------------------------------add object settings-------------------------------------

    
    //------------------------------Forces, Accelerations, Coefficients, Settings-------------------------
    double accelerationX = 0, accelerationY = 0, forceX = 0, forceY = 0, gravitationK = 5000, electricForceK = 5000;
    std::vector<universalForce> universalForces;
    std::vector<individualForce> individualForces;
    //------------------------------Forces, Accelerations, Coefficients, Settings-------------------------
    //------------------------------Object and Walls---------------------------------------------------------
    double upperWall = P2D_DEFAULT_UPPER_WALL, lowerWall = P2D_DEFAULT_LOWER_WALL, leftWall = P2D_DEFAULT_LEFT_WALL, rightWall = P2D_DEFAULT_RIGHT_WALL;
    double wallCollisionCoefficient = 1, wallFrictionCoefficient = P2D_DEFAULT_WALL_FRICTION_COEFFICIENT, wallStaticFrictionCoefficient = P2D_DEFAULT_WALL_STATIC_FRICTION_COEFFICIENT;
    bool isUpperWall = true, isLowerWall = true, isLeftWall = true, isRightWall = true;
    double upperWallV = 0, lowerWallV = 0, leftWallV = 0, rightWallV = 0;
    //P2D_Color wallColor;
    ObjectContainer objects;
    //------------------------------Object and Walls---------------------------------------------------------
    
    void go();//calculate the next frame
    
    void setCalculationTimes(unsigned int);
    unsigned int getCalculationTimes()const;
    
    void setName(const std::string&);
    std::string getName();
    void setPause(bool);
    bool getPause()const;
    void enableAirResistance(bool);
    bool getIsAirResistance()const;
    void setAirResistanceCoefficient(double);
    double getAirResistanceCoefficient()const;
    void enablePreciseAirResistance(double);
    double getIsPreciseAirResistance()const;
    void enableGravitation(bool);
    bool getIsGravitation()const;
    void enableElectroStaticForce(bool);
    bool getIsElectroStaticForce()const;
    void enableElectroDynamicForce(bool);
    bool getIsElectroDynamicForce()const;
    
    void setCollision(bool);
    bool getCollision()const;
    void setCollisionCoefficientAll(double);
    void setAccelerationProhibit(bool);
    bool getAccelerationProhibit()const;
    bool stickTogether(std::vector<size_t>);
    bool splitObject(size_t);
    void removeForces(unsigned int);
    void removeObject(P2D_Object*);
    void removeAllObject();
    bool loadConfig(std::string);
    void setDefault();
    void createObject(P2D_Object*);
    P2D_Object* createObject(P2D_AddObjectFlag);
    void createObjects(size_t);
    void createObjects(size_t, const P2D_Shape&, P2D_AddObjectFlag);
    P2D_Object* getObjectAtPosition(double, double);
    P2D_Object* getObjectAtPositionArea(double, double, double);
    
    bool isObjectPtrValid(P2D_Object*)const;
    
    
    void setWall(double, P2D_WallFlag);
    void setWall(double, double, double, double);
    void enableWall(bool, P2D_WallFlag);
    void setWallCollisionCoefficient(double);
    
    //TODO: flag
    double getNetCharge();
    double getNetKineticEnergy();
    double getNetMomentum();
    double getNetAngularMomentum();
    double getNetPotentialEnergyX(unsigned int);
    double getNetPotentialEnergyY(unsigned int);
    double getNetPotentialEnergyGravitation();
    double getNetPotentialEnergyElectric();
    
    void write(std::ofstream&);
    void read(std::ifstream&);

    P2D_World* clone();
    void operator=(const P2D_World&);
    
    static P2D_World* P2D_Create_World();
private:
    bool isAccelerationProhibit, isCollision = false, pause = true, isAirResistance = false, isPreciseAirResistance = false, isGravitation = false, isElectroStaticForce = false, isElectroDynamicForce = false;
    unsigned int calculationTimes = P2D_DEFAULT_CALCULATION_TIMES;
    
    double airResistanceCoefficient = 1;
    
    std::string name;
    size_t sizeOfName = 0;
    size_t numberOfObjects = 0;
    
    //objectsX and objectsY are for sorting for collision test along x and y axis
    //std::vector<P2D_Object*> objectsX;//TODO memory
    //std::vector<P2D_Object*> objectsY;//TODO memory
    double mainPartitionAreaX = 0, mainPartitionAreaY = 0, mainPartitionAreaW = 0, mainPartitionAreaH = 0;
    unsigned int partitionNumOfCellsX = 0, partitionNumOfCellsY = 0, partitionCellW = 100, partitionCellH = 100;
    std::vector<ObjectContainer> partitioning;//TODO finish and memory
    std::vector<P2D_GravitationGroup> gravitationGroups;
    
    void updateAccelerations();
    void updateSingleObjectAccelerations(P2D_Object*);
    void updateUniversalAccelerations(P2D_Object*, P2D_Object*);
    void updateLeveledGravitationAccelerations(P2D_GravitationGroup&, P2D_GravitationGroup&);
    void world_Collision();
    void broadObjectCollisionTest();
    void objectWallCollision(size_t);
    
    //for sorting objects along x and y axis for collision test
    static bool compareObjectsXLeftBound(P2D_Object*, P2D_Object*);
    static bool compareObjectsYLowerBound(P2D_Object*, P2D_Object*);
    
    void updateGravitationGroups();
};
//------------------------------class P2D_World----------------------------



#endif /* World_hpp */
