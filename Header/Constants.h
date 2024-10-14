#include<stdio.h>
#include<iostream>
#include<vector>
#include<assert.h>
#include<string>
#include<unordered_map>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#define SCREEN_RES_X 2160
#define SCREEN_RES_Y 1440
#define MSAA_SAMPLE_SIZE 4

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<fstream>

using v2 = glm::vec2;	// Shorthand for vectors
using v3 = glm::vec3;
using v4 = glm::vec4;

using m3 = glm::mat3;	// Matrices shorthand
using m4 = glm::mat3;

using p2 = glm::vec2; // Shorthand points for clarity when differing between points in space and vectors
using p3 = glm::vec3;
#define WINDOW_NAME "can i get ho yea int b&m"
#define OPENGL_VERSION "#version 330 core"
#define SHADOW_CAST_FARPLANE 1001.F
