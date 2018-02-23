// Defines the behavior of Xiang Qi chess

#ifndef XIANGQI_XIANGQICHESSDEF_HPP
#define XIANGQI_XIANGQICHESSDEF_HPP

#include "XiangQiDef.hpp"

class Shuai : public XiangQiChessBase {
public:
    Shuai(XiangQiChessOwner owner);

    virtual XiangQiChessType GetChessType() const;

    virtual XiangQiPackedPosType GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const;

    virtual bool IsChessPosValid(const XiangQiPosType &pos) const;
};

class Shi : public XiangQiChessBase {
public:
    Shi(XiangQiChessOwner owner);

    virtual XiangQiChessType GetChessType() const;

    virtual XiangQiPackedPosType GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const;

    virtual bool IsChessPosValid(const XiangQiPosType &pos) const;

};

class Xiang : public XiangQiChessBase {
public:
    Xiang(XiangQiChessOwner owner);

    virtual XiangQiChessType GetChessType() const;

    virtual XiangQiPackedPosType GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const;

    virtual bool IsChessPosValid(const XiangQiPosType &pos) const;
};

class Ju : public XiangQiChessBase {
public:
    Ju(XiangQiChessOwner owner);

    virtual XiangQiChessType GetChessType() const;

    virtual XiangQiPackedPosType GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const;

    virtual bool IsChessPosValid(const XiangQiPosType &pos) const;
};

class Pao : public XiangQiChessBase {
public:
    Pao(XiangQiChessOwner owner);

    virtual XiangQiChessType GetChessType() const;

    virtual XiangQiPackedPosType GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const;

    virtual bool IsChessPosValid(const XiangQiPosType &pos) const;
};

class Ma : public XiangQiChessBase {
public:
    Ma(XiangQiChessOwner owner);

    virtual XiangQiChessType GetChessType() const;

    virtual XiangQiPackedPosType GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const;

    virtual bool IsChessPosValid(const XiangQiPosType &pos) const;
};

class Bing : public XiangQiChessBase{
public:
    Bing(XiangQiChessOwner owner);

    virtual XiangQiChessType GetChessType() const;

    virtual XiangQiPackedPosType GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const;

    virtual bool IsChessPosValid(const XiangQiPosType &pos) const;
};



#endif //XIANGQI_XIANGQICHESSDEF_HPP
