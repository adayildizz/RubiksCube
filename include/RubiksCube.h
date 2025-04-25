#ifndef RUBIKS_CUBE
#define RUBIKS_CUBE

#include <vector>
#include "Angel.h"
#include "SubCube.h"

typedef vec4 color4;
typedef vec4 point4;

class RubiksCube {
public:
    std::vector<vec4> points;
    std::vector<vec4> colors;
    std::vector<SubCube> subCubes; // Store SubCubes

    RubiksCube();
    void initialize();
    void rotate(float rotationAngle);
};

#endif
