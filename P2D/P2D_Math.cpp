//
//  Mathp2d.cpp
//  physics2d
//
//  Created by Jiuhua on 6/1/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "P2D_Math.hpp"
#include "P2D_Shape.hpp"
#include "utildd.hpp"
#include "P2D_Object.hpp"
#include <math.h>
#include <iostream>
using namespace std;
using namespace dd;

#define PI 3.1415926535
#define PI_OVER_TWO PI / 2.0
#define THREE_PI_OVER_TWO 3.0 * PI / 2

/*
int main(){
}
*/
bool collision(P2D_Object* o1, P2D_Object* o2){
    P2D_ShapeType shapeType1 = o1->shape.getShapeType();
    P2D_ShapeType shapeType2 = o2->shape.getShapeType();
    //first phase of collision test (exclusion test, by maxRadius and minRadius), and some useful varaibles' definition
    /*double deltaX = o2->x - o1->x;
    double deltaY = o2->y - o1->y;
    
    double rsum = o1->shape -> getMaxRadius() + o2->shape -> getMaxRadius();
    double deltaXSquare = deltaX * deltaX;
    double deltaYSquare = deltaY * deltaY;
    double distanceSquare = deltaXSquare + deltaYSquare;
    double rsumSquare = rsum * rsum;
    if(distanceSquare >= rsumSquare)
        return false;*/
    
    if(shapeType1 == P2D_SHAPE_CIRCLE && shapeType2 == P2D_SHAPE_CIRCLE){
        //---------------------------circle-circle collision---------------------------
        collisionCC(o1, o2);  //exact test and collision
    }else if(shapeType1 == P2D_SHAPE_CIRCLE && shapeType2 == P2D_SHAPE_RECT){
        //---------------------------circle-rectangle collision
    }else if(shapeType1 == P2D_SHAPE_LINE && shapeType2 == P2D_SHAPE_LINE){
        //---------------------------line-line collision---------------------------
    }
    return true;
}


void collisionCC(P2D_Object* o1, P2D_Object* o2){
    //first phase of collision test (exclusion test, by maxRadius and minRadius), and some useful varaibles' definition
    double deltaX = o2->x - o1->x;
    double deltaY = o2->y - o1->y;
    
    double rsum = o1->shape.getMaxRadius() + o2->shape.getMaxRadius();
    double deltaXSquare = deltaX * deltaX;
    double deltaYSquare = deltaY * deltaY;
    double distanceSquare = deltaXSquare + deltaYSquare;
    double rsumSquare = rsum * rsum;
    if(distanceSquare >= rsumSquare)
        return;
    //some useful variables declaration or definition
    double vpara1, vverti1, vpara2, vverti2;
    double distance, sinTheta, cosTheta;
    double collisionk;
    
    //---------------------------circle-circle collision---------------------------
    //collision happens
    collisionk = getCollisionk(o1->getCollisionCoefficient(), o2->getCollisionCoefficient());
    distance = sqrt(distanceSquare);
    
    checkZeroDenominator(distance);
    sinTheta = deltaY/distance;
    cosTheta = deltaX/distance;
    
    //get paralell and vertical velocity from vx and vy
    vpara1 = sinTheta * o1->vx - cosTheta * o1->vy;
    vverti1 = cosTheta * o1->vx + sinTheta * o1->vy;
    vpara2 = sinTheta * o2->vx - cosTheta * o2->vy;
    vverti2 = cosTheta * o2->vx + sinTheta * o2->vy;
    
    //collision verti
    double vverti[2];
    vverti[0] = vverti1;
    vverti[1] = vverti2;
    collision2d(o1->mass, o2->mass, vverti, collisionk);
    vverti1 = vverti[0];
    vverti2 = vverti[1];
    
    //set vx, vy by para and verti
    o1->vx = vpara1 * sinTheta + vverti1 * cosTheta;
    o1->vy = -vpara1 * cosTheta + vverti1 * sinTheta;
    o2->vx = vpara2 * sinTheta + vverti2 * cosTheta;
    o2->vy = -vpara2 * cosTheta + vverti2 * sinTheta;
    
    //offset positions
    double aa = (2*distance/rsum);
    
    double a1 = deltaX/aa;
    double a2 = deltaY/aa;
    double deltaXOver2 = deltaX/2, deltaYOver2 = deltaY/2;
    
    o1->x += -a1+deltaXOver2;
    o1->y += -a2+deltaYOver2;
    o2->x += a1-deltaXOver2;
    o2->y += a2-deltaYOver2;
}


