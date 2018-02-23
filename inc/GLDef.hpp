#ifndef DEF_GLDEF_HPP
#define DEF_GLDEF_HPP

#include "CoreDef.hpp"


// freetype library
#include "ft2build.h"
#include FT_FREETYPE_H

// tells GLEW to use static library
#define GLEW_STATIC
#include "glew.h"

// glfw
#include "glfw3.h"

// extract the type of GL enums
using GLEnumType = decltype(GL_FRAGMENT_SHADER);



#endif