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
                
                vec4 pickingColor = generatePickingColor(subcubeID);

                SubCube cube(subcubeID, center, subcube_face_colors, pickingColor);
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
                for (int j = 0; j < 36; ++j) {
                    pickingColors.push_back(pickingColor);
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

float componentAlongAxis(vec4 v, vec3 axis) {
    if (axis.x != 0.0f) return v.x;
    if (axis.y != 0.0f) return v.y;
    return v.z;
}

void RubiksCube::rotateFace(int faceID, float rotationAngle)
{
    FaceData& face = faces[faceID];
    mat4 rotationMatrix = Rotate(rotationAngle, face.normal);

    std::vector<int> subCubesToRotate;

    for (int i = 0; i < subCubes.size(); ++i) {
        vec4 transformedCenter = subCubes[i].modelMatrix * subCubes[i].center;
        float axisComponent = componentAlongAxis(transformedCenter, face.normal);

        // This selects cubes with center coordinate ? 1.0 along the axis of the face
        if (abs(axisComponent - 1.0f) < 0.1f) {
            subCubesToRotate.push_back(i);
        }
    }

    for (int i : subCubesToRotate) {
        subCubes[i].rotate(rotationMatrix);
    }

    //updateFacesData();
}



void RubiksCube::updateFacesData()
{
    
    for (int f = 0; f < 6; ++f) {
        faces[f].subCubeIDs.clear();
    }

    for (int i = 0; i < subCubes.size(); ++i) {
        
        vec4 transformedCenter = subCubes[i].modelMatrix * subCubes[i].center;

        for (int f = 0; f < 6; ++f) {
            vec3 normal = faces[f].normal;

            // Project center onto normal
            float d = dot(transformedCenter, normal);

            // Check if it's "close" to +1 or -1 (we allow some tolerance due to float error)
            if (abs(d - 1.0f) < 0.1f) {
                faces[f].subCubeIDs.push_back(i);
            }
        }
    }
}


vec4 RubiksCube::generatePickingColor(int id) {
    int r = (id & 0x000000FF);
    int g = (id & 0x0000FF00) >> 8;
    int b = (id & 0x00FF0000) >> 16;

    return vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}


int RubiksCube::getFaceIDFromSubCube(int subCubeID) {
    for (auto& subCube : subCubes) {
        if (subCube.id == subCubeID) {
            vec4 worldCenter = subCube.modelMatrix * subCube.center;
            vec3 dir = normalize(vec3(worldCenter.x, worldCenter.y, worldCenter.z));

            float absX = fabs(dir.x);
            float absY = fabs(dir.y);
            float absZ = fabs(dir.z);

            std::cout << "Clicked subcube center (world): " << worldCenter << std::endl;
            std::cout << "Direction: " << dir << std::endl;

            if (absX >= absY && absX >= absZ) {
                std::cout << "Dominant axis: X\n";
                return (dir.x >= 0.0f) ? 0 : 1; // Right : Left
            }
            else if (absY >= absX && absY >= absZ) {
                std::cout << "Dominant axis: Y\n";
                return (dir.y >= 0.0f) ? 2 : 3; // Up : Down
            }
            else {
                std::cout << "Dominant axis: Z\n";
                return (dir.z >= 0.0f) ? 4 : 5; // Front : Back
            }
        }
    }
    return -1;
}

