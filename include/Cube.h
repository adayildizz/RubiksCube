
#ifndef CUBE_H
#define CUBE_H

#include "Angel.h"
#include <vector>

class Cube
{
public:
    int id;
    std::vector<vec4> vertices;
    std::vector<vec4> colors;
    std::vector<vec4> points;
    std::vector<vec4> faceColors;

    Cube(int id, const std::vector<vec4>& vertices, const std::vector<vec4>& colors);
    void quad(int a, int b, int c, int d);
    
    
};

#endif

