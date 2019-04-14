//
//  Shape.hpp
//  physics2d
//
//  Created by Jiuhua on 6/5/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef Shape_hpp
#define Shape_hpp

#include <stdio.h>
#include <math.h>
#include <cstdint>
#include <fstream>

typedef enum{
    P2D_SHAPE_CIRCLE,
    P2D_SHAPE_RECT,
    P2D_SHAPE_LINE,
    P2D_SHAPE_MIXED
}P2D_ShapeType;

struct P2D_Color{
    uint8_t r, g, b, a;
    P2D_Color& operator = (const P2D_Color&);
    
    void write(std::ofstream& of)const{
        of.write((char*)&r, sizeof(r));
        of.write((char*)&g, sizeof(g));
        of.write((char*)&b, sizeof(b));
        of.write((char*)&a, sizeof(a));
    }
    void read(std::ifstream& inf){
        inf.read((char*)&r, sizeof(r));
        inf.read((char*)&g, sizeof(g));
        inf.read((char*)&b, sizeof(b));
        inf.read((char*)&a, sizeof(a));
    }
};
//----------------------------P2D_Shape class
class P2D_Shape{
public:
    P2D_Color color;
    
    P2D_Shape();
    P2D_Shape(const P2D_Shape& sp){*this = sp;}
    
    void setCircle(double);
    void setRect(double, double);
    void setLine(double);
    
    void setRadius(double);
    double getRadius();
    void setWidth(double);
    void setHeight(double);
    
    
    
    P2D_ShapeType getShapeType()const;
    double getMaxRadius()const;
    double getMinRadius()const;
    double getWidth()const;
    double getHeight()const;
    bool getIsValid()const;
    
    void write(std::ofstream&)const;
    void read(std::ifstream&);
    P2D_Shape* clone()const;
    
    P2D_Shape& operator=(const P2D_Shape&);
    ~P2D_Shape(){}
    
private:
    P2D_ShapeType shapeType;
    double maxRadius = 0, minRadius = 0, width = 0, height = 0;
    bool isValid = false;
};
//---------------------------P2D_Shape class

#endif /* Shape_hpp */
