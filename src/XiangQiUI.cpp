#include "XiangQiUI.hpp"

bool programInitialized = false;

void Render();

Vec2f GetChessBoardPointPosition(const Vec2i &point) {
	assert(point[0] > -1);
	assert(point[1] > -1);
	float totalSpace = chessBoardBackgroundSpace + chessBoardPerimeterSpace;
	float x = totalSpace + chessBoardBlockSize[0] * (float)point[0];
	float y = totalSpace + chessBoardBlockSize[1] * (float)point[1];
	return Vec2f{ x, y };
}

Vec2f GetMyWindowSymmetricPosition(const Vec2f &pos) {
	Vec2f windowTopRight{ (float)(myWindowWidth - 1), (float)(myWindowHeight - 1) };
	return windowTopRight - pos;
}

// returns whether the mouse is on a chess, and the position of the chess
pair<bool, Vec2i> GetMousePosition(const Vec2f &mousePos) {
	pair<bool, Vec2i> result;
	result.first = false;

	auto botLeftSpace = chessBoardBackgroundSpace + chessBoardPerimeterSpace;
	Vec2f botLeft{botLeftSpace, botLeftSpace};

	Vec2f diff = mousePos - botLeft;
	diff.array() /= chessBoardBlockSize.array();
	// find the corresponding block

	Eigen::round(diff.array());
	diff += Vec2f{1.0e-3f, 1.0e-3f};

	Integer xPos = (Integer)diff[0];
	Integer yPos = (Integer)diff[1];

	Vec2i pos{ xPos, yPos };

	if (XiangQiBoard::IsChessPosInBoard(pos)) {
		Vec2f centerPos = GetChessBoardPointPosition(pos);
		float dist = (mousePos - centerPos).norm();
		if (dist <= chessRadius) {
			result.first = true;
			result.second = pos;
		}
	}

	return result;
}

void UpdateChessBoardSizeVariables() {
	AlwaysAssert<out_of_range>(chessBoardWindowScale > 0.0f && chessBoardWindowScale <= 1.0f,
		"invalid chessBoardWindowScale");
	AlwaysAssert<out_of_range>(chessBoardLineBoardScale > 0.0f && chessBoardLineBoardScale <= 1.0f,
		"invalid chessBoardLineBoardScale");
	AlwaysAssert<out_of_range>(chessRadiusFactor > 0.0f && chessRadiusFactor <= 1.0f,
		"invalid chessRadiusFactor");

	// myWindowHeight is updated
	myWindowHeight = GetMyWindowHeight();

	chessBoardBackgroundSpace = (float)myWindowWidth * (1.0f - chessBoardWindowScale) / 2.0f;
	chessBoardPerimeterSpace = (float)myWindowWidth * chessBoardWindowScale * (1.0f - chessBoardLineBoardScale) / 2.0f;

	auto sideSpace = (chessBoardBackgroundSpace + chessBoardPerimeterSpace) * 2.0f;

	auto widthLeft = (float)myWindowWidth - sideSpace;
	auto heightLeft = (float)myWindowHeight - sideSpace;
	chessBoardBlockSize = Vec2f{ widthLeft / (float)(XiangQiBoard::boardWidth - 1),
								heightLeft / (float)(XiangQiBoard::boardHeight - 1) };

	chessRadius = min({ chessBoardPerimeterSpace, chessBoardBlockSize[0], chessBoardBlockSize[1] }) * chessRadiusFactor;

	// calculate line width
	chessBoardLineWidth = ceil(min(chessBoardBlockSize[0], chessBoardBlockSize[1]) * chessBoardLineWidthFactor);

	chessFontHeight = (Integer)floor(chessRingInnerFactor * chessRadius * sqrt(2.0f));
}

