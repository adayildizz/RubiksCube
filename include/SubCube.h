
#ifndef SUBCUBE_H
#define SUBCUBE_H

#include "Angel.h"
#include <vector>

class SubCube
{
public:
    int id;
    vec4 center;
    float edgeLength = 1.0f;
    std::vector<vec4> vertices;
    std::vector<vec4> points;
    std::vector<vec4> faceColors;
    std::vector<vec4> colors;
    mat4 modelMatrix;

    SubCube(int id, vec4 centerCoordinate, std::vector<vec4>& faceColors);
    void quad(int a, int b, int c, int d, vec4& color);
    void rotate(mat4 rotationMatrix);
    void initializeVertices();
    
    
};

#endif

