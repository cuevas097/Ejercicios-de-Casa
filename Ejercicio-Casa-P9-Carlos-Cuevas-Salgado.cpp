/*---------------------------------------------------------*/
/* ----------------   Pr�ctica 9 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Alumno: Carlos Cuevas Salgado
	Grupo de laboratorio : 03
	Grupo de Teoria : 01
	Fecha de entrega 08 de Abril de 2019
	Version de Visual 2017
	Objetivo: Colocar diferentes texturas en un cubo
	Instrucciones : Para moverse atraves de la pnatalla se utilizaran las siguiente teclas :
		   Moverse en el eje X : A - Izquierda, D - Derecha
		   Moverse en el eje Y : Q - Arriba, Z - Abajo

		   Para cambiar el color de la luz se usan las teclas:
				   1 para aumentar rojo
				   2 para aumentar verde
				   3 para aumentar azul
				   4 para disminuir rojo
				   5 para disminuir verde
				   6 para disminuir azul
           Para la intensidad 
		           I para aumentar intensidad 
				   O para disminuir intensidad
           Para el cambio de luz se usan
		           B Para luz direccional
				   N Para luz posicional
				   M Para luz de linterna


		   El mouse tambien genera que se cambie de posicion la camara*/
#define STB_IMAGE_IMPLEMENTATION
#include "esfera.h"
#include "camera.h"
#include "stb_image.h"
#include "string.h"



Esfera my_sphere(1.0f);

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;
GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 3.0f, 0.0f);
glm::vec3 lightDirection(0.0f, 0.0f, -3.0f);

void myData(void);
void display(void);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f,
	    intensidad = 1.0f,
		colorX = 0.2f,
		colorY = 0.2f,
		colorZ = 0.2f,
		shader = 1.0f;

//Texture
unsigned int	t_smile,
				t_toalla,
				t_unam,
				t_white,
				t_panda,
				t_cubo,
				t_caja,
				t_caja_brillo;
char
shadervs[37] = "shaders/shader_texture_light_dir.vs",
shaderfs[37] = "shaders/shader_texture_light_dir.fs";

unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{

	t_smile = generateTextures("Texturas/awesomeface.png", 1);
	t_toalla = generateTextures("Texturas/toalla.tga", 0);
	t_unam = generateTextures("Texturas/Central.png", 1);
	t_white = generateTextures("Texturas/white.jpg", 0);
	t_panda = generateTextures("Texturas/Panda_01.png", 1);
	t_cubo = generateTextures("Texturas/Cemento-002.jpg", 0);
	t_caja = generateTextures("Texturas/caja.png", 1);
	t_caja_brillo = generateTextures("Texturas/caja_specular.png", 1);
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t_smile);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, t_toalla);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, t_unam);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, t_white);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, t_panda);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, t_cubo);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, t_caja);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, t_caja_brillo);
}