// load wood texture for the chess board
void LoadTexture() {
	auto img = move(GetImageResource("woodTexture.png"));
	img.rotate(90.0f, 1);
	woodTextureWidth = img.width();
	woodTextureHeight = img.height();

	auto interleavedImage = move(GetInverleavedImageArray(img));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &woodTextureID);
	glBindTexture(GL_TEXTURE_2D, woodTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, woodTextureWidth, woodTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		interleavedImage.get());
	// set parameters for the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);


	img = move(GetImageResource("chessTexture.png"));
	chessTextureWidth = img.width();
	chessTextureHeight = img.height();

	interleavedImage = move(GetInverleavedImageArray(img));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &chessTextureID);
	glBindTexture(GL_TEXTURE_2D, chessTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, chessTextureHeight, chessTextureWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		interleavedImage.get());
	// set parameters for the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

}

// the chess board buffer consists of two parts,
// i.e. the actual pos (2 float number) and
// texture pos (2 float number) for each point.
void UpdateChessBoardSquareVector() {
	// 4 floating point number per vertex, so there are 16 numbers
	chessBoardSquareVector.resize(16);
	float leftX, rightX, botY, topY;

	leftX = chessBoardBackgroundSpace;
	rightX = (float)(myWindowWidth - 1) - leftX;
	botY = chessBoardBackgroundSpace;
	topY = (float)(myWindowHeight - 1) - botY;

	Vec2f woodTextureSize{ (float)woodTextureWidth, (float)woodTextureHeight };
	Vec2f botLeft{ leftX, botY };

	Integer currentIndex = 0;
	auto AddToVector = [&](float x, float y) {
		Vec2f texturePos = (Vec2f{ x, y } -botLeft).array() / woodTextureSize.array();
		chessBoardSquareVector[currentIndex++] = x;
		chessBoardSquareVector[currentIndex++] = y;
		chessBoardSquareVector[currentIndex++] = texturePos[0];
		chessBoardSquareVector[currentIndex++] = texturePos[1];
	};

	AddToVector(leftX, botY);
	AddToVector(rightX, botY);
	AddToVector(rightX, topY);
	AddToVector(leftX, topY);
}

void UpdateChessBoardLineVector() {
	chessBoardLineVector.clear();
	//region Lambda Functions

	auto AddToVectorSymmetric = [&](const Vec2i &p1, const Vec2i &p2, float hComp = 0.0f) {
		Vec2f pos[4];

		Vec2f hCompVec{ hComp, 0.0f };

		pos[0] = GetChessBoardPointPosition(p1) - hCompVec;
		pos[1] = GetChessBoardPointPosition(p2) + hCompVec;
		pos[2] = GetMyWindowSymmetricPosition(pos[0]);
		pos[3] = GetMyWindowSymmetricPosition(pos[1]);

		for (Integer i = 0; i < 4; ++i) {
			chessBoardLineVector.push_back(pos[i][0]);
			chessBoardLineVector.push_back(pos[i][1]);
		}
	};

	auto AddToVectorSymmetricAbsolute = [&](const Vec2f &p1, const Vec2f &p2, float hComp = 0.0f) {
		Vec2f pos[4];

		Vec2f hCompVec{ hComp, 0.0f };

		pos[0] = p1 - hCompVec;
		pos[1] = p2 + hCompVec;
		pos[2] = GetMyWindowSymmetricPosition(p1);
		pos[3] = GetMyWindowSymmetricPosition(p2);

		for (Integer i = 0; i < 4; ++i) {
			chessBoardLineVector.push_back(pos[i][0]);
			chessBoardLineVector.push_back(pos[i][1]);
		}
	};
	//endregion

	auto hComp = chessBoardLineWidth / 4.0f;

	// add horizontal lines
	for (Integer j = 0; j < XiangQiBoard::boardHeight / 2; ++j) {
		Vec2i left{ 0, j };
		Vec2i right{ XiangQiBoard::boardWidth - 1, j };
		AddToVectorSymmetric(left, right);
	}

	Integer centerJ = XiangQiBoard::boardHeight / 2 - 1;
	// add vertical lines in the middle
	for (Integer i = 1; i < XiangQiBoard::boardWidth - 1; ++i) {
		Vec2i left{ i, 0 };
		Vec2i right{ i, centerJ };
		AddToVectorSymmetric(left, right);
	}

	// add vertical lines on the leftmost and rightmost
	AddToVectorSymmetric(Vec2i{ 0, 0 }, Vec2i{ 0, XiangQiBoard::boardHeight - 1 });

	// add diagonal lines in palace
	AddToVectorSymmetric(redPalaceBotLeftPos, redPalaceTopRightPos);
	AddToVectorSymmetric(redPalaceTopLeftPos, redPalaceBotRightPos);

	// add perimeter lines
	float leftX = chessBoardBackgroundSpace + chessBoardPerimeterLinePosFactor * chessBoardPerimeterSpace;
	float botY = leftX;
	float rightX = (float)(myWindowWidth - 1) - leftX;
	float topY = (float)(myWindowHeight - 1) - botY;

	Vec2f botLeft{ leftX, botY };
	Vec2f topLeft{ leftX, topY };
	Vec2f botRight{ rightX, botY };

	AddToVectorSymmetricAbsolute(botLeft, topLeft);
	AddToVectorSymmetricAbsolute(botLeft, botRight, hComp);
}

