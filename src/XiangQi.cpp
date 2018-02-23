#include "XiangQi.hpp"


XiangQiChessPtrType CreateChess(XiangQiChessOwner owner, XiangQiChessType chessType) {

    switch (chessType) {
        case XiangQiChessType::SHUAI:
            return make_unique<Shuai>(owner);
        case XiangQiChessType::SHI:
            return make_unique<Shi>(owner);
        case XiangQiChessType::XIANG:
            return make_unique<Xiang>(owner);
        case XiangQiChessType::JU:
            return make_unique<Ju>(owner);
        case XiangQiChessType::PAO:
            return make_unique<Pao>(owner);
        case XiangQiChessType::MA:
            return make_unique<Ma>(owner);
        case XiangQiChessType::BING:
            return make_unique<Bing>(owner);
        default:
            throw runtime_error{"invalid chess type"};
    }

}

void XiangQiBoard::SetToInitialState() {
    this->__myChessOnBoard.clear();
    this->ClearEventLog();

    for (Integer i = 0; i < (Integer) redInitialXiangQiBoardState.size(); ++i) {
        const auto &currentChessInfo = redInitialXiangQiBoardState[i];

        this->__myChessOnBoard.insert(
                make_pair(get<1>(currentChessInfo), CreateChess(XiangQiChessOwner::RED, get<2>(currentChessInfo))));
        XiangQiPosType swappedPosition = GetSwappedBasePos(get<1>(currentChessInfo));
        this->__myChessOnBoard.insert(
                make_pair(swappedPosition, CreateChess(XiangQiChessOwner::BLACK, get<2>(currentChessInfo))));
    }

	this->__GenerateHashCode();
}

XiangQiBoard::XiangQiBoard(const XiangQiBoard &other) {
    *this = other;
}

XiangQiBoard::XiangQiBoard(XiangQiBoard &&other) {
    *this = move(other);
}

XiangQiBoard &XiangQiBoard::operator=(const XiangQiBoard &other) {
    // copy boolean
    this->__keepEventLog = other.__keepEventLog;

    // clear containers
    this->__myEventLog.clear();
    this->__myChessOnBoard.clear();

	this->__hashCodeRange = other.__hashCodeRange;

    //copy event log
    copy(other.__myEventLog.begin(), other.__myEventLog.end(), back_inserter(this->__myEventLog));

    // copy chess
    for (auto iter = other.__myChessOnBoard.begin(); iter != other.__myChessOnBoard.end(); ++iter) {
        unique_ptr<XiangQiChessBase> newChessPtr = CreateChess(iter->second->GetOwner(), iter->second->GetChessType());
		newChessPtr->SetHashCode(iter->second->GetHashCode());
        this->__myChessOnBoard.insert(make_pair(iter->first, move(newChessPtr)));
    }

    return *this;
}

XiangQiBoard &XiangQiBoard::operator=(XiangQiBoard &&other) {
    this->__keepEventLog = other.__keepEventLog;
    this->__myEventLog = move(other.__myEventLog);
    this->__myChessOnBoard = move(other.__myChessOnBoard);
	this->__hashCodeRange = move(other.__hashCodeRange);

    return *this;
}
