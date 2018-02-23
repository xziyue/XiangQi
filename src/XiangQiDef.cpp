#include "XiangQiDef.hpp"

// defines npos for XiangQiBoard
const XiangQiPosType XiangQiBoard::npos{-1, -1};

//region Predefined Positions

const XiangQiPosType redPalaceBotLeftPos{3, 0};
const XiangQiPosType redPalaceBotRightPos{5, 0};
const XiangQiPosType redPalaceTopRightPos{5, 2};
const XiangQiPosType redPalaceTopLeftPos{3, 2};
const XiangQiPosType blackPalaceBotLeftPos{3, 7};
const XiangQiPosType blackPalaceTopRightPos{5, 9};
const XiangQiPosType redLandBotLeftPos{0, 0};
const XiangQiPosType redLandTopRightPos{8, 4};
const XiangQiPosType blackLandBotLeftPos{0, 5};
const XiangQiPosType blackLandTopRightPos{8, 9};
const XiangQiPosType _XiangQiBoardBotLeftPos{0, 0};
const XiangQiPosType _XiangQiBoardTopRightPos{8, 9};

//endregion

//region Predefined Xiang Qi State

// the initial state of the red side of a standard Xiang Qi game
const vector<XiangQiStateElementType> redInitialXiangQiBoardState{
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{0, 0}, XiangQiChessType::JU},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{1, 0}, XiangQiChessType::MA},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{2, 0}, XiangQiChessType::XIANG},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{3, 0}, XiangQiChessType::SHI},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{4, 0}, XiangQiChessType::SHUAI},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{5, 0}, XiangQiChessType::SHI},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{6, 0}, XiangQiChessType::XIANG},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{7, 0}, XiangQiChessType::MA},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{8, 0}, XiangQiChessType::JU},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{1, 2}, XiangQiChessType::PAO},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{7, 2}, XiangQiChessType::PAO},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{0, 3}, XiangQiChessType::BING},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{2, 3}, XiangQiChessType::BING},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{4, 3}, XiangQiChessType::BING},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{6, 3}, XiangQiChessType::BING},
        XiangQiStateElementType{XiangQiChessOwner::RED, XiangQiPosType{8, 3}, XiangQiChessType::BING},
};

//endregion

const vector<string> chessBoardStringArtMap{
        string{"S"},
        string{"H"},
        string{"X"},
        string{"J"},
        string{"P"},
        string{"M"},
        string{"B"},
};


XiangQiBoard::XiangQiBoard(bool setToInitialState, bool keepingEventLog) : __keepEventLog{keepingEventLog} {
    if (setToInitialState) {
        this->SetToInitialState();
    }
}

XiangQiPosType GoTowardsDirection(const XiangQiPosType &pos, const XiangQiChessDirection &dir) {
    switch (dir) {
        case XiangQiChessDirection::NORTH:
            return pos + XiangQiPosType{0, 1};
        case XiangQiChessDirection::SOUTH:
            return pos + XiangQiPosType{0, -1};
        case XiangQiChessDirection::EAST:
            return pos + XiangQiPosType{1, 0};
        case XiangQiChessDirection::WEST:
            return pos + XiangQiPosType{-1, 0};
        default:
            throw runtime_error{"undefined direction"};
    }
}

XiangQiChessBase::XiangQiChessBase(XiangQiChessOwner owner) : __myOwner{owner} {

}

XiangQiChessOwner XiangQiChessBase::GetOwner() const {
    return this->__myOwner;
}

Integer XiangQiChessBase::GetHashCode() const
{
	return this->__myHashCode;
}

void XiangQiChessBase::SetHashCode(Integer hashCode)
{
	this->__myHashCode = hashCode;
}

XiangQiChessType XiangQiChessBase::GetChessType() const {
    throw runtime_error{"undefined method"};
}

XiangQiPackedPosType XiangQiChessBase::GetValidSteps(const XiangQiBoard &, const XiangQiPosType &) const {
    throw runtime_error{"undefined method"};
}

bool XiangQiChessBase::IsChessPosValid(const XiangQiPosType &) const {
    throw runtime_error{"undefined method"};
}

XiangQiPosType XiangQiBoard::GetSwappedBasePos(const XiangQiPosType &pos) {
    assert(IsChessPosInBoard(pos));
    XiangQiPosType result = pos;
    result[0] = boardWidth - 1 - result[0];
    result[1] = boardHeight - 1 - result[1];
    assert(IsChessPosInBoard(result));
    return result;
}

