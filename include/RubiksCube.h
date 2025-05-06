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
    enum FACE_COLOR {
        RED,
        ORANGE,
        WHITE,
        YELLOW,
        GREEN,
        BLUE
    };

    struct FaceData
    {
        int faceID;
        vec4 center;
        std::vector<int> subCubeIDs;
        vec4 face_color;
    };

    std::map<FACE_COLOR, FaceData> faces;


    RubiksCube();
    void initialize();
    void rotateFace(FACE_COLOR faceColor, float rotationAngle);
    vec4 getColor(int axis, int direction);
    void updateCubeData();
    
};

#endif
