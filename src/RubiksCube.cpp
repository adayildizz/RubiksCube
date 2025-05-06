#include "RubiksCube.h"
#include "SubCube.h"
#include <vector>

// RGBA colors

RubiksCube::RubiksCube() {
    initialize();
}

void RubiksCube::initialize()
{
    std::vector<vec4> subcube_face_colors;
    subcube_face_colors.reserve(6);

    for (int i = 0; i < 6; i++)
    {
        faces[i].normal = FACE_NORMALS[i];
        faces[i].color = FACE_COLORS[i];
    }

    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            for (int z = -1; z < 2; z++) {

                int subcubeID = (x + 1) * 9 + (y + 1) * 3 + (z + 1);
                vec4 center((float)x, (float)y, (float)z, 1.0f);
                subcube_face_colors.clear();

                // LEFT
                if (x == -1)
                {
                    subcube_face_colors.push_back(getColor(0, -1));
                    faces[1].subCubeIDs.push_back(subcubeID);
                } 
                else {
                    subcube_face_colors.push_back(vec4(0, 0, 0, 1));
                }
                // RIGHT
                if (x == 1)
                {
                    subcube_face_colors.push_back(getColor(0, 1));
                    faces[0].subCubeIDs.push_back(subcubeID);
                }
                else {
                    subcube_face_colors.push_back(vec4(0, 0, 0, 1));
                }
                // DOWN
                if (y == -1)
                {
                    subcube_face_colors.push_back(getColor(1, -1));
                    faces[3].subCubeIDs.push_back(subcubeID);
                }
                else {
                    subcube_face_colors.push_back(vec4(0, 0, 0, 1));
                }
                // UP
                if (y == 1)
                {
                    subcube_face_colors.push_back(getColor(1, 1));
                    faces[2].subCubeIDs.push_back(subcubeID);
                }
                else {
                    subcube_face_colors.push_back(vec4(0, 0, 0, 1));
                }
                // BACK
                if (z == -1)
                {
                    subcube_face_colors.push_back(getColor(2, -1));
                    faces[5].subCubeIDs.push_back(subcubeID);
                }
                else {
                    subcube_face_colors.push_back(vec4(0, 0, 0, 1));
                }
                //FRONT
                if (z == 1)
                {
                    subcube_face_colors.push_back(getColor(2, 1));
                    faces[4].subCubeIDs.push_back(subcubeID);
                }
                else {
                    subcube_face_colors.push_back(vec4(0, 0, 0, 1));
                }
                


                SubCube cube(subcubeID, center, subcube_face_colors);
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
    

    if (axis == 0) { // X axis: Left/Right
        return direction > 0 ? FACE_COLORS[0] : FACE_COLORS[1];
    }
    else if (axis == 1) { // Y axis: Up/Down
        return direction > 0 ? FACE_COLORS[2] : FACE_COLORS[3];
    }
    else { // Z axis: Front/Back
        return direction > 0 ? FACE_COLORS[4] : FACE_COLORS[5];
    }
}




mat4 Rotate(float angle, const vec3& axis) {
    // Convert angle to radians (Angel.h uses degrees, but some math functions use radians)
    float a = angle * M_PI / 180.0f;
    float c = cos(a);
    float s = sin(a);

    vec3 u = normalize(axis); // Ensure axis is a unit vector
    float x = u.x;
    float y = u.y;
    float z = u.z;

    // Build the rotation matrix (Rodrigues' rotation formula)
    mat4 rotationMatrix(
        x * x * (1 - c) + c, x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0.0f,
        y * x * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - x * s, 0.0f,
        z * x * (1 - c) - y * s, z * y * (1 - c) + x * s, z * z * (1 - c) + c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    return rotationMatrix;
}

void RubiksCube::rotateFace(int faceID, float rotationAngle)
{
    FaceData face = faces[faceID];
    mat4 rotationMatrix;
    for (int i = 0; i < 9; i++)
    {
        std::cout << face.subCubeIDs[i] << std::endl;
        rotationMatrix = Rotate(rotationAngle, face.normal);
        std::cout << rotationMatrix << std::endl;
        subCubes[face.subCubeIDs[i]].rotate(rotationMatrix);
    };
}
