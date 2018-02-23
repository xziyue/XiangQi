#include "XiangQiChessDef.hpp"

//region Helper Arrays

// the steps that a Shuai can take
const array<XiangQiPosType, 4> _ShuaiSteps{
        XiangQiPosType{1, 0},
        XiangQiPosType{0, 1},
        XiangQiPosType{-1, 0},
        XiangQiPosType{0, -1}
};

const array<XiangQiPosType, 4> redPalaceCornerPositions{
        XiangQiPosType{3, 0},
        XiangQiPosType{5, 0},
        XiangQiPosType{3, 2},
        XiangQiPosType{5, 2}
};

const XiangQiPosType redPalaceCenterPosition{4, 1};

const array<XiangQiPosType, 4> _XiangSteps{
        XiangQiPosType{2, 2},
        XiangQiPosType{2, -2},
        XiangQiPosType{-2, 2},
        XiangQiPosType{-2, -2},
};

const array<XiangQiPosType, 4> _XiangHalfWaySteps{
        XiangQiPosType{1, 1},
        XiangQiPosType{1, -1},
        XiangQiPosType{-1, 1},
        XiangQiPosType{-1, -1},
};

// the possible positions of a red Xiang
const array<XiangQiPosType, 7> redXiangPossiblePositions{
        XiangQiPosType{2, 0},
        XiangQiPosType{4, 2},
        XiangQiPosType{6, 0},
        XiangQiPosType{8, 2},
        XiangQiPosType{6, 4},
        XiangQiPosType{2, 4},
        XiangQiPosType{0, 2},
};

const array<const array<XiangQiPosType, 2>, 4> _MaMovementGroups{
        array<XiangQiPosType, 2>{ XiangQiPosType{-2, 1}, XiangQiPosType{-2, -1}},
        array<XiangQiPosType, 2>{ XiangQiPosType{-1, 2}, XiangQiPosType{1, 2}},
        array<XiangQiPosType, 2>{ XiangQiPosType{2, 1}, XiangQiPosType{2, -1}},
        array<XiangQiPosType, 2>{ XiangQiPosType{-1, -2}, XiangQiPosType{1, -2}}
};

const array<XiangQiPosType, 4> _MaMovementLocks{
        XiangQiPosType{-1, 0},
        XiangQiPosType{0, 1},
        XiangQiPosType{1, 0},
        XiangQiPosType{0, -1}
};

const XiangQiPosType redBingStepInOwnLand{0, 1};
const XiangQiPosType blackBingStepInOwnLand{0, -1};

const array<XiangQiPosType,3> redBingStepsInEnemyLand{
        redBingStepInOwnLand,
        XiangQiPosType{1, 0},
        XiangQiPosType{-1, 0}
};

const array<XiangQiPosType,3> blackBingStepsInEnemyLand{
        blackBingStepInOwnLand,
        XiangQiPosType{1, 0},
        XiangQiPosType{-1, 0}
};

//endregion

Shuai::Shuai(XiangQiChessOwner owner) : XiangQiChessBase{owner} {

}

XiangQiChessType Shuai::GetChessType() const {
    return XiangQiChessType::SHUAI;
}

XiangQiPackedPosType Shuai::GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const {
    assert(this->IsChessPosValid(pos));

    XiangQiPackedPosType result;
    for (const auto &step : _ShuaiSteps) {
        XiangQiPosType newPos = pos + step;
        // the new position must be inside the palace
        if (this->IsChessPosValid(newPos)) {

            // if there is chess on the new position
            if (board.HasChessOnPos(newPos)) {
                // if it is not an enemy, then it can not go there
                if (!board.IsChessEnemy(this->GetOwner(), newPos)) {
                    continue;
                }
            }

            // if the new position cause the Shuai to face the enemy Shuai directly,
            // it is invalid as well
            // find the first chess on north and south direction. If enemy Shuai is found,
            // waive this step
            array<XiangQiPosType, 2> firstChessPos;
            firstChessPos[0] = board.GetFirstChessPosOnDirection(pos, XiangQiChessDirection::NORTH);
            firstChessPos[1] = board.GetFirstChessPosOnDirection(pos, XiangQiChessDirection::SOUTH);

            bool isNewPosInvalidDueToMeetingShuai = false;

            for (const auto &firstPos : firstChessPos) {
                if (firstPos != XiangQiBoard::npos) {
                    // if a chess is found
                    const auto &chessPtr = board.GetChessOnPos(pos);

                    // actually the second condition can be canceled
                    if (chessPtr->GetChessType() == XiangQiChessType::SHUAI &&
                        chessPtr->GetOwner() != this->GetOwner()) {
                        isNewPosInvalidDueToMeetingShuai = true;
                        break;
                    }
                }
            }

            if (isNewPosInvalidDueToMeetingShuai) {
                // if Shuai meets directly, then this new position is invalid
                continue;
            }

            // append this step to result
            result.push_back(step);
        }
    }

    return result;
}

bool Shuai::IsChessPosValid(const XiangQiPosType &pos) const {
    // the valid position for a Shuai is the palace
    return XiangQiBoard::IsChessInOwnPalace(this->GetOwner(), pos);
}

