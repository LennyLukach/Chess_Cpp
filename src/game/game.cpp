#include "game.h"
#include <iostream>
#include <cmath>

Game::Game()
{
    board.initialize_board();
    selectedSquare = nullptr;
    currentTurn = WHITE;
}

void Game::calculateBoardMetrics(int windowWidth, int windowHeight, int &renderedSquareSize, int &widthPadding, int &heightPadding)
{
    int boardSize = (windowHeight < windowWidth) ? windowHeight : windowWidth;
    renderedSquareSize = boardSize / 8;
    int actualBoardSize = renderedSquareSize * 8;
    widthPadding = (windowWidth - actualBoardSize) / 2;
    heightPadding = (windowHeight - actualBoardSize) / 2;
}

bool Game::canPieceReachSquare(int currentRow, int currentCol, int newRow, int newCol)
{
    Square currentSquare = board.squares[currentRow][currentCol];
    Square targetSquare = board.squares[newRow][newCol];
    int colorMultiplier = 0; // this is used to help make movements for pawn depending on its color which is important; white moves 7->0, black moves 0->7
    if (currentSquare.piece.color == WHITE)
    {
        colorMultiplier = 1;
    }
    else if (currentSquare.piece.color == BLACK)
    {
        colorMultiplier = -1;
    }

    // TODO: add pawn promotion
    // TODO: add en passant
    if (currentSquare.piece.type == PAWN)
    {
        if (targetSquare.occupied)
        {
            // pawn can take diagonally
            if (currentRow - newRow == colorMultiplier && abs(currentCol - newCol) == 1)
            {
                return true;
            }
            return false;
        }
        // * everything below this happens if the target square is in front of the pawn
        // pawn can move 2 squares
        else if (!currentSquare.piece.hasMoved && currentRow - newRow == (colorMultiplier * 2) && currentCol == newCol)
        {
            if (!board.squares[newRow + colorMultiplier][currentCol].occupied) // if square in front is not occupied
            {
                return true;
            }
        }
        // pawn can move 1 square
        else if (currentRow - newRow == colorMultiplier && currentCol == newCol)
        {
            return true;
        }
    }
    else if (currentSquare.piece.type == BISHOP)
    {
        if (abs(currentCol - newCol) == abs(currentRow - newRow))
        {
            int rowDirection = (newRow > currentRow) ? 1 : -1;
            int colDirection = (newCol > currentCol) ? 1 : -1;

            int checkRow = currentRow + rowDirection;
            int checkCol = currentCol + colDirection;

            // check diagonals for blocking pieces
            while (checkRow != newRow && checkCol != newCol)
            {
                if (board.squares[checkRow][checkCol].occupied)
                {
                    return false;
                }
                checkRow += rowDirection;
                checkCol += colDirection;
            }
            return true;
        }
    }
    else if (currentSquare.piece.type == KNIGHT)
    {
        int rowChange = abs(currentRow - newRow);
        int colChange = abs(currentCol - newCol);

        if ((rowChange == 1 && colChange == 2) || (rowChange == 2 && colChange == 1))
        {
            return true;
        }
    }
    else if (currentSquare.piece.type == ROOK)
    {
        if (currentRow == newRow || currentCol == newCol)
        {
            int rowDirection = 0;
            int colDirection = 0;

            if (newRow != currentRow)
            {
                rowDirection = (newRow > currentRow) ? 1 : -1;
            }
            else
            {
                colDirection = (newCol > currentCol) ? 1 : -1;
            }

            int checkRow = currentRow + rowDirection;
            int checkCol = currentCol + colDirection;

            // checks if anything in the row or column is blocking
            while (checkRow != newRow || checkCol != newCol)
            {
                if (board.squares[checkRow][checkCol].occupied)
                {
                    return false;
                }
                checkRow += rowDirection;
                checkCol += colDirection;
            }
            return true;
        }
    }
    else if (currentSquare.piece.type == QUEEN)
    {
        // diagonal movement
        if (abs(currentCol - newCol) == abs(currentRow - newRow))
        {
            int rowDirection = (newRow > currentRow) ? 1 : -1;
            int colDirection = (newCol > currentCol) ? 1 : -1;

            int checkRow = currentRow + rowDirection;
            int checkCol = currentCol + colDirection;

            while (checkRow != newRow && checkCol != newCol)
            {
                if (board.squares[checkRow][checkCol].occupied)
                    return false;
                checkRow += rowDirection;
                checkCol += colDirection;
            }
            return true;
        }
        // straight movement
        else if (currentRow == newRow || currentCol == newCol)
        {
            int rowDirection = (newRow != currentRow) ? ((newRow > currentRow) ? 1 : -1) : 0;
            int colDirection = (newCol != currentCol) ? ((newCol > currentCol) ? 1 : -1) : 0;

            int checkRow = currentRow + rowDirection;
            int checkCol = currentCol + colDirection;

            while (checkRow != newRow || checkCol != newCol)
            {
                if (board.squares[checkRow][checkCol].occupied)
                    return false;
                checkRow += rowDirection;
                checkCol += colDirection;
            }
            return true;
        }
    }
    else if (currentSquare.piece.type == KING)
    {
        if (abs(currentRow - newRow) <= 1 && abs(currentCol - newCol) <= 1)
        {
            return true;
        }
    }
    return false;
}

