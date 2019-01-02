#ifndef INCLUDE_Field
#define INCLUDE_Field
#include <vector>
#include "Field.hpp"
#include "Game.hpp"

enum class Color {BLACK, WHITE};
enum class Piece { King, Queen, Rook, Knight, Bishop, Pawn };

class Figure {
public:
    Figure(int x, int y);
    virtual ~Figure() = default;
    Field loc;
    virtual Piece getName() = 0;
};

class King : public Figure {
public:
    using Figure::Figure;
    virtual Piece getName() { return Piece::King; }
};

class Queen : public Figure {
public:
    using Figure::Figure;
    virtual Piece getName() { return Piece::Queen; }
};

class Rook : public Figure {
public:
    using Figure::Figure;
    virtual Piece getName() { return Piece::Rook; }
};

class Bishop : public Figure {
public:
    using Figure::Figure;
    virtual Piece getName() { return Piece::Bishop; }
};

class Knight : public Figure {
public:
    using Figure::Figure;
    virtual Piece getName() { return Piece::Knight; }
};

class Pawn : public Figure {
public:
    using Figure::Figure;
    virtual Piece getName() { return Piece::Pawn; }
};

std::vector<SmallGame> getMoveSet(int index, SmallGame game) {
    //raus finden was für ne Figure
    bool white = index < 16;
    index %= 16;
    //Zuordnung index -> Figure
    /*
    King
    Queen
    2*Bishop
    2*Knight
    2*Rook
    8*Pawn
    */
    Piece p = index > 7 ? Piece::Pawn : index > 5 ? Piece::Rook : index > 3 ? Piece::Knight : index > 1 ? Piece::Bishop : index == 1 ? Piece::Queen : Piece::King; 
    std::vector<SmallGame> temp;
    if (p == Piece::King) {
        
    }
}


#endif
