#include "RubiksCube.h"
#include "SubCube.h"
#include <vector>


// RGBA colors

RubiksCube::RubiksCube() {
    initialize();

}
void RubiksCube::initialize()
{
    std::vector<vec4> face_colors;
    face_colors.reserve(6);

    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            for (int z = -1; z < 2; z++) {

                vec4 center((float)x, (float)y, (float)z, 1.0f);
                face_colors.clear();

                // X axis: LEFT and RIGHT faces
                face_colors.push_back((x == -1) ? getColor(0, -1) : vec4(0, 0, 0, 1));
                face_colors.push_back((x == 1) ? getColor(0, 1) : vec4(0, 0, 0, 1));

                // Y axis: BOTTOM and TOP faces
                face_colors.push_back((y == -1) ? getColor(1, -1) : vec4(0, 0, 0, 1));
                face_colors.push_back((y == 1) ? getColor(1, 1) : vec4(0, 0, 0, 1));

                // Z axis: BACK and FRONT faces
                face_colors.push_back((z == -1) ? getColor(2, -1) : vec4(0, 0, 0, 1));
                face_colors.push_back((z == 1) ? getColor(2, 1) : vec4(0, 0, 0, 1));

                SubCube cube(x + y + z + 4, center, face_colors);
                subCubes.push_back(cube);

                points.insert(points.end(), cube.points.begin(), cube.points.end());
                colors.insert(colors.end(), cube.colors.begin(), cube.colors.end()); 
            }
        }
    }
}



void RubiksCube::rotate(float rotationAngle)
{

}




vec4 RubiksCube::getColor(int axis, int direction) {
    static std::vector<vec4> colors = {
        vec4(1, 0, 0, 1), // Red
        vec4(0, 1, 0, 1), // Green
        vec4(0, 0, 1, 1), // Blue
        vec4(1, 1, 0, 1), // Yellow
        vec4(1, 0, 1, 1), // Magenta
        vec4(0, 1, 1, 1)  // Cyan
    };
    return colors[axis * 2 + (direction > 0 ? 0 : 1)];
}