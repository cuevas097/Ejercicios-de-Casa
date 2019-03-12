/*---------------------------------------------------------*/
/* ----------------   Práctica 5 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Computación gráfica e interacción humano computadora ---------------
Autor: Carlos Cuevas Salgado
	Grupo de laboratorio : 03
	Grupo de Teoria : 01
	Fecha de entrega: 11 de Marzo de 2019
	Version de Visual 2017
	Instrucciones : Para moverse atraves de la pantalla se utilizaran las siguiente teclas :
	       Moverse en el eje X : A - Izquierda, D - Derecha
		   Moverse en el eje Y : Q - Arriba, Z - Abajo
		   Moverse en el eje Z : W - Arriba, S - Abajo

		   Rotar a la figura se ocupan las teclas de navegacion

		   Para que se muevan las brazos(los dos brazos se mueven de manera conjunta) de la figura se usan las teclas: 
		   Para mover el  hombro se ocupa        la tecla T o T + shift
		   Para mover el  antebrazo se ocupa     la tecla Y o Y + shift
		   Para movel el  mano se ocupa          la tecla U o U + shift
		   Para mover los dedos se ocupa         la tecla I o I + shift 

           */

#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(void);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f,
rotY = 0.0f,
angHom = 0.0f,
angCo = 0.0f,
angMu = 0.0f,
angPu = 0.0f;




