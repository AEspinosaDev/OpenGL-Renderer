#pragma once
#include <GL/glew.h>
#include <iostream>
#include <glm.hpp>


#define ASSERT(x) if (!(x))  __debugbreak();
#define GLcall(x) GLclearError();\
 x;\
ASSERT(GLlogCall(#x, __FILE__, __LINE__))


    const std::string shaderPath = "Resources/Shaders/";

    const std::string texturePath = "Resources/Textures/";

    const std::string modelPath = "Resources/Models/";


    void GLclearError();
    

    bool GLlogCall(const char* function, const char* file, int line);
   


