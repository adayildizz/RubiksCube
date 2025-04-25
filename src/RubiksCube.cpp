#include "RubiksCube.h"
#include "SubCube.h"
#include <vector>


// RGBA colors
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
    
    

    // generate center coordinates
    
    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            for (int z = -1; z < 2; z++) {

                vec4 center((float)x, (float)y, (float)z, 1.0f);
                SubCube cube(x+y+z+4, center, face_colors);
                subCubes.push_back(cube);
                points.insert(points.end(), cube.points.begin(), cube.points.end());
                colors.insert(colors.end(), cube.faceColors.begin(), cube.faceColors.end());
               
            }
        }
    }
    for (vec4 i : points) {
        std::cout << "points" << i << std::endl;
    }
    
    



    

    

    

    
}


void RubiksCube::rotate(float rotationAngle)
{

}


//void RubiksCube::generateFaceColors()
//{
//
//}