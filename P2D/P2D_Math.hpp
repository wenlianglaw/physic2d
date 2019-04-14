//
//  Mathp2d.hpp
//  physics2d
//
//  Created by Jiuhua on 6/1/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

/*
 collision2d(d, d, d*, d) : Linear collision formula :
 V1f = (k(V2 - V1) m2 + m1v1 + m2v2)/(m1 + m2)
 V2f = (k(V1 - V2) m1 + m1v1 + m2v2)/(m1 + m2)
 
 collision2d()
 */

#ifndef Mathp2d_hpp
#define Mathp2d_hpp

#include <stdio.h>
#include "P2D_Object.hpp"
#include "P2D_Shape.hpp"
#include "P2D_Flags.hpp"

#define SMALL_NUMBER 1E-30
//@return true means two objects did collide, false means two objects didn't collide.
bool collision(P2D_Object*, P2D_Object*);
void collisionCC(P2D_Object* , P2D_Object*);
void collision2d(double, double, double*, double);

void collision2d(double, double, double, double, double, double, double, double, double, double, double, double*, double, double);
void collision2dWall(P2D_WallFlag, double, double, double, double, double, double, double*, double, double, double);

double getCollisionk(double, double);
double getFrictionk(double, double);

void checkZeroDenominator(double&);

#endif /* Mathp2d_hpp */
