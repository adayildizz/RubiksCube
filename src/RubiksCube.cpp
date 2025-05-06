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

                int subcubeID = x + y + z + 4;
                vec4 center((float)x, (float)y, (float)z, 1.0f);
                face_colors.clear();

                // X axis: LEFT and RIGHT faces
                if (x == -1)
                {
                    face_colors.push_back(getColor(0, -1));
                    faceCodes[FACE::ORANGE].push_back(subcubeID);
                } 
                else {
                    face_colors.push_back(vec4(0, 0, 0, 1));
                }
                if (x == 1)
                {
                    face_colors.push_back(getColor(0, 1));
                    faceCodes[FACE::RED].push_back(subcubeID);
                }
                else {
                    face_colors.push_back(vec4(0, 0, 0, 1));
                }
                if (y == -1)
                {
                    face_colors.push_back(getColor(1, -1));
                    faceCodes[FACE::YELLOW].push_back(subcubeID);
                }
                else {
                    face_colors.push_back(vec4(0, 0, 0, 1));
                }
                if (y == 1)
                {
                    face_colors.push_back(getColor(1, 1));
                    faceCodes[FACE::WHITE].push_back(subcubeID);
                }
                else {
                    face_colors.push_back(vec4(0, 0, 0, 1));
                }

                if (z == -1)
                {
                    face_colors.push_back(getColor(2, -1));
                    faceCodes[FACE::BLUE].push_back(subcubeID);
                }
                else {
                    face_colors.push_back(vec4(0, 0, 0, 1));
                }
                if (z == 1)
                {
                    face_colors.push_back(getColor(2, 1));
                    faceCodes[FACE::GREEN].push_back(subcubeID);
                }
                else {
                    face_colors.push_back(vec4(0, 0, 0, 1));
                }
                


                SubCube cube(subcubeID, center, face_colors);
                subCubes.push_back(cube);

                points.insert(points.end(), cube.points.begin(), cube.points.end());
                colors.insert(colors.end(), cube.colors.begin(), cube.colors.end());

                // generated normals for each vertex
                for (size_t i = 0; i < cube.points.size(); i += 6) {
                    vec3 p0 = vec3(cube.points[i].x, cube.points[i].y, cube.points[i].z);
                    vec3 p1 = vec3(cube.points[i+1].x, cube.points[i+1].y, cube.points[i+1].z);
                    vec3 p2 = vec3(cube.points[i+2].x, cube.points[i+2].y, cube.points[i+2].z);

                    vec3 u = p1 - p0;
                    vec3 v = p2 - p0;
                    vec3 normal = normalize(cross(u, v));

                    for (int j = 0; j < 6; ++j) {
                        normals.push_back(normal);
                    }
                }
            }
        }
    }
}

vec4 RubiksCube::getColor(int axis, int direction) {
    // Standard Rubik's cube colors:
    // 0: Right (Red)
    // 1: Left (Orange)
    // 2: Up (White)
    // 3: Down (Yellow)
    // 4: Front (Green)
    // 5: Back (Blue)
    static std::vector<vec4> colors = {
        vec4(1, 0, 0, 1),    // Right - Red
        vec4(1, 0.5f, 0, 1), // Left - Orange
        vec4(1, 1, 1, 1),    // Up - White
        vec4(1, 1, 0, 1),    // Down - Yellow
        vec4(0, 1, 0, 1),    // Front - Green
        vec4(0, 0, 1, 1)     // Back - Blue
    };

    if (axis == 0) { // X axis: Left/Right
        return direction > 0 ? colors[0] : colors[1];
    }
    else if (axis == 1) { // Y axis: Up/Down
        return direction > 0 ? colors[2] : colors[3];
    }
    else { // Z axis: Front/Back
        return direction > 0 ? colors[4] : colors[5];
    }
}

void RubiksCube::rotateFace(int faceID, float rotationAngle)
{
    // around which point? -> center of the face

    // in which direction? calculate general rotation matrix according to the normal of the face

    // how much -> rotation angle is given

}