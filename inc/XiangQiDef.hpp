// Defines the chess and rules of Xiang Qi

#ifndef XIANGQI_XIANGQIDEF_HPP
#define XIANGQI_XIANGQIDEF_HPP

#include "CoreDef.hpp"

//region Type Definitions

// a chess is either owned by the red side or the black side
enum class XiangQiChessOwner : char {
    RED,
    BLACK
};

enum class XiangQiChessType : char {
    SHUAI,
    SHI,
    XIANG,
    JU,
    PAO,
    MA,
    BING,
    __COUNTER // just for counting the number of element
};

// directions that most types of chess can take
enum class XiangQiChessDirection : char {
    NORTH,
    SOUTH,
    WEST,
    EAST,
};

using XiangQiPosType = Vec2i;
using XiangQiPackedPosType = AVector<Vec2i>;

class XiangQiBoard;

class XiangQiChessBase;

using XiangQiChessPtrType = unique_ptr<XiangQiChessBase>;

using XiangQiStateElementType = tuple<XiangQiChessOwner, XiangQiPosType, XiangQiChessType>;
//endregion

// returns the position after going towards that direction
XiangQiPosType GoTowardsDirection(const XiangQiPosType &pos, const XiangQiChessDirection &dir);


class XiangQiChessBase {
public:
    // initialize the base with its owner
    // note that in Xiang Qi, the owner of a chess will never change
    XiangQiChessBase(XiangQiChessOwner owner);

	XiangQiChessBase(const XiangQiChessBase &other) = default;

	XiangQiChessBase &operator = (const XiangQiChessBase &other) = default;

    virtual ~XiangQiChessBase() = default;

    XiangQiChessOwner GetOwner() const;

	Integer GetHashCode() const;

	void SetHashCode(Integer hashCode);

    virtual bool IsChessPosValid(const XiangQiPosType &pos) const;

    virtual XiangQiChessType GetChessType() const;

    // returns what step the chess can take
    // the result will take into every factor into account, including existing chess on the board
    // and Xiang Qi rules.
    virtual XiangQiPackedPosType GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const;

private:
    XiangQiChessOwner __myOwner = XiangQiChessOwner::RED;
	Integer __myHashCode = 0;
};

//region Predefined Positions

extern const XiangQiPosType redPalaceBotLeftPos;
extern const XiangQiPosType redPalaceBotRightPos;
extern const XiangQiPosType redPalaceTopRightPos;
extern const XiangQiPosType redPalaceTopLeftPos;
extern const XiangQiPosType blackPalaceBotLeftPos;
extern const XiangQiPosType blackPalaceTopRightPos;
extern const XiangQiPosType redLandBotLeftPos;
extern const XiangQiPosType redLandTopRightPos;
extern const XiangQiPosType blackLandBotLeftPos;
extern const XiangQiPosType blackLandTopRightPos;
extern const XiangQiPosType _XiangQiBoardBotLeftPos;
extern const XiangQiPosType _XiangQiBoardTopRightPos;


//endregion

//region Predefined Xiang Qi State

// the initial state of the red side of a standard Xiang Qi game
extern const vector<XiangQiStateElementType> redInitialXiangQiBoardState;

//endregion

//region String Art Definitions

// string art definitons
enum class StringArtScanLineStyle : char{
    VERTICAL,
    HORIZONTAL,
    WHITESPACE,
    NONE
};

extern const vector<string> chessBoardStringArtMap;

//endregion

//region Chess Board Event Log Definition

struct EventLogChessType{
    XiangQiChessOwner owner;
    XiangQiChessType type;
};

struct EventLogFromPos{
    XiangQiPosType value;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

struct EventLogToPos{
    XiangQiPosType value;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

struct EventLogHasBeatenChess{
    bool value;
};

struct EventLogBetenChessInfo : public EventLogChessType{};

using EventLogRecord = tuple<
        EventLogChessType,
        EventLogFromPos,
        EventLogToPos,
        EventLogHasBeatenChess,
        EventLogBetenChessInfo
>;

//endregion

// assume the lower part of the chess board is red territory, and the upper part is black territory
class XiangQiBoard {
// type definitions
public:
    using ChessMapType = AMap<XiangQiPosType, XiangQiChessPtrType>;

    using EventLogType = AList<EventLogRecord>;

public:
    XiangQiBoard(bool setToInitialState = true, bool keepingEventLog = true);

    XiangQiBoard(const XiangQiBoard &other);

    XiangQiBoard(XiangQiBoard &&other);

    XiangQiBoard &operator = (const XiangQiBoard &other);

    XiangQiBoard &operator = (XiangQiBoard &&other);

public:



    // checks whether the chess on the given position is an enemy
    bool IsChessEnemy(XiangQiChessOwner myChessOwner, const XiangQiPosType &posToCheck) const;

    bool HasChessOnPos(const XiangQiPosType &pos) const;

    // returns all chess on the direction given, starting from the position given
    XiangQiPackedPosType GetAllChessPosOnDirection(const XiangQiPosType &pos, XiangQiChessDirection dir,
                                                   Integer searchDepth = -1) const;

    // returns the position of the first chess on a given direction starting from the given position.
    // returns npos if there is none
    XiangQiPosType GetFirstChessPosOnDirection(const XiangQiPosType &pos, XiangQiChessDirection dir) const;

    // gets the chess on the given position
    const XiangQiChessPtrType& GetChessOnPos(const XiangQiPosType &pos) const;

    // set the chess on the board to the initial state
    // will clear event log
    void SetToInitialState();

    // get the string representation of the board
    string GetStringArt(StringArtScanLineStyle scanLineStyle = StringArtScanLineStyle::VERTICAL) const;

    void SetIfKeepEventLog(bool status);

    // get whether the chess board is keeping event log
    bool GetIfKeepEventLog() const;

    const EventLogType& GetEventLog() const;

	const Vec2i &GetChessHashCodeRange() const;

    // will clear event log
    void ClearAllChess();

    const ChessMapType &GetChessMap() const;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

// static function and variables
public:

    // returns the position regarding the black side as the lower half on the board
    static XiangQiPosType GetSwappedBasePos(const XiangQiPosType &pos);

    static bool IsChessInOwnPalace(XiangQiChessOwner owner, const XiangQiPosType &pos);

    static bool IsChessInEnemyLand(XiangQiChessOwner owner, const XiangQiPosType &pos);

    static bool IsChessPosInBoard(const XiangQiPosType &pos);

    static const XiangQiPosType npos;

    static constexpr const Integer boardWidth = 9;
    static constexpr const Integer boardHeight = 10;

// private variables
private:
    bool __keepEventLog = true;

// private methods
private:

	void ClearEventLog();

	void MakeMove(const XiangQiPosType &chessPos, const XiangQiPosType &step);

    static bool __IsPositionInRectangle(const XiangQiPosType &pos, const XiangQiPosType &recBotLeft,
                                        const XiangQiPosType &recTopRight);

	void __GenerateHashCode();
// important private containers
private:
	Vec2i __hashCodeRange{ 0,0 };

    ChessMapType __myChessOnBoard;

    EventLogType __myEventLog;
};


#endif //XIANGQI_XIANGQIDEF_HPP
