/*	Práctica 12
	02-noviembre-2026
	Valenzuela Franco Iram Israel
	317313143 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <cstdio>

	// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
float rotRoll = 0.0f;   // Giro del cuerpo sobre su eje largo (para hacerse el muerto)
int dogAnim = 0;

// Variables separadas para cada extremidad 
float head = 0.0f;
float rotTail = 0.0f;
float rotF_LeftLeg = 0.0f;
float rotF_RightLeg = 0.0f;
float rotB_LeftLeg = 0.0f;
float rotB_RightLeg = 0.0f;



//KeyFrames
float dogPosX, dogPosY, dogPosZ;

#define MAX_FRAMES 20 
int i_max_steps = 400;
int i_curr_steps = 0;
int stepsNext = 400;   // Duración (pasos) que se guardará en el siguiente keyframe

const char* ANIM_FILE = "animacion.txt";

typedef struct _frame {


	float rotDog;
	float rotDogInc;
	float rotRoll;
	float rotRollInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;

	// Cabeza
	float head;
	float headInc;

	// Variables para interpolación de extremidades
	float rotTail, rotTailInc;
	float rotF_LeftLeg, rotF_LeftLegInc;
	float rotF_RightLeg, rotF_RightLegInc;
	float rotB_LeftLeg, rotB_LeftLegInc;
	float rotB_RightLeg, rotB_RightLegInc;

	// Pasos que tarda en llegar a ESTE keyframe desde el anterior
	int steps;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	if (FrameIndex >= MAX_FRAMES) return;

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].rotRoll = rotRoll;
	KeyFrame[FrameIndex].head = head;

	// Guardar las extremidades
	KeyFrame[FrameIndex].rotTail = rotTail;
	KeyFrame[FrameIndex].rotF_LeftLeg = rotF_LeftLeg;
	KeyFrame[FrameIndex].rotF_RightLeg = rotF_RightLeg;
	KeyFrame[FrameIndex].rotB_LeftLeg = rotB_LeftLeg;
	KeyFrame[FrameIndex].rotB_RightLeg = rotB_RightLeg;

	KeyFrame[FrameIndex].steps = stepsNext;

	printf("Keyframe %d guardado | pasos=%d | PosY=%.3f roll=%.1f head=%.1f cola=%.1f FL=%.1f FR=%.1f BL=%.1f BR=%.1f\n",
		FrameIndex, stepsNext, dogPosY, rotRoll, head, rotTail,
		rotF_LeftLeg, rotF_RightLeg, rotB_LeftLeg, rotB_RightLeg);

	FrameIndex++;
}

// Pone al perro exactamente en la pose del keyframe i
void applyFrame(int i)
{
	dogPosX = KeyFrame[i].dogPosX;
	dogPosY = KeyFrame[i].dogPosY;
	dogPosZ = KeyFrame[i].dogPosZ;

	rotDog = KeyFrame[i].rotDog;
	rotRoll = KeyFrame[i].rotRoll;
	head = KeyFrame[i].head;

	rotTail = KeyFrame[i].rotTail;
	rotF_LeftLeg = KeyFrame[i].rotF_LeftLeg;
	rotF_RightLeg = KeyFrame[i].rotF_RightLeg;
	rotB_LeftLeg = KeyFrame[i].rotB_LeftLeg;
	rotB_RightLeg = KeyFrame[i].rotB_RightLeg;
}

void resetElements(void)
{
	applyFrame(0);
}

void interpolation(void)
{
	int a = playIndex;
	int b = playIndex + 1;

	// Los pasos de este tramo son los guardados en el keyframe destino
	i_max_steps = (KeyFrame[b].steps > 0) ? KeyFrame[b].steps : 400;

	KeyFrame[a].incX = (KeyFrame[b].dogPosX - KeyFrame[a].dogPosX) / i_max_steps;
	KeyFrame[a].incY = (KeyFrame[b].dogPosY - KeyFrame[a].dogPosY) / i_max_steps;
	KeyFrame[a].incZ = (KeyFrame[b].dogPosZ - KeyFrame[a].dogPosZ) / i_max_steps;

	KeyFrame[a].rotDogInc = (KeyFrame[b].rotDog - KeyFrame[a].rotDog) / i_max_steps;
	KeyFrame[a].rotRollInc = (KeyFrame[b].rotRoll - KeyFrame[a].rotRoll) / i_max_steps;
	KeyFrame[a].headInc = (KeyFrame[b].head - KeyFrame[a].head) / i_max_steps;

	// Interpolación para las extremidades
	KeyFrame[a].rotTailInc = (KeyFrame[b].rotTail - KeyFrame[a].rotTail) / i_max_steps;
	KeyFrame[a].rotF_LeftLegInc = (KeyFrame[b].rotF_LeftLeg - KeyFrame[a].rotF_LeftLeg) / i_max_steps;
	KeyFrame[a].rotF_RightLegInc = (KeyFrame[b].rotF_RightLeg - KeyFrame[a].rotF_RightLeg) / i_max_steps;
	KeyFrame[a].rotB_LeftLegInc = (KeyFrame[b].rotB_LeftLeg - KeyFrame[a].rotB_LeftLeg) / i_max_steps;
	KeyFrame[a].rotB_RightLegInc = (KeyFrame[b].rotB_RightLeg - KeyFrame[a].rotB_RightLeg) / i_max_steps;
}

// ---------- Guardar / cargar animación en archivo ----------
void saveToFile(void)
{
	FILE* f = fopen(ANIM_FILE, "w");
	if (!f)
	{
		printf("No se pudo crear %s\n", ANIM_FILE);
		return;
	}
	fprintf(f, "%d\n", FrameIndex);
	for (int i = 0; i < FrameIndex; i++)
	{
		fprintf(f, "%f %f %f %f %f %f %f %f %f %f %f %d\n",
			KeyFrame[i].dogPosX, KeyFrame[i].dogPosY, KeyFrame[i].dogPosZ,
			KeyFrame[i].rotDog, KeyFrame[i].rotRoll, KeyFrame[i].head,
			KeyFrame[i].rotTail,
			KeyFrame[i].rotF_LeftLeg, KeyFrame[i].rotF_RightLeg,
			KeyFrame[i].rotB_LeftLeg, KeyFrame[i].rotB_RightLeg,
			KeyFrame[i].steps);
	}
	fclose(f);
	printf("Animacion guardada en %s (%d keyframes)\n", ANIM_FILE, FrameIndex);
}

bool loadFromFile(void)
{
	FILE* f = fopen(ANIM_FILE, "r");
	if (!f)
	{
		printf("No hay %s todavia, empieza a grabar keyframes con K\n", ANIM_FILE);
		return false;
	}
	int n = 0;
	if (fscanf(f, "%d", &n) != 1 || n <= 0 || n > MAX_FRAMES)
	{
		fclose(f);
		return false;
	}
	for (int i = 0; i < n; i++)
	{
		int r = fscanf(f, "%f %f %f %f %f %f %f %f %f %f %f %d",
			&KeyFrame[i].dogPosX, &KeyFrame[i].dogPosY, &KeyFrame[i].dogPosZ,
			&KeyFrame[i].rotDog, &KeyFrame[i].rotRoll, &KeyFrame[i].head,
			&KeyFrame[i].rotTail,
			&KeyFrame[i].rotF_LeftLeg, &KeyFrame[i].rotF_RightLeg,
			&KeyFrame[i].rotB_LeftLeg, &KeyFrame[i].rotB_RightLeg,
			&KeyFrame[i].steps);
		if (r != 12)
		{
			fclose(f);
			return false;
		}
	}
	fclose(f);
	FrameIndex = n;
	printf("Animacion cargada de %s (%d keyframes)\n", ANIM_FILE, n);
	return true;
}



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica12_Iram_Valenzuela", nullptr, nullptr);

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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");


	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");


	//KeyFrames Initialization
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0; KeyFrame[i].dogPosY = 0; KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0; KeyFrame[i].incY = 0; KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0; KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].rotRoll = 0; KeyFrame[i].rotRollInc = 0;
		KeyFrame[i].head = 0; KeyFrame[i].headInc = 0;

		KeyFrame[i].rotTail = 0; KeyFrame[i].rotTailInc = 0;
		KeyFrame[i].rotF_LeftLeg = 0; KeyFrame[i].rotF_LeftLegInc = 0;
		KeyFrame[i].rotF_RightLeg = 0; KeyFrame[i].rotF_RightLegInc = 0;
		KeyFrame[i].rotB_LeftLeg = 0; KeyFrame[i].rotB_LeftLegInc = 0;
		KeyFrame[i].rotB_RightLeg = 0; KeyFrame[i].rotB_RightLegInc = 0;
		KeyFrame[i].steps = 400;
	}

	// ---- Cargar animación guardada y reproducirla automáticamente ----
	if (loadFromFile() && FrameIndex > 1)
	{
		resetElements();
		playIndex = 0;
		i_curr_steps = 0;
		interpolation();
		play = true;
	}


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		//Body
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::rotate(model, glm::radians(rotRoll), glm::vec3(0.0f, 0.0f, 1.0f)); // Giro para hacerse el muerto
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);

		//Head
		glm::mat4 modelHead = glm::translate(modelTemp, glm::vec3(0.0f, 0.093f, 0.208f));
		modelHead = glm::rotate(modelHead, glm::radians(head), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHead));
		HeadDog.Draw(lightingShader);

		//Tail (Usa rotTail)
		glm::mat4 modelTail = glm::translate(modelTemp, glm::vec3(0.0f, 0.026f, -0.288f));
		modelTail = glm::rotate(modelTail, glm::radians(rotTail), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTail));
		DogTail.Draw(lightingShader);

		//Front Left Leg
		glm::mat4 modelFL = glm::translate(modelTemp, glm::vec3(0.112f, -0.044f, 0.074f));
		modelFL = glm::rotate(modelFL, glm::radians(rotF_LeftLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFL));
		F_LeftLeg.Draw(lightingShader);

		//Front Right Leg
		glm::mat4 modelFR = glm::translate(modelTemp, glm::vec3(-0.111f, -0.055f, 0.074f));
		modelFR = glm::rotate(modelFR, glm::radians(rotF_RightLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFR));
		F_RightLeg.Draw(lightingShader);

		//Back Left Leg
		glm::mat4 modelBL = glm::translate(modelTemp, glm::vec3(0.082f, -0.046, -0.218));
		modelBL = glm::rotate(modelBL, glm::radians(rotB_LeftLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBL));
		B_LeftLeg.Draw(lightingShader);

		//Back Right Leg
		glm::mat4 modelBR = glm::translate(modelTemp, glm::vec3(-0.083f, -0.057f, -0.231f));
		modelBR = glm::rotate(modelBR, glm::radians(rotB_RightLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBR));
		B_RightLeg.Draw(lightingShader);


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	//Dog Controls

	// Cola
	if (keys[GLFW_KEY_O]) rotTail += 0.5f;
	if (keys[GLFW_KEY_P]) rotTail -= 0.5f;
	// Pata Delantera Izquierda
	if (keys[GLFW_KEY_Z]) rotF_LeftLeg += 0.5f;
	if (keys[GLFW_KEY_X]) rotF_LeftLeg -= 0.5f;
	// Pata Delantera Derecha
	if (keys[GLFW_KEY_C]) rotF_RightLeg += 0.5f;
	if (keys[GLFW_KEY_V]) rotF_RightLeg -= 0.5f;
	// Pata Trasera Izquierda
	if (keys[GLFW_KEY_B]) rotB_LeftLeg += 0.5f;
	if (keys[GLFW_KEY_N]) rotB_LeftLeg -= 0.5f;
	// Pata Trasera Derecha
	if (keys[GLFW_KEY_M]) rotB_RightLeg += 0.5f;
	if (keys[GLFW_KEY_R]) rotB_RightLeg -= 0.5f;

	// Cabeza y Cuerpo 
	if (keys[GLFW_KEY_4]) head += 0.5f;
	if (keys[GLFW_KEY_5]) head -= 0.5f;
	if (keys[GLFW_KEY_2]) rotDog += 0.5f;
	if (keys[GLFW_KEY_3]) rotDog -= 0.5f;

	// Giro del cuerpo (hacerse el muerto): 1 grado por frame
	if (keys[GLFW_KEY_6]) rotRoll += 1.0f;
	if (keys[GLFW_KEY_7]) rotRoll -= 1.0f;

	if (keys[GLFW_KEY_H]) dogPosZ += 0.005f;
	if (keys[GLFW_KEY_Y]) dogPosZ -= 0.005f;
	if (keys[GLFW_KEY_G]) dogPosX -= 0.005f;
	if (keys[GLFW_KEY_J]) dogPosX += 0.005f;

	// Teclas para mover el cuerpo verticalmente
	if (keys[GLFW_KEY_Q]) dogPosY += 0.005f; // Arriba
	if (keys[GLFW_KEY_E]) dogPosY -= 0.005f; // Abajo


	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Primero se actualiza el arreglo de teclas
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

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Las siguientes acciones ocurren UNA sola vez por pulsación
	if (action != GLFW_PRESS) return;

	// L: reproducir / detener
	if (key == GLFW_KEY_L)
	{
		if (play == false && (FrameIndex > 1))
		{
			resetElements();
			playIndex = 0;
			i_curr_steps = 0;
			interpolation();
			play = true;
		}
		else
		{
			play = false;
		}
	}

	// K: guardar keyframe
	if (key == GLFW_KEY_K)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
		else
		{
			printf("Maximo de keyframes alcanzado\n");
		}
	}

	// F: guardar animación en archivo
	if (key == GLFW_KEY_F)
	{
		saveToFile();
	}

	// U: deshacer último keyframe
	if (key == GLFW_KEY_U)
	{
		if (FrameIndex > 0)
		{
			FrameIndex--;
			printf("Keyframe %d eliminado. Quedan %d\n", FrameIndex, FrameIndex);
		}
	}

	// T: borrar todos los keyframes de memoria (el archivo no se toca hasta que presiones F)
	if (key == GLFW_KEY_T)
	{
		FrameIndex = 0;
		play = false;
		printf("Todos los keyframes borrados de memoria\n");
	}

	// 9 / 0: duración del siguiente keyframe (más pasos = más lento)
	if (key == GLFW_KEY_9)
	{
		stepsNext += 100;
		if (stepsNext > 2000) stepsNext = 2000;
		printf("Pasos para el siguiente keyframe: %d (mas lento)\n", stepsNext);
	}
	if (key == GLFW_KEY_0)
	{
		stepsNext -= 100;
		if (stepsNext < 100) stepsNext = 100;
		printf("Pasos para el siguiente keyframe: %d (mas rapido)\n", stepsNext);
	}

	// ESPACIO: luz
	if (key == GLFW_KEY_SPACE)
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}


}
void Animation() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			// Ajuste exacto al keyframe destino (evita errores acumulados)
			applyFrame(playIndex + 1);

			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;

			rotDog += KeyFrame[playIndex].rotDogInc;
			rotRoll += KeyFrame[playIndex].rotRollInc;
			head += KeyFrame[playIndex].headInc;

			rotTail += KeyFrame[playIndex].rotTailInc;
			rotF_LeftLeg += KeyFrame[playIndex].rotF_LeftLegInc;
			rotF_RightLeg += KeyFrame[playIndex].rotF_RightLegInc;
			rotB_LeftLeg += KeyFrame[playIndex].rotB_LeftLegInc;
			rotB_RightLeg += KeyFrame[playIndex].rotB_RightLegInc;

			i_curr_steps++;
		}

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