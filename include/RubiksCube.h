#ifndef RUBIKS_CUBE
#define RUBIKS_CUBE

#include <vector>
#include <array>
#include "Angel.h"
#include "SubCube.h"

typedef vec4 color4;
typedef vec4 point4;

// Declare the Rotate function here
mat4 Rotate(float angle, const vec3& axis);

class RubiksCube {
public:
    std::vector<vec4> points;
    std::vector<vec4> colors;
    std::vector<vec3> normals;
    std::vector<SubCube> subCubes;

    bool isFaceAnimating = false;
    int animatingFaceID = -1;
    float currentAnimationAngle = 0.0f;
    float targetAnimationAngle = 0.0f;
    float animationSpeed = 5.0f; // Degrees per frame (adjust for desired speed)
    std::vector<int> subCubesToAnimate;
    void updateAnimation();

    struct FaceData {
        std::vector<int> subCubeIDs;
        vec3 normal;
        vec4 color;
    };

    std::array<FaceData, 6> faces;

    // Predefined face normals
    const std::array<vec3, 6> FACE_NORMALS = {
        vec3(1, 0, 0),     // RED (Right)
        vec3(-1, 0, 0),    // ORANGE (Left)
        vec3(0, 1, 0),     // WHITE (Up)
        vec3(0, -1, 0),    // YELLOW (Down)
        vec3(0, 0, 1),     // GREEN (Front)
        vec3(0, 0, -1)     // BLUE (Back)
    };

    const std::array<vec4, 6> FACE_COLORS = {
        vec4(1, 0, 0, 1),    // Right - Red
        vec4(1, 0.5f, 0, 1), // Left - Orange
        vec4(1, 1, 1, 1),    // Up - White
        vec4(1, 1, 0, 1),    // Down - Yellow
        vec4(0, 1, 0, 1),    // Front - Green
        vec4(0, 0, 1, 1)     // Back - Blue
    };


    RubiksCube();
    void initialize();
    void rotateFace(int faceID, float rotationAngle);
    vec4 getColor(int axis, int direction);
    void updateFacesData();

};

float componentAlongAxis(vec4 v, vec3 axis);


#endif