/*
void UpdateChessPositionVector() {
	chessPositionVector.clear();
	const auto &chessMap = _XiangQiUIBoard->GetChessMap();

	vector<float> redVector;
	vector<float> blackVector;
	redVector.reserve(chessMap.size());
	blackVector.reserve(chessMap.size());

	for (auto iter = chessMap.begin(); iter != chessMap.end(); ++iter) {
		vector<float> *targetVector = nullptr;

		if (iter->second->GetOwner() == XiangQiChessOwner::RED) {
			targetVector = &redVector;
		}
		else {
			targetVector = &blackVector;
		}
		// calculate positions
		Vec2f pos = GetChessBoardPointPosition(iter->first);
		for (Integer i = 0; i < 2; ++i) {
			targetVector->push_back(pos[i]);
		}
		const auto &textureTopLeft = chessTextureTopLeftVector->operator[](iter->second->GetHashCode());
		// push in top left of the texture
		for (Integer i = 0; i < 2; ++i) {
			targetVector->push_back(textureTopLeft[i]);
		}
	}

	copy(redVector.begin(), redVector.end(), back_inserter(chessPositionVector));
	copy(blackVector.begin(), blackVector.end(), back_inserter(chessPositionVector));

	chessPositionVectorBlackIndex = (Integer)redVector.size();
}
*/

void UpdateOpenGLVBO() {
	glBindBuffer(GL_ARRAY_BUFFER, chessBoardSquareVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, chessBoardSquareVector.size() * sizeof(float), chessBoardSquareVector.data());

	glBindBuffer(GL_ARRAY_BUFFER, chessBoardLineVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, chessBoardLineVector.size() * sizeof(float), chessBoardLineVector.data());
}

void UpdateFont() {
	fontRender->ClearTypeface();
	fontRender->LoadTypeface(_XiangQiUIFont, chessFontHeight);
	for (const auto &text : chessBlackText) {
		fontRender->PreloadGlyph(text);
	}
	for (const auto &text : chessRedText) {
		fontRender->PreloadGlyph(text);
	}
}

void UpdateChessTextureTopLeftVector() {
	Integer numOfChess = _XiangQiUIBoard->GetChessHashCodeRange()[1];

	mt19937 engine;
	engine.seed(1738937); // just a randomly predefined seed

	uniform_real_distribution<float> distribution{ 0.0f, 10.0f };
	for (Integer i = 0; i < numOfChess; ++i) {
		float x = distribution(engine);
		float y = distribution(engine);
		chessTextureTopLeftVector->emplace_back(Vec2f{ x, y });
	}
}

void OpenGLResizeCallback(GLFWwindow *window, GLint width, GLint height) {
	auto deltaWidth = width - myWindowWidth;

	if (deltaWidth == 0) {
		myWindowHeight = height;
		myWindowWidth = GetMyWindowWidth();
	}
	else {
		myWindowWidth = width;
		myWindowHeight = GetMyWindowHeight();
	}

	glfwSetWindowSize(window, myWindowWidth, myWindowHeight);
	glViewport(0, 0, myWindowWidth, myWindowHeight);

	UpdateChessBoardSizeVariables();
	UpdateChessBoardSquareVector();
	UpdateChessBoardLineVector();
	//UpdateChessPositionVector();
	UpdateFont();

	UpdateOpenGLVBO();
}

