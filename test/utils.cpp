#include <iostream>

#include "color_scheme.h"
#include "fatpup/position.h"

void PrintPosition(const fatpup::Position& pos)
{
    for (int row_idx = fatpup::BOARD_SIZE - 1; row_idx >= 0; --row_idx)
    {
        std::cout << (row_idx + 1) << " ";
        for (int col_idx = 0; col_idx < fatpup::BOARD_SIZE; ++col_idx)
        {
            const fatpup::Square square = pos.square(row_idx, col_idx);
            const unsigned char piece = square.piece();
            if (piece == fatpup::Empty)
            {
                if (square.isFlagSet(fatpup::EnPassant))
                    std::cout << "*";
                else
                    std::cout << (((row_idx ^ col_idx ^ lightForegroundScheme) & 1) ? emptySquare : " ");
            }
            else
            {
                std::cout << (square.isWhite() ? whitePieceColor : blackPieceColor);
                switch (piece)
                {
                case fatpup::Pawn: std::cout << "p"; break;
                case fatpup::Knight: std::cout << "N"; break;
                case fatpup::Bishop: std::cout << "B"; break;
                case fatpup::Rook: std::cout << "R"; break;
                case fatpup::Queen: std::cout << "Q"; break;
                default: std::cout << "K";
                }
                std::cout << rang::fg::reset;
            }
        }
        std::cout << "\n";
    }
    std::cout << "  ABCDEFGH" << std::endl;
}