void collision2d(double m1, double m2, double* v, double k){
    double msum = m1 + m2;
    double deltaV = v[1] - v[0];
    double c = m1 * v[0] + m2 * v[1];
    v[0] = (k * m2 * (deltaV) + c) / msum;
    v[1] = (k * m1 * (-deltaV) + c) / msum;
}


void collision2d(double alpha, double m1, double m2, double I1, double I2, double x1, double y1, double x2, double y2, double px, double py, double* v, double k, double u){
    double v1x = v[0], v1y = v[1], angv1 = v[2], v2x = v[3], v2y = v[4], angv2 = v[5];
    double deltaVX = v1x - v2x;
    double deltaVY = v1y -v2y;
    double APDeltaX = px - x1, APDeltaY = py - y1, BPDeltaX = px - x2, BPDeltaY = py - y2;
    double APS = APDeltaX * APDeltaX + APDeltaY * APDeltaY;
    double BPS = BPDeltaX * BPDeltaX + BPDeltaY * BPDeltaY;
    double AP = sqrt(APS);
    double BP = sqrt(BPS);
    
    double theta1 = atan2(APDeltaY, APDeltaX);
    double theta2 = atan2(BPDeltaY, BPDeltaX);
    double cosAlpha = cos(alpha);
    double sinAlpha = sin(alpha);
    double theta12 = theta1 + PI_OVER_TWO - alpha;
    double theta22 = theta2 + PI_OVER_TWO - alpha;
    double theta13 = PI - theta1 + alpha;
    double theta23 = PI - theta2 + alpha;
    double cosTheta12 = cos(theta12);
    double cosTheta22 = cos(theta22);
    double sinTheta13 = sin(theta13);
    double sinTheta23 = sin(theta23);
    
    double perpx = cos(alpha);
    double perpy = sin(alpha);
    
    double constant2 = -(1.0 / m1) + (APS * cosTheta12 * sinTheta13 / I1) - (1.0 / m2) + (BPS * cosTheta22 * sinTheta23 / I2);
    double constant1 = deltaVX * perpx + deltaVY * perpy + angv1 * AP * cosTheta12 - angv2 * BP * cosTheta22;
    
    double dImpulse = -constant1 / constant2;
    
    
    double a1 = dImpulse * (1 + k);
    double a11 = a1 / m1;
    double a12 = a1 / m2;
    
    //set final v1x v1y v2x v2y
    v[0] = v1x - cosAlpha * a11;
    v[1] = v1y - sinAlpha * a11;
    v[3] = v2x + cosAlpha * a12;
    v[4] = v2y + sinAlpha * a12;
    
    
    //set final angular velocity angv1, angv2
    v[2] = angv1 - (a1 * AP / I1) * cosTheta12;
    v[5] = angv2 + (a1 * BP / I2) * cosTheta22;
    
    return;
}