void OpenGLCursorCallback(GLFWwindow *window, double xPos, double yPos) {
	yPos = (float)myWindowHeight - yPos;
	auto result = move(GetMousePosition(Vec2f{(float)xPos, (float)yPos}));


}

void InitializeFontRender() {
	GLFontRender *fontRenderPtr = new GLFontRender{};
	fontRender.reset(fontRenderPtr);
}

void InitializeOpenGLProgram() {

	GLProgramManager *chessBoardRenderProgramPtr = new GLProgramManager{};
	chessBoardSquareRenderProgram.reset(chessBoardRenderProgramPtr);
	chessBoardSquareRenderProgram->CompileShaderFromFile(GL_VERTEX_SHADER, "chessBoardSquare.vs");
	chessBoardSquareRenderProgram->CompileShaderFromFile(GL_FRAGMENT_SHADER, "chessBoardSquare.fs");
	chessBoardSquareRenderProgram->LinkProgram();

	GLProgramManager *chessBoardLineRenderProgramPtr = new GLProgramManager{};
	chessBoardLineRenderProgram.reset(chessBoardLineRenderProgramPtr);
	chessBoardLineRenderProgram->CompileShaderFromFile(GL_VERTEX_SHADER, "chessBoardLine.vs");
	//chessBoardLineRenderProgram->CompileShaderFromFile(GL_GEOMETRY_SHADER, "chessBoardLine.gs");
	chessBoardLineRenderProgram->CompileShaderFromFile(GL_FRAGMENT_SHADER, "chessBoardLine.fs");
	chessBoardLineRenderProgram->LinkProgram();

	GLProgramManager *chessRenderProgramPtr = new GLProgramManager{};
	chessRenderProgram.reset(chessRenderProgramPtr);
	chessRenderProgram->CompileShaderFromFile(GL_VERTEX_SHADER, "chess.vs");
	chessRenderProgram->CompileShaderFromFile(GL_GEOMETRY_SHADER, "chess.gs");
	chessRenderProgram->CompileShaderFromFile(GL_FRAGMENT_SHADER, "chess.fs");
	chessRenderProgram->LinkProgram();

}

void InitializeOpenGLMemory() {
	// initialize the VAO and VBO for the square chess board
	glGenBuffers(1, &chessBoardSquareVBO);
	glGenVertexArrays(1, &chessBoardSquareVAO);

	// the sequence of indices to draw
	array<GLuint, 6> chessBoardSquareIndices{ 0, 1, 2, 0, 2, 3 };
	glGenBuffers(1, &chessBoardSquareEBO);

	glBindVertexArray(chessBoardSquareVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chessBoardSquareVBO);
	glBufferData(GL_ARRAY_BUFFER, chessBoardSquareVector.size() * sizeof(float),
		chessBoardSquareVector.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chessBoardSquareEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chessBoardSquareIndices.size() * sizeof(GLuint),
		chessBoardSquareIndices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid *)(2 * sizeof(float)));
	glBindVertexArray(0);

	// initialize VAO and VBO for chess board lines
	glGenBuffers(1, &chessBoardLineVBO);
	glGenVertexArrays(1, &chessBoardLineVAO);

	glBindVertexArray(chessBoardLineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chessBoardLineVBO);
	glBufferData(GL_ARRAY_BUFFER, chessBoardLineVector.size() * sizeof(float),
		chessBoardLineVector.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid *)0);
	glBindVertexArray(0);

	// initialize VAO and VBO for chess
	glGenBuffers(1, &chessVBO);
	glGenVertexArrays(1, &chessVAO);

	glBindVertexArray(chessVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chessVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid *)(2 * sizeof(float)));
	glBindVertexArray(0);
}

void InitializeChessTextureTopLeftVector() {
	AVector<Vec2f> *rawPtr = new AVector<Vec2f>{};
	chessTextureTopLeftVector.reset(rawPtr);
}

