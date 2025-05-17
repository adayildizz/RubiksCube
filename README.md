# 🧊 Rubik's Cube

**This OpenGL application brings the iconic Rubik's Cube to life with interactive manipulation and dynamic rendering.**

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT) ![Rubik's Cube Demo](path/to/your/cool_rubiks_cube_demo.gif) ## ✨ Project Overview

This OpenGL program allows users to interact with a virtual Rubik's Cube. Users can observe the cube from various angles, select faces, and perform rotations to solve the puzzle. The program incorporates smooth animations, user input handling, and Phong lighting to enhance the visual experience. [cite: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46]

## 🧊 Key Features

* **3D Cube Construction:** Accurately models a Rubik's Cube as 27 individual sub-cubes. [cite: 2, 3, 4, 7, 8]
* **Interactive Rotation:** Allows users to rotate any of the cube's faces by 90 degrees. [cite: 5, 6, 9, 10, 11, 12, 13, 14, 15, 28, 29, 30]
* **Random Initialization:** The cube starts in a randomly scrambled state. [cite: 10, 11]
* **User-Controlled Viewing:** Users can freely rotate the entire cube to view it from different angles. [cite: 12, 13, 14, 36]
* **Face Picking:** Implements a picking mechanism to select which face to rotate. [cite: 15, 16, 17, 28]
* **Smooth Animations:** Rotations are animated for a visually appealing effect. [cite: 23, 24, 31, 38]
* **Phong Lighting:** Applies Phong lighting (ambient + diffuse) to the cube for realistic shading.
* **Help Interface:** Provides a command-line help function to guide users on how to interact with the cube (activated by the 'h' key). [cite: 32, 33, 38]
* **Modern OpenGL:** Utilizes vertex and fragment shaders and avoids deprecated OpenGL functions (OpenGL 3.1+). [cite: 43, 44]
* **Assignment Compliance:** Adheres to the assignment's constraints by using only the provided `mat.h` and `InitShaders.cpp` files. [cite: 45, 46]

## 🛠️ Getting Started - Ready to Twist?

### Prerequisites

* A C++ compiler (e.g., g++, clang, Visual Studio).
* OpenGL libraries and headers.
* The provided `mat.h` and `InitShaders.cpp` files.

### Installation

1.  Clone the repository:

    ```bash
    git clone [https://github.com/yourusername/yourrepo.git](https://github.com/yourusername/yourrepo.git)
    cd yourrepo
    ```
2.  Place the `mat.h` and `InitShaders.cpp` files in your project directory.

### Compilation

Compile the program using your C++ compiler, linking with the OpenGL libraries.  Example using g++:

```bash
g++ -o rubiks_cube main.cpp InitShaders.cpp -lGL -lGLU -lglut  # Adjust linker flags as needed
