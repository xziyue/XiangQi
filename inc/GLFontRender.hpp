#ifndef DEF_GLFONTRENDER_HPP
#define DEF_GLFONTRENDER_HPP

#include "GLDef.hpp"
#include "GLProgram.hpp"
#include "ResourceManager.hpp"
#include "TransformationMatrix.hpp"


struct GlyphInfo {
    GLuint glTextureID;
    Vec2i size;
    Vec2i bearing;
    Integer advance;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

struct FontInfo {
    Vec3f foregroundColor{1.0f, 1.0f, 1.0f};
    float lineSpread = 1.0f;
    Vec2f windowSize{0.0f, 0.0f};
	Mat4f transformation = Mat4f::Identity();

	Vec2f GetSizeScale() const;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

bool IsFontValid(const char* font);

// make sure to use this class after creating OpenGL context
class GLFontRender {
public:
    GLFontRender() { this->__Initialize(); }

    ~GLFontRender() { this->__Close(); }

    GLFontRender(const GLFontRender &other) = delete;

    GLFontRender &operator=(const GLFontRender &other) = delete;

    void LoadTypeface(const char *filename, Integer pxHeight);

    bool HasTypeface() const;

    void LoadASCIIGlyph();

    FT_UInt GetGlyphIndex(FT_ULong charCode) const;

    bool HasGlyph(FT_UInt glyphIndex) const;

    void LoadGlyph(FT_UInt glyphIndex);

    void RenderPlainText(const wstring &text, const Vec2f &pos, const FontInfo &info);

    void ClearGlyph();

	void ClearTypeface();

    void PreloadGlyph(const wstring &text);

    const GlyphInfo &GetGlyph(FT_UInt glyphIndex) const;

private:
    void __DeleteGlyphs();

    void __DeleteTypeface();

    void __Initialize();

    void __Close();

private:
    FT_Library __freetypeLib;

    FT_Face __freetypeTypeface;

    unique_ptr <GLProgramManager> __glProgMan;

    map <FT_UInt, GlyphInfo> __glyphs;

    bool __hasTypeface = false;

    GLuint __myVAO, __myVBO;

    float __baseLinespread = 0.0f;
};

#endif