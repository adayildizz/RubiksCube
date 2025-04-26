#include "SubCube.h"
#include "RubiksCube.h"

SubCube::SubCube(int id, vec4 centerCoordinate, std::vector<vec4>& faceColors)
{
    this->id = id;

    this->center = centerCoordinate;

    points.reserve(36);
    colors.reserve(36);
    modelMatrix = mat4(1.0f);

    initializeVertices();

    // Front face (+Z): Vertices 0, 1, 2, 3 - Color index 5 (Green)
    quad(0, 1, 2, 3, faceColors[5]);

    // Back face (-Z): Vertices 4, 5, 6, 7 - Color index 4 (Blue)
    quad(4, 5, 6, 7, faceColors[4]); // Original quad uses (a,b,c) and (a,c,d) triangles

    // Left face (-X): Vertices 4, 0, 1, 5 - Color index 0 (Orange)
    quad(4, 0, 1, 5, faceColors[0]);

    // Right face (+X): Vertices 3, 7, 6, 2 - Color index 1 (Red)
    quad(3, 7, 6, 2, faceColors[1]);

    // Top face (+Y): Vertices 1, 5, 6, 2 - Color index 3 (White)
    quad(1, 5, 6, 2, faceColors[3]);

    // Bottom face (-Y): Vertices 4, 7, 3, 0 - Color index 2 (Yellow)
    quad(4, 7, 3, 0, faceColors[2]);

    // Assign the member faceColors if needed later (optional)
    this->faceColors = faceColors;
}

void SubCube::quad(int a, int b, int c, int d, vec4& color)
{
    // First triangle
    points.push_back(vertices[a]);
    colors.push_back(color);

    points.push_back(vertices[b]);
    colors.push_back(color);

    points.push_back(vertices[c]);
    colors.push_back(color);

    // Second triangle
    points.push_back(vertices[a]);
    colors.push_back(color);

    points.push_back(vertices[c]);
    colors.push_back(color);

    points.push_back(vertices[d]);
    colors.push_back(color);
}


void SubCube::rotate(mat4 rotationMatrix) {
    modelMatrix = rotationMatrix * modelMatrix;
}


void SubCube::initializeVertices()
{
    float scale = 0.95f; // Slightly shrink each subcube
    float half = (edgeLength * scale) / 2;

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
