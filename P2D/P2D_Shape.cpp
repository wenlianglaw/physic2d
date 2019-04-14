//
//  Shape.cpp
//  physics2d
//
//  Created by Jiuhua on 6/5/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "P2D_Shape.hpp"
#include "P2D_Flags.hpp"
#include <iostream>
using namespace std;

P2D_Color& P2D_Color::operator=(const P2D_Color & c){
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
    return *this;
}
//------------------------------------P2D_Shape definitions--------------------------------
P2D_Shape::P2D_Shape(){
    color.r = DEFAULT_RED;
    color.g = DEFAULT_GREEN;
    color.b = DEFAULT_BLUE;
    color.a = DEFAULT_ALPHA;
}

void P2D_Shape::setCircle(double r){
    if(r > 0){
        shapeType = P2D_SHAPE_CIRCLE;
        maxRadius = r;
        minRadius = r;
        width = 2 * r;
        height = 2 * r;
        isValid = true;
    }else
        cerr << "setCircle failed(invalid r)!" << endl;
}
void P2D_Shape::setRect(double w, double h){
    if(w > 0 && h > 0){
        shapeType = P2D_SHAPE_RECT;
        double wOver2 = w/2, hOver2 = h/2;
        maxRadius = sqrt(wOver2 * wOver2 + hOver2 * hOver2);
        if(w > h){
            minRadius = hOver2;
        }else
            minRadius = wOver2;
        width = w;
        height = h;
        isValid = true;
    }else{
        cout << "setRect failed(invalid w or h)!" << endl;
    }
}
void P2D_Shape::setLine(double l){
    if(l > 0){
        shapeType = P2D_SHAPE_LINE;
        maxRadius = l/2;
        width = l;
        isValid = true;
    }else{
        cout << "setLine failed(invalid l)!" << endl;
    }
}


void P2D_Shape::setRadius(double r){
    if(r > 0 && shapeType == P2D_SHAPE_CIRCLE){
        maxRadius = r;
        minRadius = r;
        width = 2 * r;
        height = 2 * r;
        isValid = true;
    }else if(r > 0 && shapeType == P2D_SHAPE_LINE){
        maxRadius = r;
        width = 2 * r;
        isValid = true;
    }else{
        cout << "setRadius failed(invalid r or type does not match)!" << endl;
    }
}
double P2D_Shape::getRadius(){
    if(shapeType != P2D_SHAPE_CIRCLE)
        cout << "getRadius Warning: shape does not match, returned maxRadius!" << endl;
    return maxRadius;
}
void P2D_Shape::setWidth(double w){
    if(w > 0 && shapeType == P2D_SHAPE_RECT){
        if(height > 0){
            double wOver2 = w/2, hOver2 = height/2;
            maxRadius = sqrt(wOver2 * wOver2 + hOver2 * hOver2);
            if(w > height){
                minRadius = hOver2;
            }else
                minRadius = wOver2;
            width = w;
            isValid = true;
        }else{
            width = w;
            isValid = false;
        }
    }else if(w > 0 && shapeType == P2D_SHAPE_LINE){
        maxRadius = w / 2;
        width = w;
        isValid = true;
    }else{
        cout << "setWidth failed(invalid w or type does not match)!" << endl;
    }
}
void P2D_Shape::setHeight(double h){
    if(h > 0 && shapeType == P2D_SHAPE_RECT){
        if(width > 0){
            double wOver2 = width/2, hOver2 = h/2;
            maxRadius = sqrt(wOver2 * wOver2 + hOver2 * hOver2);
            if(width > h){
                minRadius = hOver2;
            }else
                minRadius = wOver2;
            height = h;
            isValid = true;
        }else{
            height = h;
            isValid = false;
        }
    }else{
        cout << "setHeight failed(invalid h or type does not match)!" << endl;
    }
}

P2D_ShapeType P2D_Shape::getShapeType()const{
    return shapeType;
}
double P2D_Shape::getMaxRadius()const{
    return maxRadius;
}
double P2D_Shape::getMinRadius()const{
    return minRadius;
}
double P2D_Shape::getWidth()const{
    return width;
}
double P2D_Shape::getHeight()const{
    return height;
}
bool P2D_Shape::getIsValid()const{
    return isValid;
}
//-----------------------------------------------------------------------------
void P2D_Shape::write(ofstream& of)const{
    of.write((char*)&shapeType, sizeof(shapeType));
    color.write(of);
    of.write((char*)&maxRadius, sizeof(maxRadius));
    of.write((char*)&minRadius, sizeof(minRadius));
    of.write((char*)&width, sizeof(width));
    of.write((char*)&height, sizeof(height));
    of.write((char*)&isValid, sizeof(isValid));
}
void P2D_Shape::read(ifstream& inf){
    inf.read((char*)&shapeType, sizeof(shapeType));
    color.read(inf);
    inf.read((char*)&maxRadius, sizeof(maxRadius));
    inf.read((char*)&minRadius, sizeof(minRadius));
    inf.read((char*)&width, sizeof(width));
    inf.read((char*)&height, sizeof(height));
    inf.read((char*)&isValid, sizeof(isValid));
}
P2D_Shape* P2D_Shape::clone()const{
    P2D_Shape* newShape = new P2D_Shape();
    newShape->shapeType = shapeType;
    newShape->color = color;
    newShape->maxRadius = maxRadius;
    newShape->minRadius = minRadius;
    newShape->width = width;
    newShape->height = height;
    newShape->isValid = isValid;
    return newShape;
}

P2D_Shape& P2D_Shape::operator=(const P2D_Shape& sp){
    shapeType = sp.shapeType;
    color = sp.color;
    maxRadius = sp.maxRadius;
    minRadius = sp.minRadius;
    width = sp.width;
    height = sp.height;
    isValid = sp.isValid;
    return *this;
}
