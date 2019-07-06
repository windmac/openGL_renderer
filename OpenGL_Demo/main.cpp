#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include "Model.h"
#include <iostream>
#include "Shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

unsigned int loadTexture(const char *path);
unsigned int loadCubemap(vector<std::string> faces);
void renderQuad();
void renderSphere();
//void renderCube();
// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1600;
bool set_hdr = true;
bool hdrKeyPressed = false;
float exposure = 1.2f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//Event Call back
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//Configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CW);

	//Shader myShader1("./shaders/vertexshader/test2.vs", "./shaders/fragmentshader/test2.fs");
	//Shader myShader2("./shaders/vertexshader/test3.vs", "./shaders/fragmentshader/test3.fs");
	Shader myShader3("./shaders/vertexshader/test4.vs", "./shaders/fragmentshader/test2.fs");
	Shader basiclightsource("./shaders/vertexshader/lighted_norm.vs", "./shaders/fragmentshader/basic_color.fs");
	Shader lightedShader("./shaders/vertexshader/lighted_norm.vs", "./shaders/fragmentshader/ambien_diffuse.fs");
	Shader phongShader("./shaders/vertexshader/lighted_norm.vs", "./shaders/fragmentshader/phong.fs");
	Shader phongMatShader("./shaders/vertexshader/lighted_norm.vs", "./shaders/fragmentshader/phong_material.fs");
	Shader texmaterialshader("./shaders/vertexshader/texture_material.vs", "./shaders/fragmentshader/texture_material.fs");
	Shader multiLightMat("./shaders/vertexshader/multi_light_material.vs", "./shaders/fragmentshader/multi_light_material.fs");
	Shader multiLightMat2("./shaders/vertexshader/multi_light_material.vs", "./shaders/fragmentshader/multi_light_material.fs");
	Shader skyboxShader("./shaders/vertexshader/skybox.vs", "./shaders/fragmentshader/skybox.fs");
	Shader reflectionShader("./shaders/vertexshader/reflection.vs", "./shaders/fragmentshader/reflection.fs");
	Shader hdr("./shaders/vertexshader/hdr.vs", "./shaders/fragmentshader/hdr.fs");
	Shader pbr("./shaders/vertexshader/CT_brdf.vs", "./shaders/fragmentshader/CT_brdf.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices1[] = {
		// positions         // colors
		-1.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		 0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 

	};
	float vertices2[] = {
		// positions         
		 1.0f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 0.5f,
		 0.0f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f
	};

	float vertices3[] = {
		// positions         // colors
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,  0.5f, 0.5f, 0.5f

	};

	float vertices4[] = {
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	};

	float vertices5[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

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



	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
//	unsigned int VBO1,VBO2, VAO1,VAO2, EBO;
	unsigned int VBO[2], VAO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	//4th shape

	unsigned int VBO4, VAO4;
	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);

	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4), vertices4, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//lighted cube
	unsigned int lightVAO, VBO5;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO5);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices5), vertices5, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Textured lighted cube
	float vertices6[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};


	unsigned int VBO6, texcubeVAO;
	glGenVertexArrays(1, &texcubeVAO);
	glGenBuffers(1, &VBO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices6), vertices6, GL_STATIC_DRAW);

	glBindVertexArray(texcubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load textures 

	unsigned int diffuseMap = loadTexture("./texture/container2.png");
	unsigned int specularMap = loadTexture("./texture/container2_specular.png");



	unsigned int diffuseMap2 = loadTexture("./texture/white.jpg");
	unsigned int specularMap2 = loadTexture("./texture/white.jpg");



	//Position of point lights
	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(0.0f,  0.0f,  0.0f),
	};

	//Cube map
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<std::string> faces
	{
		"./texture/skybox/right.jpg",
		"./texture/skybox/left.jpg",
		"./texture/skybox/top.jpg",
		"./texture/skybox/bottom.jpg",
		"./texture/skybox/front.jpg",
		"./texture/skybox/back.jpg"
	};
	unsigned int skyTexture = loadCubemap(faces);

	//HDR
	unsigned int hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	// create floating point color buffer
	unsigned int colorBuffer;
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	// attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);





	// shader configuration
	// --------------------
	multiLightMat.use();
	multiLightMat.setInt("material.diffuse", 0);
	multiLightMat.setInt("material.specular", 1);

	multiLightMat2.use();
	multiLightMat2.setInt("material.diffuse", 0);
	multiLightMat2.setInt("material.specular", 1);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	reflectionShader.use();
	reflectionShader.setInt("skybox", 0);

	hdr.use();
	hdr.setInt("hdrBuffer", 0);

	pbr.use();
	pbr.setInt("albedoMap", 0);
	pbr.setInt("normalMap", 1);
	pbr.setInt("metallicMap", 2);
	pbr.setInt("roughnessMap", 3);
	pbr.setInt("aoMap", 4);

	//1st material pbr
