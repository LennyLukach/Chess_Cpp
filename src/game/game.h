#pragma once
#include "../board/board.h"

class Game
{
public:
    Board board;
    Square* selectedSquare;
    Color currentTurn;
    Color gameWinner = NO_COLOR;

    Game();

    void handleClick(int mousePosX, int mousePosY, int windowWidth, int windowHeight);

private:
    void calculateBoardMetrics(int windowWidth, int windowHeight, int &renderedSquareSize, int &widthPadding, int &heightPadding);
    bool canPieceReachSquare(int currentRow, int currentCol, int newRow, int newCol);
    bool movePieceToSquare(int currentRow, int currentCol, int newRow, int newCol);
    bool turnPlayerInCheck(int kingRow, int kingCol);
    bool isInCheckmate(int kingRow, int kingCol);
};