void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{	
		GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	//glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
	//glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	glm::mat4 model_tem = glm::mat4(1.0f);
	glm::mat4 model_tem2 = glm::mat4(1.0f);
	
	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view,glm::radians(rotY),glm::vec3(0.0f,1.0f,0.0f));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Cabeza
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5, 2.5, 1));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Cuello
	projectionShader.setVec3("aColor", glm::vec3(0.75f, 0.0f, 0.0f));
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, -1.5, 0));
	model = glm::scale(model, glm::vec3(0.5, 0.5, 1));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Cuerpo
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.5f, 0.75f));
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, -4.25, 0));
	model = glm::scale(model, glm::vec3(4, 5, 1));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Cinturon
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.56f));
	model = glm::translate(model, glm::vec3(0, -.7, 0));
	model = glm::scale(model, glm::vec3(1, 0.4, 1));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Piernas
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-1.5, -10.25, 0));
	model = glm::scale(model, glm::vec3(1, 3, 1));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::translate(model, glm::vec3(3, 0, 0));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Zapatos
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.25f, 0.0f));
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-3, -12.25, 0));
	model = glm::scale(model, glm::vec3(4, 1, 1));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.25f, 0.0f));
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Hombro
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(2, -2.25, 0));
	model = glm::rotate(model, glm::radians(angHom), glm::vec3(0, 0, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 1, 0));
	model = glm::translate(model, glm::vec3(.5, 0, 0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	
	//Bisep
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model_tem = model;
	model = glm::scale(model, glm::vec3(2, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);
    model = model_tem;

	//Antebrazo
	model = glm::translate(model, glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(angCo), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(3, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 0));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;
    
	//Mano
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model = glm::rotate(model, glm::radians(angMu), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.5, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(1, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 1, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	//Pulgar
	model_tem2 = model = model_tem;
	model = glm::translate(model, glm::vec3(-0.35, 0.5, 0));
	model = glm::rotate(model, glm::radians(angPu), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .3, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;

	//Pulgar 2
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	model = glm::rotate(model, glm::radians(angPu*0.7f), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .3, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.65f, 0.0f, 0.65f));
	glDrawArrays(GL_QUADS, 0, 24);

	//indice
	model = model_tem2;
	model = glm::translate(model, glm::vec3(0.5, 0.38, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.09f, 0.45f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;

	//indice 2
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-angPu*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.65f, 0.0f, 0.65f));
	glDrawArrays(GL_QUADS, 0, 24);

	//indice 3
	model = model_tem;
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-angPu * 0.85f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.5f));
	glDrawArrays(GL_QUADS, 0, 24);


	//medio
	model = model_tem2;
	model = glm::translate(model, glm::vec3(0.5, 0.10, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.2, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.4, .25, .4));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.09f, 0.45f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;

	//medio 2
	model = glm::translate(model, glm::vec3(0.2, 0, 0));
	model = glm::rotate(model, glm::radians(-angPu * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.65f, 0.0f, 0.65f));
	glDrawArrays(GL_QUADS, 0, 24);

	//medio 3
	model = model_tem;
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-angPu * 0.85f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.5f));
	glDrawArrays(GL_QUADS, 0, 24);


	//anular
	model = model_tem2;
	model = glm::translate(model, glm::vec3(0.5, -0.18, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.09f, 0.45f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;

	//anular 2
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-angPu * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.65f, 0.0f, 0.65f));
	glDrawArrays(GL_QUADS, 0, 24);

	//anular 3
	model = model_tem;
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-angPu * 0.85f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.5f));
	glDrawArrays(GL_QUADS, 0, 24);

	//meñique
	model = model_tem2;
	model = glm::translate(model, glm::vec3(0.5, -0.45, 0));
	model = glm::rotate(model, glm::radians(-angPu), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.2, .2, .2));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.09f, 0.45f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;

	//meñique 2
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	model = glm::rotate(model, glm::radians(-angPu * 0.3f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .2, .2));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.65f, 0.0f, 0.65f));
	glDrawArrays(GL_QUADS, 0, 24);

	//meñique 3
	model = model_tem;
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(-angPu * 0.4f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .2, .2));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.5f));
	glDrawArrays(GL_QUADS, 0, 24);


	//Segundo Brazo
	//Hombro
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-2, -2.25, 0));
	model = glm::rotate(model, glm::radians(-angHom), glm::vec3(0, 0, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 1, 0));
	model = glm::translate(model, glm::vec3(-0.5, 0, 0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Bisep
	model = glm::translate(model, glm::vec3(-1.5, 0, 0));
	model_tem = model;
	model = glm::scale(model, glm::vec3(2, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;

	//Antebrazo
	model = glm::translate(model, glm::vec3(-1, 0, 0));
	model = glm::rotate(model, glm::radians(-angCo), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-1.5, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(3, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 0));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;

	//Mano
	model = glm::translate(model, glm::vec3(-1.5, 0, 0));
	model = glm::rotate(model, glm::radians(-angMu), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.5, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(1, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 1, 0));
	glDrawArrays(GL_QUADS, 0, 24);
	
	//Pulgar
	model_tem2 = model = model_tem;
	model = glm::translate(model, glm::vec3(0.35, 0.5, 0));
	model = glm::rotate(model, glm::radians(angPu), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .3, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;
	
	//Pulgar 2
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	model = glm::rotate(model, glm::radians(angPu*0.7f), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .3, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.65f, 0.0f, 0.65f));
	glDrawArrays(GL_QUADS, 0, 24);
	
	//indice
	model = model_tem2;
	model = glm::translate(model, glm::vec3(-0.5, 0.38, 0));
	model = glm::rotate(model, glm::radians(angPu), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.09f, 0.45f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;

	//indice 2
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angPu * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.65f, 0.0f, 0.65f));
	glDrawArrays(GL_QUADS, 0, 24);

	//indice 3
	model = model_tem;
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angPu * 0.85f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.5f));
	glDrawArrays(GL_QUADS, 0, 24);


	//medio
	model = model_tem2;
	model = glm::translate(model, glm::vec3(-0.5, 0.10, 0));
	model = glm::rotate(model, glm::radians(angPu), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.2, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.4, .25, .4));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.09f, 0.45f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;

	//medio 2
	model = glm::translate(model, glm::vec3(-0.2, 0, 0));
	model = glm::rotate(model, glm::radians(angPu * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.65f, 0.0f, 0.65f));
	glDrawArrays(GL_QUADS, 0, 24);

	//medio 3
	model = model_tem;
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angPu * 0.85f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.5f));
	glDrawArrays(GL_QUADS, 0, 24);


	//anular
	model = model_tem2;
	model = glm::translate(model, glm::vec3(-0.5, -0.18, 0));
	model = glm::rotate(model, glm::radians(angPu), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.09f, 0.45f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;

	//anular 2
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angPu * 0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.65f, 0.0f, 0.65f));
	glDrawArrays(GL_QUADS, 0, 24);

	//anular 3
	model = model_tem;
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angPu * 0.85f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .25, .3));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.5f));
	glDrawArrays(GL_QUADS, 0, 24);

	//meñique
	model = model_tem2;
	model = glm::translate(model, glm::vec3(-0.5, -0.45, 0));
	model = glm::rotate(model, glm::radians(angPu), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.1, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.2, .2, .2));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.09f, 0.45f));
	glDrawArrays(GL_QUADS, 0, 24);
	model = model_tem;

	//meñique 2
	model = glm::translate(model, glm::vec3(-0.1, 0, 0));
	model = glm::rotate(model, glm::radians(angPu * 0.3f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .2, .2));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.65f, 0.0f, 0.65f));
	glDrawArrays(GL_QUADS, 0, 24);

	//meñique 3
	model = model_tem;
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angPu * 0.4f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	projectionShader.setMat4("model", model);
	model_tem = model;
	model = glm::scale(model, glm::vec3(.3, .2, .2));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.5f));
	glDrawArrays(GL_QUADS, 0, 24);
	
	///////__________________________________________________________________________

	glBindVertexArray(0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 5", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color
        glClearColor(0.43f, 0.57f, 0.74f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.2f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.2f;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		movY += 0.2f;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		movY -= 0.2f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.2f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.2f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 2.18f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 2.18f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY -= 2.2f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY += 2.2f;
	if (glfwGetKey(window, GLFW_KEY_T))
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) {
			if(angHom<90)
			   angHom += 2.2f;
		}
		else {
			if(angHom>-90)
				angHom -= 2.2f;
		}

	if (glfwGetKey(window, GLFW_KEY_Y))
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) {
			if (angCo < 0)
				angCo += 2.2f;
		}
		else {
			if (angCo > -140)
				angCo -= 2.2f;
		}

	if (glfwGetKey(window, GLFW_KEY_U))
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) {
			if (angMu < 20)
				angMu += 2.2f;
		}
		else {
			if (angMu > -140)
				angMu -= 2.2f;
		}

	if (glfwGetKey(window, GLFW_KEY_I))
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) {
			if (angPu < 80)
				angPu += 2.2f;
		}
		else {
			if (angPu > 0)
				angPu -= 2.2f;
		}

	
	/*if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		if (mode == GLFW_MOD_SHIFT) {
			hombro += 2.0f;
		}
		else {
			//lowercase
			hombro -= 2.0f;
		}
	}*/

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}