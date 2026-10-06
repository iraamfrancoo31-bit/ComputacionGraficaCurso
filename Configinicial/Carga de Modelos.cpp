/*  Práctica 8
    21-Septiembre-2026
    Valenzuela Franco Iram Israel
    317313143*/

    // Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 8.0f, 12.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Light attributes
glm::vec3 lightPos(0.0f, 12.0f, 3.5f);
float movelightPos = 0.0f;

GLfloat vertices[] =
{
    -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,  -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,  -0.5f,  0.5f, -0.5f,  -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,  -0.5f, -0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,   0.5f,  0.5f, -0.5f,   0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,  -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,   0.5f,  0.5f, -0.5f,   0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,  -0.5f,  0.5f, -0.5f
};

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica6_Iram_Valenzuela", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    /*glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );*/

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Load models
    Model objeto((char*)"Models/OBJ.obj");
    Model dog((char*)"Models/RedDog.obj");
    Model tree1((char*)"Models/Spruce+Cycles.obj");
    Model tree2((char*)"Models/Spruce+Cycles.obj");
    //Model tree3((char*)"Models/Spruce+Cycles.obj");
    Model rottweiler((char*)"Models/Rottweiler_ligero.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);



    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Light position
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), lightPos.x + movelightPos, lightPos.y, lightPos.z);

        // Camera position
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Light properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

        // Material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 35.0f);

        // Draw the loaded model

        //Pasto
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        objeto.Draw(lightingShader);


        // Perro
        glm::mat4 modelDog(1.0f);

        modelDog = glm::translate(modelDog, glm::vec3(0.0f, 6.0f, 0.0f));
        modelDog = glm::scale(modelDog, glm::vec3(6.0f, 6.0f, 6.0f));

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
        dog.Draw(lightingShader);

        //model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //dog.Draw(shader);

        // Árbol1
        glm::mat4 modelTree1(1.0f);

        modelTree1 = glm::translate(modelTree1, glm::vec3(5.0f, 4.0f, 0.0f));
        modelTree1 = glm::scale(modelTree1, glm::vec3(1.0f, 3.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTree1));
        tree1.Draw(lightingShader);

        // Árbol
        glm::mat4 modelTree2(1.0f);

        modelTree2 = glm::translate(modelTree2, glm::vec3(-5.0f, 4.0f, -3.0f));
        modelTree2 = glm::scale(modelTree2, glm::vec3(1.0f, 2.5f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTree2));
        tree2.Draw(lightingShader);

        // Rottweiler
        glm::mat4 modelRottweiler(1.0f);

        modelRottweiler = glm::translate(modelRottweiler, glm::vec3(-5.0f, 2.0f, 3.0f));
        modelRottweiler = glm::rotate(modelRottweiler, glm::radians(90.0f), glm::vec3(0.0f, 7.0f, 0.0f));
        modelRottweiler = glm::scale(modelRottweiler, glm::vec3(8.0f, 8.0f, 8.0f));

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelRottweiler));
        rottweiler.Draw(lightingShader);


        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }


}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_O])
    {
        movelightPos += 0.1f;
    }

    if (keys[GLFW_KEY_L])
    {
        movelightPos -= 0.1f;
    }


}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}