Shi::Shi(XiangQiChessOwner owner) : XiangQiChessBase{owner} {

}

XiangQiChessType Shi::GetChessType() const {
    return XiangQiChessType::SHI;
}

bool Shi::IsChessPosValid(const XiangQiPosType &pos) const {
    if (!XiangQiBoard::IsChessInOwnPalace(this->GetOwner(), pos)) {
        return false;
    }
    // it can only be on the corners and the center of the palace
    XiangQiPosType myPos = pos;
    if (this->GetOwner() != XiangQiChessOwner::RED) {
        myPos = XiangQiBoard::GetSwappedBasePos(pos);
    }

    if (myPos == redPalaceCenterPosition) {
        return true;
    } else {
        auto iter = find(redPalaceCornerPositions.begin(), redPalaceCornerPositions.end(), myPos);
        return iter != redPalaceCornerPositions.end();
    }
}

XiangQiPackedPosType Shi::GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const {
    assert(this->IsChessPosValid(pos));
    XiangQiPackedPosType result;

    XiangQiPosType myPos = pos;
    if (this->GetOwner() != XiangQiChessOwner::RED) {
        // change the base of the positions
        myPos = XiangQiBoard::GetSwappedBasePos(pos);
    }
    XiangQiPackedPosType possiblePositions;

    if (myPos == redPalaceCenterPosition) {
        possiblePositions.reserve(redPalaceCornerPositions.size());
        copy(redPalaceCornerPositions.begin(), redPalaceCornerPositions.end(), back_inserter(possiblePositions));
    } else {
        possiblePositions.push_back(redPalaceCenterPosition);
    }

    for (const auto &newPos : possiblePositions) {
        XiangQiPosType actualPos = newPos;
        if (this->GetOwner() != XiangQiChessOwner::RED) {
            // change the base of positions back
            actualPos = XiangQiBoard::GetSwappedBasePos(newPos);
        }
        if (!board.HasChessOnPos(actualPos)) {
            // if there is none, append the STEP TAKEN to result
            result.emplace_back(actualPos - pos);
        } else {
            // if the chess is an enemy, append the step as well
            if (board.IsChessEnemy(this->GetOwner(), actualPos)) {
                result.emplace_back(actualPos - pos);
            }
        }
    }

    return result;
}

Xiang::Xiang(XiangQiChessOwner owner) : XiangQiChessBase{owner} {

}

XiangQiChessType Xiang::GetChessType() const {
    return XiangQiChessType::XIANG;
}

XiangQiPackedPosType Xiang::GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const {
    XiangQiPackedPosType result;

    for (Integer i = 0; i < (Integer) _XiangSteps.size(); ++i) {
        const auto &step = _XiangSteps[i];
        XiangQiPosType newPos = pos + step;

        if (!XiangQiBoard::IsChessPosInBoard(pos)) {
            continue;
        }

        // Xiang can not step into enemy territory
        if (XiangQiBoard::IsChessInEnemyLand(this->GetOwner(), newPos)) {
            continue;
        }

        // if there is a chess half way, Xiang can not go there
        XiangQiPosType halfWayPos = pos + _XiangHalfWaySteps[i];
        if (board.HasChessOnPos(halfWayPos)) {
            continue;
        }

        if (board.HasChessOnPos(newPos)) {
            // if it is a friendly chess, can not go there
            if (!board.IsChessEnemy(this->GetOwner(), newPos)) {
                continue;
            }
        }

        result.push_back(step);
    }

    return result;
}

bool Xiang::IsChessPosValid(const XiangQiPosType &pos) const {
    if (!XiangQiBoard::IsChessPosInBoard(pos)) {
        return false;
    }

    XiangQiPosType myPos = pos;
    if (this->GetOwner() != XiangQiChessOwner::RED) {
        myPos = XiangQiBoard::GetSwappedBasePos(pos);
    }

    // the chess should not be in enemy land
    if (XiangQiBoard::IsChessInEnemyLand(XiangQiChessOwner::RED, myPos)) {
        return false;
    }

    auto iter = find(redXiangPossiblePositions.begin(), redXiangPossiblePositions.end(), myPos);
    return iter != redXiangPossiblePositions.end();
}

Ju::Ju(XiangQiChessOwner owner) : XiangQiChessBase{owner} {

}

XiangQiChessType Ju::GetChessType() const {
    return XiangQiChessType::JU;
}

XiangQiPackedPosType Ju::GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const {
    XiangQiPackedPosType result;

    // there are four directions in XiangQiChessDirection in total
    for (Integer i = 0; i < 4; ++i) {
        auto dir = static_cast<XiangQiChessDirection>(i);

        XiangQiPosType newPos = pos;
        while (true) {
            newPos = GoTowardsDirection(newPos, dir);

            if (!XiangQiBoard::IsChessPosInBoard(newPos)) {
                break;
            }

            // if there is a chess on its path...
            if (board.HasChessOnPos(newPos)) {
                // if it is an enemy chess, append it to the results as well
                if (board.IsChessEnemy(this->GetOwner(), newPos)) {
                    result.emplace_back(newPos - pos);
                }
                // exit loop
                break;
            }

            result.emplace_back(newPos - pos);
        }

    }

    return result;
}

