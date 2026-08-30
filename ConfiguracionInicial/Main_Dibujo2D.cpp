//Practica2
//Sánchez Luján César Ricardo
//Fecha de entrega 30 de agosto 2026
//Número de cuenta: 321195414


#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica2_Cesar Sanchez", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
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

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		// BASE DE LA PATA DEL CABALLETE
		0.62f,  -0.8f, 0.0f,    1.0f,1.0f,0.15f,  // 0 
		0.6f, -0.8f, 0.0f,    1.0f,1.0f,0.15f,  // 1
		0.62f, -0.82f, 0.0f,   1.0f,1.0f,0.15f,  // 2
		0.6f,  -0.82f, 0.0f,   1.0f,1.0f,0.15f, // 3
		// PATA DEL CABALLETE
		0.605f,  -0.79f, 0.0f,    0.4f, 0.3f, 0.2f, // 4
		0.63f,  -0.79f, 0.0f,    0.4f, 0.3f, 0.2f, // 5
		0.65f,  -0.2f, 0.0f,    0.4f, 0.3f, 0.2f, // 6
		0.61f,  -0.2f, 0.0f,    0.4, 0.3f, 0.2f, // 7
		// BASE DEL LIENZO
		0.65f,  -0.2f, 0.0f,    1.0f, 0.0f, 0.0f, // 8 
		0.58f,  -0.2f, 0.0f,    1.0f, 0.0f, 0.0f, // 9
		0.65f,  -0.13f, 0.0f,    1.0f, 0.0f, 0.0f, // 10
		0.58f,  -0.12f, 0.0f,    1.0f, 0.0f, 0.0f, // 11
		// LIENZO
		0.65f,  -0.13f, 0.0f,    0.3f, 0.3f, 0.2f, // 12
		0.60f,  -0.12f, 0.0f,    0.3f, 0.3f, 0.2f, // 13
		0.70f,  0.5f, 0.0f,    0.3f, 0.3f, 0.2f, // 14
		0.65f,  0.52f, 0.0f,    0.3f, 0.3f, 0.2f, // 15
		// PARTE DE ARRIBA DEL CABALLETE
		0.62f,  0.5f, 0.0f,    1.0f, 0.6f, 0.0f, // 16
		0.64f,  0.48f, 0.0f,    1.0f, 0.6f, 0.0f, // 17
		0.68f,  0.57f, 0.0f,    1.0f, 0.6f, 0.0f, // 18
		0.65f,  0.61f, 0.0f,    1.0f, 0.6f, 0.0f, // 19
		// PARTE ROSA DEL CABALLETE
		0.65f, 0.05f, 0.0f,    1.0f, 0.2f, 0.3f, // 20
		0.7f, 0.03f, 0.0f,    1.0f, 0.2f, 0.3f, // 21
		0.68f, -0.08f, 0.0f,    1.0f, 0.2f, 0.3f, // 22
		0.63f, -0.12f, 0.0f,    1.0f, 0.2f, 0.3f, // 23
		// SEGUNDO PIE DEL CABALLETE
		0.66f, -0.1f, 0.0f,    1.0f, 0.8f, 0.0f, // 24
		0.69f, -0.05f, 0.0f,    1.0f, 0.8f, 0.0f, // 25
		0.85f, -0.83f, 0.0f,    1.0f, 0.8f, 0.0f,  // 26
		0.81f, -0.83f, 0.0f,    1.0f, 0.8f, 0.0f, // 27

		//  TRIANGULO ROSA DEL PAJARITO
		-0.82f, -0.28f, 0.0f,   1.0f, 0.2f, 0.5f, // 28
		-0.54f, -0.25f, 0.0f,   1.0f, 0.2f, 0.5f, // 29 
 		-0.70f, -0.48f, 0.0f,   1.0f, 0.2f, 0.5f, // 30
		// CABEZA Y CUERPO DEL PAJARITO 1
		-0.82f, -0.28f, 0.0f,	1.0f, 0.8f, 0.0f, // 31
		-0.74f, -0.15f, 0.0f,	1.0f, 0.8f, 0.0f, // 32
		-0.54f, -0.25f, 0.0f,	1.0f, 0.8f, 0.0f, // 33
		// CABEZA Y CUERPO DEL PAJARITO 2
		-0.82f, -0.28f, 0.0f,	1.0f, 0.8f, 0.0f, // 34
		-0.70f, -0.48f, 0.0f,	1.0f, 0.8f, 0.0f, // 35
		-0.80f, -0.45f, 0.0f,	1.0f, 0.8f, 0.0f, // 36
		// CABEZA Y CUERPO DEL PAJARITO 3
		-0.54f, -0.25f, 0.0f,	1.0f, 0.8f, 0.0f, // 37
		-0.70f, -0.48f, 0.0f,	1.0f, 0.8f, 0.0f, // 38
		-0.52f, -0.30f, 0.0f,	1.0f, 0.8f, 0.0f, // 39
		// CABEZA Y CUERPO DEL PAJARITO 4
		-0.72f, -0.75f, 0.0f,	1.0f, 0.8f, 0.0f, // 40
		-0.70f, -0.48f, 0.0f,	1.0f, 0.8f, 0.0f, // 41
		-0.60f, -0.75f, 0.0f,	1.0f, 0.8f, 0.0f, // 42
		-0.65f, -0.43f, 0.0f,	1.0f, 0.8f, 0.0f, // 43
		// CABEZA Y CUERPO DEL PAJARITO 5
		-0.72f, -0.75f, 0.0f,	1.0f, 0.8f, 0.0f, // 44
		-0.70f, -0.82f, 0.0f,	1.0f, 0.8f, 0.0f, // 45
		-0.60f, -0.75f, 0.0f,	1.0f, 0.8f, 0.0f, // 46
		// CABEZA Y CUERPO DEL PAJARITO 6
		-0.75f, -0.82f, 0.0f,	1.0f, 0.8f, 0.0f, // 47
		-0.55f, -0.82f, 0.0f,	1.0f, 0.8f, 0.0f, // 48
		-0.78f, -0.86f, 0.0f,	1.0f, 0.8f, 0.0f, // 49
		-0.57f, -0.86f, 0.0f,	1.0f, 0.8f, 0.0f, // 50
		// COLA VERDE DEL PAJARITO
		-0.72f, -0.75f, 0.0f,	0.1f, 1.0f, 0.3f, // 51
		-0.87f, -0.72f, 0.0f,	0.1f, 1.0f, 0.3f, // 52
		-0.87f, -0.80f, 0.0f,	0.1f, 1.0f, 0.3f, // 53
		// OJO DEL PAJARITO
		-0.68f, -0.33f, 0.0f,   0.0f, 0.0f, 0.0f, // 54
		-0.68f, -0.29f, 0.0f,   0.0f, 0.0f, 0.0f, // 55 
		-0.65f, -0.31f, 0.0f,   0.0f, 0.0f, 0.0f, // 56

		// SOMBRERO SNOOPY 1
		-0.45f, 0.85f, 0.0f,	1.0f, 0.2f, 0.1f, // 57
		-0.2f, 0.85f, 0.0f,		1.0f, 0.2f, 0.1f, // 58
		-0.51f, 0.64f, 0.0f,	1.0f, 0.2f, 0.1f, // 59
		-0.47f, 0.46f, 0.0f,	1.0f, 0.2f, 0.1f, // 60
		// SOMBRERO SNOOPY 2
		-0.2f, 0.85f, 0.0f,		0.1f, 1.0f, 0.2f, // 61
		-0.47f, 0.46f, 0.0f,	0.1f, 1.0f, 0.2f, // 62
		-0.12f, 0.64f, 0.0f,	0.1f, 1.0f, 0.2f, // 63
		// SOMBRERO SNOOPY 3
		-0.47f, 0.46f, 0.0f,	0.0f, 0.0f, 0.0f, // 64
		-0.12f, 0.64f, 0.0f,	0.0f, 0.0f, 0.0f, // 65
		-0.45f, 0.42f, 0.0f,	0.0f, 0.0f, 0.0f, // 66
		-0.1f, 0.60f, 0.0f,		0.0f, 0.0f, 0.0f, // 67
		// SOMBRERO SNOOPY 4
		-0.45f, 0.42f, 0.0f,	1.0f, 0.0f, 0.0f, // 68
		-0.63f, 0.21f, 0.0f,	1.0f, 0.0f, 0.0f, // 69
		-0.43f, 0.38f, 0.0f,	1.0f, 0.0f, 0.0f, // 70
		-0.1f, 0.60f, 0.0f,		1.0f, 0.0f, 0.0f, // 71
		-0.08f, 0.56f, 0.0f,	1.0f, 0.0f, 0.0f, // 72
		0.15f, 0.63f, 0.0f,		1.0f, 0.0f, 0.0f, // 73

		// CABEZA SNOOPY 1
		-0.43f, 0.38f, 0.0f,	0.55f, 0.8f, 1.0f, // 74
		-0.08f, 0.56f, 0.0f,	0.55f, 0.8f, 1.0f, // 75
		-0.30f, 0.30f, 0.0f,	0.55f, 0.8f, 1.0f, // 76
		// CABEZA SNOOPY 2
		-0.30f, 0.30f, 0.0f,	1.0f, 1.0f, 0.1f, // 77
		-0.08f, 0.56f, 0.0f,	1.0f, 1.0f, 0.1f, // 78
		-0.12f, -0.02f, 0.0f,	1.0f, 1.0f, 0.1f, // 79
		-0.25f, 0.1f, 0.0f,		1.0f, 1.0f, 0.1f, // 80
		-0.23f, 0.18f, 0.0f,	1.0f, 1.0f, 0.1f, // 81
		// CABEZA SNOOPY 3
		0.20f, 0.23f, 0.0f,		1.0f, 0.6f, 0.1f, // 82
		-0.08f, 0.56f, 0.0f,	1.0f, 0.6f, 0.1f, // 83
		-0.12f, -0.02f, 0.0f,	1.0f, 0.6f, 0.1f, // 84
		// CABEZA SNOOPY 4
		0.20f, 0.23f, 0.0f,		1.0f, 1.0f, 0.1f, // 85
		-0.08f, 0.56f, 0.0f,	1.0f, 1.0f, 0.1f, // 86
		0.16f, 0.45f, 0.0f,		1.0f, 1.0f, 0.1f, // 87
		0.22f, 0.36f, 0.0f,		1.0f, 1.0f, 0.1f, // 88
		//NARIZ SNOOPY 
		0.21f, 0.34f, 0.0f,		0.0f, 0.0f, 0.0f, // 89
		0.26f, 0.34f, 0.0f,		0.0f, 0.0f, 0.0f, // 90
		0.26f, 0.39f, 0.0f,		0.0f, 0.0f, 0.0f, // 91
		0.20f, 0.39f, 0.0f,		0.0f, 0.0f, 0.0f, // 92
		// OJO SNOOPY 
		-0.05f, 0.35f, 0.0f,	0.0f, 0.0f, 0.0f, // 93
		-0.01f, 0.39f, 0.0f,	0.0f, 0.0f, 0.0f, // 94
		-0.01f, 0.35f, 0.0f,	0.0f, 0.0f, 0.0f, // 95
		// OREJA SNOOPY NARANJA
		-0.43f, 0.38f, 0.0f,	1.0f, 0.65f, 0.1f, // 96
		-0.30f, 0.30f, 0.0f,	1.0f, 0.65f, 0.1f, // 97
		-0.23f, 0.18f, 0.0f,	1.0f, 0.65f, 0.1f, // 98
		-0.25f, 0.1f, 0.0f,		1.0f, 0.65f, 0.1f, // 99
		-0.46f, 0.18f, 0.0f,	1.0f, 0.65f, 0.1f, // 100
		-0.34f, 0.07f, 0.0f,	1.0f, 0.65f, 0.1f, // 101
		// OREJA SNOOPY NEGRO 
		-0.40f, 0.32f, 0.0f,	0.0f, 0.0f, 0.0f, // 102
		-0.34f, 0.10f, 0.0f,	0.0f, 0.0f, 0.0f, // 103
		-0.43f, 0.185f, 0.0f,	0.0f, 0.0f, 0.0f, // 104
		-0.28f, 0.18f, 0.0f,	0.0f, 0.0f, 0.0f, // 105

		// CUERPO SNOOPY VERDE
		-0.12f, -0.02f, 0.0f,	0.0f, 1.0f, 0.0f, // 106
		0.0f, 0.07f, 0.0f,	0.0f, 1.0f, 0.0f, // 107
		-0.14f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f, // 108
		0.08f, -0.3f, 0.0f,	0.0f, 1.0f, 0.0f, // 109
	};
	unsigned int indices[] = {  // note that we start from 0!
		//PATA DEL CABALLETE
		4,5,6,
		7,6,4,
		// BASE DEL LIENZO
		8, 9, 10,
		11,10,9,
		// LIENZO
		12,13,14,
		15,14,12,
		// PARTE DE ARRIBA DEL CABALLETE
		16,17,18,
		18,19,16,
		// PARTE ROSA DEL CABALLETE
		20,21,22,
		22,23,20,
		// SEGUNDO PIE DEL CABALLETE
		24,25,26,
		26,27,24,
		//  TRIANGULO ROSA DEL PAJARITO
		28,29,30,
		// CABEZA Y CUERPO DEL PAJARITO 1
		31,32,33,
		// CABEZA Y CUERPO DEL PAJARITO 2
		34,35,36,
		// CABEZA Y CUERPO DEL PAJARITO 3
		37,38,39,
		// CABEZA Y CUERPO DEL PAJARITO 4
		40,41,42,
		41,42,43,
		// CABEZA Y CUERPO DEL PAJARITO 5
		44,45,46,
		// CABEZA Y CUERPO DEL PAJARITO 6
		47,48,49,
		48,50,49,
		// COLA VERDE PAJARITO
		51,52,53,
		// OJO DEL PAJARITO
		54,55,56,

		// SOMBRERO SNOOPY 1
		57,58,59,
		58,59,60,
		// SOMBRERO SNOOPY 2
		61,62,63,
		//SOMBRERO SNOOPY 3
		64,65,66,
		65,66,67,
		//SOMBRERO SNOOPY 4
		68,69,70,
		71,68,70,
		70,71,72,
		73,72,71,

		//CABEZA SNOOPY 1
		74,75,76,
		//CABEZA SNOOPY 2
		77,78,79,
		81,79,80,
		//CABEZA SNOOPY 3
		82,83,84,
		//CABEZA SNOOPY 4
		85,86,87,
		85,87,88,
		//NARIZ SNOOPY
		89,90,91,
		90,91,92,
		//OJO SNOOPY
		93,94,95,
		//OREJA SNOOPY NARANJA
		96,97,100,
		97,98,100,
		98,99,100,
		99,100,101,
		//OREJA SNOOPY NEGRO
		102,103,104,
		102,103,105,
		//CUERPO SNOOPY VERDE
		106,107,108,
		107,108,109
	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.173f, 0.516f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);


        glPointSize(10);
        glDrawArrays(GL_POINTS,0,4);
        
        //glDrawArrays(GL_LINES,0,4);
        //glDrawArrays(GL_LINE_LOOP,1,3);
        
        //glDrawArrays(GL_TRIANGLES,4,3);
        glDrawElements(GL_TRIANGLES, 147,GL_UNSIGNED_INT,0);

		
		
		//glDrawArrays(GL_LINES, 8, 4);

		//glPointSize(8.0f);
		//glDrawArrays(GL_POINTS, 12, 1);

        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}