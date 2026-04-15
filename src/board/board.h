#pragma once
#include <string>


const size_t boardRows = 8;
const size_t boardCols = 8;

enum PieceType
{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NONE
};

enum Color
{
    WHITE,
    BLACK,
    NO_COLOR
};

struct Piece
{
    PieceType type;
    Color color;
    bool hasMoved;
};

struct Square
{
    bool occupied = false;
    Piece piece{NONE, NO_COLOR, false};
    int row = 0;
    int col = 0;
};

struct Board
{
    Square squares[boardRows][boardCols];

    Board();

    void display_board();
    void initialize_board();
};

// Piece to_string functions
std::string to_string(PieceType type);
std::string to_string(Color color);