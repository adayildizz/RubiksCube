#include "RubiksCube.h"
#include "SubCube.h"
#include <vector>



// Vertices of a unit cube centered at origin, sides aligned with axes
std::vector<point4> vertices = {
    point4(-0.5, -0.5,  0.5, 1.0),
    point4(-0.5,  0.5,  0.5, 1.0),
    point4(0.5,  0.5,  0.5, 1.0),
    point4(0.5, -0.5,  0.5, 1.0),
    point4(-0.5, -0.5, -0.5, 1.0),
    point4(-0.5,  0.5, -0.5, 1.0),
    point4(0.5,  0.5, -0.5, 1.0),
    point4(0.5, -0.5, -0.5, 1.0)
};

// RGBA olors
std::vector<color4> face_colors = {
    color4(1.0, 0.0, 0.0, 1.0),  // red
    color4(1.0, 1.0, 0.0, 1.0),  // yellow
    color4(0.0, 1.0, 0.0, 1.0),  // green
    color4(0.0, 0.0, 1.0, 1.0),  // blue
    color4(1.0, 0.0, 1.0, 1.0),  // magenta
    color4(0.0, 1.0, 1.0, 1.0)   // cyan
};

RubiksCube::RubiksCube() {
    initialize();

}
void RubiksCube::initialize()
{
    // create 27 subcube, with correct color values
    for (int i = 0; i < 1; i++)
    {
        SubCube cube(i, vertices, face_colors); // create the cube in terms of 6 faces each of which is made of two triangles
        colors = cube.faceColors;
        points = cube.points;

    }

    // apply translation on the sub cubes };

    
}


void RubiksCube::rotate(float rotationAngle)
{

}