// defines the extern variables in the header

#include "XiangQiUI.hpp"
#include "XiangQiUIPresetValues.hpp"

Integer GetMyWindowHeight(){
    return (Integer)ceil((float)myWindowWidth * _XiangQiUIWHRatio);
}

Integer GetMyWindowWidth(){
    return (Integer)ceil((float)myWindowHeight / _XiangQiUIWHRatio);
}

Integer myWindowWidth = _XQUIPRESET_windowWidth;
Integer myWindowHeight = GetMyWindowHeight();

float chessBoardWindowScale = _XQUIPRESET_chessBoardWindowScale;

float chessBoardLineBoardScale = _XQUIPRESET_chessBoardLineBoardScale;

float chessRadiusFactor = _XQUIPRESET_chessRadiusFactor;

float chessBoardPerimeterLinePosFactor = _XQUIPRESET_chessBoardPerimeterLinePosFactor;

float chessRingOuterFactor = _XQUIPRESET_chessRingOuterFactor;

float chessRingInnerFactor = _XQUIPRESET_chessRingInnerFactor;

Vec2f chessBoardBlockSize{};
float chessBoardPerimeterSpace = 0.0;
float chessBoardBackgroundSpace = 0.0;
float chessRadius = 0.0;
float chessBoardLineWidthFactor = _XQUIPRESET_chessBoardLineWidthFactor;
float chessBoardLineWidth = 0.0f;

Integer chessFontHeight = 0;

Vec3f chessBoardColor = _XQUIPRESET_chessBoardColor;
Vec3f chessBoardBackgroundColor = _XQUIPRESET_chessBoardBackgroundColor;
Vec3f chessColor = _XQUIPRESET_chessColor;
Vec3f chessRingColor = _XQUIPRESET_chessRingColor;
Vec3f chessBlackTextColor = _XQUIPRESET_chessBlackTextColor;
Vec3f chessRedTextColor = _XQUIPRESET_chessRedTextColor;

const char* myWindowTitle = _XQUIPRESET_myWindowTitle;
const char *_XiangQiUIFont = _XQUIPRESET_XiangQiUIFont;

GLFWwindow* myWindow = nullptr;

Integer chessPositionVectorBlackIndex = 0;

GLuint woodTextureID = 0;
Integer woodTextureWidth = 0;
Integer woodTextureHeight = 0;

GLuint chessTextureID = 0;
Integer chessTextureWidth = 0;
Integer chessTextureHeight = 0;


vector<float> chessBoardSquareVector;
vector<float> chessBoardLineVector;
//vector<float> chessPositionVector;

unique_ptr<AVector<Vec2f>> chessTextureTopLeftVector;

GLuint chessBoardSquareVAO = 0;
GLuint chessBoardSquareVBO = 0;
GLuint chessBoardSquareEBO = 0;

GLuint chessBoardLineVAO = 0;
GLuint chessBoardLineVBO = 0;

GLuint chessVAO = 0;
GLuint chessVBO = 0;

unique_ptr<GLFontRender> fontRender;
unique_ptr<GLProgramManager> chessBoardSquareRenderProgram;
unique_ptr<GLProgramManager> chessBoardLineRenderProgram;
unique_ptr<GLProgramManager> chessRenderProgram;

shared_ptr<XiangQiBoard> _XiangQiUIBoard;