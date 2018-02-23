/*
 * defines global variables of XiangQiUI
 * the default values of these variables can be found
 * in XiangQiUIPresetValues.hpp
 */

#ifndef XIANGQI_XIANGQIUIVARIABLES_HPP
#define XIANGQI_XIANGQIUIVARIABLES_HPP

#include "XiangQi.hpp"
#include "GLDef.hpp"

const vector<wstring> chessBlackText{
	wstring{ L"Œ¢" },
	wstring{ L"Ê¿" },
	wstring{ L"Ïó" },
	wstring{ L"Ü‡" },
	wstring{ L"³h" },
	wstring{ L"ñR" },
	wstring{ L"×ä" },
};

const vector<wstring> chessRedText{
	wstring{ L"Ž›" },
	wstring{ L"ÊË" },
	wstring{ L"Ïà" },
	wstring{ L"‚e" },
	wstring{ L"ÅÚ" },
	wstring{ L"‚Ø" },
	wstring{ L"±ø" },
};

// the aspect ratio of the window is fixed, so there is no need
// to manually update myWindowHeight
// myWindowHeight and the following chess board variables can be calculated by
// calling UpdateChessBoardSizeVariables()
extern Integer myWindowWidth;
// do not modify manually, modify width only
extern Integer myWindowHeight;

// returns the window height, given window width
Integer GetMyWindowHeight();

// return the window width, given window height
Integer GetMyWindowWidth();


//region Chess Board Related Definitions

// Note that all of the following scaling factors are
// defined according to the width of the window

// the proportion of the chess board in the window
// (regarding its side length)
// this factor should be always less than or equal to 1.0f and greater than 0.0f
extern float chessBoardWindowScale;

// the proportion of the lines on board in the board
// this factor should be always less than or equal to 1.0f and greater than 0.0f
extern float chessBoardLineBoardScale;

// how big the chess should be
// this factor should be always less than or equal to 1.0f and greater than 0.0f
extern float chessRadiusFactor;

// specifies the proper font height on chess
extern Integer chessFontHeight;


// the side lengths of chess blocks [(w, h)order]
extern Vec2f chessBoardBlockSize;
// the perimeter space of chess board
extern float chessBoardPerimeterSpace;
// the background space of chess board
extern float chessBoardBackgroundSpace;
// the radius of Xiang Qi chess on the board
extern float chessRadius;
// determines the ratio between chessBoardBlockSize
// and line width
extern float chessBoardLineWidthFactor;
// specify the width of the chess ring
// range : [0,1]
extern float chessRingInnerFactor;

extern float chessRingOuterFactor;

// specify the position of the perimeter line of the chess board
// should be a value in (0,1)
extern float chessBoardPerimeterLinePosFactor;


// specify the line width, which is calculated according
// to the line width factor
// should not be modified manually
// the calculation of line width should be identical to the
// calculation in the geometry shader
extern float chessBoardLineWidth;

// the color of chessboard
extern Vec3f chessBoardColor;
// the color of background
extern Vec3f chessBoardBackgroundColor;
// the color of the chess
extern Vec3f chessColor;

extern Vec3f chessBlackTextColor;

extern Vec3f chessRedTextColor;

extern Vec3f chessRingColor;
//endregion

//region GL related variables

// the title of the window
extern const char *myWindowTitle;

// the font to be loaded by the XiangQiUI program
extern const char *_XiangQiUIFont;


// the window that this program uses, this program only uses one window
// the default value of it will be nullptr, it will be initialized by
// calling InitializeUI()
extern GLFWwindow *myWindow;

extern Integer chessPositionVectorBlackIndex;

// the pointer to the wood texture
// this variable will be initialized by calling
// LoadTexture()
// it should not be modified manually
extern GLuint woodTextureID;
// the width and height of wood texture
// should not be modified manually
extern Integer woodTextureWidth;
extern Integer woodTextureHeight;

extern GLuint chessTextureID;
extern Integer chessTextureWidth;
extern Integer chessTextureHeight;

// these are buffers to draw elements, they will be updated
// by calling corresponding update functions
// they should not be modified manually
extern vector<float> chessBoardSquareVector;
extern vector<float> chessBoardLineVector;
//extern vector<float> chessPositionVector;

extern GLuint chessBoardSquareVAO;
extern GLuint chessBoardSquareVBO;
extern GLuint chessBoardSquareEBO;

extern GLuint chessBoardLineVAO;
extern GLuint chessBoardLineVBO;

extern GLuint chessVAO;
extern GLuint chessVBO;

extern unique_ptr<GLFontRender> fontRender;
extern unique_ptr<GLProgramManager> chessBoardSquareRenderProgram;
extern unique_ptr<GLProgramManager> chessBoardLineRenderProgram;
extern unique_ptr<GLProgramManager> chessRenderProgram;

extern unique_ptr<AVector<Vec2f>> chessTextureTopLeftVector;
//endregion

// the chess board to be drawn on the screen
extern shared_ptr<XiangQiBoard> _XiangQiUIBoard;


#endif //XIANGQI_XIANGQIUIVARIABLES_HPP
