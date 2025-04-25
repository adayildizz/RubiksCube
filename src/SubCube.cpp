#include "SubCube.h"
#include "RubiksCube.h"

SubCube::SubCube(int id, vec4 centerCoordinate, std::vector<vec4>& faceColors)
{
    this->id = id;
    this->faceColors = faceColors;
    this->center = centerCoordinate;

    points.reserve(36);    
    modelMatrix = mat4(1.0f);
    
    initializeVertices();

    quad(1, 0, 3, 2, faceColors[0]);
    quad(2, 3, 7, 6, faceColors[1]);
    quad(3, 0, 4, 7, faceColors[2]);
    quad(6, 5, 1, 2, faceColors[3]);
    quad(4, 5, 6, 7, faceColors[4]);
    quad(5, 4, 0, 1, faceColors[5]);

}

void SubCube::quad(int a, int b, int c, int d, vec4& color)
{
    // First triangle
    faceColors.push_back(color);
    points.push_back(vertices[a]);

    faceColors.push_back(color);
    points.push_back(vertices[b]);

    faceColors.push_back(color);
    points.push_back(vertices[c]);

    // Second triangle
    faceColors.push_back(color);
    points.push_back(vertices[a]);

    faceColors.push_back(color);
    points.push_back(vertices[c]);

    faceColors.push_back(color);
    points.push_back(vertices[d]);
}

void SubCube::rotate(mat4 rotationMatrix) {
    modelMatrix = rotationMatrix * modelMatrix;
}

void SubCube::initializeVertices()
{
    float half = edgeLength / 2;
    vertices.clear();
    
    vertices.push_back(point4(center.x - half, center.y - half, center.z + half, 1.0)); 
    vertices.push_back(point4(center.x - half, center.y + half, center.z + half, 1.0)); 
    vertices.push_back(point4(center.x + half, center.y + half, center.z + half, 1.0)); 
    vertices.push_back(point4(center.x + half, center.y - half, center.z + half, 1.0)); 
    vertices.push_back(point4(center.x - half, center.y - half, center.z - half, 1.0)); 
    vertices.push_back(point4(center.x - half, center.y + half, center.z - half, 1.0)); 
    vertices.push_back(point4(center.x + half, center.y + half, center.z - half, 1.0)); 
    vertices.push_back(point4(center.x + half, center.y - half, center.z - half, 1.0)); 

    
}