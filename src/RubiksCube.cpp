#include "RubiksCube.h"
#include "SubCube.h"
#include <vector>

// RGBA colors

RubiksCube::RubiksCube() {
    initialize();
}

void RubiksCube::initialize()
{
    std::vector<vec4> face_colors;
    face_colors.reserve(6);

    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            for (int z = -1; z < 2; z++) {

                vec4 center((float)x, (float)y, (float)z, 1.0f);
                face_colors.clear();

                // X axis: LEFT and RIGHT faces
                face_colors.push_back((x == -1) ? getColor(0, -1) : vec4(0, 0, 0, 1)); // Left
                face_colors.push_back((x == 1)  ? getColor(0, 1)  : vec4(0, 0, 0, 1)); // Right

                // Y axis: BOTTOM and TOP faces
                face_colors.push_back((y == -1) ? getColor(1, -1) : vec4(0, 0, 0, 1)); // Bottom
                face_colors.push_back((y == 1)  ? getColor(1, 1)  : vec4(0, 0, 0, 1)); // Top

                // Z axis: BACK and FRONT faces
                face_colors.push_back((z == -1) ? getColor(2, -1) : vec4(0, 0, 0, 1)); // Back
                face_colors.push_back((z == 1)  ? getColor(2, 1)  : vec4(0, 0, 0, 1)); // Front

                SubCube cube(x + y + z + 4, center, face_colors);

                points.insert(points.end(), cube.points.begin(), cube.points.end());
                colors.insert(colors.end(), cube.colors.begin(), cube.colors.end());

                // --- Modified Normal Generation (Alternative) ---
                // Iterate through the points of the current sub-cube in chunks of 6 (representing one face)
                for (size_t i = 0; i < cube.points.size(); i += 6) {
                    // Get the first three vertices of the current face's triangles
                    vec3 p0 = vec3(cube.points[i].x, cube.points[i].y, cube.points[i].z);
                    vec3 p1 = vec3(cube.points[i+1].x, cube.points[i+1].y, cube.points[i+1].z);
                    vec3 p2 = vec3(cube.points[i+2].x, cube.points[i+2].y, cube.points[i+2].z);

                    // Calculate the normal using the cross product
                    vec3 u = p1 - p0;
                    vec3 v = p2 - p0;
                    vec3 calculated_normal = normalize(cross(u, v));

                    // Determine the expected outward normal for this face based on the iteration index (i)
                    vec3 expected_normal;
                    size_t face_index = i / 6; // 0 for first face, 1 for second, etc.

                    switch (face_index) {
                        case 0: // Front face (+Z)
                            expected_normal = vec3(0.0f, 0.0f, 1.0f);
                            break;
                        case 1: // Back face (-Z)
                            expected_normal = vec3(0.0f, 0.0f, -1.0f);
                            break;
                        case 2: // Left face (-X)
                            expected_normal = vec3(-1.0f, 0.0f, 0.0f);
                            break;
                        case 3: // Right face (+X)
                            expected_normal = vec3(1.0f, 0.0f, 0.0f);
                            break;
                        case 4: // Top face (+Y)
                            expected_normal = vec3(0.0f, 1.0f, 0.0f);
                            break;
                        case 5: // Bottom face (-Y)
                            expected_normal = vec3(0.0f, -1.0f, 0.0f);
                            break;
                        default:
                            // Should not happen if SubCube generates exactly 6 faces
                            expected_normal = vec3(0.0f, 0.0f, 0.0f);
                            break;
                    }


                    // Compare the calculated normal with the expected outward normal.
                    // If they are in opposite directions, flip the calculated normal.
                    if (dot(calculated_normal, expected_normal) < 0) {
                        calculated_normal = -calculated_normal;
                    }

                    // Assign the corrected normal to all 6 vertices of this face
                    for (int j = 0; j < 6; ++j) {
                        normals.push_back(calculated_normal);
                    }
                }
                // --- End Modified Normal Generation (Alternative) ---

                subCubes.push_back(cube); // Keep this if you need subCubes for other logic
            }
        }
    }
}

vec4 RubiksCube::getColor(int axis, int direction) {
    // Standard Rubik's cube colors:
    // 0: Right (Red)
    // 1: Left (Orange)
    // 2: Up (White)
    // 3: Down (Yellow)
    // 4: Front (Green)
    // 5: Back (Blue)
    static std::vector<vec4> colors = {
        vec4(1, 0, 0, 1),    // Right - Red
        vec4(1, 0.5f, 0, 1), // Left - Orange
        vec4(1, 1, 1, 1),    // Up - White
        vec4(1, 1, 0, 1),    // Down - Yellow
        vec4(0, 1, 0, 1),    // Front - Green
        vec4(0, 0, 1, 1)     // Back - Blue
    };

    if (axis == 0) { // X axis: Left/Right
        return direction > 0 ? colors[0] : colors[1];
    }
    else if (axis == 1) { // Y axis: Up/Down
        return direction > 0 ? colors[2] : colors[3];
    }
    else { // Z axis: Front/Back
        return direction > 0 ? colors[4] : colors[5];
    }
}