bool Ju::IsChessPosValid(const XiangQiPosType &pos) const {
    return XiangQiBoard::IsChessPosInBoard(pos);
}

Pao::Pao(XiangQiChessOwner owner) : XiangQiChessBase{owner} {

}

XiangQiChessType Pao::GetChessType() const {
    return XiangQiChessType::PAO;
}

XiangQiPackedPosType Pao::GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const {
    XiangQiPackedPosType result;

    // there are four directions in XiangQiChessDirection in total
    for (Integer i = 0; i < 4; ++i) {
        auto dir = static_cast<XiangQiChessDirection>(i);

        XiangQiPackedPosType findResult = move(board.GetAllChessPosOnDirection(pos, dir, 2));

        // if there is nothing on this direction, add all positions as steps
        if (findResult.empty()) {

            XiangQiPosType newPos = pos;
            while (true) {
                newPos = GoTowardsDirection(newPos, dir);

                if (!XiangQiBoard::IsChessPosInBoard(newPos)) {
                    break;
                }

                result.emplace_back(newPos - pos);
            }

        } else {
            // if the size of findResult is 1 or two, they both perform the same process
            XiangQiPosType newPos = pos;
            while (true) {
                newPos = GoTowardsDirection(newPos, dir);
                if (newPos == findResult.front()) {
                    break;
                }
                result.emplace_back(newPos - pos);
            }

        }

        // if there are two chess along this direction, Pao can make its move
        // towards the second one, should it is an enemy chess
        if (findResult.size() == 2) {
            if (board.IsChessEnemy(this->GetOwner(), findResult.back())) {
                result.emplace_back(findResult.back() - pos);
            }
        }

    }

    return result;
}

bool Pao::IsChessPosValid(const XiangQiPosType &pos) const {
    return XiangQiBoard::IsChessPosInBoard(pos);
}

Ma::Ma(XiangQiChessOwner owner) : XiangQiChessBase(owner) {

}

XiangQiChessType Ma::GetChessType() const {
    return XiangQiChessType::MA;
}

XiangQiPackedPosType Ma::GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const {
    XiangQiPackedPosType result;

    for(Integer i = 0; i < (Integer)_MaMovementGroups.size(); ++i){

        XiangQiPosType lockPos = pos + _MaMovementLocks[i];
        // if there is a chess on lock position, then the entire movement group
        // can no longer be performed
        if(board.HasChessOnPos(lockPos)){
            continue;
        }

        // analyze each step in the movement group
        for(const auto &step : _MaMovementGroups[i]){
            XiangQiPosType newPos = pos + step;

            if(!XiangQiBoard::IsChessPosInBoard(newPos)){
                continue;
            }

            if(board.HasChessOnPos(newPos)){
                if(!board.IsChessEnemy(this->GetOwner(), newPos)){
                    continue;
                }
            }

            result.push_back(step);
        }

    }

    return result;
}

bool Ma::IsChessPosValid(const XiangQiPosType &pos) const {
    return XiangQiBoard::IsChessPosInBoard(pos);
}

Bing::Bing(XiangQiChessOwner owner) : XiangQiChessBase{owner} {

}

XiangQiChessType Bing::GetChessType() const {
    return XiangQiChessType ::BING;
}

XiangQiPackedPosType Bing::GetValidSteps(const XiangQiBoard &board, const XiangQiPosType &pos) const {
    XiangQiPackedPosType result;

    const XiangQiPosType *stepInOwnLand = nullptr;
    const array<XiangQiPosType,3> *stepsInEnemyLand = nullptr;

    if(this->GetOwner() == XiangQiChessOwner::RED){
        stepInOwnLand = &redBingStepInOwnLand;
        stepsInEnemyLand = &redBingStepsInEnemyLand;
    } else{
        stepInOwnLand = &blackBingStepInOwnLand;
        stepsInEnemyLand = &blackBingStepsInEnemyLand;
    }

    // check whether the new position is valid, if so, append
    // the step to result
    auto CheckAndAppend = [&](const XiangQiPosType &newPos){
        if(XiangQiBoard::IsChessPosInBoard(newPos)){
            if(board.HasChessOnPos(newPos)){
                if(board.IsChessEnemy(this->GetOwner(), newPos)){
                    result.emplace_back(newPos - pos);
                }
            }else{
                result.emplace_back(newPos - pos);
            }
        }
    };

    if(XiangQiBoard::IsChessInEnemyLand(this->GetOwner(), pos)){
        for(const auto &step : *stepsInEnemyLand){
            XiangQiPosType newPos = pos + step;
            CheckAndAppend(newPos);
        }
    }else{
        XiangQiPosType newPos = pos + *stepInOwnLand;
        CheckAndAppend(newPos);
    }


    return result;
}

bool Bing::IsChessPosValid(const XiangQiPosType &pos) const {
    return XiangQiBoard::IsChessPosInBoard(pos);
}