bool XiangQiBoard::__IsPositionInRectangle(const XiangQiPosType &pos, const XiangQiPosType &recBotLeft,
                                           const XiangQiPosType &recTopRight) {
    assert(less<XiangQiPosType>{}(recBotLeft, recTopRight));
    if (pos[0] >= recBotLeft[0] && pos[1] >= recBotLeft[1]) {
        if (pos[0] <= recTopRight[0] && pos[1] <= recTopRight[1]) {
            return true;
        }
    }
    return false;
}

void XiangQiBoard::__GenerateHashCode()
{
	Integer hashCode = 0;
	for (auto &item : this->__myChessOnBoard) {
		item.second->SetHashCode(hashCode++);
	}
	this->__hashCodeRange = Vec2i{0, hashCode};
}

bool XiangQiBoard::IsChessInEnemyLand(XiangQiChessOwner owner, const XiangQiPosType &pos) {

    if (owner == XiangQiChessOwner::RED) {
        return __IsPositionInRectangle(pos, blackLandBotLeftPos, blackLandTopRightPos);
    } else {
        return __IsPositionInRectangle(pos, redLandBotLeftPos, redLandTopRightPos);
    }
}

bool XiangQiBoard::IsChessInOwnPalace(XiangQiChessOwner owner, const XiangQiPosType &pos) {
    if (owner == XiangQiChessOwner::RED) {
        return __IsPositionInRectangle(pos, redPalaceBotLeftPos, redPalaceTopRightPos);
    } else {
        return __IsPositionInRectangle(pos, blackPalaceBotLeftPos, blackPalaceTopRightPos);
    }
}

bool XiangQiBoard::IsChessPosInBoard(const XiangQiPosType &pos) {
    return __IsPositionInRectangle(pos, _XiangQiBoardBotLeftPos, _XiangQiBoardTopRightPos);
}

bool XiangQiBoard::HasChessOnPos(const XiangQiPosType &pos) const {
    return this->__myChessOnBoard.find(pos) != this->__myChessOnBoard.end();
}

bool XiangQiBoard::IsChessEnemy(XiangQiChessOwner myChessOwner, const XiangQiPosType &posToCheck) const {
    assert(this->HasChessOnPos(posToCheck));
    const auto &chess = this->GetChessOnPos(posToCheck);
    return myChessOwner != chess->GetOwner();
}

XiangQiPackedPosType XiangQiBoard::GetAllChessPosOnDirection
        (const XiangQiPosType &pos, XiangQiChessDirection dir, Integer searchDepth) const {
    assert(searchDepth >= -1);
    XiangQiPackedPosType result;

    // depth is unsigned
    auto depth = static_cast<size_t>(searchDepth);

    XiangQiPosType newPos = pos;
    while (true) {
        if (result.size() >= depth) {
            break;
        }

        newPos = GoTowardsDirection(newPos, dir);

        // if newPos is out of range, break
        if (!IsChessPosInBoard(newPos)) {
            break;
        }

        // if there is a chess on this position, append it to result
        if (this->HasChessOnPos(newPos)) {
            result.push_back(newPos);
        }
    }

    return result;
}

XiangQiPosType XiangQiBoard::GetFirstChessPosOnDirection
        (const XiangQiPosType &pos, XiangQiChessDirection dir) const {
    XiangQiPosType result = npos;

    XiangQiPosType newPos = pos;
    while (true) {
        newPos = GoTowardsDirection(newPos, dir);

        // if newPos is out of range, break and return npos
        if (!IsChessPosInBoard(newPos)) {
            break;
        }

        // if a chess is found, returns its position
        if (this->HasChessOnPos(newPos)) {
            result = newPos;
            break;
        }
    }

    return result;
}

const XiangQiChessPtrType &XiangQiBoard::GetChessOnPos(const XiangQiPosType &pos) const {
    assert(this->HasChessOnPos(pos));
    return this->__myChessOnBoard.at(pos);
}

