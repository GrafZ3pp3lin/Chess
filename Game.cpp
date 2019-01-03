#include <vector>
#include "Game.hpp"

// Color ist in game gespeichert. index gibt Figur an die bewegt werden soll.
// weiß ist kleiner 20 / schwarz ist >= 20 und 2 für Bauer
std::vector<Game> getMoveSet(int index, Game &game) {
    int type = game.board[index];
    std::vector<Game> moveset;
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

//Is Move from Player possible
bool isMovePossible(int from, int to, Game &game) {
    int type = game.board[from];
    //illegal move
    if (!(game.board[to] == 0
    || (game.active == Color::WHITE && (game.board[to] >= 20 || game.board[to] == -2))
    || (game.active == Color::BLACK && game.board[to] < 20 && game.board[to] != -2))) {
        return false;
    }
    int diff = std::abs(to - from);
    // Bewertung des Spiels anpassen, wenn Figur geschlagen wird.
    //King:
    if (type == 10 || type == 20) {
        return (checkDirection(from, to, 1, game, false) || checkDirection(from, to, -1, game, false)
        || checkDirection(from, to, 10, game, false) || checkDirection(from, to, -10, game, false)
        || checkDirection(from, to, 11, game, false) || checkDirection(from, to, -11, game, false)
        || checkDirection(from, to, 9, game, false) || checkDirection(from, to, -9, game, false));
    }
    //Queen
    else if (type == 14 || type == 24) {
        return (checkDirection(from, to, 1, game, true) || checkDirection(from, to, -1, game, true)
        || checkDirection(from, to, 10, game, true) || checkDirection(from, to, -10, game, true)
        || checkDirection(from, to, 11, game, true) || checkDirection(from, to, -11, game, true)
        || checkDirection(from, to, 9, game, true) || checkDirection(from, to, -9, game, true));
    }
    //Bishop
    else if (type == 13 || type == 23) {
        return (checkDirection(from, to, 11, game, true) || checkDirection(from, to, -11, game, true)
        || checkDirection(from, to, 9, game, true) || checkDirection(from, to, -9, game, true));
    }
    //Knight
    else if (type == 12 || type == 22) {
        return (checkDirection(from, to, 21, game, false) || checkDirection(from, to, -21, game, false)
        || checkDirection(from, to, 19, game, false) || checkDirection(from, to, -19, game, false)
        || checkDirection(from, to, 12, game, false) || checkDirection(from, to, -12, game, false)
        || checkDirection(from, to, 8, game, false) || checkDirection(from, to, -8, game, false));
    }
    //Rook
    else if (type == 11 || type == 21) {
        return (checkDirection(from, to, 1, game, true) || checkDirection(from, to, -1, game, true)
        || checkDirection(from, to, 10, game, true) || checkDirection(from, to, -10, game, true));
    }
    //white Pawn
    else if (type = 1) {
        return ((from - to == 10 && game.board[from - to] == 0) //einzelner Zug vor
        || ((int)(from / 10) == 8 && from - to == 20 && game.board[from - to] == 0) // doppelter Zug vor
        || ((from - to == -9 || from - to == -11) && (game.board[from - to] >= 20 || game.board[from - to] == 2 // Diagonaler Schlag
        || game.enPassant == to))); // enPassant
    }
    //black Pawn
    else if (type == 2) {
        return ((from - to == -10 && game.board[from - to] == 0) //einzelner Zug vor
        || ((int)(from / 10) == 3 && from - to == -20 && game.board[from - to] == 0) // doppelter Zug vor
        || ((from - to == 9 || from - to == 11) && (game.board[from - to] <= 20 && game.board[from - to] != 2 // Diagonaler Schlag
        || game.enPassant == to))); // enPassant
    }
}

bool checkDirection(int from, int to, int con, Game &game, bool multiple) {
    if (from + con == to) {
        return true;
    }
    else if (multiple) {
        if (con > 0) {
            while (from + con < to && game.board[from + con] == 0);
        }
        else if (con < 0) {
            while (from + con > to && game.board[from + con] == 0);
        }
        if (from + con == to) {
            return true;
        }
    }
    return false;
}