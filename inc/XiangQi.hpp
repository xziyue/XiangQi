// provides all functionality of Xiang Qi gameplay

#ifndef XIANGQI_XIANGQI_HPP
#define XIANGQI_XIANGQI_HPP


#include "XiangQiDef.hpp"
#include "XiangQiChessDef.hpp"

XiangQiChessPtrType CreateChess(XiangQiChessOwner owner, XiangQiChessType chessType);

using XiangQiChessStateType = typename XiangQiBoard::ChessMapType::value_type;


#endif //XIANGQI_XIANGQI_HPP
