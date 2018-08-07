// OpenGLTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "logger.h"
#include <GL/glew.h> //inlcude GLEW and new version of GL on windows
#define GLFW_DLL
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

// Window resize functions
// reported window size may be good to know for a few things
int g_win_width = 640;
int g_win_height = 480;
// keep track of framebuffer size for things like the viewpoer and the mouse cursor
int g_fb_width = 640;
int g_fb_height = 480;

void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
	g_win_width = width;
	g_win_height = height;
}

void glfw_framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
	g_fb_width = width;
	g_fb_height = height;

	/* later update any perspective matrices used here */
}

int main()
{
	if(!restart_gl_log()) { /* quit? */ }
	// Start GL context and O/S window using the GLFW helper library
	gl_log("Starting GLFW\n%s\n", glfwGetVersionString());
	// Register the error call-back function that we wrote, above
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: Could not start GLFW3\n");
		return 1;
	}

#ifdef _APPLE
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	glfwWindowHint(GLFW_SAMPLES, 4);
	/*GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* vmode = glfwGetVideoMode(mon);
	GLFWwindow* window = glfwCreateWindow(vmode->width, vmode->height, "Logger", mon, NULL);*/
	GLFWwindow* window = glfwCreateWindow(g_fb_width, g_fb_height, "Logger", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: Could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	
	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_resize_callback);
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	/* OTHER STUFF GOES HERE NEXT */
	GLfloat points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	// Vertex Buffer Object (VBO)
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Vertex Attribute Object (VAO)
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Shaders
	const char* vertex_shader =
		"#version 410\n"
		"in vec3 vp;"
		"void main () {"
		"    gl_Position = vec4(vp, 1.0);"
		"}";

	// Using Platinum(#E5E4E2) color for now
	const char* fragment_shader =
		"#version 410\n"
		"out vec4 frag_colour;"
		"void main () {"
		"    frag_colour = vec4(0.898f, 0.894f, 0.886f, 1.0);"
		"}";
	
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	// Drawing
	while (!glfwWindowShouldClose(window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_fb_width, g_fb_height);
		glUseProgram(shader_programme);
		glBindVertexArray(vao);
		// draw points from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);

		

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
    return 0;
}

