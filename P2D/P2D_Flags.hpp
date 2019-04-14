//
//  P2D_Flags.hpp
//  physics2d
//
//  Created by Jiuhua on 6/7/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef P2D_Flags_hpp
#define P2D_Flags_hpp

#include <stdio.h>

#define DEFAULT_RED 255
#define DEFAULT_GREEN 255
#define DEFAULT_BLUE 255
#define DEFAULT_ALPHA 255

typedef enum{
    P2D_UPPER_WALL,
    P2D_LOWER_WALL,
    P2D_LEFT_WALL,
    P2D_RIGHT_WALL,
    P2D_ALL_WALL
}P2D_WallFlag;
typedef enum{
    P2D_REMOVE_ALL_FORCES,
    P2D_REMOVE_ALL_INDIVIDUAL_FROCES,
    P2D_REMOVE_ALL_UNIVERSAL_FORCES
}P2D_RemoveForceFlag;

typedef enum{
    P2D_ADD_OBJECT_RANDOM_WITHIN_WALL,
    P2D_ADD_OBJECT_BY_STORED_SETTING
}P2D_AddObjectFlag;

typedef enum{
    P2D_SW_BOOL,
    P2D_SW_DOUBLE,
    P2D_SW_STRING,
    P2D_SW_INT,
    P2D_SW_ADD_OBJECT_FLAG,
    P2D_SW_SHAPE,
    P2D_SW_OBJECT,
    P2D_SW_SHAPE_TYPE,
    P2D_SW_COLOR
}P2D_SaveWorldDataType;



#endif /* P2D_Flags_hpp */