bool Game::movePieceToSquare(int currentRow, int currentCol, int newRow, int newCol)
{
    if (!canPieceReachSquare(currentRow, currentCol, newRow, newCol))
    {
        return false;
    }
    // if piece can reach square, simulate moving to square and checking if it puts piece in check.
    // move to new square
    Piece capturedPiece = board.squares[newRow][newCol].piece;
    bool wasOccupied = board.squares[newRow][newCol].occupied;
    board.squares[newRow][newCol].piece = board.squares[currentRow][currentCol].piece;
    board.squares[newRow][newCol].occupied = true;
    // clear old square
    board.squares[currentRow][currentCol].piece.color = NO_COLOR;
    board.squares[currentRow][currentCol].piece.type = NONE;
    board.squares[currentRow][currentCol].occupied = false;
    for (size_t row = 0; row < std::size(board.squares); ++row)
    {
        for (size_t col = 0; col < std::size(board.squares[0]); ++col)
        {
            if (board.squares[row][col].piece.type == KING && board.squares[row][col].piece.color == currentTurn)
            {
                if (turnPlayerInCheck(row, col))
                {
                    // set old square
                    board.squares[currentRow][currentCol].piece = board.squares[newRow][newCol].piece;
                    board.squares[currentRow][currentCol].occupied = true;
                    // clear new square
                    board.squares[newRow][newCol].piece = capturedPiece;
                    board.squares[newRow][newCol].occupied = wasOccupied;
                    return false;
                }
            }
        }
    }

    return true;
}

bool Game::turnPlayerInCheck(int kingRow, int kingCol)
{
    // if the turn player is in check after a simulated move, this returns true and then undoes the move in Game::movePieceToSquare()
    Color kingColor = board.squares[kingRow][kingCol].piece.color;
    for (size_t row = 0; row < std::size(board.squares); ++row)
    {
        for (size_t col = 0; col < std::size(board.squares[0]); ++col)
        {
            if (board.squares[row][col].piece.color != kingColor && canPieceReachSquare(row, col, kingRow, kingCol))
            {
                return true;
            }
        }
    }
    return false;
}

