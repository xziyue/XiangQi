// OpenGL UI for the Xiang Qi gameplay

#ifndef XIANGQI_XIANGQIUI_HPP
#define XIANGQI_XIANGQIUI_HPP

#include "GLDef.hpp"
#include "GLProgram.hpp"
#include "GLFontRender.hpp"
#include "XiangQiUIVariables.hpp"
#include "XiangQi.hpp"
#include "ResourceManager.hpp"


struct FontLoadingError : public runtime_error{
    using runtime_error::runtime_error;
};

Vec2f GetMyWindowSymmetricPosition(const Vec2f &pos);

Vec2f GetChessBoardPointPosition(const Vec2i &point);

void InitializeXiangQiUI();

void RunXiangQiUI();



#endif //XIANGQI_XIANGQIUI_HPP
