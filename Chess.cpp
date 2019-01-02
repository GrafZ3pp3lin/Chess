#include "Figures.hpp"

Game game;

void initGame() {
    game.white[0] = std::make_unique<King>(4, 0);
    //{ new Queen(3, 0), new Bishop(2, 0), new Bishop(5, 0), new Knight(1, 0), new Knight(6, 0), new Rook(0, 0), new Rook(7, 0) };
}

int main() {
    return 0;
}