void collision2dWall(P2D_WallFlag wall, double m2, double I2, double x2, double y2, double px, double py, double* v, double k, double u, double uStatic){
    if(wall == P2D_LOWER_WALL || wall == P2D_UPPER_WALL){
        double v2x = v[0], v2y = v[1], angv2 = v[2];
        double deltaVY = -v2y;
        double BPDeltaX = px - x2, BPDeltaY = py - y2;
        double BPS = BPDeltaX * BPDeltaX + BPDeltaY * BPDeltaY;
        double BP = sqrt(BPS);
        double theta2 = atan2(BPDeltaY, BPDeltaX);
        double theta23 = THREE_PI_OVER_TWO - theta2;
        double cosTheta22 = cos(theta2);
        double sinTheta23 = sin(theta23);
        
        double constant2 = - 1.0 / m2 + BPS * cosTheta22 * sinTheta23 / I2;
        double constant1 = deltaVY - angv2 * BP * cosTheta22;
        
        double dImpulse = -constant1 / constant2;
        
        double a1 = dImpulse * (1 + k);
        //set final v1x v1y v2x v2y
        v[0] = v2x;
        v[1] = v2y + a1 / m2;
        
        //set final angular velocity angv1, angv2
        v[2] = angv2 + (a1 * BP / I2) * cosTheta22;
        
        /*
         u = 0.1;
         double v2para1, v2para2, avg, fi, fiv;
         v2para1 = v2x * cos(PI/2.0 + alpha) + angv2 * BP * cos(theta2 - alpha);
         cout << "v2para1 = " << v2para1 << endl;
         v2para2 = v[0] * cos(PI/2.0 + alpha) + v[2] * BP * cos(theta2 - alpha);
         cout <<"v2para2 = " << v2para2 << endl;
         avg =(v2para1 + v2para2)/2.0;
         fi  = u * abs(dImpulse);
         if(avg > 0)
         fi *= -1;
         cout << "fi  = " << fi <<endl;
         fiv = fi * cos(PI/2.0 + alpha) * cos(PI/2.0 + alpha) / m2 + fi * cos(alpha) * cos(alpha) / m2 + fi * cos(theta2 - alpha) * BPS / I2 * cos(theta2 - alpha);
         cout << "fiv  = " << fiv << endl;
         if(abs(fiv) > abs(avg))
         fiv = -avg;
         cout << "fivb = " << fiv << endl << endl << endl;
         
         
         v[0] += fiv * cos(alpha + PI/2.0);
         v[1] += fiv * cos(alpha);
         v[2] += fiv * cos(theta2 - alpha) / BP;
         */
        
        //friction
        if(u != 0){
            double v2para1, v2para2, avgpara2, frictionImpulse, frictionImpulseVpara;
            double cosTheta3 = cos(theta2 - PI_OVER_TWO);

            double cosTheta3TimesBP = BP * cosTheta3;
            v2para1 = -v2x + angv2 * cosTheta3TimesBP;
            v2para2 = -v[0] + v[2] * cosTheta3TimesBP;
            
            avgpara2 =(v2para1 + v2para2)/2.0;
            if(avgpara2 > 10)
                frictionImpulse  = u * abs(dImpulse);
            else
                frictionImpulse = uStatic * abs(dImpulse);
            if(avgpara2 > 0)
                frictionImpulse *= -1;
            
            frictionImpulseVpara = frictionImpulse / m2 + frictionImpulse * cosTheta3 * cosTheta3 * BPS / I2;
            if(abs(frictionImpulseVpara) > abs(avgpara2))
                frictionImpulseVpara = -avgpara2;
            
            v[0] += -frictionImpulseVpara;
            v[2] += frictionImpulseVpara * cosTheta3 / BP;
        }
        return;
    }else if(wall == P2D_LEFT_WALL || wall == P2D_RIGHT_WALL){
        double v2x = v[0], v2y = v[1], angv2 = v[2];
        double deltaVX = -v2x;
        double BPDeltaX = px - x2, BPDeltaY = py - y2;
        double BPS = BPDeltaX * BPDeltaX + BPDeltaY * BPDeltaY;
        double BP = sqrt(BPS);
        double theta2 = atan2(BPDeltaY, BPDeltaX);
        double theta22 = theta2 + PI_OVER_TWO;
        double theta23 = PI - theta2;
        double cosTheta22 = cos(theta22);
        double sinTheta23 = sin(theta23);
        
        double constant2 = - (1.0 / m2) + (BPS * cosTheta22 * sinTheta23 / I2);
        double constant1 = deltaVX - angv2 * BP * cosTheta22;
        
        double dImpulse = -constant1 / constant2;
        
        double a1 = dImpulse * (1 + k);
        //set final v1x v1y v2x v2y
        v[0] = v2x + a1 / m2;
        v[1] = v2y;
        
        //set final angular velocity angv1, angv2
        v[2] = angv2 + (a1 * BP / I2) * cosTheta22;
        
        //friction
        if(u != 0){
            double v2para1, v2para2, avg, fi, fiv;
            double cosTheta2 = cos(theta2);
            double cosTheta2TimesBP = BP * cosTheta2;
            
            v2para1 = angv2 * cosTheta2TimesBP;
            v2para2 = v[2] * cosTheta2TimesBP;
            
            avg =(v2para1 + v2para2)/2.0;
            fi  = u * abs(dImpulse);
            if(avg > 0)
                fi *= -1;
            
            fiv = fi / m2 + fi * cosTheta2 * cosTheta2 * BPS / I2;
            
            if(abs(fiv) > abs(avg))
                fiv = -avg;
            
            v[1] += fiv;
            v[2] += fiv * cosTheta2 / BP;
        }
        return;
    }
}

double getCollisionk(double k1, double k2){
    //collision coefficient is defined as the sqrt average of the two
    if(k1 == k2)
        return k1;
    else
        return sqrt(k1 * k2);
}
double getFrictionk(double u1, double u2){
    if(u1 == u2)
        return u1;
    else
        return sqrt(u1 * u2);
}

void checkZeroDenominator(double& d){
    if(d == 0)
        d = SMALL_NUMBER;
}
