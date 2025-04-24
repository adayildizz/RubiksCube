
#ifndef SUBCUBE_H
#define SUBCUBE_H

#include "Angel.h"
#include <vector>

class SubCube
{
public:
    int id;
    std::vector<vec4> vertices;
    std::vector<vec4> points;
    std::vector<vec4> faceColors;

    SubCube(int id, std::vector<vec4>& vertices, std::vector<vec4>& faceColors);
    void quad(int a, int b, int c, int d, vec4& color);
    
    
    
};

#endif

