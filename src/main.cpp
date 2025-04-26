//
//  Display a rotating cube
//

#include "Angel.h"
#include "RubiksCube.h"
#include "light.h"

std::vector<point4> points;
std::vector<color4> colors;

#include <filesystem>

mat4 model_view;
bool isRotating = false;
float lastX = 0.0f;
float lastY = 0.0f;
float rotationSensitivity = 1.0f;

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

//Light global variables
Light mainLight;
GLint ambientColorLoc;
GLint ambientIntensityLoc;
GLint lightDirectionLoc;
GLint diffuseIntensityLoc;

//----------------------------------------------------------------------------



void init()
{
    // Load shaders and use the resulting shader program
    std::filesystem::path shaderDir = std::filesystem::path(__FILE__).parent_path() / "../shaders";
    GLuint program = InitShader((shaderDir / "vshader.glsl").string().c_str(),
                                (shaderDir / "fshader.glsl").string().c_str());
    glUseProgram(program);

    // Create the Rubik's Cube
    RubiksCube cube;

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Calculate buffer sizes
    GLsizeiptr pointsSize  = cube.points.size()  * sizeof(vec4);
    GLsizeiptr colorsSize  = cube.colors.size()  * sizeof(vec4);
    GLsizeiptr normalsSize = cube.normals.size() * sizeof(vec3);

    // Allocate total buffer space
    glBufferData(GL_ARRAY_BUFFER, pointsSize + colorsSize + normalsSize, NULL, GL_STATIC_DRAW);

    // Fill buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, pointsSize, cube.points.data());
    glBufferSubData(GL_ARRAY_BUFFER, pointsSize, colorsSize, cube.colors.data());
    glBufferSubData(GL_ARRAY_BUFFER, pointsSize + colorsSize, normalsSize, cube.normals.data());

    // Set up vertex attribute pointers
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(pointsSize));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(pointsSize + colorsSize));

    // Set up projection matrix (orthographic)
    mat4 projection = Ortho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
    Projection = glGetUniformLocation(program, "Projection");
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

    //set up lighting
    ambientColorLoc = glGetUniformLocation(program, "directionalLight.color");
    ambientIntensityLoc = glGetUniformLocation(program, "directionalLight.ambientIntensity");
    lightDirectionLoc = glGetUniformLocation(program, "directionalLight.direction");
    diffuseIntensityLoc = glGetUniformLocation(program, "directionalLight.diffuseIntensity");

    mainLight.UseLight(ambientIntensityLoc, ambientColorLoc, diffuseIntensityLoc, lightDirectionLoc);

    // Get model-view uniform location
    ModelView = glGetUniformLocation(program, "ModelView");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1.0); // Dark background
}


//---------------------------------------------------------------------
//
// display
//

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Generate the model-view matrix
    model_view = Translate(0.0, 0.0, 0.0);
  
    model_view = model_view *
        RotateX(Theta[Xaxis]) *
        RotateY(Theta[Yaxis]) *
        RotateZ(Theta[Zaxis]);

    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

    // Draw ALL cubes (36 vertices per cube × 27 cubes)
    glDrawArrays(GL_TRIANGLES, 0, 36 * 27);

    glFlush();
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (isRotating)
    {
        // Calculate mouse movement delta
        float dx = (xpos - lastX);
        float dy = (ypos - lastY);

        // Apply rotation to both axes simultaneously
        Theta[Yaxis] += rotationSensitivity * dx;
        Theta[Xaxis] += rotationSensitivity * dy;

        

       
        if (Theta[Yaxis] > 360.0) {
            Theta[Yaxis] -= 360.0;
        }
        if (Theta[Zaxis] > 360.0) {
            Theta[Zaxis] -= 360.0;
        }
        if (Theta[Xaxis] > 360.0) {
            Theta[Xaxis] -= 360.0;
        }
    }

    // Update last position
    lastX = xpos;
    lastY = ypos;
    


}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_RELEASE) {
        // stop rotating
        isRotating = false;
    }
    else if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            std::cout << "in callback" << std::endl;
            // rotate the cube in one axis firstly
            isRotating = true;
        }
    }

}

//---------------------------------------------------------------------
//
// main
//

int main()
{
   
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(512, 512, "Rubik's Cube", NULL, NULL);
    

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
/*
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
*/
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
         

        display();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
