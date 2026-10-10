
/*  Práctica 8
    05-Octubre-2026
    Valenzuela Franco Iram Israel
    317313143*/

    // Std. Includes
#include <string>
#include <iostream>
#include <cmath>

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
#include <glm/gtc/constants.hpp>

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
bool keys[1024] = { false };
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Sistema orbital: Sol y Luna
float orbitAngle = 0.0f;
const float orbitRadius = 18.0f;
const float orbitCenterY = 8.0f;
const float orbitSpeed = 0.8f;

glm::vec3 sunPosition(0.0f);
glm::vec3 moonPosition(0.0f);

GLfloat daylight = 0.0f;

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
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 - Iram Valenzuela", nullptr, nullptr);

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

    // GLEW
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Load models
    Model objeto((char*)"Models/OBJ.obj");
    Model dog((char*)"Models/RedDog.obj");
    Model tree1((char*)"Models/Spruce+Cycles.obj");
    Model tree2((char*)"Models/Spruce+Cycles.obj");
    Model rottweiler((char*)"Models/Rottweiler_ligero.obj");

    // Modelos de las fuentes de luz
    Model sun((char*)"Models/Sun.obj");
    Model moon((char*)"Models/MOON.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Movimiento orbital de ambas fuentes
        if (keys[GLFW_KEY_O])
        {
            orbitAngle += orbitSpeed * deltaTime;
        }

        if (keys[GLFW_KEY_L])
        {
            orbitAngle -= orbitSpeed * deltaTime;
        }

        orbitAngle = glm::mod(orbitAngle, glm::two_pi<float>());

        if (orbitAngle < 0.0f)
        {
            orbitAngle += glm::two_pi<float>();
        }

        // Posición del Sol
        sunPosition.x = orbitRadius * cos(orbitAngle);
        sunPosition.y = orbitCenterY + orbitRadius * sin(orbitAngle);
        sunPosition.z = 0.0f;

        // Posición de la Luna: opuesta al Sol
        float moonAngle = orbitAngle + glm::pi<float>();

        moonPosition.x = orbitRadius * cos(moonAngle);
        moonPosition.y = orbitCenterY + orbitRadius * sin(moonAngle);
        moonPosition.z = 0.0f;

        // Transición gradual de día y noche
        float sunHeight = sin(orbitAngle);

        float t = glm::clamp((sunHeight + 0.15f) / 0.30f, 0.0f, 1.0f);
        daylight = t * t * (3.0f - 2.0f * t);

        float night = 1.0f - daylight;

        // Color del cielo
        glm::vec3 nightSky(0.025f, 0.045f, 0.14f);
        glm::vec3 daySky(0.4f, 0.7f, 1.0f);

        glm::vec3 skyColor = nightSky * night + daySky * daylight;

        glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();

        glm::mat4 view = camera.GetViewMatrix();

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Posiciones de las luces
        glUniform3fv(glGetUniformLocation(lightingShader.Program, "light.position"), 1, glm::value_ptr(sunPosition));
        glUniform3fv(glGetUniformLocation(lightingShader.Program, "light2.position"), 1, glm::value_ptr(moonPosition));

        // Camera position
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Luz cálida del Sol
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"),
            0.28f * daylight, 0.22f * daylight, 0.12f * daylight);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"),
            1.00f * daylight, 0.90f * daylight, 0.65f * daylight);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"),
            1.00f * daylight, 0.90f * daylight, 0.75f * daylight);

        // Luz azul de la Luna
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"),
            0.12f * night, 0.15f * night, 0.24f * night);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"),
            0.35f * night, 0.45f * night, 0.85f * night);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"),
            0.45f * night, 0.55f * night, 1.00f * night);

        // Material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.4f, 0.4f, 0.4f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // Pasto
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

        // Árbol1
        glm::mat4 modelTree1(1.0f);

        modelTree1 = glm::translate(modelTree1, glm::vec3(5.0f, 4.0f, 0.0f));
        modelTree1 = glm::scale(modelTree1, glm::vec3(1.0f, 3.0f, 1.0f));

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTree1));
        tree1.Draw(lightingShader);

        // Árbol2
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

        // Dibujar las fuentes visibles
        lampShader.Use();

        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Sol
        glm::mat4 modelSun(1.0f);
        modelSun = glm::translate(modelSun, sunPosition);
        modelSun = glm::scale(modelSun, glm::vec3(1.2f));

        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSun));
        glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 1.0f, 0.55f, 0.12f);
        glUniform1i(glGetUniformLocation(lampShader.Program, "useTexture"), GL_FALSE);

        sun.Draw(lampShader);

        // Luna
        glm::mat4 modelMoon(1.0f);
        modelMoon = glm::translate(modelMoon, moonPosition);
        modelMoon = glm::scale(modelMoon, glm::vec3(0.9f));

        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMoon));
        glUniform1i(glGetUniformLocation(lampShader.Program, "useTexture"), GL_TRUE);

        moon.Draw(lampShader);

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
}


void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos;

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
