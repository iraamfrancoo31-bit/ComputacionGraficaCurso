/*	- Práctca4
	- 11-septiembre-2026-viernes
	- Valenzuela Franco Iram Israel
	- 317313143*/

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;
float rotY = 0.0f;
float rotZ = 0.0f;
int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica4_Iram_Valenzuela", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificaci�n de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
	float vertices[] = {
		
		-0.5f, -0.5f, 0.5f, 0.72f, 0.48f, 0.05f,//Front
		 0.5f, -0.5f, 0.5f, 0.72f, 0.48f, 0.05f,
		 0.5f,  0.5f, 0.5f, 0.72f, 0.48f, 0.05f,
		 0.5f,  0.5f, 0.5f, 0.72f, 0.48f, 0.05f,
		-0.5f,  0.5f, 0.5f, 0.72f, 0.48f, 0.05f,
		-0.5f, -0.5f, 0.5f, 0.72f, 0.48f, 0.05f,
		
		-0.5f, -0.5f, -0.5f, 0.72f, 0.48f, 0.05f,//Back
		 0.5f, -0.5f, -0.5f, 0.72f, 0.48f, 0.05f,
		 0.5f,  0.5f, -0.5f, 0.72f, 0.48f, 0.05f,
		 0.5f,  0.5f, -0.5f, 0.72f, 0.48f, 0.05f,
		-0.5f,  0.5f, -0.5f, 0.72f, 0.48f, 0.05f,
		-0.5f, -0.5f, -0.5f, 0.72f, 0.48f, 0.05f,
		
		 0.5f, -0.5f,  0.5f,  0.82f, 0.58f,0.03f,
		 0.5f, -0.5f, -0.5f,  0.82f, 0.58f,0.03f,
		 0.5f,  0.5f, -0.5f,  0.82f, 0.58f,0.03f,
		 0.5f,  0.5f, -0.5f,  0.82f, 0.58f,0.03f,
		 0.5f,  0.5f,  0.5f,  0.82f, 0.58f,0.03f,
		 0.5f,  -0.5f, 0.5f, 0.82f, 0.58f,0.03f,
      
		-0.5f,  0.5f,  0.5f,  0.82f, 0.58f,0.03f,
		-0.5f,  0.5f, -0.5f,  0.82f, 0.58f,0.03f,
		-0.5f, -0.5f, -0.5f,  0.82f, 0.58f,0.03f,
		-0.5f, -0.5f, -0.5f,  0.82f, 0.58f,0.03f,
		-0.5f, -0.5f,  0.5f,  0.82f, 0.58f,0.03f,
		-0.5f,  0.5f,  0.5f,  0.82f, 0.58f,0.03f,
		
		-0.5f, -0.5f, -0.5f, 0.92f, 0.68f,0.01f,
		0.5f, -0.5f, -0.5f,  0.92f, 0.68f,0.01f,
		0.5f, -0.5f,  0.5f,  0.92f, 0.68f,0.01f,
		0.5f, -0.5f,  0.5f,  0.92f, 0.68f,0.01f,
		-0.5f, -0.5f,  0.5f, 0.92f, 0.68f,0.01f,
		-0.5f, -0.5f, -0.5f, 0.92f, 0.68f,0.01f,
		
		-0.5f,  0.5f, -0.5f, 0.92f, 0.68f,0.01f,
		0.5f,  0.5f, -0.5f,  0.92f, 0.68f,0.01f,
		0.5f,  0.5f,  0.5f,  0.92f, 0.68f,0.01f,
		0.5f,  0.5f,  0.5f,  0.92f, 0.68f,0.01f,
		-0.5f,  0.5f,  0.5f, 0.92f, 0.68f,0.01f,
		-0.5f,  0.5f, -0.5f, 0.92f, 0.68f,0.01f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
	

		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, glm::radians(rotY), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint colorLoc = glGetUniformLocation(ourShader.Program, "objectColor");
		GLint useObjectColorLoc = glGetUniformLocation(ourShader.Program, "useObjectColor");

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
	

		glBindVertexArray(VAO);
		//Torso
	    model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Cabeza
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f)); 
		model = glm::translate(model, glm::vec3(0.0f, 0.35f, 0.20f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//// Pata 1 izquierda perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(3.0f, -1.72f, -0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//// muslo 1 izquierdo perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.4f, 0.5f));
		model = glm::translate(model, glm::vec3(3.0f, -2.28f, -0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//// pie 1 izquierdo perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(3.0f, -5.05f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 0.42f, 0.18f, 0.05f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

		// Pata 2 derecha perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(-3.0f, -1.72f, -0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//// muslo 2 derecho perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.4f, 0.5f));
		model = glm::translate(model, glm::vec3(-3.0f, -2.28f, -0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//// pie 1 derecho perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(-3.0f, -5.05f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 0.42f, 0.18f, 0.05f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

		// Pata 3
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.2f));
		model = glm::translate(model, glm::vec3(-1.0f, -1.72f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// pie derecho perro 3
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(-1.0f, -5.05f, 4.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 0.42f, 0.18f, 0.05f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

		// Pata 4
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.2f));
		model = glm::translate(model, glm::vec3(1.0f, -1.72f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// pie izquierdo perro 4
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(1.0f, -5.05f, 4.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 0.42f, 0.18f, 0.05f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

		// Boca
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Nariz
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.35f, 0.15f, 0.15f));
		model = glm::translate(model, glm::vec3(0.0f, 1.55f, 4.55f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 0.42f, 0.18f, 0.05f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

		// Orejas
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.4f, 0.3f, 0.2f));
		model = glm::translate(model, glm::vec3(0.0f, 1.45f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 0.42f, 0.18f, 0.05f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

		// Oreja izquierda perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(6.5f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 0.42f, 0.18f, 0.05f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

		// Oreja derecha perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(-6.5f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 0.42f, 0.18f, 0.05f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

		//Ojo Derecho perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::translate(model, glm::vec3(-2.0f, 1.55f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

		//Ojo Izquierdo perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::translate(model, glm::vec3(2.0f, 1.55f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

		//iris Izquierdo perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::translate(model, glm::vec3(2.0f, 2.55f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//iris derecho perro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::translate(model, glm::vec3(-2.0f, 2.55f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Colita
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.10f, 0.45f));
		model = glm::translate(model, glm::vec3(0.0f, -6.3f, -1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(useObjectColorLoc, 1);
		glUniform3f(colorLoc, 0.42f, 0.18f, 0.05f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		 movY += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		 movY -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.002f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		 rotY += 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		 rotY -= 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		 rotZ += 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		 rotZ -= 0.02f;
 }
 