// initialize OpenGL context, create window
void InitializeOpenGL() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// enable multisampling by default
	glfwWindowHint(GLFW_SAMPLES, 6);

	myWindow = glfwCreateWindow(myWindowWidth, myWindowHeight, myWindowTitle, NULL, NULL);
	glfwMakeContextCurrent(myWindow);
	glViewport(0, 0, myWindowWidth, myWindowHeight);

	glfwSetWindowSizeCallback(myWindow, &OpenGLResizeCallback);
	glfwSetCursorPosCallback(myWindow, &OpenGLCursorCallback);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void InitializeXiangQiUI() {
	AlwaysAssert(!programInitialized, "Initialize() is called more than once");

	InitializeResource();

	// check whether the font could be loaded
	if (!IsFontValid(_XiangQiUIFont)) {
		throw FontLoadingError{ "invalid font" };
	}

	UpdateChessBoardSizeVariables();
	InitializeOpenGL();
	LoadTexture();
	InitializeOpenGLProgram();
	InitializeFontRender();
	InitializeChessTextureTopLeftVector();
	
	// initialize _XiangQiUIBoard
	XiangQiBoard *newBoard = new XiangQiBoard{ true, false };
	_XiangQiUIBoard.reset(newBoard);
	auto size = _XiangQiUIBoard->GetChessMap().size();

	UpdateChessTextureTopLeftVector();

	// update vectors
	UpdateChessBoardSquareVector();
	UpdateChessBoardLineVector();
	//UpdateChessPositionVector();
	
	UpdateFont();

	// initialize VBOs and VAOs
	InitializeOpenGLMemory();

	programInitialized = true;
}

void RenderChessBoardSquare(const Mat4f &projectionMatrix) {
	chessBoardSquareRenderProgram->UseProgram();

	auto chessBoardColorLocation = chessBoardSquareRenderProgram->GetUniformLocation("chessBoardColor");
	auto projectionLocation = chessBoardSquareRenderProgram->GetUniformLocation("projection");

	glUniform3fv(chessBoardColorLocation, 1, chessBoardColor.data());
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix.data());

	glBindTexture(GL_TEXTURE_2D, woodTextureID);
	//glBindTexture(GL_TEXTURE_2D, chessTextureID);
	glBindVertexArray(chessBoardSquareVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid *)0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderChessBoardLine(const Mat4f &projectionMatrix) {
	chessBoardLineRenderProgram->UseProgram();
	auto projectionLoc = chessBoardLineRenderProgram->GetUniformLocation("projection");
	auto lineWidthLoc = chessBoardLineRenderProgram->GetUniformLocation("pixelLineWidth");

	float lineWidth = chessBoardLineWidth;
	AlwaysAssert(lineWidth > 0.1, "invalid line width");
	glUniform1f(lineWidthLoc, lineWidth);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMatrix.data());

	glLineWidth(chessBoardLineWidth);
	glBindVertexArray(chessBoardLineVAO);
	glDrawArrays(GL_LINES, 0, chessBoardLineVector.size() / 2);
	glBindVertexArray(0);
}