string XiangQiBoard::GetStringArt(StringArtScanLineStyle scanLineStyle) const {
    array<array<string, boardHeight>, boardWidth> stringStorage;

    for (Integer x = 0; x < boardWidth; ++x) {
        for (Integer y = 0; y < boardHeight; ++y) {
            XiangQiPosType currentPos{x, y};
            string &currentString = stringStorage[x][y];

            if (this->HasChessOnPos(currentPos)) {
                const auto &chessInfo = this->GetChessOnPos(currentPos);
                auto indexOfChessType = static_cast<Integer>(chessInfo->GetChessType());
                currentString += chessBoardStringArtMap[indexOfChessType];
                if (chessInfo->GetOwner() == XiangQiChessOwner::RED) {
                    currentString += string{"R"};
                } else {
                    currentString += string{"B"};
                }
            } else {
                currentString += string{"--"};
            }

        }
    }

    stringstream sstr;


    // rendering from the last line
    for (auto y = boardHeight - 1; y > -1; --y) {

        // separates the chess board at the middle
        if (y == (boardHeight) / 2 - 1) {
            // find the width of one line
            Integer lineWidth = (Integer)sstr.str().find('\n');
            for (Integer j = 0; j < 2; ++j) {
                for (Integer i = 0; i < lineWidth; ++i) {
                    sstr << "*";
                }
                sstr << "\n";
            }
        }

        for (auto x = 0; x < boardWidth; ++x) {
            sstr << stringStorage[x][y];

            switch (scanLineStyle) {
                case StringArtScanLineStyle::HORIZONTAL:
                    sstr << "-";
                    break;
                case StringArtScanLineStyle::VERTICAL:
                    sstr << "|";
                    break;
                case StringArtScanLineStyle::WHITESPACE:
                    sstr << " ";
                    break;
                default:
                    sstr << "";
                    break;
            }
        }
        if (y != 0) {
            sstr << "\n";
        }
    }

    return sstr.str();
}

void XiangQiBoard::SetIfKeepEventLog(bool status) {
    this->__keepEventLog = status;
}

bool XiangQiBoard::GetIfKeepEventLog() const {
    return this->__keepEventLog;
}

const XiangQiBoard::EventLogType &XiangQiBoard::GetEventLog() const {
    return this->__myEventLog;
}

const Vec2i & XiangQiBoard::GetChessHashCodeRange() const
{
	return this->__hashCodeRange;
}

void XiangQiBoard::ClearEventLog() {
    this->__myEventLog.clear();
}

void XiangQiBoard::MakeMove(const XiangQiPosType &chessPos, const XiangQiPosType &step) {
    XiangQiPosType toPos = chessPos + step;

    auto fromIter = this->__myChessOnBoard.find(chessPos);
    assert(fromIter != this->__myChessOnBoard.end());

#ifndef NDEBUG
    // in debug mode, assert that the step is valid
    auto validSteps = move(fromIter->second->GetValidSteps(*this, chessPos));
    auto findIter = find(validSteps.begin(), validSteps.end(), step);
    assert(findIter != validSteps.end());
#endif

    auto toIter = this->__myChessOnBoard.find(toPos);

    if (this->GetIfKeepEventLog()) {
        EventLogRecord record;
        get<0>(record).owner = fromIter->second->GetOwner();
        get<0>(record).type = fromIter->second->GetChessType();
        get<1>(record).value = chessPos;
        get<2>(record).value = toPos;

        // create a temp pointer
        XiangQiChessPtrType tempPtr{fromIter->second.release()};
        // erase the chess on this position
        this->__myChessOnBoard.erase(fromIter);

        if (toIter == this->__myChessOnBoard.end()) {
            // if there is no chess on toPos position
            get<3>(record).value = false;
            this->__myChessOnBoard.insert(make_pair(toPos, move(tempPtr)));
        } else {
            get<3>(record).value = true;
            get<4>(record).owner = toIter->second->GetOwner();
            get<4>(record).type = toIter->second->GetChessType();
            this->__myChessOnBoard.erase(toIter);
            this->__myChessOnBoard.insert(make_pair(toPos, move(tempPtr)));
        }

        this->__myEventLog.emplace_back(move(record));

    } else {
        // create a temp pointer
        XiangQiChessPtrType tempPtr{fromIter->second.release()};
        // erase the chess on this position
        this->__myChessOnBoard.erase(fromIter);

        if (toIter == this->__myChessOnBoard.end()) {
            // if there is no chess on 'toPos' position
            this->__myChessOnBoard.insert(make_pair(toPos, move(tempPtr)));
        } else {
            // if there is chess on 'toPos' position
            this->__myChessOnBoard.erase(toIter);
            this->__myChessOnBoard.insert(make_pair(toPos, move(tempPtr)));
        }
    }
}

const XiangQiBoard::ChessMapType &XiangQiBoard::GetChessMap() const {

    return this->__myChessOnBoard;
}

void XiangQiBoard::ClearAllChess() {
    this->__myChessOnBoard.clear();
    this->ClearEventLog();
}









