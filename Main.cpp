#include <ShaderClass.h>
#include <CameraClass.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#define PI 3.14159265358979323846264338327950288419716939937510
#define toRadians (PI/180.0)

/* Function declerations */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

/* False, when mouse moves ( utility bool for the adjusting initial mouse configuration )*/
bool isMouseDeactive = true;

/* This configuration is for fullscreen since generally all computers have this resolution */
const unsigned int SCR_WIDTH  = 1920;
const unsigned int SCR_HEIGHT = 1080;

/* initialize camera object with a constructor 0,0,3*/
Camera camera = Hector3(0.0f, 0.0f, 3.0f);
float firstXofMouse = SCR_WIDTH / 2.0f;
float firstYofMouse = SCR_HEIGHT / 2.0f;

/* to syncronize the rotation and translation of the camera */
float timeInterval = 0.0f;
float lastTime = 0.0f;
float fov = 50.0f;


int main()
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Fullscreen setup */
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Assignment 5", monitor, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	/* mouse capture */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	
	glEnable(GL_DEPTH_TEST);

	
	Shader spotlighObject("spotlight.vs", "spotlight.fs");

	
	float cubeVertices[] = {
		
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

	float planeVertices[] = {
		
		38.5f,  38.5f, 0.0f,	1.0f, 1.0f,
		38.5f, -38.5f, 0.0f,	1.0f, 0.0f,
	   -38.5f, -38.5f, 0.0f,    0.0f, 0.0f,
	   -38.5f,  38.5f, 0.0f,    0.0f, 1.0f

	};
	unsigned int planeIndices[] = {

		0, 1, 3,
		1, 2, 3
	};
	/* ----------------------------------------------------------------------------------------------------------------------- */
	/* Plane setup */
	unsigned int planeVBO, planeVAO, planeEBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeEBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);

	/* vertex position */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* texture position */
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/* ----------------------------------------------------------------------------------------------------------------------- */
	/* Cubes setup */
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/* ----------------------------------------------------------------------------------------------------------------------- */
	/* Spotlight setup */
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* ----------------------------------------------------------------------------------------------------------------------- */

	
	unsigned int cubeMap = loadTexture("texture_cube.png");
	unsigned int groundMap = loadTexture("texture_ground.bmp");


	// shader configuration
	// --------------------
	spotlighObject.use();
	spotlighObject.setInt("material.diffuse", 0);
	spotlighObject.setInt("material.specular", 1);

	/* setting camera to the initial position */
	camera.Position = vectorAdditionWithReturn(camera.Position, Hector3(67.07f, 13.19f, 21.28f));
	
	//bool isLightsOff = false;
	

	while (!glfwWindowShouldClose(window))
	{
		/* calculate the time between frames */
		float thisTime = glfwGetTime();
		timeInterval = thisTime - lastTime;
		lastTime = thisTime;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		spotlighObject.setVec3("light.diffuse", 2.1f, 2.1f, 2.1f);


		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			spotlighObject.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);


		/*
		if (isLightsOff) {
			spotlighObject.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);
		}
		else {
			spotlighObject.setVec3("light.diffuse", 2.1f, 2.1f, 2.1f);
		}*/
		

		spotlighObject.use();
		//std::cout << camera.Position.x << "     " <<  camera.Position.y  << "       " << camera.Position.z;
		spotlighObject.setVec3("light.position", camera.Position.x, camera.Position.y, camera.Position.z);
		spotlighObject.setVec3("light.direction", camera.Front.x, camera.Front.y, camera.Front.z);
		spotlighObject.setFloat("light.cutOff", cos(toRadians*10.5f));
		spotlighObject.setFloat("light.outerCutOff", cos(toRadians*(20.5f)));
		spotlighObject.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

		/* set light properties */
		spotlighObject.setVec3("light.ambient", 0.7f, 0.7f, 0.7f);
		spotlighObject.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		spotlighObject.setFloat("light.constant", 1.0f);
		spotlighObject.setFloat("light.linear", 0.022);
		spotlighObject.setFloat("light.quadratic", 0.0019);

		/* set material properties */
		spotlighObject.setFloat("material.shininess", 0.1f);

		/* calculate projection matrix */
		float *projection = myProjectionFunction(toRadians*(fov), 2.0f, 0.1f, 100.0f);	
		spotlighObject.setMat44("projection", projection);

		/* calculate view matrix */
		float* view = camera.lookAtFunc();
		spotlighObject.setMat44("view", view);


		/* calculate model matrix */
		float model[] =
		{
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f
		};
		spotlighObject.setMat44("model", model);
		
		/* activate first texture */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeMap);
		
		/* place 625 cubes on the plane */
		glBindVertexArray(cubeVAO);
		for (int i = 0; i < 75;)
		{
			for (int y = 0; y < 75;)
			{
				float model[] =
				{
					1.0f,	0.0f,	0.0f,	0.0f,
					0.0f,	1.0f,	0.0f,	0.0f,
					0.0f,	0.0f,	1.0f,	0.0f,
					i,	0.0f,	y,	1.0f
				};
				
				spotlighObject.setMat44("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);

				y = y + 3;
			}

			i = i + 3;
		}
		
		/* activate first texture */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, groundMap);
	
		/* rotate and draw the plane */
		glBindVertexArray(planeVAO);
		float planeModel[] =
		{
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			36.0f,	-0.51f,	36.0f,	1.0f
		};
		spotlighObject.setMat44("model", planeModel);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, timeInterval);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, timeInterval);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, timeInterval);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, timeInterval);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, timeInterval);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, timeInterval);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (isMouseDeactive)
	{
		firstXofMouse = xpos;
		firstYofMouse = ypos;
		isMouseDeactive = false;
	}

	float xoffset = xpos - firstXofMouse;
	float yoffset = firstYofMouse - ypos;

	firstXofMouse = xpos;
	firstYofMouse = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

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
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(data);

	return textureID;
}

