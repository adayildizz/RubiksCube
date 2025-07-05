//
//  Display a rotating cube
//

int count = 0;

#include "Angel.h"
#include "RubiksCube.h"

std::vector<point4> points;
std::vector<color4> colors;

#include <filesystem>

// Create the Rubik's Cube
RubiksCube cube;

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
GLuint program;
GLuint vao;
GLuint buffer;


// for picking 
GLuint pickingProgram;
GLuint pickingFBO, pickingDepth, pickingTexture;
GLuint pickingVAO;
int pickedID;
int faceID;
// 
//----------------------------------------------------------------------------

void createPickingFBO() {
 
    const int width = 512;
    const int height = 512;

    glGenFramebuffers(1, &pickingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);

    glGenTextures(1, &pickingTexture);
    glBindTexture(GL_TEXTURE_2D, pickingTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, pickingTexture, 0);

    glGenRenderbuffers(1, &pickingDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, pickingDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, pickingDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Picking framebuffer is not complete." << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    

}

int pickObjectAt(double mouseX, double mouseY, int windowWidth, int windowHeight)
{

    // Render the scene to the picking framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(pickingProgram);
    glBindVertexArray(pickingVAO);


    GLint modelViewLoc = glGetUniformLocation(pickingProgram, "ModelView");
    GLint projectionLoc = glGetUniformLocation(pickingProgram, "Projection");

    mat4 projection = Ortho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
    glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, projection);

    model_view = Translate(0.0, 0.0, 0.0) *
        RotateX(Theta[Xaxis]) *
        RotateY(Theta[Yaxis]) *
        RotateZ(Theta[Zaxis]);

    for (int i = 0; i < cube.subCubes.size(); ++i)
    {
        mat4 mv = model_view * cube.subCubes[i].modelMatrix;
        glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, mv);
        glDrawArrays(GL_TRIANGLES, 36 * i, 36);
    }

    glFlush();

    // Read pixel color from the FBO
    unsigned char pixel[4];
    int y_inverted = windowHeight - static_cast<int>(mouseY);  // Invert Y for OpenGL

    glReadPixels(static_cast<int>(mouseX), y_inverted, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);

    

    // Decode ID from pixel color
    int id = pixel[0] + (pixel[1] << 8) + (pixel[2] << 16);

    // return to the main program 
    glUseProgram(program);
    // bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(vao);

    return id;
}

void init()
{
    // Load shaders
    std::filesystem::path shaderDir = std::filesystem::path(__FILE__).parent_path() / "../shaders";

    pickingProgram = InitShader((shaderDir / "pickingv.glsl").string().c_str(),
        (shaderDir / "pickingf.glsl").string().c_str());

    program = InitShader((shaderDir / "vshader.glsl").string().c_str(),
        (shaderDir / "fshader.glsl").string().c_str());

    // Create Picking FBO
    createPickingFBO();

    // Create buffer and VAOs
  
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    GLsizeiptr pointsSize = cube.points.size() * sizeof(vec4);
    GLsizeiptr colorsSize = cube.colors.size() * sizeof(vec4);
    GLsizeiptr normalsSize = cube.normals.size() * sizeof(vec3);
    GLsizeiptr pickingColorsSize = cube.pickingColors.size() * sizeof(vec4);

    glBufferData(GL_ARRAY_BUFFER,
        pointsSize + colorsSize + normalsSize + pickingColorsSize,
        NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, pointsSize, cube.points.data());
    glBufferSubData(GL_ARRAY_BUFFER, pointsSize, colorsSize, cube.colors.data());
    glBufferSubData(GL_ARRAY_BUFFER, pointsSize + colorsSize, normalsSize, cube.normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, pointsSize + colorsSize + normalsSize,
        pickingColorsSize, cube.pickingColors.data());

    // === Picking VAO ===
   
    glGenVertexArrays(1, &pickingVAO);
    glBindVertexArray(pickingVAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glUseProgram(pickingProgram);

    GLuint vPickPos = glGetAttribLocation(pickingProgram, "vPosition");
    glEnableVertexAttribArray(vPickPos);
    glVertexAttribPointer(vPickPos, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vPickColor = glGetAttribLocation(pickingProgram, "vPickingColor");
    glEnableVertexAttribArray(vPickColor);
    glVertexAttribPointer(vPickColor, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(pointsSize + colorsSize + normalsSize));

    // === Normal Rendering VAO ===
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glUseProgram(program);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(pointsSize));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(pointsSize + colorsSize));

    // Set projection
    mat4 projection = Ortho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
    Projection = glGetUniformLocation(program, "Projection");
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

    // Lighting uniforms
    vec3 lightColor(1.0f, 1.0f, 1.0f);
    float ambientStrength = 0.3f;
    vec3 lightDirection(0.0f, 1.0f, 0.0f);
    float diffuseIntensity = 0.7f;

    glUniform3fv(glGetUniformLocation(program, "directionalLight.color"), 1, &lightColor[0]);
    glUniform1f(glGetUniformLocation(program, "directionalLight.ambientIntensity"), ambientStrength);
    glUniform3fv(glGetUniformLocation(program, "directionalLight.direction"), 1, &lightDirection[0]);
    glUniform1f(glGetUniformLocation(program, "directionalLight.diffuseIntensity"), diffuseIntensity);

    ModelView = glGetUniformLocation(program, "ModelView");

    // Final OpenGL states
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1.0);
}




//---------------------------------------------------------------------
//
// display
//

void display(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Must be bound to default framebuffer!

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Generate the model-view matrix
    model_view = Translate(0.0, 0.0, 0.0);
  
    model_view = model_view *
        RotateX(Theta[Xaxis]) *
        RotateY(Theta[Yaxis]) *
        RotateZ(Theta[Zaxis]);

    for (int i = 0; i < cube.subCubes.size(); ++i)
    {
        // Per-subcube model matrix (translation, face rotation, etc.)
        mat4 finalModelView = model_view * cube.subCubes[i].modelMatrix;

        glUniformMatrix4fv(ModelView, 1, GL_TRUE, finalModelView);

        // Draw this subcube: 36 vertices, offset by 36 * i
        glDrawArrays(GL_TRIANGLES, 36 * i, 36);
    }

    

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
            
            // rotate the cube in one axis firstly
            isRotating = true;
        }
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            pickedID = pickObjectAt(mouseX, mouseY, width, height);
            faceID = cube.getFaceIDFromSubCube(pickedID);
            std::cout << "Clicked face: " << faceID << std::endl;
            
            
            
        }
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

            

    if (pickedID >= 0 && pickedID < cube.subCubes.size())
    {
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            
            cube.rotateFace(faceID, 90.0f);

        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS) {
           
            cube.rotateFace(faceID, -90.0f);

        }
    }


    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        std::cout << "Right click + drag to rotate the cube. " << std::endl;
        std::cout << "Left click to select a face. " << std::endl;
        std::cout << "key A or key D to rotate the selected face. " << std::endl;
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
    }*/

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

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
