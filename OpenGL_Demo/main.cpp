#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include "Camera.h"

#include <iostream>
#include "Shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


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


	//Shader myShader1("./shaders/vertexshader/test2.vs", "./shaders/fragmentshader/test2.fs");
	//Shader myShader2("./shaders/vertexshader/test3.vs", "./shaders/fragmentshader/test3.fs");
	Shader myShader3("./shaders/vertexshader/test4.vs", "./shaders/fragmentshader/test2.fs");

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

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
//	unsigned int VBO1,VBO2, VAO1,VAO2, EBO;
	unsigned int VBO[2], VAO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	/*
	//First Shape
	//glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Second Shape
	
	
	//Texture generation
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load("./texture/wall.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	

	//3rd shape
	unsigned int VBO3, VAO3, EBO;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	*/
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



	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(1);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Going to 3D





	//Event Registration
	

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



		// update the uniform color
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		float redValue = sin((timeValue+1.57)*2) / 2.0f + 0.5f;
		float blueValue = sin((timeValue+3.14)*3) / 2.0f + 0.5f;
		float alphaValue = sin(timeValue + 5) / 2.0f + 0.5f;
		//std::cout << greenValue << std::endl;




		/*
		//1st shape
		myShader1.use();
		int vertexColorLocation = glGetUniformLocation(myShader1.ID, "ourColor2");
		glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, alphaValue);
		glBindVertexArray(VAO[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0); // no need to unbind it every time 
		
		//2nd shape
		glBindTexture(GL_TEXTURE_2D, texture);
		myShader2.use();
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		//3nd shape

		//MVP
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 mvp;
		mvp = projection * view*model;


		int vertexColorLocation = glGetUniformLocation(myShader3.ID, "ourColor2");
		glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, alphaValue);
		int mvpLoc = glGetUniformLocation(myShader3.ID, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
		myShader3.use();
		glBindVertexArray(VAO3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
		//4th shape box
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 mvp;
		mvp = projection * view*model;


		int vertexColorLocation = glGetUniformLocation(myShader3.ID, "ourColor2");
		glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, alphaValue);
		int mvpLoc = glGetUniformLocation(myShader3.ID, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
		myShader3.use();
		glBindVertexArray(VAO4);
		glDrawArrays(GL_TRIANGLES, 0, 36);
			*/
		//Boxes scene
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		//glm::mat4 view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 mvp;
		
		int vertexColorLocation = glGetUniformLocation(myShader3.ID, "ourColor2");
		glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, alphaValue);
		
		
		myShader3.use();

		glBindVertexArray(VAO4);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle)+ (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			mvp = projection * view*model;
			int mvpLoc = glGetUniformLocation(myShader3.ID, "mvp");
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



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