/*	unsigned int albedo = loadTexture("./texture/pbr_metal/streaked-metal1-albedo.png");
	unsigned int normal = loadTexture("./texture/pbr_metal/streaked-metal1-normal-dx.png");
	unsigned int metallic = loadTexture("./texture/pbr_metal/streaked-metal1-metalness.png");
	unsigned int roughness = loadTexture("./texture/pbr_metal/streaked-metal1-rough.png");
	unsigned int ao = loadTexture("./texture/pbr_metal/streaked-metal1-ao.png");
*/
	//2st material pbr
	unsigned int albedo = loadTexture("./texture/rock/layered-rock1-albedo.png");
	unsigned int normal = loadTexture("./texture/rock/layered-rock1-normal-ogl.png");
	unsigned int metallic = loadTexture("./texture/rock/layered-rock1-metalic.png");
	unsigned int roughness = loadTexture("./texture/rock/layered-rock1-height.png");
	unsigned int ao = loadTexture("./texture/rock/layered-rock1-ao.png");
	//pbr.setFloat("metallic", 0.8f)

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(1);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Going to 3D

	//Load Sphere model
	Model sphere1("./Model/globe-sphere.obj");
	
	


	//Event Registration
	

	//Before render
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//HDR begin
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		



		//Model View Projection matrix
		glm::mat4 view = camera.GetViewMatrix();
		 
		glm::mat4 model = glm::mat4(1.0f);


		// update the uniform color
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		float redValue = sin(timeValue+1.57) / 2.0f + 0.5f;
		float blueValue = sin(timeValue+3.14) / 2.0f + 0.5f;
		//float alphaValue = sin(timeValue + 5) / 2.0f + 0.5f;
		//std::cout << greenValue << std::endl;

		//Lights 1
		multiLightMat.use();
		multiLightMat.setVec3("viewPos", camera.Position);
		multiLightMat.setFloat("material.shininess", 64.0f);

		glm::vec3 lightColor(glm::vec3(redValue, greenValue, blueValue));
		glm::vec3 diffuseColor = lightColor * glm::vec3(2.0f);   // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.4f); // low influence

		glm::vec3 lightPosition1(cos(timeValue) * 3, 1.5f, sin(timeValue) * 2);
		glm::vec3 lightPosition2(cos(timeValue+3.14) * 3, 1.5f, sin(timeValue+3.14) * 2);
		//Directional light
		multiLightMat.setVec3("dirLight.direction", 0.2f, 1.0f, -0.3f);
		multiLightMat.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		multiLightMat.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		multiLightMat.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// point light 1
		multiLightMat.setVec3("pointLights[0].position", lightPosition1);
		multiLightMat.setVec3("pointLights[0].ambient", ambientColor);
		multiLightMat.setVec3("pointLights[0].diffuse", diffuseColor*2.0f);
		multiLightMat.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		multiLightMat.setFloat("pointLights[0].constant", 1.0f);
		multiLightMat.setFloat("pointLights[0].linear", 0.09);
		multiLightMat.setFloat("pointLights[0].quadratic", 0.1);
		// point light 2
		multiLightMat.setVec3("pointLights[1].position", lightPosition2);
		multiLightMat.setVec3("pointLights[1].ambient", ambientColor);
		multiLightMat.setVec3("pointLights[1].diffuse", diffuseColor*2.0f);
		multiLightMat.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		multiLightMat.setFloat("pointLights[1].constant", 1.0f);
		multiLightMat.setFloat("pointLights[1].linear", 0.09);
		multiLightMat.setFloat("pointLights[1].quadratic", 0.1);

		//Light2

		multiLightMat2.use();
		multiLightMat2.setVec3("viewPos", camera.Position);
		multiLightMat2.setFloat("material.shininess", 64.0f);


		//Directional light
		multiLightMat2.setVec3("dirLight.direction", 0.2f, 1.0f, -0.3f);
		multiLightMat2.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		multiLightMat2.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		multiLightMat2.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// point light 1
		multiLightMat2.setVec3("pointLights[0].position", lightPosition1);
		multiLightMat2.setVec3("pointLights[0].ambient", ambientColor);
		multiLightMat2.setVec3("pointLights[0].diffuse", diffuseColor*2.0f);
		multiLightMat2.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		multiLightMat2.setFloat("pointLights[0].constant", 1.0f);
		multiLightMat2.setFloat("pointLights[0].linear", 0.09);
		multiLightMat2.setFloat("pointLights[0].quadratic", 0.1);
		// point light 2
		multiLightMat2.setVec3("pointLights[1].position", lightPosition2);
		multiLightMat2.setVec3("pointLights[1].ambient", ambientColor);
		multiLightMat2.setVec3("pointLights[1].diffuse", diffuseColor*2.0f);
		multiLightMat2.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		multiLightMat2.setFloat("pointLights[1].constant", 1.0f);
		multiLightMat2.setFloat("pointLights[1].linear", 0.09);
		multiLightMat2.setFloat("pointLights[1].quadratic", 0.1);



		//4th colored shape box
		myShader3.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f,0.0f,0));
		view = camera.GetViewMatrix();
		projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 mvp;
		model = glm::rotate(model,  (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
		//model = glm::scale(model, glm::vec3(0.5f));
		mvp = projection * view*model;
		myShader3.setVec4("ourColor2",glm::vec4(redValue, greenValue, blueValue, 1.0f));
		myShader3.setMat4("mvp", mvp);
		/*glBindVertexArray(VAO4);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/
		sphere1.Draw(myShader3);
	
		//5th lighted box
	
		/*

		multiLightMat.use();
		model = glm::mat4(1.0f);
		multiLightMat.setMat4("projection", projection);
		multiLightMat.setMat4("view", view);
		multiLightMat.setMat4("model", model);
		//lightedShader.setMat4("mvp", projection*view*model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/

		//Sphere1 
		multiLightMat2.use();
		model = glm::mat4(1.0f);
		multiLightMat2.setMat4("projection", projection);
		multiLightMat2.setMat4("view", view);
		multiLightMat2.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap2);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap2);
		
		sphere1.Draw(multiLightMat2);

		//Sphere2
		reflectionShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		reflectionShader.setMat4("projection", projection);
		reflectionShader.setMat4("view", view);
		reflectionShader.setMat4("model", model);
		reflectionShader.setVec3("cameraPos", camera.Position);

		sphere1.Draw(reflectionShader);


	/*	multiLightMat.use();
		multiLightMat.setMat4("projection", projection);
		multiLightMat.setMat4("view", view);
		multiLightMat.setMat4("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		*/

		//PBR sphere
		pbr.use();
		view = camera.GetViewMatrix();
		pbr.setMat4("view", view);
		pbr.setMat4("projection", projection);
		pbr.setVec3("camPos", camera.Position);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedo);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallic);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughness);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, ao);

		//	pbr.setFloat("metallic", 0.8f);
		

		//	pbr.setFloat("roughness", 0.2f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f,0.0f,2.0f));
			pbr.setMat4("model", model);
			
			//pbr.setInt("nb_light", 2);
			pbr.setVec3("lightPositions[0]", lightPosition1);
			pbr.setVec3("lightColors[0]", lightColor);

			pbr.setVec3("lightPositions[1]", lightPosition2);
			pbr.setVec3("lightColors[1]", lightColor);

			renderSphere();

		//	sphere1.Draw(pbr);
			
		




		multiLightMat.use();
		multiLightMat.setMat4("projection", projection);
		multiLightMat.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0));
		multiLightMat.setMat4("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(texcubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//6th lamp
		basiclightsource.use();
		basiclightsource.setVec4("ourColor", glm::vec4(lightColor, 1.0f));
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition1);
		model = glm::scale(model, glm::vec3(0.2f));
		basiclightsource.setMat4("projection", projection);
		basiclightsource.setMat4("view", view);
		basiclightsource.setMat4("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//7th lamp
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition2);
		model = glm::scale(model, glm::vec3(0.2f));
		basiclightsource.setMat4("projection", projection);
		basiclightsource.setMat4("view", view);
		basiclightsource.setMat4("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default



		//HDR end
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		// --------------------------------------------------------------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		hdr.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		hdr.setInt("hdr", set_hdr);
		hdr.setFloat("exposure", exposure);
		renderQuad();

		



		//glBindVertexArray(1); // no need to unbind it every time 
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
//	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !hdrKeyPressed)
	{
		set_hdr = !set_hdr;
		hdrKeyPressed = true;
		std::cout << "hdr: " << (set_hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		hdrKeyPressed = false;
		//std::cout << "hdr: " << (set_hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

//Key call back event
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
/*	static bool polygon_mode = false;

	//Open and close polygon mode
	if (key ==GLFW_KEY_W && action==GLFW_PRESS)
	{
		polygon_mode = polygon_mode ^ 1;
		std::cout << " Polygon mode: " + polygon_mode << std::endl;
	}


	if (polygon_mode)
	{
		std::cout << " Polygon mode: Open" + polygon_mode << std::endl;

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		std::cout << " Polygon mode: Close" + polygon_mode << std::endl;

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}*/

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

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
	if (sphereVAO == 0)
	{
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI)/2;
				float yPos = std::cos(ySegment * PI)/2;
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI)/2;

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();

		std::vector<float> data;
		for (int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}