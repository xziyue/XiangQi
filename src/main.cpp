#include "CoreDef.hpp"
#include "XiangQiUI.hpp"

#include <cstdlib>

int main() {
    //XiangQiBoard board{};
    //cout << board.GetStringArt() << "\n\n";
    //board.MakeMove(XiangQiPosType{1,2}, XiangQiPosType{0,7});
    //ParseXPM(woodTextureXPMData);

    InitializeXiangQiUI();
    RunXiangQiUI();

    //system("pause");
    return 0;
}