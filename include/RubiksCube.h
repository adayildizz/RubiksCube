#ifndef RUBIKS_CUBE
#define RUBIKS_CUBE

#include <vector>
#include <map>
#include "Angel.h"
#include "SubCube.h"

typedef vec4 color4;
typedef vec4 point4;

class RubiksCube {
public:
    std::vector<vec4> points;
    std::vector<vec4> colors;
    std::vector<vec3> normals;
    std::vector<SubCube> subCubes;
    enum FACE {
        RED,
        ORANGE,
        WHITE,
        YELLOW,
        GREEN,
        BLUE
    };

    std::map<FACE, std::vector<int>> faceCodes;


    RubiksCube();
    void initialize();
    void rotateFace(int faceID, float rotationAngle);
    vec4 getColor(int axis, int direction);
    
};

#endif
