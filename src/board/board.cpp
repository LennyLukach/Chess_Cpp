#include "board.h"
#include <string>
#include <iostream>

Board::Board() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            squares[row][col].row = row;
            squares[row][col].col = col;
        }
    }
}

void Board::display_board() {
     for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Square& sq = this->squares[row][col];
            if (sq.occupied) {
                std::string name = to_string(sq.piece.type);
                if (sq.piece.type == KNIGHT) {
                    std::cout << name[1]; // Use N for Knight
                }
                else {
                    std::cout << name[0]; // first letter of the piece type
                }
            } else {
                std::cout << ".";
            }
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void Board::initialize_board() {
    for (int col = 0; col < 8; ++col) {
        this->squares[1][col].occupied = true;
        this->squares[1][col].piece = {PAWN, BLACK, false};
        this->squares[6][col].occupied = true;
        this->squares[6][col].piece = {PAWN, WHITE, false};
    }

    PieceType piece_orderWhite[8] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
    for (int col = 0; col < 8; ++col) {
        this->squares[7][col].occupied = true;
        this->squares[7][col].piece = {piece_orderWhite[col], WHITE, false};
    }

    PieceType piece_orderBlack[8] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
    for (int col = 0; col < 8; ++col) {
            this->squares[0][col].occupied = true;
        this->squares[0][col].piece = {piece_orderBlack[col], BLACK, false};
    }
}

std::string to_string(PieceType type) {
    switch (type) {
        case KING:
            return "KING";
        case QUEEN:
            return "QUEEN";
        case ROOK:
            return "ROOK";
        case BISHOP:
            return "BISHOP";
        case KNIGHT:
            return "KNIGHT";
        case PAWN:
            return "PAWN";
        case NONE:
            return "NONE";
        default:
            return "UNKNOWN";
    }
}

std::string to_string(Color color) {
    switch (color) {
        case WHITE:
            return "WHITE";
        case BLACK:
            return "BLACK";
        case NO_COLOR:
            return "NO_COLOR";
        default:
            return "UNKNOWN";
    }
}