void RenderChess(const Mat4f &projectionMatrix) {
	chessRenderProgram->UseProgram();

	Vec2f textureSpan{ 2.0f * chessRadius / (float)chessTextureWidth,
					   2.0f * chessRadius / (float)chessTextureHeight };

	AlwaysAssert(chessRingOuterFactor >= chessRingInnerFactor, "invalid chess ring factors");
	float ringInnerRadius = chessRingInnerFactor * chessRadius;
	float ringOuterRadius = chessRingOuterFactor * chessRadius;

	auto projectionLoc = chessRenderProgram->GetUniformLocation("projection");
	auto radiusLoc = chessRenderProgram->GetUniformLocation("radius");
	auto colorLoc = chessRenderProgram->GetUniformLocation("chessColor");
	auto textureSpanLoc = chessRenderProgram->GetUniformLocation("textureSpan");
	auto innerRingLoc = chessRenderProgram->GetUniformLocation("ringRadiusInner");
	auto outerRingLoc = chessRenderProgram->GetUniformLocation("ringRadiusOuter");
	auto ringColorLoc = chessRenderProgram->GetUniformLocation("ringColor");

	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMatrix.data());
	glUniform1f(radiusLoc, chessRadius);
	glUniform1f(innerRingLoc, ringInnerRadius);
	glUniform1f(outerRingLoc, ringOuterRadius);
	glUniform3fv(colorLoc, 1, chessColor.data());
	glUniform2fv(textureSpanLoc, 1, textureSpan.data());
	glUniform3fv(ringColorLoc, 1, chessRingColor.data());

	for (const auto &chess : _XiangQiUIBoard->GetChessMap()) {
		// modify the color of the chess
		if (chess.second->GetOwner() == XiangQiChessOwner::RED) {
			glUniform3fv(ringColorLoc, 1, chessRedTextColor.data());
		}
		else {
			glUniform3fv(ringColorLoc, 1, chessBlackTextColor.data());
		}

		Vec2f position = GetChessBoardPointPosition(chess.first);
		const auto &texturePos = chessTextureTopLeftVector->operator[](chess.second->GetHashCode());

		// form temp array
		float data[4] = {position[0], position[1], texturePos[0], texturePos[1]};

		glBindTexture(GL_TEXTURE_2D, chessTextureID);
		// buffer data and draw
		glBindBuffer(GL_ARRAY_BUFFER, chessVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float), data);

		glBindVertexArray(chessVAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void RenderChessText(const Mat4f &projectionMatrix) {
	FontInfo fontInfo;
	fontInfo.windowSize = Vec2f{ (float)myWindowWidth, (float)myWindowHeight };

	auto GetOffset = [&](const wstring &str)->Vec2f {
		auto targetChar = str[0];
		auto charIndex = fontRender->GetGlyphIndex(targetChar);
		if (!fontRender->HasGlyph(charIndex)) {
			fontRender->PreloadGlyph(str);
		}

		const auto &glyph = fontRender->GetGlyph(charIndex);


		float x = (float)glyph.size[0] / 2.0f;
		//float y = (float)(glyph.size[1] - glyph.bearing[1]) / 2.0f;
		float y = (float)(glyph.bearing[1]) / 2.0f;
		Vec2f offset{ x, y };
		offset.array() *= fontInfo.GetSizeScale().array();
		return offset;

	};

	for (const auto &iter : _XiangQiUIBoard->GetChessMap()) {
		auto chessOwner = iter.second->GetOwner();
		const vector<wstring> *strPointer = nullptr;

		if (chessOwner == XiangQiChessOwner::RED) {
			strPointer = &chessRedText;
			fontInfo.foregroundColor = chessRedTextColor;
			fontInfo.transformation = Mat4f::Identity();
		}
		else {
			strPointer = &chessBlackText;
			fontInfo.foregroundColor = chessBlackTextColor;
			fontInfo.transformation = GetRotationMatrix<float>((float)M_PI, Vec3f::UnitZ());
		}

		// scale the font 
		fontInfo.transformation *= GetScaleMatrix<float>(1.1f, 1.1f, 1.0f);

		auto chessIndex = static_cast<Integer>(iter.second->GetChessType());
		const auto &text = (*strPointer)[chessIndex];

		Vec2f offset = GetOffset(text);
		Vec2f center = GetChessBoardPointPosition(iter.first);

		fontRender->RenderPlainText(text, center - offset, fontInfo);
	}
}

void Render() {

	glfwPollEvents();
	glClearColor(chessBoardBackgroundColor[0], chessBoardBackgroundColor[1], chessBoardBackgroundColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Mat4f projectionMatrix = GetOrthoMatrix(0.0f, (float)myWindowWidth, 0.0f, (float)myWindowHeight);

	RenderChessBoardSquare(projectionMatrix);
	RenderChessBoardLine(projectionMatrix);
	RenderChess(projectionMatrix);
	RenderChessText(projectionMatrix);



	glfwSwapBuffers(myWindow);
}

void RunXiangQiUI() {
	AlwaysAssert(programInitialized, "XiangQiUI is not initialized");

	while (!glfwWindowShouldClose(myWindow)) {
		Render();
	}

	glfwTerminate();
}