#pragma once

#include <time.h>
#include <stdarg.h>
#define GL_LOG_FILE "gl.log"
#include <GL/glew.h> //inlcude GLEW and new version of GL on windows
#define GLFW_DLL
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>


bool restart_gl_log();
bool gl_log(const char* message, ...);
bool gl_log_err(const char* message, ...);
void glfw_error_callback(int error, const char* description);
void log_gl_params();


