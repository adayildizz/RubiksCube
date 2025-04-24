#include "Cube.h"

Cube::Cube(int id, const std::vector<vec4>& vertices, const std::vector<vec4>& colors)
{
    this->id = id;
    this->vertices = vertices;
    this->colors = colors;

    points.reserve(36);      
    faceColors.reserve(36);

    quad(1, 0, 3, 2);
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(6, 5, 1, 2);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);

}

void Cube::quad(int a, int b, int c, int d)
{
    // First triangle
    faceColors.push_back(colors[a]);
    points.push_back(vertices[a]);

    faceColors.push_back(colors[b]);
    points.push_back(vertices[b]);

    faceColors.push_back(colors[c]);
    points.push_back(vertices[c]);

    // Second triangle
    faceColors.push_back(colors[a]);
    points.push_back(vertices[a]);

    faceColors.push_back(colors[c]);
    points.push_back(vertices[c]);

    faceColors.push_back(colors[d]);
    points.push_back(vertices[d]);
}