bool Game::isInCheckmate(int kingRow, int kingCol)
{
    if (!turnPlayerInCheck(kingRow, kingCol))
    {
        return false;
    }

    Color kingColor = board.squares[kingRow][kingCol].piece.color;

    // try every possible move for every friendly piece
    for (size_t fromRow = 0; fromRow < std::size(board.squares); ++fromRow)
    {
        for (size_t fromCol = 0; fromCol < std::size(board.squares); ++fromCol)
        {
            if (!board.squares[fromRow][fromCol].occupied || board.squares[fromRow][fromCol].piece.color != kingColor)
                continue;

            // try moving this piece to every square
            for (size_t toRow = 0; toRow < std::size(board.squares); ++toRow)
            {
                for (size_t toCol = 0; toCol < std::size(board.squares); ++toCol)
                {
                    if (!canPieceReachSquare(fromRow, fromCol, toRow, toCol))
                    {
                        continue;
                    }

                    // simulate the move
                    Piece capturedPiece = board.squares[toRow][toCol].piece;
                    bool wasOccupied = board.squares[toRow][toCol].occupied;
                    board.squares[toRow][toCol].piece = board.squares[fromRow][fromCol].piece;
                    board.squares[toRow][toCol].occupied = true;
                    board.squares[fromRow][fromCol].piece.color = NO_COLOR;
                    board.squares[fromRow][fromCol].piece.type = NONE;
                    board.squares[fromRow][fromCol].occupied = false;

                    // find the king
                    int checkRow = kingRow, checkCol = kingCol;
                    if (board.squares[toRow][toCol].piece.type == KING)
                    {
                        checkRow = toRow;
                        checkCol = toCol;
                    }

                    bool stillInCheck = turnPlayerInCheck(checkRow, checkCol);

                    // undo the move
                    board.squares[fromRow][fromCol].piece = board.squares[toRow][toCol].piece;
                    board.squares[fromRow][fromCol].occupied = true;
                    board.squares[toRow][toCol].piece = capturedPiece;
                    board.squares[toRow][toCol].occupied = wasOccupied;

                    if (!stillInCheck)
                        return false; // not checkmate
                }
            }
        }
    }

    return true; // checkmate
}

void Game::handleClick(int mousePosX, int mousePosY, int windowWidth, int windowHeight)
{
    int renderedSquareSize;
    int widthPadding;
    int heightPadding;

    calculateBoardMetrics(windowWidth, windowHeight, renderedSquareSize, widthPadding, heightPadding);

    int boardPosX = mousePosX - widthPadding;
    int boardPosY = mousePosY - heightPadding;

    int boardSize = renderedSquareSize * 8;

    if (boardPosX < 0 || boardPosX >= boardSize || boardPosY < 0 || boardPosY >= boardSize)
    {
        return; // outside of board
    }

    int col = boardPosX / renderedSquareSize;
    int row = boardPosY / renderedSquareSize;

    if (selectedSquare == nullptr) // selects an unselected square with a piece of the current turn color
    {
        if (board.squares[row][col].occupied && board.squares[row][col].piece.color == currentTurn)
        {
            selectedSquare = &board.squares[row][col];
            // std::cout << "selected square at: (" << row << ", " << col << ")" << std::endl;
        }
    }
    else if (selectedSquare && selectedSquare->row == row && selectedSquare->col == col) // deselect square if clicked again
    {
        selectedSquare = nullptr;
        // std::cout << "unselected square at: (" << row << ", " << col << ")" << std::endl;
    }

    else if (selectedSquare && (selectedSquare->row != row || selectedSquare->col != col)) // user has square selected and clicks another square
    {
        // std::cout << "selected different square at (" << row << ", " << col << ")" << std::endl;
        Square &newSelectedSquare = board.squares[row][col];
        if (newSelectedSquare.piece.color != currentTurn) // does check for target square not having a piece of the same color
        {
            // movePieceToSquare checks if piece can move there
            if (movePieceToSquare(selectedSquare->row, selectedSquare->col, newSelectedSquare.row, newSelectedSquare.col))
            {
                board.squares[newSelectedSquare.row][newSelectedSquare.col].piece.hasMoved = true;
                selectedSquare = nullptr;
                currentTurn = (currentTurn == WHITE) ? BLACK : WHITE; // flip the current turn player (WHITE -> BLACK || BLACK -> WHITE)
                // after x makes a move, current turn is now y so checking if y king is in checkmate
                for (size_t row = 0; row < std::size(board.squares); ++row)
                {
                    for (size_t col = 0; col < std::size(board.squares[0]); ++col)
                    {
                        if (board.squares[row][col].piece.type == KING && board.squares[row][col].piece.color == currentTurn)
                        {
                            if (isInCheckmate(row, col))
                            {
                                gameWinner = (currentTurn == WHITE) ? BLACK : WHITE; // game winner is opposite of the current turn
                                std::cout << gameWinner << " won the game" << std::endl;
                            }
                        }
                    }
                }
            }
        }
        else if (newSelectedSquare.piece.color == currentTurn) // user clicks on square with a matching piece, switch to that square
        {
            // TODO: probably could add castle and shit here
            selectedSquare = &newSelectedSquare;
        }
    }
}