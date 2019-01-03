#include <vector>
#include "Game.hpp"

// Color ist in game gespeichert. index gibt Figur an die bewegt werden soll.
// weiß ist kleiner 20 / schwarz ist >= 20 und 2 für Bauer
std::vector<int> getMoveSet(int index, Game &game) {
    int type = game.board[index];
    std::vector<int> moveset;
    //King:
    if (type == 10 || type == 20) {

    }
    //Queen
    else if (type == 14 || type == 24) {

    }
    //Bishop
    else if (type == 13 || type == 23) {

    }
    //Knight
    else if (type == 12 || type == 22) {

    }
    //Rook
    else if (type == 11 || type == 21) {

    }
    //white Pawn
    else if (type = 1) {

    }
    //black Pawn
    else if (type == 2) {

    }
}