void myData()
{	
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,     0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.36203f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.36203f, 0.83945f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,     0.83945f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.50127f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.85822f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.85822f,  0.83945f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.50127f,  0.83945f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.36203f,  0.83945f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.50127f,  0.83945f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.50127f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.36203f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.85822f,  0.83945f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.99747f,  0.83945f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.99747f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.85822f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,


		//////Segunda caja

		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.12532f, 0.83945f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.23924f, 0.83945f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.23924f, 0.99541f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.12532f, 0.99541f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.62025f,  0.83945f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.73418f,  0.83945f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.73418f,  0.99541f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.62025f,  0.99541f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.45190f,  0.99541f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.49113f,  0.99541f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.49113f,  0.83945f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.45190f,  0.83945f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.86456f,  0.99541f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.90633f,  0.99541f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.90633f,  0.83945f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.86456f,  0.83945f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,

		/*//Cara de atr�s
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,		0.0f,  0.0f, -1.0f,//Bottom Left
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,		0.0f,  0.0f, -1.0f,//Bottom Right
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		0.0f,  0.0f, -1.0f,//Top Right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		0.0f,  0.0f, -1.0f,//Top Left

		//Cara de enfrente
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		0.0f,  0.0f, 1.0f,//Bottom Left
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,		0.0f,  0.0f, 1.0f,//Bottom Right
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,		0.0f,  0.0f, 1.0f,//Top Right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,		0.0f,  0.0f, 1.0f,//Top Left

		//Cara de la izquierda
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,		-1.0f,  0.0f, 0.0f,//Bottom Left
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,		-1.0f,  0.0f, 0.0f,//Bottom Right
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,		-1.0f,  0.0f, 0.0f,//Top Right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		-1.0f,  0.0f, 0.0f,//Top Left

		//Cara de la derecha
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		1.0f,  0.0f, 0.0f,//Bottom Left
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,		1.0f,  0.0f, 0.0f,//Bottom Right
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		1.0f,  0.0f, 0.0f,//Top Right
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f,		1.0f,  0.0f, 0.0f,//Top Left

		//Cara de abajo
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,		0.0f,  -1.0f, 0.0f,//Bottom Left
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,		0.0f,  -1.0f, 0.0f,//Bottom Right
		0.5f, -0.5f,  0.5f,  1.0f,  1.0f,		0.0f,  -1.0f, 0.0f,//Top Right
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,		0.0f,  -1.0f, 0.0f,//Top Left

		//Cara de arriba
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,		0.0f,  1.0f, 0.0f,//Bottom Left
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,		0.0f,  1.0f, 0.0f,//Bottom Right
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		0.0f,  1.0f, 0.0f,//Top Right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		0.0f,  1.0f, 0.0f,//Top Left*/
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void animate(void)
{

}

void display(void)
{
	//Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs");
	//Shader projectionShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");

	Shader lightingShader(shadervs, shaderfs); 
	Shader lampShader("shaders/shader_lamp.vs", "shaders/shader_lamp.fs");

	//To Use Lighting
	lightingShader.use();

	//If the light is Directional, we send the direction of the light
	if(shader==1.0f)
		lightingShader.setVec3("light.direction", lightDirection);	

	//If the light is Positional, we send the position of the light
	if (shader == 2.0f)
		lightingShader.setVec3("light.position", lightPosition);

	//If the light is Spotlight, we put the light in the camera
	if (shader == 3.0f)
		lightingShader.setVec3("light.position", camera.Position);
	

	lightingShader.setVec3("light.direction", camera.Front);
	lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));

	lightingShader.setVec3("viewPos", camera.Position);
	
	//Modificar la intensidad de la luz
	lightingShader.setInt("intensidad", intensidad);

	// light properties
	lightingShader.setVec3("light.ambient", colorX, colorY, colorZ);//Color de la luz
	lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//For Positional and Spotlight
	lightingShader.setFloat("light.constant", 1.0f);
	lightingShader.setFloat("light.linear", 0.09f);
	lightingShader.setFloat("light.quadratic", 0.032f);

	// material properties
	lightingShader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	//lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setMat4("model", model);
	lightingShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	lightingShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Biblioteca
	
	lightingShader.setVec3("ambientColor", 0.0f, 0.0f, 0.0f);
	lightingShader.setVec3("diffuseColor", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("specularColor", 1.0f, 0.0f, 0.0f);
	model = glm::scale(model, glm::vec3(3.0f, 2.0f, 2.0f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_unam);
	glDrawArrays(GL_QUADS, 0, 16);
	lightingShader.setInt("material_diffuse", t_cubo);
	glDrawArrays(GL_QUADS, 16, 8);
	
	//Parte superior de la biblioteca
	model = glm::mat4(1.0f);
	lightingShader.setVec3("ambientColor", 0.0f, 0.0f, 0.0f);
	lightingShader.setVec3("diffuseColor", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("specularColor", 1.0f, 0.0f, 0.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.145f, 0.85f));
	model = glm::scale(model, glm::vec3(0.70f, 0.3f, 0.3f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_unam);
	glDrawArrays(GL_QUADS, 24, 16);
	lightingShader.setInt("material_diffuse", t_white);
	glDrawArrays(GL_QUADS, 40, 4);
	

	//Light
	lampShader.use();
	lampShader.setMat4("projection", projection);
	lampShader.setMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPosition);
	model = glm::scale(model, glm::vec3(0.25f));
	lampShader.setMat4("model", model);

	//glBindVertexArray(lightVAO);
	glDrawArrays(GL_QUADS, 0, 24);	//Light

	
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 9", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	my_sphere.init();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        // -----
        my_input(window);
		animate();

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi funci�n de dibujo
		display();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//intensidad
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		if (intensidad == 2.0f) { 
			intensidad = 2.0f; 
		}
		else {
			intensidad += 0.1f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		if (intensidad == 0.0f) {
			intensidad = 0.0f;
		}
		else {
			intensidad -= 0.1f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) { 
		if (colorX >= 1.0f) {
			colorX = 1.0f;
		}
		else
			colorX += 0.01f;
	
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		if (colorY >= 1.0f) {
			colorY = 1.0f;
		}
		else
			colorY += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		if (colorZ >= 1.0f) {
			colorZ = 1.0f;
		}
		else
			colorZ += 0.01f;		
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) { 
		if (colorX <= 0.0f) {
			colorX = 0.0f;
		}
		else
			colorX -= 0.01f;

	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		if (colorY <= 0.0f) {
			colorY = 0.0f;
		}
		else
			colorY -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		if (colorZ <= 0.0f) {
			colorZ = 0.0f;
		}
		else
			colorZ -= 0.01f;
	}
	//Cambio de Shader
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		shader = 1.0f;
		strcpy_s(shadervs, "shaders/shader_texture_light_dir.vs");
		strcpy_s(shaderfs, "shaders/shader_texture_light_dir.fs");
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		shader = 2.0f;
		strcpy_s(shadervs, "shaders/shader_texture_light_pos.vs");
		strcpy_s(shaderfs, "shaders/shader_texture_light_pos.fs");

	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		shader = 3.0f;
		strcpy_s(shadervs, "shaders/shader_texture_light_spot.vs");
		strcpy_s(shaderfs, "shaders/shader_texture_light_spot.fs");

	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}